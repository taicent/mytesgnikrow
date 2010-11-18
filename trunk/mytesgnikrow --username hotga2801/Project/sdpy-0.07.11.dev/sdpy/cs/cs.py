#!/usr/bin/env python

__all__ = ['arrayd','approx','sdcpp_info','dtype_numpy_to_C']

from scipy import alterdot
from numpy import ascontiguousarray, asarray, all, abs

# e.g. dtype_numpy_to_C[a.dtype.name]
dtype_numpy_to_C = {
    'int8': 'char',
    'uint8': 'unsigned char',
    'int16': 'short',
    'uint16': 'unsigned short',
    'int32': 'long',
    'uint32': 'unsigned long',
    'int64': 'long long',
    'uint64': 'unsigned long long',
    'float32': 'float',
    'float64': 'double'
}

#------------------------------------------------------------------------------------------

# Some initializations

# from ..system_info import get_info
# sdcpp_info = get_info('sdcpp',0)
# paypack_info = get_info('paypack',0)
# for x in paypack_info:
    # sdcpp_info[x] += paypack_info[x]

alterdot()

def arrayd(A):
    """Convert A into a contiguous numpy.array of dtype('float64'), if necessary"""
    return ascontiguousarray(A,'d')

def approx(a,b,epsilon = 10**-6):
    """Returns true if a and b are of the same shape, and their elements are approximately equal.

    :Parameters:
        a : array
            the left operand
        b : array
            the right operand
        epsilon: double
            the maximum deviance
            
    :Returns:
        out: boolean
            True if a and b are of the same shape, and their elements are approximately equal.
    """
    try:
        return all(abs(a-b) < epsilon)
    except:
        return False
