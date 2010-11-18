#-------------------------------------------------------------------------------
# Module:      __init__
# Package:     sd.cs.stats
# Purpose:     Statistics subroutines
#
# Author:      minhtri
#
# Created:     25/03/2007
# Copyright:   (c) minhtri 2007
# Licence:     Lesser GPL
#-------------------------------------------------------------------------------
#!/usr/bin/env python

__all__ = ['weighted','weighted_outer','sum_outer','moment0','moment1',
    'moment2','moment2e','moment11','mean','covariance','stde',
    'statslist','statsliste','correlation','BasicStats',
    'Stats2e','Stats2m','Stats2','stdnorm']

from .. import arrayd
from math import fabs, sqrt
from numpy.random import randint as np_randint
from numpy import ascontiguousarray
from numpy import dot, array, zeros, outer, std, concatenate, add, cov, \
    prod, multiply, ravel
from numpy import mean as np_mean
from numpy import sqrt as np_sqrt
from numpy import log as np_log
from scipy.linalg import pinv, eig
from scipy.weave import inline
from random import randint
from scipy.stats import norm

################################################################################
## Basic statistical sub-routines
################################################################################

def weighted(w,A):
    """Compute w[0]*A[0] + w[1]*A[1] + ... until the end.

    Input:
        w: a 1D numpy.array
        A: a multi-dimensional numpy.array
    """
    return dot(arrayd(w),A.reshape(len(A),A[0].size)).reshape(A[0].shape)


def weighted_outer(w,A,B = None):
    """Compute w[0]*outer(A[0],B[0]) + w[1]*outer(A[1],B[1]) + ... until the end.

    Input:
        w: a 1D numpy.array
        A: a multi-dimensional numpy.array
        B: a multi-dimensional numpy.array (optional)
    """
    if B is None: B = A
    AA = multiply( A.reshape(len(A),A[0].size).T, arrayd(w) )
    return dot(AA,B.reshape(len(B),B[0].size)).reshape(A[0].shape+B[0].shape)

def sum_outer(A,B = None):
    """Compute outer(A[0],B[0]) + outer(A[1],B[1]) + ... until the end.

    Input:
        A: a multi-dimensional numpy.array
        B: a multi-dimensional numpy.array (optional)
    """
    AA = arrayd(A.reshape(len(A),A[0].size))
    if B is None:
        return dot(AA.T,AA).reshape(A[0].shape*2)
    else:
        return dot(AA,B.reshape(len(B),B[0].size)).reshape(A[0].shape+B[0].shape)

def moment0(data,weights = None):
    """Return the 0th order (weighted) moment of the data
    
    Input:
        data: a numpy.array
        weights: an array of weights for the instances [optional]
    Output:
         0th order moment -- number of (weighted) instances -- equivalent to len(data)
    """
    if weights is None:
        return len(data)
    else:
        return weights.sum()

def moment1(data,weights = None):
    """Return the 1st order (weighted) moment of the data

    Input:
        data: a numpy.array
        weights: an array of weights for the instances [optional]
    Output:
         s1: 1st order (weighted) moment
    """
    if weights is None:
        return arrayd(data.sum(0))
    else:
        return weighted(weights,data)

def moment2(data,weights = None):
    """Return the 2nd order (weighted) moment of the data

    Input:
        data: a numpy.array
        weights: an array of weights for the instances [optional]
    Output:
         s2: 2nd order (weighted) moment
    """
    if weights is None:
        return sum_outer(data)
    else:
        return weighted_outer(weights,data)

def moment2e(data,weights = None):
    """Return the per-element 2nd order (weighted) moment of the data

    Input:
        data: a numpy.array
        weights: an array of weights for the instances [optional]
    Output:
         s2e: per-element 2nd order (weighted) moment
    """
    data = arrayd(data)
    data = multiply(data,data)
    if weights is None:
        return data.sum(0)
    else:
        return weighted(weights,data)

def moment11(data1,data2,weights = None):
    """Return the (1,1)-th order (weighted) cross moment the two data (point1 * point2^T)

    Input:
        data1: a numpy.array
        data2: a numpy.array
        weights: an array of weights for the instances [optional]
    Output:
        s11: (1,1)-th order (weighted) cross moment
    """
    if weights is None:
        return sum_outer(data1,data2)
    else:
        return weighted_outer(weights,data1,data2)

