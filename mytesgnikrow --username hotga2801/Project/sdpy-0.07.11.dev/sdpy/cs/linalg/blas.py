#!/usr/bin/env python

__all__ = ['dger', 'daxpy']

try:
    from scipy.lib.blas.fblas import daxpy, dger
except ImportError:
    # Make this usable by people who don't have BLAS installed
    from numpy import multiply
        
    def daxpy(x,y,n=(len(x)-offx)/abs(incx),a=1.0,offx=0,incx=1,offy=0,incy=1):
        """Return a*x+y, ignoring all other parameters."""
        return a*x+y
    def dger(alpha,x,y,incx=1,incy=1,a=0.0,overwrite_x=1,overwrite_y=1,overwrite_a=0):
        """Return alpha*multiply.outer(x,y)+a, ignoring all other paramters."""
        if a == 0.0:
            return alpha*multiply.outer(x,y)
        return alpha*multiply.outer(x,y)+a
