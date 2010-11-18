#-------------------------------------------------------------------------------
# Name:        integralimage
# Purpose:     Integrate an IplImage and vectorize it
#
# Author:      minhtri
#
# Created:     04/07/2007
# Copyright:   (c) minhtri 2007
# Licence:     Lesser GPL
#-------------------------------------------------------------------------------
#!/usr/bin/env python

__all__ = ['make_vi','integral','stats_integral','compute_Stats2_integral']

from numpy import array, add, zeros, dot, arange, argmin, ones, concatenate
from scipy.weave import inline
from ..linalg import daxpy, dger
from ..stats import mean, covariance, Stats2

def integral(img,iimg):
    """Convert an image of type uint8 into an image integral of type double.
    
    Input:
        img: a 2D numpy.array of type uint8
        iimg: a 2D numpy.array of type double of the same size to hold the data
    Output:
        data filled into img2, where iimg[x,y] = int( img[x,y], i = 0..x, j = 0..y )
    """
    h, w = img.shape
    wss = img.strides[0] # for uint8
    wsd = iimg.strides[0] / 8 # for double
    code = """
        double *ddst;
        unsigned char *dsrc;
        int y, x;
        
        ddst = iimg;
        dsrc = img;
        
        // The top left element
        ddst[0] = dsrc[0];
        
        // The first row
        for( x = 1; x < w; ++x ) ddst[x] = ddst[x-1] + dsrc[x];
        
        // The remaining rows
        for( y = 1; y < h; ++y )
        {
            // The left most element
            ddst += wsd;
            dsrc += wss;
            ddst[0] = ddst[-wsd] + dsrc[0];
            
            // The remaining elements
            for( x = 1; x < w; ++x ) ddst[x] = ddst[x-1] + ddst[x-wsd] - ddst[x-wsd-1] + dsrc[x];
        }
        return_val = 1;
    """
    inline(code,['img','iimg','w','h','wss','wsd'])


def make_vi(input_data):
    """Given a list of image patches, integrate then vectorize them all.

    Input:
        input_data: a numpy.array of shape (N,d,d) type uint8
    Output:
        output_data: the vectorized-integrated version, a numpy.array of shape (N,d*d) type float64
    """
    theshape = input_data.shape
    N = theshape[0]
    d = theshape[1]
    output_data = zeros([N,d*d])
    map(integral,input_data,output_data.reshape(N,d,d))
    return output_data


def stats_integral(input_data,weights = None):
    """Similar to stats.statslist(), but integrate each image of type uint8 first, then vectorize, then do the stats.

    Input:
        input_data: a array 2D images of type uint8 (numpy.array)
    Output:
        w, mean, cov: total weight, mean vector and covariance matrix
    """
    
    n, h, w = input_data.shape
    sz = w*h
    tmp = zeros([h,w])
    tmpr = tmp.ravel()
    sum1r = zeros(sz)
    sum2r = zeros([sz,sz])
    if weights is None:
        tw = n
        for x in input_data:
            integral(x,tmp)
            sum1r = daxpy(tmpr,sum1r)
            sum2r = dger(1,tmpr,tmpr,a=sum2r,overwrite_a=1)
    else:
        tw = weights.sum()
        for i in xrange(n):
            integral(input_data[i],tmp)
            sum1r = daxpy(tmpr,sum1r,a=weights[i])
            sum2r = dger(weights[i],tmpr,tmpr,a=sum2r,overwrite_a=1)
    themean = mean(tw,sum1r)
    thecov = covariance(tw,sum2r,themean)
    return (tw,themean,thecov)


def compute_Stats2_integral(classification_dataset, vi = True):
    """Given a J-class ClassificationDataset of image patches.
    The patches are either in 'uint8' raw form (vi = False),
    or in 'double' vectorintegrated form (vi = True)

    Input:
        classification_dataset: a ClassificationDataset
        vi: input data are vectorintegrated?
    Output:
        stats2: a Stats2 holding the obtained statistics
    """

    if vi:
        return classification_dataset.compute_Stats2()
    else:
        cd = classification_dataset
        J = cd.J
        if cd.weights is None:
            mlist = map( stats_integral, cd.input_data )
        else:
            mlist = map( stats_integral, cd.input_data, cd.weights )
        ldim = mlist[0][1].size
        A = zeros([J,1+ldim+ldim*ldim])
        for i in xrange(J):
            A[i,0] = mlist[i][0]
            A[i,1:ldim+1] = mlist[i][1].ravel()
            A[i,ldim+1:] = mlist[i][2].ravel()
        return Stats2(J, (ldim,), A)


