#-------------------------------------------------------------------------------
# Module:      thresh1d
# Package:     sd.cs.ml.cla
# Purpose:     Subroutines for classification methods with 1d input and thresholds
#
# Author:      minhtri
#
# Created:     15/09/2007
# Copyright:   (c) minhtri 2007
# Licence:     Lesser GPL
#-------------------------------------------------------------------------------
#!/usr/bin/env python

__all__ = ['UAC','thresh_1d','thresh_normal_1d','sort_1d','histogram_1d']

from math import ceil
from scipy.weave import inline
from numpy import zeros, arange
from .. import Predictor, Dataset
from ...dm import argsort, argsort_and_cut

    
#------------------------------------------------------------------------------------------    

def _sdSolveUAC(s,sid,y,nspc,x,bid,B,minF0,bnd2,typ):
    bparam = zeros(2)
    bsol = zeros(2,'int')
    
    code = """
        sdSolveUAC(
            Ns[0], s,
            Nsid[0], (int *)sid,
            Ny[0], (int *)y,
            Nnspc[0], (int *)nspc,
            Nx[0], x,
            Nbid[0], (int *)bid,
            NB[0], B,
            NminF0[0], (int *)minF0,
            Nbnd2[0], (int *)bnd2,
            (int)typ,
            Nbparam[0], bparam,
            Nbsol[0], (int *)bsol
        );
    """
    inline(code, arg_names=['s','sid','y','nspc','x','bid','B','minF0',
        'bnd2','typ','bparam','bsol'], headers=['"uac.h"'], \
        sources=['uac.cpp'], libraries=['sdcpp', 'cblas_ext', 'cephesd', 'plat_det'],
        include_dirs=['.']
        )
    
    return bparam, bsol
    

class UAC:
    """Univariate Additive Classifier

    A classifier of the form:
    	Classify x as class y = sgn( s(x) + c sgn(x-b) )
    where s(x) is a score function, (c,b) are unknown parameters.
    Suppose there are N training samples (x_n, y_n, s(x_n)).
    Let:
    	+ B(b) = (sgn(x_1-b), sgn(x_2-b), ..., sgn(x_N-b))
    	+ F(c,b) = (sgn(s(x_1) + c sgn(x_1-b)), ..., sgn(s(x_N) + c sgn(x_N-b)))
    When training with N samples (x_n, y_n, F(x_n)), it is provable that
    	+ R can be partitioned into at most N+1 intervals such that in any
    	  interval, B(b) is fixed for different values of b
    	+ Given b is known, R can be partitioned into at most N+1 intervals such
    	  that in any interval, F(c,b) is fixed for different values of c
    	+ Given b is fixed but unknown, R can be partitioned into at most 2N+1
    	  intervals such that in any interval, F(c,b) is fixed for different
    	  values of c
    s(x) must not be 0 for all x_n
    """
    def __init__(self,s,y):
        """Initialize the class.

        Input:
            s, y: see the class' docstring.
        """
        if len(s) != len(y):
            raise ValueError("Size of s is not equal size of y")
        self.N = len(s)

        self.s = s
        self.y = y

        # find sid
        self.sid = argsort(abs(s))

        # find nspc
        self.nspc = zeros(2,'int')
        self.nspc[1] = (y != 0).sum()
        self.nspc[0] = self.N - self.nspc[1]

        # find minF0
        self.minF0 = zeros(2,'int')
        self.minF0[0] = ((y != 0)&(s < 0)).sum()
        self.minF0[1] = ((y == 0)&(s > 0)).sum()

    def setx(self,x):
        self.x = x
        
        # find bid and B
        self.bid, self.B = argsort_and_cut(x)


    def solve2(self,typ,bnd):
        """Get the current best solution.

        Input:
            typ: = 0 if to constrain FRR <= maxFRR and minimize FAR < maxFAR
                 = 1 if to constrain FAR <= maxFAR and minimize FRR < maxFRR
            bnd: a numpy.array of 2 doubles: maxFRR and maxFAR
        Output:
            bparam: best (b,c)
            bsol: best (FRR,FAR)
        """
        typ = int(typ)
        bnd2 = zeros(2,'int')
        bnd2[typ] = int(bnd[typ]*self.nspc[1-typ])
        bnd2[1-typ] = int(ceil(bnd[1-typ]*self.nspc[typ]))
        
        bparam, bsol = _sdSolveUAC(self.s, self.sid, self.y, self.nspc, self.x, \
            self.bid, self.B, self.minF0,bnd2,typ)
        
        bsol2 = zeros(2)
        bsol2[0] = float(bsol[0])/self.nspc[1]
        bsol2[1] = float(bsol[1])/self.nspc[0]
        
        return bparam, bsol2
        
    def solve(self,typ,bnd):
        """If typ == 0, constrain FRR <= bnd and minimize FAR. Otherwise constrain FAR <= bnd and minimize FRR.

        Input:
            typ: type of optimization, minimize FAR if typ == 0. Otherwise minimize FRR.
            bnd: upper bound to the other, in [0,1].
        """
        bnd2 = zeros(2,'int')
        if typ == 0:
            bnd2[0] = int(round(bnd*self.nspc[1]))
            bnd2[1] = self.N+1
        else:
            bnd2[0] = self.N+1
            bnd2[1] = int(round(bnd*self.nspc[0]))

        bparam, bsol = _sdSolveUAC(self.s, self.sid, self.y, self.nspc, self.x, \
            self.bid, self.B, self.minF0,bnd2,typ)
        
        bsol2 = zeros(2)
        bsol2[0] = float(bsol[0])/self.nspc[1]
        bsol2[1] = float(bsol[1])/self.nspc[0]

        return bparam, bsol2

    def get(self,crit,param1):
        """Find b, c such that:
            If crit = 0: to minimize thelambda*FRR(b,c)+FAR(b,c),
                where thelambda = param1
            If crit = 1: to minimize FAR(b,c) subject to FRR(b,c) <= 1-minDR,
                where minDR = param1
            If crit = 2: to minimize FRR(b,c) subject to FAR(b,c) <= maxFAR,
                where maxFAR = param1
            Return:
                bparam: (b,c)
                bsol: (FRR,FAR)
        """
        if crit == 0:
            # Get maxFAR
            bparam, bsol = self.solve(0,0)
            maxFAR = bsol[1]

            # Get maxFRR
            bparam, bsol = self.solve(1,0)
            maxFRR = bsol[0]

            if(maxFAR < maxFRR): # domain in FRR better
                def func(x):
                    bparam, bsol = self.solve(0,x)
