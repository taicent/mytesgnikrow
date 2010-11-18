#-------------------------------------------------------------------------------
# Module:      __init__
# Package:     sd.cs.ml.reg
# Purpose:     Subroutines for regression methods
#
# Author:      minhtri
#
# Created:     25/03/2007
# Copyright:   (c) minhtri 2007
# Licence:     Lesser GPL
#-------------------------------------------------------------------------------
#!/usr/bin/env python

__all__ = [
    'RegressionDataset',
    'LinearRegressor',
    'train_LinearRegressor'
    ]

from numpy import array, zeros, dot, prod
from scipy.linalg import pinv
from ...stats import statslist, mean, moment1, correlation, moment11
from .... import tprint
from .. import Predictor, Dataset

#-------------------------------------------------------------------------------
# Representation of a regression data set
#-------------------------------------------------------------------------------
class RegressionDataset(Dataset):
    def __init__(self, input_data, output_data, weights = None ):
        Dataset.__init__(self,len(input_data))
        self.ishape = input_data[0].shape
        self.isize = prod(self.ishape)
        self.oshape = output_data[0].shape
        self.osize = prod(self.oshape)
        self.input_data = input_data
        self.output_data = output_data
        self.weights = weights


#-------------------------------------------------------------------------------
# Linear Regressor
#-------------------------------------------------------------------------------
class LinearRegressor(Predictor):
    def __init__( self, ishape, oshape, A = None, b = None):
        self.ishape = ishape
        self.oshape = oshape
        if A is None:
            self.A = zeros(ishape+oshape)
        else:
            self.A = A
        if b is None:
            self.b = zeros(oshape)
        else:
            self.b = b
    
    def predict(self,input_point):
        return (dot(self.A,input_point.ravel())+self.b).reshape(self.oshape)


def train_LinearRegressor( regression_dataset ):
    """Train a LinearRegressor using a RegressionDataset
    
    Input:
        regression_dataset: a RegressionDataset
    Output:
        a LinearRegressor
    """
    
    rd = regression_dataset
    idata = rd.input_data.reshape(rd.N,rd.isize)
    odata = rd.output_data.reshape(rd.N,rd.osize)

    n, m1, cov1 = statslist(idata,rd.weights)
    m2 = mean(n,moment1(odata,weights))
    cor = correlation(n,moment11(idata,odata,weights),m1,m2)

    self.A = dot(pinv(cov1),cor).T
    self.b = m2 - dot(self.A,m1)

    return LinearRegressor(rd.ishape,rd.oshape,A,b)

