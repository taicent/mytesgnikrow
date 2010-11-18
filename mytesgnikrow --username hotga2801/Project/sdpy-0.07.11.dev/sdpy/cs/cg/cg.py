#!/usr/bin/env python

__all__ = ['isInsidePoly','smallestEnclosingBall','distanceToPlane']

from numpy import allclose, dot, zeros
from numpy.linalg import norm
from scipy.weave import inline
from .. import arrayd

def isInsidePoly(x,y,poly):
    """
    Check if point (x,y) is inside a polygon or not.
    
    Input:
        x: x location
        y: y location
        poly: a numpy array of shape (N,2) representing a polygon
    Output:
        1 if (x,y) is inside the polygon, 0 otherwise.
        
    Reference:
        W. Randolph Franklin (WRF) at 
            http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
    """
    # convert all to double
    x = float(x)
    y = float(y)
    poly = arrayd(poly)
    
    # prepare variables
    N = len(poly)
    
    code = """
        int i, j, c = 0;
        double *Pi, *Pj;
        for( j = N-1, Pj = &poly[j*2], i = 0; i < N; Pj = Pi, j = i++ )
        {
            Pi = &poly[i*2];
            if(
                ((Pi[1] <= y && y < Pj[1]) || (Pj[1] <= y && y < Pi[1])) &&                
                (x-Pi[0] < (Pj[0]-Pi[0]) * (y-Pi[1]) / (Pj[1]-Pi[1]))
            ) c = !c;
        }
        return_val = c;
    """
    return inline(code,['N','x','y','poly'])

def smallestEnclosingBall(a):
    """Compute a (hyper-)ball of smallest radius enclosing a point set.
    
    :Parameters:
        a : array(shape=(N,d), dtype='d')
            an array of N d-dimensional points
            
    :Returns:
        p : array(shape=(d,), dtype='d')
            center point of the (hyper-)ball
        r : double
            its *squared* radius, supposed to be the smallest possible value
        support_array : array(shape=(M,d), dtype='d')
            an array of supporting points, M is the number of supporting points
        accuracy : double
            relative accuracy of the solution
        slack : double
            slack value
    
    Reference:
        This is a re-implementation from scratch based on the idea of
        Bernd Gaertner at http://www.inf.ethz.ch/personal/gaertner
        by Minh-Tri Pham
    """
    a = arrayd(a)
    
    (N,d) = a.shape
    outvec = zeros(d+1)
    supvec = zeros((N,d))
    statsi = zeros(2,'int')
    statsd = zeros(2,'double')
    
    code = """
        int i, j;
        
        SdMinBall mb(d);        
        for( i = 0; i < N; ++i ) mb.check_in(SdVector(&A2(i,0),d));
        
        mb.build();
        
        for( i = 0; i < d; ++i ) outvec[i] = mb.center()[i];
        outvec[d] = mb.squared_radius();
        
        int nsp = mb.nr_support_points();
        SdMinBall::Cit it;
        for (i=0, it=mb.support_points_begin(); it!=mb.support_points_end(); ++it, ++i)
            for( j = 0; j < d; ++j ) SUPVEC2(i,j) = (*it)[j];
        
        statsd[0] = mb.accuracy(statsd[1]);
        
        return_val = nsp;
    """
#    nsp = inline(code, arg_names=['a','N','d','outvec','supvec','statsd'], headers=['"minball.h"'],include_dirs=['.'],verbose=2)
    
    nsp = inline(code, arg_names=['a','N','d','outvec','supvec','statsd'], \
        headers=['<SdMinBall.h>'], libraries=['sdcpp', 'cblas_ext', 'cephesd', 'plat_det'])
    
    return outvec[:d],float(outvec[d]),supvec[:nsp],float(statsd[0]),float(statsd[1])

def _distanceToPlane(a,plane):
    """Compute the value(s) proportional to the L2 distance(s) of a point or a
     set of points to a plane.

    :Parameters:
        a : array(shape=(*,d), dtype='d')
            a point or a set of points in a d-dimensional space
        plane : array(shape=(d+1,), dtype='d')
            a set of parameters representing the plane
                i.e. a0*x0 + a1*x1 + ... + a_{d-1}*x_{d-1} + a_d = 0

    :Returns:
        b : array(shape=(*), dtype ='d')
            a value or a set of values representing the value(s) proportional
            to the L2 distance(s)
        alpha : double
            a value representing the proportion, divide the value(s) by this
            number to get the L2 distance(s)
    """
    d = plane.size - 1

    ishape = a.shape[:-1]
    if d != a.shape[-1]:
        raise IndexError('The shape of the first argument is not correct.')

    pa = plane[:d]
    x0 = plane[d]

    alpha = norm(pa)
    if allclose(alpha,0):
        raise ValueError('The specified (hyper-)plane is singular.')

    b = dot(a.reshape(a.size / d, d), pa) + x0
    return b.reshape(ishape), alpha


def distanceToPlane(a,plane):
    """Compute the L2 distance(s) of a point or a set of points to a plane.

    :Parameters:
        a : array(shape=(*,d), dtype='d')
            a point or a set of points in a d-dimensional space
        plane : array(shape=(d+1,), dtype='d')
            a set of parameters representing the plane
                i.e. a0*x0 + a1*x1 + ... + a_{d-1}*x_{d-1} + a_d = 0

    :Returns:
        b : array(shape=(*), dtype ='d')
            a value or a set of values representing the L2 distance(s)
    """
    b, alpha = _distanceToPlane(a,plane)
    return b/alpha