##                    print "at x ="+str(x)+" bparam="+str(bparam)+" and bsol="+str(bsol)
                    return param1*x + bsol[1]
                xmin = fminbound(func,0,maxFRR,xtol=1.0/self.N)
                return self.solve(0,xmin)
                
            else: # domain in FAR better
                def func(x):
                    bparam, bsol = self.solve(1,x)
##                    print "at x ="+str(x)+" bparam="+str(bparam)+" and bsol="+str(bsol)
                    return param1*bsol[0] +  x
                xmin = fminbound(func,0,maxFAR,xtol=1.0/self.N)
                return self.solve(1,xmin)

            return bparam, bsol
            
        if crit == 1:
            return self.solve(0,1-param1)
        
        if crit == 2:
            return self.solve(1,param1)
        
        raise ValueError("crit is not 0, 1, nor 2")

    
#------------------------------------------------------------------------------------------    

def histogram_1d(wcd, nbins, minValue = None, maxValue = None):
    """Compute a histogram from a (weighted) classification dataset with ishape=()

    :Parameters:
        wcd : WeightedCDataset
            a (weighted) dataset of J classes and ishape ()

        nbins : integer
            the number of bins

        minValue : double
            minimum value of the view, default is the smallest value in the dataset

        maxValue : double
            maximum value of the view, default is the largest value in the dataset

    :Returns:
        hist : array(shape=(J,nbins), dtype='double')
            J histograms of J classes

        bin_interval : array(shape=(nbins,2),dtype='double')
            array of nbins bin intervals

    """

    # figure out the bin intervals
    nbins = int(nbins)
    bin_interval = zeros((nbins,2))

    if minValue is None:
        minValue = min([x.min() for x in wcd.input_data]) - 1E-10
    minValue = float(minValue)

    if maxValue is None:
        maxValue = max([x.max() for x in wcd.input_data]) + 1E-10
    maxValue = float(maxValue)

    resolution = (maxValue - minValue) / nbins
    bin_interval[:,0] = arange(nbins) * resolution + minValue
    bin_interval[:,1] = bin_interval[:,0] + resolution

    # figure out the histograms
    hist = zeros((wcd.J,nbins))
    if wcd.weights is None:
        for j in xrange(wcd.J):
            input_array = wcd.input_data[j]
            hist_array = hist[j]

            code = """
                sdHistogram1D(Ninput_array[0], input_array, minValue, maxValue,
                    nbins, hist_array);
                return_val = 1;
            """
            inline(code, arg_names=['input_array', 'minValue', 'maxValue',
                'nbins', 'hist_array'], headers=['<SdThresh1D.h>']
                , libraries=['sdcpp', 'cblas_ext', 'cephesd', 'plat_det'])
    else:
        for j in xrange(wcd.J):
            input_array = wcd.input_data[j]
            hist_array = hist[j]
            weights = wcd.weights[j]

            code = """
                sdHistogram1D(Ninput_array[0], input_array, minValue, maxValue,
                    nbins, hist_array, weights);
                return_val = 1;
            """
            inline(code, arg_names=['input_array', 'minValue', 'maxValue',
                'nbins', 'hist_array', 'weights'], headers=['<SdThresh1D.h>'],
                libraries=['sdcpp', 'cblas_ext', 'cephesd', 'plat_det'])

    return hist, bin_interval


