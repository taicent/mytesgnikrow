#-------------------------------------------------------------------------------
# Module:      boosting
# Package:     sd.cs.ml.cla
# Purpose:     Subroutines for boosted classifiers
#
# Author:      minhtri
#
# Created:     26/10/2006
# Copyright:   (c) minhtri 2007
# Licence:     Lesser GPL
#-------------------------------------------------------------------------------
#!/usr/bin/env python

__all__ = ['DiscreteBoostedClassifier','train_DBC','train_AdaBoost','train_VJ','train_PC']

from math import log, exp, sqrt
from numpy import array, where, zeros, ones, dot, prod
from numpy import exp as NP_exp
from numpy import log as NP_log
from numpy import sqrt as NP_sqrt
from cla import BinaryClassifier
from dataset import WeightedCDataset
from nb import train_NBClassifier
from .... import tprint, ordinal

#-------------------------------------------------------------------------------
# Boosted Classifiers
#-------------------------------------------------------------------------------

class DiscreteBoostedClassifier(BinaryClassifier): # binary classifier
    """Discrete Boosted Classifier of this form:
        F_M(x) = \sum_{m=1}^M c_m f_m(x),
        where
            c_m: voting weight
            f_m(x): a binary classifier outputing {-1,1}
    """
    
    def __init__(self,M,weaks,c,k,balancing,polaritybalancing = 1):
        """
        Initialize the DiscreteBoostedClassifier with some parameters.

        Input:
            M: number of weak classifiers
            weaks: list of weak classifiers trained
            c: array of M voting coefficients for M the weak classifiers respectively
            k: false negatives penalized k times more than false positives
            balancing: type of balancing among weak classifiers
                0 = no balancing at all, this is the original AdaBoost's method
                1 = asymmetric weight balancing, Viola-Jones (NIPS'02)
                2 = skewness balancing, Pham-Cham (CVPR'07)
            polaritybalancing: use polarity balancing?
                0 = no polarity balancing, same as Oza-Rusell (ICSMC'05)
                1 = polarity balancing, Pham-Cham (CVPR'07)
        """
        BinaryClassifier.__init__(self)
        self.M = len(weaks)
        self.weaks = weaks
        self.c = c
        self.k = k
        self.balancing = balancing
        self.polaritybalancing = polaritybalancing
        self.tw = 0 # total weights
        if polaritybalancing == 1: # Pham-Cham (CVPR'07 oral)
            self.g = zeros(M) # gamma
            self.v = ones((M,2,2)) # Pham-Cham's v
        elif polaritybalancing == 0: # Oza-Rusell (ICSMC'05)
            self.v = ones((M,2)) # Oza-Rusell's lambda
        else:
            raise IndexError('The value of polaritybalancing is out of bound.')
        
    def confidence(self,input_point):
        p = array([w.predict(input_point) for w in self.weaks])
        val = where(p,self.c,-self.c).sum()
        return val

    def predict(self,input_point):
        return int(self.confidence(input_point) >= 0)

    def can_learn(self):
        for weak in self.weaks:
            if not weak.can_learn():
                return False
        return True

    def learn( self, input_point, j, weight = None):
        """Learn incrementally with a new input point, its class, and optionally its weight.

        Other parameters like k and balancing are derived from the class itself.

        TODO: Re-test this function extensively.

        Input:
            input_point: a new input point
            j: its corresponding class
            w: optionally its weight, or 1 if not specified
            polaritybalancing: use polarity balancing?
                0 = no polarity balancing, same as Oza-Rusell (ICSMC'05)
                1 = polarity balancing, Pham-Cham (CVPR'07)
        """
        M = self.M
        a = zeros(M)
        e = zeros(M)
        kk = zeros(M)
        if self.balancing == 1:
            k2 = exp(log(self.k)/self.M)
        elif self.balancing == 2:
            logk = log(self.k)

        if weight is None:
            v = 1.0
        else:
            v = weight
        self.tw += v
            
        for m in xrange(self.M):
            # compute k_m
            if self.balancing == 0:
                if m == 0:
                    kk[m] = self.k
                else:
                    kk[m] = 1.0
            elif self.balancing == 1:
                kk[m] = k2
            elif self.balancing == 2:
                log_k_m = (logk-NP_log(kk[:m]).sum() + (M-m-1)*self.g[m]) / (M-m)
                kk[m] = exp(log_k_m)

            # update weights to deal with k_m
            k3 = sqrt(kk[m])
            if j != 0: # positive
                w = v*k3
            else:
                w = v/k3

            # update weak classifier
            self.weaks[m].learn(input_point,j,w)

            # rerun
            j2 = self.weaks[m].predict(input_point)

            # propagate the weights
            if self.polaritybalancing == 1: # Pham-Cham
                self.v[m,j,j2] += v
                a[m] = self.v[m,1].sum() / self.v[m].sum()
                if j2 > 0: # predicted as positive
                    v *= a[m] * 0.5 * self.tw / self.v[m,j,j2]
                else: # predicted as negative
                    v *= (1-a[m]) * 0.5 * self.tw / self.v[m,j,j2]
            else: # Oza-Rusell
                j3 = int(j == j2)
                self.v[m,j3] += v
                v *= 0.5 * self.tw / self.v[m,j3]

            # update other parameters
            if self.polaritybalancing == 1: # Pham-Cham
                self.g[m] = log((1-a[m])/a[m])
                e[m] = (self.v[m,0,1]/k3 + self.v[m,1,0]*k3) / \
                    ((self.v[m,0].sum())/k3 + (self.v[m,1].sum())*k3)
            else: # Oza-Rusell
                e[m] = self.v[m,0] / self.v[m].sum()
            self.c[m] = 0.5 * log((1-e[m])/e[m])


