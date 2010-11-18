#-------------------------------------------------------------------------------
# Module:      nb
# Package:     sd.cs.ml.cla
# Purpose:     Subroutines for Naive Bayes classification methods
#
# Author:      minhtri
#
# Created:     26/03/2007
# Copyright:   (c) minhtri 2007
# Licence:     Lesser GPL
#-------------------------------------------------------------------------------
#!/usr/bin/env python

__all__ = ['NBClassifier','train_NBClassifier']

from copy import copy
from math import fabs, log, exp
from numpy import array, where, zeros, ones, dot, prod
from numpy import log as NP_log
from numpy import exp as NP_exp
from numpy import sqrt as NP_sqrt
from scipy.weave import inline
from cla import PClassifier
from dataset import WeightedCDataset
from .... import tprint

#-------------------------------------------------------------------------------
# Naive Bayes Classifier
#-------------------------------------------------------------------------------

class NBClassifier(PClassifier): # Naive Bayes classifier

    def __init__(self,nclasses,stats2e):
        PClassifier.__init__(self,nclasses)
        self.stats2e = stats2e

    def predict_pdf(self,input_point):
        ip = input_point
        pdf = zeros(self.nclasses)

        #gstat = self.stats2e.get_cond_stat
        # for j in xrange(self.nclasses):
            # pdf[j] = NP_log(gstat(j,0).squeeze())
            # pdf[j] -= 0.5 * (((ip - gstat(j,1)) / gstat(j,2))**2).ravel().sum()
            # pdf[j] -= NP_log(gstat(j,2)).sum()
        # pdf = NP_exp(pdf)
        #return pdf/pdf.sum()
        
        A = self.stats2e.A
        d = len(ip)
        J = self.nclasses
        code = """
            int i, j, sz = 1+d*2;
            double x, v, *AA;
            
            for( j = 0; j < J; ++j )
            {
                AA = &A[j*sz];
                
                pdf[j] = log(AA[0]);
                for( i = 0; i < d; ++i )
                {
                    x = AA[i+d+1];
                    v = (ip[i] - AA[i+1])/x;
                    pdf[j] -= 0.5*v*v + log(x);
                }
                pdf[j] = exp(pdf[j]);
            }
            
            // normalize
            x = 0;
            for( j = 0; j < J; ++j ) x += pdf[j];
            for( j = 0; j < J; ++j ) pdf[j] /= x;
            
            return_val = 1;
        """
        inline(code,['ip','pdf','A','d','J'])
        return pdf
        
    def can_learn(self):
        return True

    def learn(self,input_point,j,w = None):
        """Learn incrementally with a new input point, its class, and optionally its weight.

        Input:
            input_point: a new input point
            j: its corresponding class
            w: optionally its weight, or 1 if not specified
        """
        self.stats2e.learn(input_point,j,w)


def train_NBClassifier( cd ):
    """Train a NBClassifier using a WeightedCDataset

    Input:
        classification_dataset: a WeightedCDataset
    Output:
        an NBClassifier
    """
    return NBClassifier(cd.J,cd.compute_Stats2e())