def mean(n,s1):
    """Get the mean tensor based on 0th and 1st order moments

    Input:
        n: 0th order moment
        s1: 1st order moment
    Output:
        mean: mean tensor
    """
    return s1/n

def covariance(n,s2,mean):
    """Get the covariance tensor based on 0th and 2nd order moments and mean

    Input:
        n: 0th order moment
        s2: 2nd order moment
        mean: mean tensor -- see mean()
    Output:
        thecov: covariance tensor
    """
    return (s2/n) - outer(mean,mean).reshape(s2.shape)

def stde(n,s2e,mean):
    """Get the per-element standard deviation tensor based on 0th and per-element 2nd order moments and mean

    Input:
        n: 0th order moment
        s2e: per-element 2nd order moment -- see moment2e()
        mean: mean temsor -- see mean()
    Output:
        stde: per-element stdandard deviation tensor
    """
    return ((s2e/n) - mean**2)**0.5


def statslist(data,weights = None,masks = None):
    """Return the (weighted) total weight, mean, and covariance of the data

    Input:
        data: a numpy.array
        weights: an array of weights for the instances [optional]
        masks: an array of masks for the instances [optional]
    Output:
         n: total weight
         mean: mean tensor
         thecov: covariance tensor
    """
    
    if masks is not None:
        data = data[masks]
        if weights is not None: weights = weights[masks]
    
    n = moment0(data,weights)
    if weights is None:
        mu = np_mean(data,0)
        theshape = data[0].shape
        thesize = data[0].size
        thecov = cov(data.reshape(n,thesize),None,0).reshape(theshape*2)
    else:
        mu = mean(n,moment1(data,weights))
        thecov = covariance(n,moment2(data,weights),mu)
    return (n,mu,thecov)

def statsliste(data,weights = None,masks = None):
    """Return the per-element (weighted) total weight, means, and standard deviations of the data

    Input:
        data: a numpy.array
        weights: an array of weights for the instances [optional]
        masks: an array of masks for the instances [optional]
    Output:
         n: total weight
         mean: mean tensor
         thestd: standard deviation tensor
    """

    if masks is not None:
        data = data[masks]
        weights = weights[masks]

    n = moment0(data,weights)
    if weights is None:
        mu = np_mean(data,0)
        thestd = std(data,0)
    else:
        mu = mean(n,moment1(data,weights))
        thestd = stde(n,moment2e(data,weights),mu)
    return (n,mu,thestd)

def correlation(n,s11,mean1,mean2):
    """Compute the correlation of 2 datasets based on their 0th and (1,1)-th order cross moment and their mean tensors.

    Input:
        n: 0th order moment
        s12: cross moment of the two datasets -- see moment11()
        mean1: mean tensor of the first dataset -- see mean()
        mean2: mean tensor of the second dataset -- see mean()
    Output:
        cor: correlation tensor of the two dataset (point1*point2^T)
    """
    return (s11/n) - outer(mean1,mean2).reshape(s11.shape)


################################################################################
## Sub-routines dealing with statistics packed as a tuple
################################################################################