def train_DBC( classification_dataset, trainfunc, M, k = 1.0, balancing = 2, polaritybalancing = 1 ):
    """Train a DiscreteBoostedClassifier

    Input:
        classification_dataset: a WeightedCDataset of 2 classes
        trainfunc: a function that takes a WeightedCDataset as input
            and returns a BinaryClassifier as a weak classifier
        M: the maximum number of weak classifier
        k: false negatives penalized k times more than false positives
        balancing: type of balancing among weak classifiers
            0 = no balancing at all, this is the original AdaBoost's method
            1 = asymmetric weight balancing, Viola-Jones (NIPS'02)
            2 = skewness balancing, Pham-Cham (CVPR'07)
        polaritybalancing: use polarity balancing for online-learning?
            0 = no polarity balancing, same as Oza-Rusell (ICSMC'05)
            1 = polarity balancing, Pham-Cham (CVPR'07)
    Output:
        a DiscreteBoostedClassifier
    """
    cd = classification_dataset.new()
    cd.initialize_weights()

    if balancing == 0:
        cd.weights[1] *= k
    elif balancing == 1:
        kk = exp(log(k)/M)
    elif balancing == 2:
        logk = log(k)
    else:
        raise IndexError('balancing value is out of bound.')

    weaks = []
    cs = []
    tprint("Training a boosted classifier with at most "+str(M)+" weak classifiers...")
    for m in xrange(M):
        if balancing == 1:
            cd.weights[1] *= kk
        elif balancing == 2:
            prior = cd.get_twpc()
            gamma = cd.get_skewness()

            # tprint("Before applying k_m:")
            # tprint("m="+str(m))
            # tprint("log k="+str(logk))
            # tprint("prior="+str(prior))
            # tprint("gamma="+str(gamma))

            # compute k_m
            log_k_m = (logk + (M-m-1)*gamma) / (M-m)
            k_m = exp(log_k_m)
            # tprint("log k_m="+str(log_k_m))
            # tprint("k_m="+str(k_m))

            # update weights to deal with k_m
            cd.weights[1] *= k_m

        cd.normalize_weights()

        if balancing == 2:
            # tprint("After applying k_m:")
            prior = cd.get_twpc()
            gamma = cd.get_skewness()
            # tprint("prior="+str(prior))
            # tprint("gamma="+str(gamma))

        # train the new weak classifier
        tprint("Training the "+ordinal(m+1)+" weak classifier...")
        f = trainfunc(cd)

        err = [f.test(cd.input_data[j]) != j for j in xrange(2)]
        werr = sum([where(err[j],cd.weights[j],0).sum() for j in xrange(2)])

        if werr < 1e-20: # no single error? too good to be true
            weaks = [f]
            cs = [1]
            break

        c = 0.5 * log((1-werr)/werr)

        # append to the boost
        weaks.append(f)
        cs.append(c)

        # update the weights
        for j in xrange(2): cd.weights[j] *= where(err[j],exp(c),exp(-c))

        if balancing == 2:
            logk -= log_k_m

    return DiscreteBoostedClassifier(M,weaks,array(cs),k,balancing,polaritybalancing)

