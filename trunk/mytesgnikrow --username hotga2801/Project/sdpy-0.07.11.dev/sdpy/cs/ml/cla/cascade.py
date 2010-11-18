#-------------------------------------------------------------------------------
# Module:      cascade
# Package:     sd.cs.ml.cla
# Purpose:     Subroutines for cascades
#
# Author:      minhtri
#
# Created:     05/02/2007
# Copyright:   (c) minhtri 2007
# Licence:     Lesser GPL
#-------------------------------------------------------------------------------
#!/usr/bin/env python

__all__ = ['SimpleCascade','train_SC','DirectFilteringCascade','train_DFC',
    'GeneralizedCascade']

from math import log, exp, sqrt
from numpy import array, where, zeros, ones, dot, prod, concatenate
from numpy import exp as NP_exp
from numpy import sqrt as NP_sqrt
from cla import *
from dataset import WeightedCDataset
from boosting import train_PC
from nb import *
from .... import tprint


#-------------------------------------------------------------------------------
# A simple cascade of binary classifiers. The cascade predicts true iff all
# binary classifiers predict true
#-------------------------------------------------------------------------------

class SimpleCascade(BinaryClassifier):
    """A cascade of binary classifiers. The cascade predicts true iff all binary
    classifiers predict true.
    """

    def __init__(self,*binary_classifiers):
        BinaryClassifier.__init__(self)
        self.bclassifiers = list(binary_classifiers)

    def add_binary_classifier(self,binary_classifier):
        self.bclassifiers.append(binary_classifier)

    def predict(self,input_point):
        for x in self.bclassifiers:
            if x.predict(input_point) == 0:
                return 0
        return 1


def train_SC( cd, trainfunc, maxM, maxFR = 0.9 ):
    """Train a cascade of at most maxM filters (binary classifiers).

    Input:
        cd: a WeightedCDataset -- modifiable
        trainfunc: a function that takes a WeightedCDataset as input
            and returns a BinaryClassifier
        maxM: the maximum number of filters to be trained
        maxFR: if the filtering rate (the number of negatives after training
            divided by the number of negatives before training) rises above
            'maxFR' then stop
    Output:
        cascade: a Cascade of filters
        new_classification_dataset: a new WeightedCDataset filtered by the
            cascade, useful for subsequent training
    """
    cascade = Cascade()
    tprint("Training the cascade...")
    for i in xrange(maxM):
        tprint("Training the "+str(i+1)+"th filter with instances="+str(cd.nspc))
        bc = trainfunc(cd)
        cascade.add_binary_classifier(bc)

        testres = bc.test(cd.input_data[0]).astype('bool')
        FR = cd.dofilter(0,testres)
        tprint("Filtering rate="+str(FR))
        if FR > maxFR:
            break
    tprint("Instances after training the cascade="+str(cd.nspc))
    return (cascade, cd)


#-------------------------------------------------------------------------------
# Cascade with scores, abstract class
#-------------------------------------------------------------------------------
class ScoredCascade(BinaryClassifier):
    def __init__(self):
        BinaryClassifier.__init__(self)

    def score(self,input_point):
        """Compute the score of an input point.

        Input:
            input_point: an input point
        Output:
            z: (boolean) whether it passes the cascade
            y: predicted class
            s: the score value
        """
        pass

    def scores(self,input_data):
        """An array version of score(). Take an array of input points and produce their scores.

        Input:
            input_data: an array of input points
        Output:
            z: a boolean array telling whether the points pass the cascade
            y: an array of predicted classes
            s: an array of score values
        """
        N = len(input_data)
        z = zeros(N,'bool')
        y = zeros(N,'int')
        s = zeros(N,'double')
        for i in xrange(N):
            z[i], y[i], s[i] = self.score(input_data[i])
        return z, y, s

    def generate_negatives(self,nsamples,cdgenerator):
        """Generate 'nsamples' negative samples that pass the cascade and have scores >= 0.
        May take time if samples are getting more difficult to pass the cascade.

        Input:
            nsamples: number of negative samples to be generated
            cdgenerator: a CDGenerator that can generate negative samples
        Output:
            train_neg: a numpy.array of newly generated samples
            scores_neg: an array of scores
        """
        train_neg = zeros((nsamples,)+cdgenerator.input_shape,cdgenerator.dtype)
        scores_neg = zeros(nsamples)

        for n in xrange(nsamples):
            while True:
                ipoint = cdgenerator.generate(0)
                (z,y,s) = self.score(ipoint)
                if z and s > 0: break
            train_neg[n] = ipoint
            scores_neg[n] = s

        return (train_neg,scores_neg)

    def predict(self,input_point):
        (z,y,s) = self.score(input_point)
        return y