def sort_1d(cd):
    """Take a classification dataset with ishape=(), sort them in ascending order.

    :Parameters:
        cd : CDataset
            a dataset of J classes and ishape ()

    :Returns:
        sorted_id : array(shape=(cd.N,2),'int')
            each tuple (j,index) represents an input value, which is identified
            by its class 'j' and its index 'index' in the class
    """
    val = zeros(cd.N)
    vj = zeros((cd.N,2),'int')
    i2 = 0
    for j in xrange(cd.J):
        i1 = i2
        i2 += cd.nspc[j]
        val[i1:i2] = cd.input_data[j]
        vj[i1:i2,0] = j
        vj[i1:i2,1] = arange(cd.nspc[j])

    return vj[argsort(val)]
    
def thresh_1d(criterion,param1,wcd,sort_id = None):
    """Solve threshold-based 1D binary classifier.
    
    The function solves the following problem:
    Given two sets of samples of two classes, a positive one and a negative one,
    a threshold-based classifier classifies a value x into a positive or a negative
    class: sign(x - \theta). The optimal \theta is chosen based on different criteria:
    - Minimize the classification error: \lambda * p(pos)*FRR + p(neg)*FAR
    - Minimize the error without prior: \lambda * FRR + FAR
    - Minimize FAR with constraint FRR <= maxFRR
    - Minimize FRR with constraint FAR <= maxFAR

    :Parameters:
        criterion : integer from 0 to 3
            0: minimize classification error with prior probabilities
            1: minimize classification error without prior probabilities
            2: minimize FAR while constraining FRR
            3: minimize FRR while constraining FAR
        param1 : double
            a parameter representing
                \lambda if criterion < 2
                maxFRR if criterion == 2
                maxFAR if criterion == 3
        wcd : WeightedCDataset(J=2,ishape=()) (in cs.ml.cla package)
            a dataset of sample values of the two classes
        sort_id : array
            the result of calling sort_1d(wcd), 
            if sort_id is None, sort_1d(wcd) is called
            

    :Returns:
        result : array(shape=(2,),dtype='d')
            an argout array representing
            - result[0]: the threshold
            - result[1]: the optimized function value at that threshold
    """
    criterion = int(criterion)
    param1 = float(param1)
    idata0 = wcd.input_data[0]
    idata1 = wcd.input_data[1]
    if sort_id is None:
        sort_id = sort_1d(wcd)
    result = zeros(2)
        
    if wcd.weights is None:
        code = """
            int nspc[2];
            nspc[0] = Nidata0[0]; nspc[1] = Nidata1[0];
            
            double *input_data[2];
            input_data[0] = idata0;
            input_data[1] = idata1;
            
            sdTSolve(criterion, param1, nspc, input_data, (int *)sort_id, result);
            return_val = 1;
        """
        inline(code, arg_names=['criterion','param1','idata0','idata1','sort_id',
            'result'], headers=['<SdThresh1D.h>'], 
            libraries=['sdcpp', 'cblas_ext', 'cephesd', 'plat_det'])        
    else:
        w0 = wcd.weights[0]
        w1 = wcd.weights[1]
        code = """
            int nspc[2];
            nspc[0] = Nidata0[0]; nspc[1] = Nidata1[0];
            
            double *input_data[2];
            input_data[0] = idata0;
            input_data[1] = idata1;
            
            double *w[2];
            w[0] = w0; w[1] = w1;
            
            sdTSolve(criterion, param1, nspc, input_data, (int *)sort_id, result, w);
            return_val = 1;
        """
        inline(code, arg_names=['criterion','param1','idata0','idata1','sort_id',
            'w0','w1','result'], headers=['<SdThresh1D.h>'],
            libraries=['sdcpp', 'cblas_ext', 'cephesd', 'plat_det'])

    return result


def thresh_normal_1d(criterion,param1,stats):
    """
    Solve the class-conditional Gaussian-assumed classification with thresholding and goal.

    These sets of functions solve the following problem:
    Given two classes normally distributed, a positive one and a negative one,
    a threshold-based classifier classifies a value x into a positive or a negative
    class: sign(x - \theta). The optimal \theta is chosen based on different criteria:
     - Minimize the classification error: \lambda * p(pos)*FRR + p(neg)*FAR
     - Minimize the error without prior: \lambda * FRR + FAR
     - Minimize FAR with constraint FRR <= maxFRR
     - Minimize FRR with constraint FAR <= maxFAR

    :Parameters:
        criterion : integer from 0 to 3
            0: minimize classification error with prior probabilities
            1: minimize classification error without prior probabilities
            2: minimize FAR while constraining FRR
            3: minimize FRR while constraining FAR
        param1 : double
            a parameter representing
                \lambda if criterion < 2
                maxFRR if criterion == 2
                maxFAR if criterion == 3
        stats : Stats2e(J=2,d=1)
            REQUIREMENT: mean of class 0 <= mean of class 1

    :Returns:
        result : array(shape=(2,),dtype='d')
            an argout array representing
            - result[0]: the threshold
            - result[1]: the optimized function value at that threshold
    """
    criterion = int(criterion)
    param1 = float(param1)
    st = stats.A.ravel()
    result = zeros(2)
    code = """
        sdGTSolve(criterion, param1, st, result);
        return_val = 1;
    """
    inline(code, arg_names=['criterion','param1','st','result'], headers=['<SdGaussian1D.h>'], 
        libraries=['sdcpp', 'cblas_ext', 'cephesd', 'plat_det'])
    
    return result

