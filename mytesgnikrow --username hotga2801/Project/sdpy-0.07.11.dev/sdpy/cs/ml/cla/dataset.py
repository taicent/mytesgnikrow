#-------------------------------------------------------------------------------
# Module:      dataset
# Package:     sd.cs.ml.cla
# Purpose:     Subroutines dealing with different types of datasets
#
# Author:      minhtri
#
# Created:     26/03/2007
# Copyright:   (c) minhtri 2007
# Licence:     Lesser GPL
#-------------------------------------------------------------------------------
#!/usr/bin/env python

__all__ = ['WeightedCDataset','ScoredWCDataset','CDataGenerator']

from math import log, exp
from numpy import array, zeros, ones, prod, concatenate
from numpy import exp as NP_exp
from .... import tprint
from .. import DataGenerator
from cla import CDataset
from ...stats import Stats2, Stats2e, Stats2m, \
    moment0, moment1, moment2, moment2e, \
    statsliste, statslist
    
#-------------------------------------------------------------------------------
# Variants of the representation of a classification data set
#-------------------------------------------------------------------------------

class WeightedCDataset(CDataset):
    """A CDataset with weights attached to the samples."""
    
    def __init__(self, separated_input_data, separated_weights = None, approx = -1 ):
        CDataset.__init__(self,separated_input_data)

        self.weights = separated_weights

        # If approx > 0 then approximately compute mu, cov, var, std using
        # 100*approx percent of the total weights
        self.approx = approx 
        
    def new(self):
        """Make a copy of WeightedCDataset with a copy of weights and a reference to input_data.
        """
        if self.weights is None:
            return WeightedCDataset(self.input_data,None,self.approx)
        else:
            return WeightedCDataset(self.input_data,[x.copy() for x in self.weights],self.approx)
        

    def initialize_weights(self,value=-1):
        """Initialize weights if 'weights' is None
        """
        if self.weights is None:
            if value == -1:
                self.weights = [ones(x)/self.N for x in self.nspc]
            else:
                self.weights = [value*ones(x) for x in self.nspc]

    def set_weights(self,value=1):
        """Set all weight values to 'value', initialize if necessary
        """
        self.weights = [value*ones(x) for x in self.nspc]

    def get_twpc(self):
        """Get total weights per class.
        """
        if self.weights is None:
            return nspc.astype('float64')
        return array([x.sum() for x in self.weights])
    
    def get_skewness(self):
        if self.weights is None:
            return log(float(self.nspc[0])/self.nspc[1])
        return log(self.weights[0].sum()/self.weights[1].sum())

    def get_tw(self):
        """Get total weights of all class(es).
        """
        if self.weights is None:
            return float(self.N)
        return sum([x.sum() for x in self.weights])
        
    def normalize_weights(self):
        """Normalize the weights so that the sum of all weights is equal to 1.
        """
        self.initialize_weights()
        tw = sum([x.sum() for x in self.weights])
        self.weights = [x/tw for x in self.weights]
        
    def scale_weights(self,i,thelambda):
        """Scale the weights of class i by thelambda.
        """
        self.initialize_weights()
        self.weights[i] *= thelambda
        
    def dofilter(self,j,filterarray):
        """Filter away some samples in class j.
        
        Input:
            j: class j
            filterarray: a 'bool' numpy.array of size self.nspc[j]
        Output:
            fr: filtering rate, the new number of samples of class j divided
                by the old number of samples of class j.
                The WeightedCDataset is updated.
        """
        if self.weights is not None:
            self.weights[j] = self.weights[j][filterarray]

        return CDataset.dofilter(self,j,filterarray)

    def concat(self,j,input_data,weights = None):
        """Concatenate a few samples to class j.

        Input:
            j: class j
            input_data: an array of samples
            weights: the additional weights of the samples
                if weights is None xor self.weights is None:
                    raise an error
                else:
                    concatenate the new weights to the old weights list
        Output:
            The WeightedCDataset is updated.
        """
        CDataset.concat(self,j,input_data)

        if (weights is None) ^ (self.weights is None):
            raise ValueError('weights and self.weights are not compatible')
        
        if weights is not None:
            self.weights[j] = concatenate([self.weights[j],weights])
            

    def compute_Stats2m(self):
        """Compute moments up to 2nd-order.
        The results are stored as a single Stats2m.

        Output:
            B: a Stats2m representing the obtained statistics
        """
        A = self.input_data
        w = self.weights
        J = self.J
        d = self.isize
        
        B = Stats2m(J,self.ishape)
        B0 = B.get_stat(0)
        B1 = B.get_stat(1)
        B2 = B.get_stat(2)
        for j in xrange(J):
            AA = A[j].reshape(len(A[j]),d)
            if w is None:
                B0[j] = moment0(AA)
                B1[j] = moment1(AA)
                B2[j] = moment2(AA)
            else:
                B0[j] = moment0(AA,w[j])
                B1[j] = moment1(AA,w[j])
                B2[j] = moment2(AA,w[j])
        return B
    
    def _get_masks(self,j):
        """Get the masks for instances of class j if approximation is needed.
        """
        if self.approx <= 0:
            return None
        
        masks = zeros(self.nspc[j],'bool')
        
        if self.weights is None:
            n = int(self.nspc[j]*self.approx)
            tprint("Used approximately "+str(n)+" samples")
            masks[:n] = True
        else:
            tw = self.weights[j].sum()*self.approx
            ind = (-self.weights[j]).argsort(kind='mergesort')
            for i in xrange(self.nspc[j]):
                masks[ind[i]] = True
                tw -= self.weights[j][ind[i]]
                if tw <= 0: break
            tprint("Used approximately "+str(i)+" samples")

        return masks

    def compute_Stats2e(self):
        """Compute per-element moments up to 2nd-order.
        The results are stored as a single Stats2e.

        Output:
            B: a Stats2e representing the obtained statistics
        """
        A = self.input_data
        w = self.weights
        J = self.J
        d = self.isize

        B = Stats2e(J,self.ishape)
        B0 = B.get_stat(0)
        B1 = B.get_stat(1)
        B2 = B.get_stat(2)
        for j in xrange(J):
            AA = A[j].reshape(len(A[j]),d)
            if w is None:
                B0[j], B1[j], B2[j] = statsliste(AA)
            else:
                B0[j], B1[j], B2[j] = statsliste(AA,w[j])
        return B

    def compute_Stats2(self):
        """Compute (total weights, means, and covariances).
        The results are stored as a single Stats2.

        Output:
            B: a Stats2 representing the obtained statistics
        """
        A = self.input_data
        w = self.weights
        J = self.J
        d = self.isize

        B = Stats2(J,self.ishape)
        B0 = B.get_stat(0)
        B1 = B.get_stat(1)
        B2 = B.get_stat(2)
        for j in xrange(J):
            masks = self._get_masks(j)
            AA = A[j].reshape(len(A[j]),d)
            if w is None:
                B0[j] = moment0(AA)
                tmp, B1[j], B2[j] = statslist(AA,None,masks)
            else:
                B0[j] = moment0(AA,w[j])
                tmp, B1[j], B2[j] = statslist(AA,w[j],masks)
        return B