class BasicStats:
    """
    A few statistics of a random tensor (RT) are packed as a tuple.
    This class represents the collection of some statistics of J RTs.
    The class contains a few variables:
        J: the number of RTs
        shape: the shape represents of how the dimensions of an RT are organized
        d: the number of dimensions of an RT
        orders: an array of statistic orders
        A: the numpy.array (tensor) representing the whole data
    """
    
    def __init__( self, J, shape, orders, A = None ):
        """Initialize BasicStats with J, shape, orders, and optionally A.
        """
        self.J = int(J)
        self.d = int(prod(shape))
        self.shape = shape
        self.orders = orders

        self.T = len(orders)
        self.ofs = zeros(self.T,'int32')
        self.size = zeros(self.T,'int32')
        
        self.ldim = 0
        for i in xrange(self.T):
            self.ofs[i] = self.ldim
            self.size[i] = self.d**orders[i]
            self.ldim += self.size[i]
            
        if A is None:
            self.A = zeros([J,self.ldim])
        else:
            self.A = A

    def get_indices( self, i ):
        """Get the indices of the i-th type of statistics.

        Input:
            i: the index
        Output:
            (istart,iend): the indices
        """
        if i < 0 or i >= self.T:
            raise IndexError("wrong index")

        return self.ofs[i], self.ofs[i]+self.size[i]

    def get_stat( self, i ):
        """Get the tensor of statistics of the i-th type.

        Input:
            i: the index
        Output:
            A: the tensor of statistics of the i-th type with shape (J,)+shape^rank[i]
        """
        istart, iend = self.get_indices(i)
        return self.A[:,istart:iend].reshape((self.J,)+self.shape*self.orders[i])

    def get_cond_stat( self, j, i ):
        """Get the class-conditional tensor of statistics of the ith-type.

        Input:
            j: the index
            i: the class
        Output:
            A: the class-conditional tensor of statistics of the i-th type
                of shape shape^rank[i]
        """
        istart, iend = self.get_indices(i)
        return self.A[j,istart:iend].reshape(self.shape*self.orders[i])
            
    def set_cond_stat( self, j, i, A ):
        """Set the class-conditional tensor of statistics of the ith-type.

        Input:
            j: the index
            i: the class
            A: the class-conditional tensor of statistics of the i-th type
                of shape shape^rank[i]
        """
        istart, iend = self.get_indices(i)
        self.A[j,istart:iend] = ravel(A)

    def _new_dA( self, new_shape, new_A ):
        """This function is to be virtuallized and used internally only.
        To create a copy of this 'self' with new 'shape' and new 'A'.
        """
        return BasicStats( self.J, shape, self.orders, new_A )

    def take( self, indices ):
        """Flatten the shape of the RTs, then take only dimensions indexed in 'indices' and remove the remaining dimensions.

        Input:
            indices: a numpy.array of dimensions to be kept.
        Output:
            B: the BasicStats obtained from the remaining dimensions.
        """
        d1 = len(indices)
        
        size = 0
        for i in xrange(self.T): size += d1**self.orders[i]
        theindices = zeros(size,'int32')
        ofs = 0
        for i in xrange(self.T):
            if self.orders[i] == 0:
                a = array([ofs])
            else:
                a = indices
                for j in xrange(self.orders[i]-1):
                    a = add.outer(a*self.d,indices).ravel()
            sz = d1**self.orders[i]
            theindices[ofs:ofs+sz] = a+self.ofs[i]
            ofs += sz
            
        return self._new_dA((d1,),self.A.take(theindices,1))


class Stats2e(BasicStats):
    """
    Statistics of a random tensor (RT) up to 2nd-order are packed as a tuple.
    The statistics used in this class:
        - a total weight
        - a mean tensor
        - a tensor of standard deviations per dimension.
    """
    
    def __init__( self, J, shape, A = None ):
        """Initialize Stats2e with J and shape and optional A.
        """
        BasicStats.__init__(self,J,shape,array([0,1,1]),A)
            
    def _new_dA( self, new_shape, new_A ):
        """This function is to be virtuallized and used internally only.
        To create a copy of this 'self' with new 'shape' and new 'A'.
        """
        return Stats2e( self.J, new_shape, new_A )

    def learn( self, input_point, j, w = None ):
        """Add a new input point of class j with optional weight w into the stats.

        Input:
            input_point: input point, same shape with 'shape'
            j: its class
            w: optional weight, or 1 if not specified
        Output:
            nothing, just update the stats
        """
        if w is None: w = 1.0
        else: w = float(w)
        x = ravel(input_point)

        # tw0 = ravel(self.get_cond_stat(j,0))
        # tw1 = ravel(self.get_cond_stat(j,1))
        # tw2 = ravel(self.get_cond_stat(j,2))
        
        # tw0a = tw0 + w
        # tw1a = (tw1*tw0+x*w)/tw0a
        # tw2a = np_sqrt(((tw2*tw2+tw1*tw1)*tw0+(x*x)*w)/tw0a - tw1a*tw1a)

        # self.set_cond_stat(j,0,tw0a)
        # self.set_cond_stat(j,1,tw1a)
        # self.set_cond_stat(j,2,tw2a)

        A = self.A[j]
        d = len(x)
        code = """
            int i;
            double tw0, tw1, tw2;
            
            tw0 = A[0];
            A[0] = tw0+w;
            
            for( i = 0; i < d; ++i )
            {
                tw1 = A[i+1];
                tw2 = A[i+1+d];
                A[i+1] = (tw1*tw0+x[i]*w)/A[0];
                A[i+1+d] = sqrt(((tw2*tw2+tw1*tw1)*tw0+x[i]*x[i]*w)/A[0] - A[i+1]*A[i+1]);
            }
            
            return_val = 1;
        """
        inline(code,['x','d','w','A'])
        