def train_AdaBoost( classification_dataset, trainfunc, M, polaritybalancing = 1 ):
    """Train a DiscreteBoostedClassifier using AdaBoost (Friend et al's DiscreteAdaboost)
    
    Warning:
        This function is now obsolete, use train_DBC() instead.

    Input:
        classification_dataset: a WeightedCDataset of 2 classes
        trainfunc: a function that takes a WeightedCDataset as input
            and returns a BinaryClassifier
        M: the maximum number of stages
        polaritybalancing: use polarity balancing for online-learning?
            0 = no polarity balancing, same as Oza-Rusell (ICSMC'05)
            1 = polarity balancing, Pham-Cham (CVPR'07)
    Output:
        a DiscreteBoostedClassifier
    """
    cd = classification_dataset.new()
    cd.initialize_weights()

    weaks = []
    cs = []
    for m in xrange(M):
        # train the new weak classifier
        tprint("Training the "+ordinal(m+1)+" weak classifier.")
        f = trainfunc(cd)

        err = [f.test(cd.input_data[j]) != j for j in xrange(2)]
        werr = sum([where(err[j],cd.weights[j],0).sum() for j in xrange(2)])

        if werr < 1e-20: # no single error? too good to be true
            weaks = [f]
            cs = [1]
            break

        c = 0.5 * log((1-werr)/werr)

        # append to the boost
        weaks.append(f)
        cs.append(c)

        # update the weights, then normalize
        for j in xrange(2): cd.weights[j] *= where(err[j],exp(c),exp(-c))
        cd.normalize_weights()

    cs = array(cs)
    return DiscreteBoostedClassifier(M,weaks,cs,1.0,0,polaritybalancing)


def train_VJ( classification_dataset, trainfunc, M, k, evenly = True, polaritybalancing = 1 ):
    """Train a DiscreteBoostedClassifier using Viola and Jones'
        asymmetric boost (NIPS'02)

    Warning:
        This function is now obsolete, use train_DBC() instead.

    Input:
        classification_dataset: a WeightedCDataset of 2 classes
        trainfunc: a function that takes a WeightedCDataset as input
            and returns a BinaryClassifier
        M: the maximum number of stages
        k: false negatives penalized k times more than false positives
        evenly: distribute lambda evenly among the weak classifiers
        polaritybalancing: use polarity balancing for online-learning?
            0 = no polarity balancing, same as Oza-Rusell (ICSMC'05)
            1 = polarity balancing, Pham-Cham (CVPR'07)
    Output:
        a DiscreteBoostedClassifier
    """
    cd = classification_dataset.new()
    cd.initialize_weights()

    if evenly == True:
        kk = exp(log(k)/M)
    else:
        cd.weights[1] *= k

    weaks = []
    cs = []
    tprint("Training a boosted classifier with at most "+str(M)+" weak classifiers...")
    for m in xrange(M):
        if evenly == True: cd.weights[1] *= kk
        cd.normalize_weights()

        # train the new weak classifier
        tprint("Training the "+ordinal(m+1)+" weak classifier...")
        f = trainfunc(cd)

        err = [f.test(cd.input_data[j]) != j for j in xrange(2)]
        werr = sum([where(err[j],cd.weights[j],0).sum() for j in xrange(2)])
        
        if werr < 1e-20: # no single error? too good to be true
            weaks = [f]
            cs = [1]
            break

        c = 0.5 * log((1-werr)/werr)

        # append to the boost
        weaks.append(f)
        cs.append(c)

        # update the weights
        for j in xrange(2): cd.weights[j] *= where(err[j],exp(c),exp(-c))

    cs = array(cs)
    return DiscreteBoostedClassifier(M,weaks,cs,k,1,polaritybalancing)


def train_PC( classification_dataset, trainfunc, M, k, polaritybalancing = 1 ):
    """Train a DiscreteBoostedClassifier using our (Pham and Cham's)
        asymmetric boost (CVPR'07)

    Warning:
        This function is now obsolete, use train_DBC() instead.

    Input:
        classification_dataset: a WeightedCDataset of 2 classes
        trainfunc: a function that takes a WeightedCDataset as input
            and returns a BinaryClassifier
        M: the maximum number of stages
        k: false negatives penalized k times more than false positives
        polaritybalancing: use polarity balancing for online-learning?
            0 = no polarity balancing, same as Oza-Rusell (ICSMC'05)
            1 = polarity balancing, Pham-Cham (CVPR'07)
    Output:
        a DiscreteBoostedClassifier
    """
    cd = classification_dataset.new()
    cd.initialize_weights()

    logk = log(k)

    weaks = []
    cs = []
    for m in xrange(M):
        # train the new weak classifier
        tprint("Training the "+ordinal(m+1)+" weak classifier.")

        prior = cd.get_twpc()
        gamma = cd.get_skewness()

        # tprint("Before applying k_m:")
        # tprint("m="+str(m))
        # tprint("log k="+str(logk))
        # tprint("prior="+str(prior))
        # tprint("gamma="+str(gamma))

        # compute k_m
        log_k_m = (logk + (M-m-1)*gamma) / (M-m)
        k_m = exp(log_k_m)
        # tprint("log k_m="+str(log_k_m))
        # tprint("k_m="+str(k_m))

        # update weights to deal with k_m
        cd.weights[1] *= k_m

        # tprint("After applying k_m:")
        cd.normalize_weights()
        prior = cd.get_twpc()
        gamma = cd.get_skewness()
        # tprint("prior="+str(prior))
        # tprint("gamma="+str(gamma))

        f = trainfunc(cd)

        err = [f.test(cd.input_data[j]) != j for j in xrange(2)]
        werr = sum([where(err[j],cd.weights[j],0).sum() for j in xrange(2)])

        if werr < 1e-20: # no single error? too good to be true
            weaks = [f]
            cs = [1]
            break

        c = 0.5 * log((1-werr)/werr)

        # append to the boost
        weaks.append(f)
        cs.append(c)

        # update the weights, then normalize
        for j in xrange(2): cd.weights[j] *= where(err[j],exp(c),exp(-c))
        cd.normalize_weights()
        logk -= log_k_m

    cs = array(cs)
    return DiscreteBoostedClassifier(M,weaks,cs,k,2,polaritybalancing)