class ScoredWCDataset(WeightedCDataset):
    """A WeightedCDataset with score s[i] attached to the sample i.
    Weight w[i] = exp(s[i]) but to be manually updated by calling update_weights().
    No weight is generated until one explicitly calls update_weights().
    """

    def update_weights(self):
        self.weights = [NP_exp(x) for x in self.scores]

    def __init__(self, separated_input_data, separated_scores = None ):
        WeightedCDataset.__init__(self,separated_input_data)

        if separated_scores is None:
            self.scores = [zeros(x) for x in self.nspc]
        else:
            self.scores = separated_scores
            
        self.weights = None

    def new(self):
        """Make a copy of ScoredWCDataset with a copy of scores and a reference to input_data.
        """
        return ScoredWCDataset(self.input_data,[x.copy() for x in self.scores])


    def set_scores(self,value=0):
        """Set all weight values to 'value', initialize if necessary
        """
        self.scores = [value*ones(x) for x in self.nspc]


    def dofilter(self,j,filterarray):
        """Filter away some samples in class j.

        Input:
            j: class j
            filterarray: a 'bool' numpy.array of size self.nspc[j]
        Output:
            fr: filtering rate, the new number of samples of class j divided
                by the old number of samples of class j.
                The ScoredCDataset is updated.
        """
        self.scores[j] = self.scores[j][filterarray]
        return WeightedCDataset.dofilter(self,j,filterarray)

    def concat(self,j,input_data,scores = None):
        """Concatenate a few samples to class j.

        Input:
            j: class j
            input_data: an array of samples
            scores: the additional scores of the samples
                if scores is None:
                    all scores of new samples are set to 0
                else:
                    concatenate the new scores to the old scores list
        Output:
            The ScoredCDataset is updated.
        """
        WeightedCDataset.concat(self,j,input_data)

        if scores is None:
            self.scores[j] = concatenate([self.scores[j],zeros(len(input_data))])
        else:
            self.scores[j] = concatenate([self.scores[j],scores])


#-------------------------------------------------------------------------------
# Representation of a classification data generator
#-------------------------------------------------------------------------------

class CDataGenerator(DataGenerator):

    def __init__( self, input_shape, J, dtype = 'float64' ):
        DataGenerator.__init__(self,input_shape)
        self.J = J
        self.dtype = dtype

    def generate(self,j):
        """
        Generate a random sample of class j.

        Input:
            j: class j
        Output:
            input_point: a point of class j.
        """
        return zeros( self.input_shape, self.dtype )

    def generate_set(self,j,N):
        """
        Generate a set of random samples of class j.

        Input:
            j: class j
        Output:
            input_data: an array of data points of class j.
        """
        input_data = zeros((N,)+self.input_shape,self.dtype)
        for n in xrange(N):
            input_data[n] = self.generate(j)
        return input_data