class Stats2m(BasicStats):
    """
    Statistics of a random tensor (RT) consisting of:
        - a total weight
        - a weighted sum -- 1st-order moment
        - a weighted sum-of-outer-products -- 2nd-order moment
    are packed as a tuple.
    """

    def __init__( self, J, shape, A = None ):
        """Initialize Stats2m with J and shape and optional A.
        """
        BasicStats.__init__(self,J,shape,array([0,1,2]),A)

    def _new_dA( self, new_shape, new_A ):
        """This function is to be virtuallized and used internally only.
        To create a copy of this 'self' with new 'shape' and new 'A'.
        """
        return Stats2m( self.J, new_shape, new_A )

    def learn( self, input_point, j, w = None ):
        """Add a new input point of class j with optional weight w into the stats.

        Input:
            input_point: input point, same shape with 'shape'
            j: its class
            w: optional weight, or 1 if not specified
        Output:
            nothing, just update the stats
        """
        if w is None: w = 1.0
        else: w = float(w)
        x = ravel(input_point)
        
        d = self.d
        A = self.A
        A[j,0] += w
        A[j,1:1+d] += x*w
        A[j,1+d:1+d+d*d] += (outer(x,x)*w).ravel()


class Stats2(BasicStats):
    """
    Statistics of a random tensor (RT) consisting of:
        - a total weight
        - a mean tensor
        - a covariance tensor
    are packed as a tuple.
    """

    def __init__( self, J, shape, A = None ):
        """Initialize Stats2 with J and shape and optional A.
        """
        BasicStats.__init__(self,J,shape,array([0,1,2]),A)

    def _new_dA( self, new_shape, new_A ):
        """This function is to be virtuallized and used internally only.
        To create a copy of this 'self' with new 'shape' and new 'A'.
        """
        return Stats2( self.J, new_shape, new_A )

    def project( self, w ):
        """Linearly project the RTs down to a line using direction w.
        The projected variances are replaced by the projected standard deviations.
        Return the projected statistics.
        
        Input:
            w: the projecting direction.
        Output:
            B: the Stats2e obtained from projecting the RTs.
        """
        w = w.ravel()
        
        B0 = self.get_stat(0).reshape(self.J,1)
        
        M1 = self.get_stat(1).reshape(self.J,self.d)
        B1 = dot(M1,w).reshape(self.J,1)
        
        M2 = self.get_stat(2).reshape(self,J,self.d,self.d)
        B2 = np_sqrt(dot(dot(M2,w),w)).reshape(self.J,1) # this is actually sqrt(w^T Cov w) for each class
        BB = concatenate((B0,B1,B2),1)
        return Stats2e(self.J,(1,),BB)

    def learn( self, input_point, j, w = None ):
        """Add a new input point of class j with optional weight w into the stats.

        Input:
            input_point: input point, same shape with 'shape'
            j: its class
            w: optional weight, or 1 if not specified
        Output:
            nothing, just update the stats
        """
        if w is None: w = 1.0
        else: w = float(w)
        x = ravel(input_point)

        tw0 = ravel(self.get_cond_stat(j,0))
        tw1 = ravel(self.get_cond_stat(j,1))
        tw2 = self.get_cond_stat(j,2).reshape((self.d,)*2)

        tw0a = tw0 + w
        tw1a = (tw1*tw0+x*w)/tw0a
        tw2a = ((tw2+outer(tw1,tw1))*tw0+outer(x,x)*w)/tw0a-outer(tw1a,tw1a)

        self.set_cond_stat(j,0,tw0a)
        self.set_cond_stat(j,1,tw1a)
        self.set_cond_stat(j,2,tw2a)


################################################################################
## Sub-routines dealing with normal distribution
################################################################################

# Standard normal distribution
stdnorm = norm(0,1)
