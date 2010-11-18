#-------------------------------------------------------------------------------
# Module:      __init__
# Package:     sd.cs.ml
# Purpose:     Initiate the 'ml' package -- machine learning
#
# Author:      minhtri
#
# Created:     25/03/2007
# Copyright:   (c) minhtri 2007
# Licence:     Lesser GPL
#-------------------------------------------------------------------------------
#!/usr/bin/env python

__all__ = [
    'Predictor',
    'Dataset',
    'DataGenerator'
    ]

from numpy import array


#-------------------------------------------------------------------------------
# Representation of a data set
#-------------------------------------------------------------------------------
class Dataset:
    def __init__(self,N):
        self.N = N


#-------------------------------------------------------------------------------
# Representation of a data generator
#-------------------------------------------------------------------------------
class DataGenerator:
    def __init__(self,input_shape):
        self.input_shape = input_shape


#-------------------------------------------------------------------------------
# Machine-learning predictor (a filter, a classifier, or a regressor)
#-------------------------------------------------------------------------------
class Predictor:
    
    def predict( self, input_point ):
        pass

    def test( self, input_data ):
        return array(map(self.predict,input_data))

    def can_learn(self):
        """Check if this perdictor can learn incrementally."""
        return False