##def train_PC_NIPS( classification_dataset, trainfunc, M, k  ):
##    """Train a DiscreteBoostedClassifier using our (Pham and Cham's)
##        asymmetric boost (NIPS'07)
##
##    Goal:
##        Assume D^+ is the distribution of x given x positive.
##        Assume D^- is the distribution of x given x negative.
##        We wish to find F_M(x) to minimize:
##            k J^-(F_M) + J^+(F_M) (1)
##        where
##            J^+(F_M) = E_{D^+} [(F_M(x)-1)^2]
##            J^-(F_M) = E_{D^-} [(F_M(x)+1)^2]
##
##        Let:
##            pi^+_m = 1 - E_{D^+} [F_M(x)]
##            pi^-_m = 1 - E_{D^-} [F_M(x)]
##            D^+_m a distribution such that p_{D^+_m}(x) = p_{D^+}(x) (1-F_m(x)) / pi^+_m
##            D^-_m a distribution such that p_{D^-_m}(x) = p_{D^-}(x) (1+F_m(x)) / pi^-_m
##            FRR_m(f) = E_{D^+_m}[f(x) == -1]
##            FAR_m(f) = E_{D^-_m}[f(x) == +1]
##
##        I proved that:
##            J^+(F_M) = J^+(F_m) + c^2 + 2 c pi^+_m (2FRR_m(f) - 1)
##            J^-(F_M) = J^-(F_m) + c^2 + 2 c pi^-_m (2FAR_m(f) - 1)
##
##        Let's say we want to minimize (1) then we need to
##            1) choose f minimizing: (weak classifier)
##                epsilon(f) = k pi^-_m FRR_m(f) + k pi^+_m FAR_m(f)
##            2) choose  minimizing: (voting coefficient)
##                (k J^- + J^+)(F_m) + (k+1) c^2 + 4c epsilon(f) - 2c(k pi^-_m + pi^+_m)
##                which means:
##                    c* = \frac{k pi^-_m + pi^+_m - 2 epsilon(f)} {k+1}
##
##        I also proved that: (not quite)
##            |F_M(x)| <= \sum_{m=1}^M |c_m| <= 1 for all M
##
##
##    Input:
##        classification_dataset: a WeightedCDataset of 2 classes
##        trainfunc: a function that takes a WeightedCDataset as input
##            and returns a BinaryClassifier
##        M: the maximum number of stages
##        k: false negative rate penalized k times more than false positive rate
##    Output:
##        a DiscreteBoostedClassifier
##    """
##    pass



##class RealAdaboostClassifier(BinaryClassifier): # binary classifier
##
##    def __init__(self,maxM,weak_probabilistic_binary_classifier):
##        classifier.__init__(self,2)
##        self.maxM = maxM
##        self.apclassifier = weak_probabilistic_binary_classifier
##
##    def train( self, input_data, weights = None, *args ):
##        BinaryClassifier.train( self, input_data, weights, *args )
##        N = map(len,input_data)
##        if weights is None:
##            w = [ones(n)/sum(N) for n in N]
##        else:
##            w = weights.copy()
##
##        self.weaks = []
##        self.c = []
##        for m in xrange(maxM):
##            # train the new weak classifier
##            weak = copy(self.apclassifier)
##            weak.train(input_data,w,*args)
##
##            # append to the boost
##            self.weaks.append(weak)
##
##            # update the weights
##            for j in xrange(2):
##                z = weak.test_pdf(input_data[j])
##                w[j] *= NP_sqrt(z[1-j]/z[j])
##
##    def predict(self,input_point):
##        val = 0
##        for w in self.weaks:
##            pdf = w.predict_pdf(input_point)
##            val += 0.5 * log(pdf[1]/pdf[0])
##        return int(val >= 0)
##

def main():
    # main idea is for testing
    a = [array([[0,1],[1,1],[1,0]]),array([[0,2],[1,2],[2,2],[2,1],[2,0]])]
    b = WeightedCDataset(a)
    c = train_PC(b,train_NBClassifier,10,10000)
    tprint(c.test(a[0]))
    tprint(c.test(a[1]))

if __name__ == '__main__':
    main()