#-------------------------------------------------------------------------------
# A cascade of binary classifiers that propagates the difficulty over the
# binary classifiers.
#-------------------------------------------------------------------------------

class DirectFilteringCascade(ScoredCascade):
    """
    A cascaded rejector of M*N weak rejectors f_1(x), ..., f_{M*N}(x).

    At the m-th stage, let:
        F_M(x) = \sum_{m=1}^M c_m f_m(x),
        where
            c_m: voting weight
            f_m(x): a binary classifier outputing {-1,1}

    At any stage m \in {N, 2*N, ..., M*N}, if F_m(x) < 0 then classify as 0.
    Else classify as 1.
    """

    def __init__(self,M,N,weaks,c):
        ScoredCascade.__init__(self)
        self.M = M
        self.N = N
        self.weaks = weaks
        self.c = c

    def score(self,input_point):
        c0 = 0
        for m in xrange(M*N):
            if self.weaks[m].predict(input_point) > 0:
                c0 += self.c[m]
            else:
                c0 -= self.c[m]
                if m%N == 0 and c0 < 0: return (False,0,c0)
        return (True,1,c0)


def train_DFC( cd, cdgenerator, initfunc, trainfunc, M, N, alpha  ):
    """
    Train a DirectFilteringCascade using my method in my notebook dated 2 May 2007.

    Goal: to train a cascade such that at decisive stages, FAR is minimized
        and FRR is upper-bounded by alpha.
    Input:
        cd: a ScoredWCDataset of 2 classes -- modifiable
        cdgenerator: a CDataGenerator for the 'cd'
        initfunc(cd): a function that takes WeightedCDataset 'cd',
            initializes some data, and returns 'data'
        trainfunc(data,minDR): a function that takes initalized 'data'
            and constant 'minDR' as input and returns a BinaryClassifier. The
            function must be able to train to have at least 'minDR' detection
            rate.
        M: the maximum number of stages
        N: the number of weak classifiers per stage
        alpha: the maximum value of an exponential-based upper bound of FRR,
            0 <= alpha <= 1
    Output:
        a DirectFilteringCascade
    """
    cascade = None

    weaks = []
    cs = []
    tprint("Training a cascaded rejector with at most "+str(M)+" weak rejectors...")
    for m in xrange(M):
        # train the new weak classifier
        tprint("Training the "+str(m+1)+"th stage...")
        
        for n in xrange(N):
            cd.update_weights()
            data = initfunc(cd)
            w0 = cd.weights[0]

            # start with a guess of c (setting maxFRR to 1)
            c = -log(alpha)+0.00001
            bcrit = 10000000

            while True:
                # given c, train weak classifier f(z) where
                # z = maxFRR
                # f(z)= maxFAR(maxFRR)
                maxFRR = (alpha - exp(-c)) / (exp(c) - exp(-c))
                f = trainfunc( data, 1-maxFRR )

                # estimate maxFAR
                err0 = f.test(cd.input_data[0])
                maxFAR = w0[err0.astype('bool')].sum() / w0.sum()

                # compare with the current
                crit = exp(-c)*(1-maxFAR) + exp(c)*maxFAR
                if crit*1.005 < bcrit:
                    bcrit = crit
                    bc = c
                    bmaxFRR = maxFRR
                    bmaxFAR = maxFAR
                    bf = f
                else:
                    break

                # given maxFRR, maxFAR, train c
                delta = alpha*alpha - 4*bmaxFRR*(1-bmaxFRR)
                if delta < 0:
                    break
                c1 = log(0.5*(alpha-sqrt(delta))/maxFRR)
                if c1 < 0: c1 = 0
                c2 = log(0.5*(alpha+sqrt(delta))/maxFRR)
                if c1 > c2:
                    break

                c = 0.5 * log((1-maxFAR)/maxFAR)
                if c < c1: c = c1
                if c > c2: c = c2


            # append to the boost
            weaks.append(bf)
            cs.append(bc)

            tprint("Weak filter: (c,maxFRR,maxFAR)=("+str(bc)+","+str(bmaxFRR)+
                ","+str(maxFAR)+")")

            # estimate errors
            err = [bf.test(cd.input_data[j]) != j for j in xrange(2)]

            # update the scores
            for j in xrange(2): cd.scores[j] += err[j]*(2*bc)-bc

            # measure errors
            FAR = (cd.scores[0] >= 0).sum() / cd.nspc[0]
            FRR = (cd.scores[1] <  0).sum() / cd.nspc[1]
            tprint("Measured errors: (FRR,FAR)=("+str(FRR)+","+str(FAR)+")")

        # filter negatively rejected data
        tprint("Filtering training samples:")
        for j in xrange(2):
            ar = cd.dofilter(j,cd.scores[j] >= 0)
            tprint("  Acceptance rate for class "+str(j)+"="+str(ar))

        # build up the current cascade
        cascade = DirectFilteringCascade(m,N,weaks,array(cs))

        # generate new negative samples
        if cd.nspc[0] < cd.nspc[1]:
            Nnew = cd.nspc[1]-cd.nspc[0]
            tprint("needs "+str(Nnew)+" more nonfaces")
            (tn, sn) = cascade.generate_negatives(Nnew,cdgenerator)
            cd.concat(0,tn,sn)

        tprint("nspc="+str(cd.nspc))

    return cascade


