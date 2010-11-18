#-------------------------------------------------------------------------------
# Module:      __init__
# Package:     sd.cs.ml.cla
# Purpose:     Subroutines for classification methods
#
# Author:      minhtri
#
# Created:     26/03/2007
# Copyright:   (c) minhtri 2007
# Licence:     Lesser GPL
#-------------------------------------------------------------------------------
#!/usr/bin/env python

__all__ = ['CDataset','Classifier','BinaryClassifier','PClassifier',
    'BinaryPClassifier','TLinearClassifier']

from numpy import array, zeros, prod, concatenate
from .. import Predictor, Dataset
    

#-------------------------------------------------------------------------------
# Representation of a classification data set
#-------------------------------------------------------------------------------
class CDataset(Dataset):

    def __init__(self, separated_input_data ):
        self.nspc = array([len(x) for x in separated_input_data]) # number of samples per class
        Dataset.__init__(self,int(self.nspc.sum()))

        self.J = len(separated_input_data) # the number of classes

        self.ishape = separated_input_data[0][0].shape # get the shape of an input point
        self.isize = prod(self.ishape)

        self.input_data = separated_input_data

    def dofilter(self,j,filterarray):
        """Filter away some samples in class j.

        Input:
            j: class j
            filterarray: a 'bool' numpy.array of size self.nspc[j]
        Output:
            fr: filtering rate, the new number of samples of class j divided
                by the old number of samples of class j.
                The CDataset is updated.
        """
        nnew = filterarray.sum()
        nold = self.nspc[j]

        self.input_data[j] = self.input_data[j][filterarray]
        self.nspc[j] = nnew
        self.N += nnew - nold

        return float(nnew) / nold

    def concat(self,j,input_data):
        """Concatenate a few samples to class j.

        Input:
            j: class j
            input_data: an array of samples
        Output:
            The CDataset is updated.
        """
        nnew = len(input_data)
        self.nspc[j] += nnew
        self.N += nnew

        self.input_data[j] = concatenate([self.input_data[j],input_data])



#-------------------------------------------------------------------------------
# Classifiers
#-------------------------------------------------------------------------------

class Classifier(Predictor):

    def __init__( self, nclasses ):
        self.nclasses = nclasses

    def predict( self, input_point ): # return class 0 for now
        return 0

# Probabilistic Classifier
class PClassifier(Classifier):

    def __init__(self,nclasses):
        Classifier.__init__(self,nclasses)

    def predict_pdf(self,input_point): # return class 0 for now
        pdf = zeros(self.nclasses)
        pdf[0] = 1
        return pdf

    def predict(self,input_point):
        return self.predict_pdf(input_point).argmax()

    def test_pdf(self,input_data):
        return array(map(self.predict_pdf,input_data))


#-------------------------------------------------------------------------------
# Binary Classifiers
#-------------------------------------------------------------------------------

class BinaryClassifier(Classifier):
    def __init__(self):
        Classifier.__init__(self,2)

class BinaryPClassifier(PClassifier):
    def __init__(self):
        PClassifier.__init__(self,2)


#-------------------------------------------------------------------------------
# Linear Binary Classifiers
#-------------------------------------------------------------------------------

class TLinearClassifier(BinaryClassifier):
    """Thresholded linear classifier.
        f(x) = w^T x - b
        If f(x) >= 0 then return 1. Otherwise return 0.
        w, b are parameters.
    """
    def __init__(self,w,b):
        BinaryClassifier.__init__(self)
        self.w = w.ravel()
        self.b = b

    def confidence(self,input_point):
        return dot(self.w,input_point.ravel()) - self.b

    def predict(self,input_point):
        return int(dot(self.w,input_point.ravel()) >= self.b)

    def test(self,input_data):
        idata = input_data.reshape(len(input_data),input_data[0].size)
        return (dot(idata,self.w).ravel() >= self.b).astype('int')
