#-------------------------------------------------------------------------------
# Module:      lda
# Package:     sd.cs.ml.cla
# Purpose:     Subroutines to deal with LDA projection and LDA classifiers
#
# Author:      minhtri
#
# Created:     26/03/2007
# Copyright:   (c) minhtri 2007
# Licence:     Lesser GPL
#-------------------------------------------------------------------------------
#!/usr/bin/env python

__all__ = ['project_LDA','train_LDA']

from math import fabs, sqrt
from numpy import dot
from numpy import log as NP_log
from scipy.linalg import norm as l_norm
from cla import *
from dataset import WeightedCDataset
from ...stats.gaussian import find_classification_threshold, find_filtering_threshold, find_filtering_threshold2

#-------------------------------------------------------------------------------
# Sub-routines dealing with Fisher Linear Discriminant Analysis (LDA)
#-------------------------------------------------------------------------------

def project_LDA(stats2):
    """Project down to a line using the LDA projection. Return the direction.

    Input:
        stats2: a Stats2 of J classes
    Output:
        w: the projection direction (vector) -- if input is a tensor, flatten it into a vector
    """
    # Get some common information
    nclasses = stats2.J
    theshape = stats2.shape
    thedim = stats2.d
    ww = stats2.get_stat(0)
    themean = stats2.get_stat(1).reshape(nclasses,d)
    thecov = stats2.get_stat(2).reshape(nclasses,d,d)

    # Compute the within scatter matrix
    Sw = weighted(ww,thecov)

    # Find the best projection
    if nclasses == 2:
        w = dot(pinv(Sw), themean[1] - themean[0])
    else:
        # Compute the covariance matrix of the means
        # (between scatter matrix divided by total weight)
        b0, b1, Sb = statslist(themean,ww)

        # w is the eigenvector of the largest eigenvalue of the (Sw^{-1} Sb) matrix
        eigval, eigvec = eig(Sb,Sw)
        w = eigvec[:,eigval.real.argmax()]

    # Normalize
    lw = l_norm(w)
    if lw < 0.00000000001: # same means
        w[0] = 1
    else:
        w /= lw

    return w


def train_LDA(classification_dataset,crit,param1):
    """Take a 2-class WeightedCDataset, then train a TLinearClassifier.
    The projection direction is LDA. The threshold is trained using one of three criteria:
        crit = 0: param1 is 'thelambda', then call gaussian.find_classification_threshold()
        crit = 1: param1 is 'minDR', then call gaussian.find_filtering_threshold()
        crit = 2: param1 is 'maxFAR', then call gaussian.find_filtering_threshold2()
        
    Input:
        classification_dataset: a 2-class WeightedCDataset
        crit, param1: as mentioned above
    Output:
        lc: a LinearClassifier, with lc.err as the estimated 'error'
    """
    stats2 = classification_dataset.compute_Stats2()
    w = project_LDA(stats2)
    stats2e = stats2.project(w)
    
    if stats2e.A[1,1] < stats2e.A[0,1]: # needs refining
        stats2e.A[:,1] = - stats2e.A[:,1]
        w = -w
    
    if crit == 0:
        b = find_classification_threshold(stats2e,param1)
    elif crit == 1:
        b = find_filtering_threshold(stats2e,param1)
    else:
        b = find_filtering_threshold2(stats2e,param1)
        
    lc = TLinearClassifier(w,b[0,0])
    lc.err = b[0,1]
    return lc