class GeneralizedCascade(ScoredCascade):
    """A cascade of M weak binary classifiers. Each classifier f_m(x) \in
    {-1,+1} is associated with an score c_m, and a boolean value z_m which
    tells whether the weak classifier is 'an improver' (z_m = 1) or 'a filter'
    (z_m = 0) (to be explained below). Define the total scores of point x
    up to stage m:
        F_m(x) = \sum{i=1}^m z_i c_i f_i(x)
    An improver at stage m does not classify x, but improves F_m(x)
    to reach a certain goal (e.g. to minimize the total classification error up
    to stage m). A filter, on the other hand, classifies x as negative
    immediately iff:
        G_m(x) = F_{m-1}(x) + c_i f_i(x) < 0
    Or pass x to the next stage. Finally, the cascade classifies x as
    positive iff F_M(x) >= 0, and negative otherwise.
    """
    def __init__(self):
        ScoredCascade.__init__(self)
        self.weaks = [BinaryClassifier()]
        self.c = array([-1],'double')
        self.z = ones(1,'bool')

    def add_binary_classifier(self,binary_classifier,c,is_improver):
        self.weaks.append(binary_classifier)
        self.c = concatenate([self.c,[c]])
        self.z = concatenate([self.z,[is_improver]])

    def score(self,input_point):
        s = 0
        for m in xrange(len(self.weaks)):
            f_m = self.weaks[m].predict(input_point)
            s2 = s + self.c[m] * (f_m*2 - 1)
            if not self.z[m]: # filter
                if s2 < 0:
                    return (False,0,s2)
            else:
                s = s2
        return (True,s >= 0,s)

def main():
    # main idea is for testing
    a = [array([[0,1],[1,1],[1,0]]),array([[0,2],[1,2],[2,2],[2,1],[2,0]])]
    b = WeightedCDataset(a)
    c = train_PC(b,train_NBClassifier,10,10000)
    tprint(c.test(a[0]))
    tprint(c.test(a[1]))

if __name__ == '__main__':
    main()
