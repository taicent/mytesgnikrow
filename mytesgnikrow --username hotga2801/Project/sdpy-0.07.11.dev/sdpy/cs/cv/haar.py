# ---------------------------------------------------------------------
# sdpy, a collection of Python modules developed by Minh-Tri Pham
# Copyright (C) 2007  Minh-Tri Pham
# Copyright (C) 2007  Nanyang Technological University

# This program is free software: you can redistribute it and/or modify
# it under the following term, and the terms of the GNU General Public 
# License as published by the Free Software Foundation, either version 
# 3 of the License, or (at your option) any later version:
# 1. All submitted papers (or any publicly available text) that uses 
# or talks about sdpy must cite the following paper: 
# Minh-Tri Pham and Tat-Jen Cham. Fast Training and Selection of Haar 
# features using Statistics in Boosting-based Face Detection. In Proc. 
# 11th IEEE International Conference on Computer Vision (ICCV'07), Rio 
# de Janeiro, Brazil, 2007.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# ---------------------------------------------------------------------

__all__ = ['generate_Haars','subsample_Haars','THaarClassifier',
    'train_Haar','train_Haar_with_local_stats','get_local_stats',
    'train_Haar_GC','train_Haar_UAC_filter','train_SRClassifier',
    'project_Haar','project_SR_stats','train_refined_SRClassifier']

from cPickle import load, dump
from math import sqrt
from numpy import array, add, zeros, dot, arange, argmin, ones, concatenate
from numpy.random import rand
from os.path import exists, join
from scipy.weave import inline

from cv import datapath
from integralimage import make_vi, integral, stats_integral, \
    compute_Stats2_integral
from ... import tprint
from .. import arrayd
from ..ml.cla import BinaryClassifier, GeneralizedCascade, ScoredWCDataset, \
    CDataGenerator, UAC, thresh_1d, sort_1d, histogram_1d
from ..stats.gaussian import find_classification_threshold, \
    find_filtering_threshold, find_filtering_threshold2
from ..stats import mean, covariance, stdnorm, Stats2e, Stats2
from ..dm import argsort

def _features_from_shape(M,N,appearance):
    """Generate all rectangular Haar-like features, of a specific 'appearance'.
    All rectangles are bounded by a M-by-N image patch.

    Input:
        M, N: number of rows and columns of an image patch
        appearance: an 'int' 2D numpy.array describing the appearance. E.g. array([[1,-1],[-1,1]]).
    Output:
        A: a 1D numpy.array of projection coefficients, in vectorintegrated form
        ind: a 2D numpy.array of indices of each feature, in vectorintegrated form
        ind2: a 2D numpy.array of 4 corners of each feature in the patch
    """
    (ny,nx) = appearance.shape
    A = zeros([ny+1,nx+1],'int')
    sum1 = appearance.sum()

    a1 = appearance*(ny*nx-sum1)
    a0 = (1-appearance)*sum1
    aa = a1-a0
    
    A[0:-1,0:-1] += aa
    A[1:,1:] += aa
    A[0:-1,1:] -= aa
    A[1:,0:-1] -= aa
    
    mask = A.astype('bool')
    A = A.ravel()[mask.ravel()]
    
    cny = 0
    for y1 in xrange(M): cny += (M-y1-1)/ny
    cnx = 0
    for x1 in xrange(N): cnx += (N-x1-1)/nx
    
    cnt = cnx*cny

    ind = zeros([cnt,len(A)],'int')
    ind2 = zeros([cnt,4],'int')
    
    i = 0
    for y1 in xrange(M):
        for y2 in xrange(y1+ny,M,ny):
            y = arange(y1,y2+1,(y2-y1)/ny)*N
            for x1 in xrange(N):
                for x2 in xrange(x1+nx,N,nx):
                    x = arange(x1,x2+1,(x2-x1)/nx)
                    ind[i] = add.outer(y,x)[mask]
                    ind2[i] = [y1,x1,y2,x2]
                    i += 1

    return (A, ind, ind2)


def generate_Haars(M,N):
    """Find all Haar-like features, applied on a M-by-N image patch.

    Input:
        M, N: number of rows and columns of an image patch
    Output:
        haars: a list of tuples (A,ind,ind2) output from _features_from_shape()
    """
    filename = join( datapath, 'haars_h'+str(M)+'_w'+str(N)+'.sdd' )
    if exists(filename):
        tprint("Loading Haar features from file...")
        haars = load(open(filename))
        tprint("Total features="+str(sum([len(x[1]) for x in haars])))
        return haars
    
    ftypes = [
        [[0,1]],
        [[1],[0]],

        [[1,0],[0,0]],
        [[0,1],[0,0]],
        [[0,0],[0,1]],
        [[0,0],[1,0]],

        [[1,0],[0,1]],
        [[1,0,0],[0,1,0],[0,0,1]],
        [[0,0,1],[0,1,0],[1,0,0]],

        [[1,1,0],[1,1,0],[0,0,0]],
        [[0,1,1],[0,1,1],[0,0,0]],
        [[0,0,0],[0,1,1],[0,1,1]],
        [[0,0,0],[1,1,0],[1,1,0]],

        [[0],[1],[0]],
        [[0,1,1,0]],
        [[0],[1],[1],[0]],
        [[0,1,0]],

        [[0,0,0],[0,1,0],[0,0,0]],
        [[0,1,0],[1,1,1],[0,1,0]],

    ]

    haars = [_features_from_shape(M,N,array(x,'int')) for x in ftypes]

    tprint("Saving Haar features to file...")
    dump(haars,open(filename,'w'))
    tprint("Total features="+str(sum([len(x[1]) for x in haars])))

    return haars

def subsample_Haars(haars,ratio):
    """Take a set of Haar features and subsample it.

    Input:
        haars: a set of Haar features, output of generate_Haars()
        ratio: the ratio to subsample, if 'haars' has N features,
            the output 'haars' will have roughly ratio*N features
    Output:
        ohaars: a smaller set of Haar features
    """
    cnt = 0
    ohaars = []
    for x in haars:
        arr = rand(len(x[1])) < ratio
        ohaars.append((x[0],x[1][arr],x[2][arr]))
        cnt += arr.sum()
    tprint("There are "+str(cnt)+ " Haar features after subsampling.")
    return ohaars

def _project_Haar_stats(stats2,haars):
    """Consider T Haar features generated by generate_Haars().
    For each feature: project, compute the statistics of the two classes,
    then refine if needed. Return a list of J*2-class Stats2e, each for each
    type of Haar features.

    Input:
        stats2: The 'global' statistics of the two classes, can be obtained
            by calling compute_Stats2_integral()
        haars: the T Haar features generated, by caling generate_Haars()
    Output:
        B: a list of J*2-class Stats2e, each for each type of Haar features.
        invert: a list of numpy.arrays, each array tells whether which feature
            needs negating its projection direction

    """
    B = []
    invert = []
    
    theweight = stats2.get_stat(0)
    themean = stats2.get_stat(1)
    meanofs = int(themean.strides[0]) / 8
    thecov = stats2.get_stat(2)
    covofs = int(thecov.strides[0]) / 8

    d = themean[0].size
    for h in haars:
        w = h[0]
        I = len(w)
        J = len(h[1])
        index = h[1]

        b = zeros([J,2,3])
        b[:,0,0] = theweight[0]
        b[:,1,0] = theweight[1]
        
        inve = zeros([J],'int8')
        
        # Compute projected means and vars
        code = """
            int i, j;
            long int *dindex;
            double *dsrc, *dmean, *dcov, *ddst;
            int y, x;
            double sum;

            for( j = 0; j < J; ++j )
            {
                dindex = index + j * I;
                for( i = 0; i < 2; ++i )
                {
                    dmean = themean + i*meanofs;
                    dcov = thecov + i*covofs;
                    ddst = b + (j * 2 + i) * 3;

                    // Filter then project the mean vector
                    ddst[1] = 0;
                    for( x = 0; x < I; ++x ) ddst[1] += w[x]*dmean[dindex[x]];

                    // Filter then project the covariance matrix
                    ddst[2] = 0;
                    for( y = 0; y < I; ++y )
                    {
                        sum = 0;
                        dsrc = dcov + dindex[y]*d;
                        for( x = 0; x < I; ++x ) sum += w[x]*dsrc[dindex[x]];
                        ddst[2] += w[y]*sum;
                    }
                    ddst[2] = sqrt(ddst[2]);
                }
                
                // needs refining?
                ddst = b + j * 6;
                if(ddst[4] < ddst[1])
                {
                    inve[j] = 1;
                    ddst[1] = -ddst[1];
                    ddst[4] = -ddst[4];
                }
            }
            return_val = 1;
        """
        inline(code,['index','themean','thecov','b','I','J','d','w',
            'meanofs','covofs','inve'])

        B.append(Stats2e(J*2,(I,),b.reshape(J*2,3)))
        invert.append(inve.astype('bool'))

    return (B,invert)


def get_local_stats(cd,haars):
    """
    Take a 2-class ClassificationDataset, obtain the global statistics,
    then project using different Haar directions.

    Input:
        cd: a 2-class ClassificationDataset
        haars: the generated Haar feature set of J features
    Output: output of _project_Haar_stats() actually
        B: a list of J*2-class Stats2e, each for each type of Haar features.
        invert: a list of numpy.arrays, each array tells whether which feature
            needs negating its projection direction
    """
    tprint("Accumulating data...")
    stats2 = compute_Stats2_integral(cd,True)

    tprint("Projecting all the Haar features...")
    return _project_Haar_stats(stats2,haars)



# ---------------------------------------------------------------------------------------------
# Project the input data into a line using haar direction specified by coefficients and indices
# ---------------------------------------------------------------------------------------------

def project_Haar(input_data,A,ind):
    """Project the input data into a line using haar direction specified by coefficients 'A' and indices 'ind'

    Input:
        input_data: a vectorintegrated 2D numpy.array
        A,ind: coefficients and indices of a Haar feature
    Output:
        x: a 1D numpy.array of projected values
    """

    (N,d) = input_data.shape
    w = int(input_data.strides[0]) / 8
    I = len(A)
    A = arrayd(A)
    ind = ind.ravel()

    x = zeros(N)

    code = """
        int n, i;
        double *dsrc, *ddst;
        double sum;

        ddst = x;
        for( n = 0; n < N; ++n )
        {
            dsrc = &INPUT_DATA2(n,0);
            sum = 0;
            for( i = 0; i < I; ++i ) sum += A[i] * dsrc[ind[i]];
            ddst[n] = sum;
        }

        return_val = 1;
    """
    inline(code,['N','A','I','ind','x','input_data'])

    return x

def project_SR_stats(stats2, A, ind):
    """Project the high-dim statistics into a line using a SR direction specified by coefficients 'A' and indices 'ind'

    :Parameters:
        stats2: Stats2
            a vectorintegrated 2D numpy.array
        ind: array(shape=(n,), dtype='int')
            indices of the SR feature
        A: array(shape=(n,), dtype='double')
            coefficients of the SR feature
    
    :Returns:
        stats2e: a 1D Stats2e representing the projected result
    """
    ofs = stats2.ofs
    size = stats2.size
    stats2A = stats2.A.ravel()
    
    A = arrayd(A)
    ind = ind.ravel()
    outA = zeros(6,'d')

    code = """
        project_SR_with_stats((int*)ofs, (int*)size, stats2A, Nind[0], (int*)ind, A, outA);
        return_val = 1;
    """
    inline(code, arg_names=['ofs', 'size', 'stats2A', 'ind', 'A', 'outA'], \
        headers=['<SdSRFeature.h>'], libraries=['sdcpp', 'cblas_ext', 'cephesd', 'plat_det'])

    return Stats2e(2, (), outA)



#-------------------------------------------------------------------------------
# Binary classifier using Haar features. Technically it's a linear classifier...
#-------------------------------------------------------------------------------
class THaarClassifier(BinaryClassifier):

    def __init__(self,p,w,b):
        BinaryClassifier.__init__(self)
        self.p = p # list of indices to keep
        self.w = w # projection direction
        self.b = b # threshold

    def predict(self,input_point): # input image point *must* be vectorintegrated
        return int(dot(input_point[self.p],self.w) >= self.b)

    def test(self,input_data): # input data *must* be vectorintegrated
##        tprint("sdata="+str((dot(input_data[:,self.p],self.w) >= self.b).astype('int')))
##        idata = input_data[:,self.p]
##        pdata = dot(idata,self.w)
##        tdata = pdata >= self.b
##        jdata = tdata.astype('int')
##        tprint("jdata="+str(jdata))
##        return jdata
        return (dot(input_data[:,self.p],self.w) >= self.b).astype('int')

def train_Haar_with_local_stats(Binvert,haars,crit,param1):
    """Given local statistics, train a THaarClassifier.
    Select the 'best' Haar feature in the set.
    The features are trained using one of three criteria:
        crit = 0: param1 is 'thelambda', then call gaussian.find_classification_threshold()
        crit = 1: param1 is 'minDR', then call gaussian.find_filtering_threshold()
        crit = 2: param1 is 'maxFAR', then call gaussian.find_filtering_threshold2()

    Input: 
        Binvert: (B,invert), is output of get_local_stats() actually
            B: a list of J*2-class Stats2e, each for each type of Haar features.
            invert: a list of numpy.arrays, each array tells whether which feature
                needs negating its projection direction
        haars: the generated Haar feature set of J features
        crit, param1: as mentioned above
    Output:
        lc: a THaarClassifier, with lc.err as the estimated 'error'
    """
    (B,invert) = Binvert
    
    if crit == 0:
        tprint("To minimize "+str(param1)+" FRR + FAR...")
    elif crit == 1:
        tprint("To minimize FAR with FRR <= "+str(1-param1)+"...")
    else:
        tprint("To minimize FRR with FAR <= "+str(param1)+"...")

    bee = 1.1 # best estimated error
    for i in xrange(len(haars)):
        h = haars[i]
        b = B[i]
        iv = invert[i]

        if crit == 0:
            b2 = find_classification_threshold(b,param1)
        elif crit == 1:
            b2 = find_filtering_threshold(b,param1)
        else:
            b2 = find_filtering_threshold2(b,param1)

        j = argmin(b2[:,1])
        if b2[j,1] < bee: # better than the current best
            tprint("Found a feature of type "+str(i)+" with ce = "+str(b2[j,1]))
            bee = b2[j,1]
            if iv[j]:
                bw = -h[0]
            else:
                bw = h[0]
            bp = h[1][j]
            bb = b2[j,0]

    tprint("Finished finding the best Haar feature...")

    lc = THaarClassifier(bp,bw,bb)
    lc.err = bee
    return lc


def train_Haar(cd,haars,crit,param1):
    """Take a 2-class ClassificationDataset, then train a THaarClassifier.
    Select the 'best' Haar feature in the set.
    This function actually calls get_local_stats(),
    followed by train_Haar_with_local_stats()

    Input:
        cd: a 2-class ClassificationDataset
        haars: the generated Haar feature set
        crit, param1: as mentioned above
    Output:
        lc: a THaarClassifier, with lc.err as the estimated 'error'
    """
    B, invert = get_local_stats(cd,haars)
    return train_Haar_with_local_stats((B,invert),haars,crit,param1)



#-------------------------------------------------------------------------------
# Train Haar features using Univariate Additive Classifiers (UAC)
#-------------------------------------------------------------------------------

def train_Haar_UAC_filter(scd,haars,rmaxFRR=0.001,rmaxFAR=0.8):
    """Take a ScoredWCDataset, then train a THaarClassifier 'filter' using UAC.
    Select the 'best' Haar feature in the set.

    Input:
        scd: a ScoredWCDataset -- must be vectorintegrated
        haars: the generated Haar feature set
        rmaxFRR, rmaxFAR: train 'filter' to reject if there is a classifier
            that can do FRR <= rmaxFRR and FAR <= rmaxFAR
    Output:
        lc: a THaarClassifier, with:
            lc.sol: training (FRR,FAR)
        c: voting coefficient
        both lc and c will be None if there's no filter satisfied
    """
    s = concatenate([x for x in scd.scores])
    y = ones(scd.N,'int')
    y[:scd.nspc[0]] = 0

    bnd = zeros(2)
    bnd[0] = rmaxFRR
    bnd[1] = rmaxFAR

    bsol = array([1.1,1.1])

    uac = UAC(s,y)
    for i in xrange(len(haars)):
        h = haars[i]
        tprint("Training type "+str(i)+" with "+str(len(h[1]))+" features")
        for t in xrange(len(h[1])):
            x = [project_Haar(u,h[0],h[1][t]) for u in scd.input_data]
            uac.setx(concatenate(x))
            param, sol = uac.solve2(0,bnd)

            if sol[0] < 1: # got solution
                bparam = param
                bsol = sol
                bnd[1] = sol[1]
                bi = i
                bt = t
                # TODO: perhaps print some best results if got new
                tprint("Current best: "+str(bsol))

    if bsol[0] < 1: # got solution
        lc = THaarClassifier(haars[bi][1][bt],haars[bi][0],bparam[0])
        lc.sol = bsol
        return lc, bparam[1]

    return (None, None)

def train_Haar_UAC_improver2(scd,haars,rmaxFRR=0.001,rmaxFAR=0.8):
    """Take a ScoredWCDataset, then train a THaarClassifier 'filter' using UAC.
    Select the 'best' Haar feature in the set.

    Input:
        scd: a ScoredWCDataset -- must be vectorintegrated
        haars: the generated Haar feature set
        rmaxFRR, rmaxFAR: train 'filter' to reject if there is a classifier
            that can do FRR <= rmaxFRR and FAR <= rmaxFAR
    Output:
        lc: a THaarClassifier, with:
            lc.sol: training (FRR,FAR)
        c: voting coefficient
        both lc and c will be None if there's no filter satisfied
    """
    s = concatenate([x for x in scd.scores])
    y = ones(scd.N,'int')
    y[:scd.nspc[0]] = 0

    bnd = zeros(2)
    bnd[0] = rmaxFRR
    bnd[1] = rmaxFAR

    bsol = array([1.1,1.1])

    uac = UAC(s,y)
    for i in xrange(len(haars)):
        h = haars[i]
        for t in xrange(len(h[1])):
            x = [project_Haar(u,h[0],h[1][t]) for u in scd.input_data]
            uac.setx(concatenate(x))
            param, sol = uac.solve2(1,bnd)

            if sol[0] < 1: # got solution
                bparam = param
                bsol = sol
                bnd[0] = sol[0]
                bi = i
                bt = t
                # TODO: perhaps print some best results if got new

    if bsol[0] < 1: # got solution
        lc = THaarClassifier(haars[bi][1][bt],haars[bi][0],bparam[0])
        lc.sol = bsol
        return lc, bparam[1]

    return (None, None)

def train_Haar_UAC_improver(scd,haars,thelambda,bsol):
    """Take a ScoredWCDataset, then train a THaarClassifier 'improver' using UAC.
    Select the 'best' Haar feature in the set.

    Input:
        scd: a ScoredWCDataset -- must be vectorintegrated
        haars: the generated Haar feature set
        thelambda: the goal is to minimize thelambda * FRR + FAR
        bsol: previously best (FRR,FAR)
    Output:
        lc: a THaarClassifier, with:
            lc.sol: training (FRR,FAR)
        c: voting coefficient
        both lc and c will be None if there's no filter satisfied
    """
    s = concatenate([x for x in scd.scores])
    y = ones(scd.N,'int')
    y[:scd.nspc[0]] = 0

    gotsol = False

    uac = UAC(s,y)
    for i in xrange(len(haars)):
        h = haars[i]
        for t in xrange(len(h[1])):
            x = [project_Haar(u,h[0],h[1][t]) for u in scd.input_data]
            uac.setx(concatenate(x))
            param, sol = uac.get(0,thelambda)

            if sol[0]*thelambda+sol[1] < bsol[0]*thelambda+bsol[1]: # got solution
                gotsol = True
                bparam = param
                bsol = sol
                bi = i
                bt = t
                # TODO: perhaps print some best results if got new

    if gotsol is True: # got solution
        lc = THaarClassifier(haars[bi][1][bt],haars[bi][0],bparam[0])
        lc.sol = bsol
        return lc, bparam[1]

    return (None, None)

def train_Haar_GC(scd,cdgenerator,haars,thelambda=1.0,rmaxFRR=0.001,rmaxFAR=0.8,maxM=20):
    """Take a CDataset, then train a GeneralizedCascade of THaarClassifiers.

    Input:
        scd: a 2-class ScoredWCDataset -- must be vectorintegrated
            scd *is modified* after the training process
        cdgenerator: a CDGenerator that can generate negative points for scd
        haars: the generated Haar feature set
        thelambda: the goal for 'improver' is to minimize thelambda*FRR+FAR
        rmaxFRR, rmaxFAR: train 'filter' to reject if there is a classifier
            that can do FRR <= rmaxFRR and FAR <= rmaxFAR
    Output:
        gc: a GeneralizedCascade
    """

    gc = GeneralizedCascade()
    scd.scores = [gc.scores(x)[2] for x in scd.input_data]

    cnt = 0
    bsol = array([1.1,1.1])
    
    while True:
        tprint("Weak classifier "+str(len(gc.c))+":")
        (lc,c) = train_Haar_UAC_filter(scd,haars,rmaxFRR,rmaxFAR)
        if lc is not None:
            cnt += 1
            tprint("A filter with sol="+str(lc.sol))
            gc.add_binary_classifier(lc,c,False)

            # filter negatively predicted examples
            for j in xrange(2):
                y = lc.test(scd.input_data[j])
                s = scd.scores[j] + c*(y*2-1)
##                tprint(s)
                s1 = (s >= 0)
                if j == 0:
                    nerr = s1.sum()
                else:
                    nerr = scd.nspc[1]-s1.sum()
                tprint("(nerr/nspc) of "+str(j)+" is ("+str(nerr)+"/"+str(scd.nspc[j])+")="+str(float(nerr)/scd.nspc[j]))
##                tprint("Class "+str(j)+" from "+str(scd.nspc[j])+" to ("+str(n)+","+str(scd.nspc[j]-n)+")")
                scd.dofilter(j,s1)

            # add new negative points if necessary
            if scd.nspc[0] < scd.nspc[1]:
                train_neg, scores_neg = gc.generate_negatives(scd.nspc[1]-scd.nspc[0],cdgenerator)
                scd.concat(0,train_neg,scores_neg)

            bsol = array([1.1,1.1])

        else:
            (lc,c) = train_Haar_UAC_improver(scd,haars,thelambda,bsol)
##            (lc,c) = train_Haar_UAC_improver2(scd,haars,1.1,rmaxFAR)
            if lc is not None:
                tprint("An improver with sol="+str(lc.sol))
                bsol = lc.sol
                gc.add_binary_classifier(lc,c,True)

                for j in xrange(2):
                    y = lc.test(scd.input_data[j])
                    nerr = (y != j).sum()
                    scd.scores[j] += c*(y*2-1)
                    
                nerr = zeros(2,'int')
                nerr[0] = (scd.scores[0] > 0).sum()
                nerr[1] = (scd.scores[1] < 0).sum()
                for j in xrange(2):
                    tprint("(nerr/nspc) of "+str(j)+" is ("+str(nerr[j])+"/"+str(scd.nspc[j])+")="+str(float(nerr[j])/scd.nspc[j]))
                    
                if nerr[0]+nerr[1] < 1:
                    break
            else:
                break
            
        if cnt >= maxM or len(gc.c) >= 50:
            break

    return gc

    

def train_SRClassifier(stats2,criterion,param1,nrects,nseconds):
    """Train Sparse Rectangular features and select the best feature classifier.

    Suppose we have 2 classes c: positive and negative.
    Suppose the vector-integrated version of an image of size 'len'-by-'len'
    is a random vector y.
    Let 'stats' be the Stats2 statistics of the two classes.
    Consider 'criterion' and 'param1' described in sdGTSolve().

    Now consider the following features called Sparse Rectangular features:
       H = \sum_{k=1}^{nrects} a_k \pi(x_k,y_k,w_k,h_k)
    This function trains and selects the 'best' Sparse Rectangular feature
    classifier based on sdGTSolve()'s criterion.
    The classifier is as follows:
        return sign( (H \convolve I) - threshold )

    The underlying algorithm is Differential Evolution.
    It runs for 'nseconds' seconds.

    :Parameters:
        stats : Stats2
            Statistics of the 2 classes
        criterion : int
            from 0 to 3, used in conjunction with param1, see sdGTSolve()
        param1 : double
            see 'criterion'
        nrects : int
            number of rectangles of the sparse feature
        nseconds : double
            this algorithm uses Differential Evolution, so this tells at
            most how many seconds to run

    :Returns:
        fc: THaarClassifier
            the best feature classifier obtained
            fc.err is the estimated 'error' obtained

        ngen : int
            number of generations used in DE
        ngenome : int
            number of genomes generated in DE
        nvgenome : int
            number of *valid* genomes generated in DE
    """
    
    len = int(round(sqrt(stats2.d)))
    ofs = stats2.ofs
    size = stats2.size
    stats2A = stats2.A.ravel()
    criterion = int(criterion)
    param1 = float(param1)
    nrects = int(nrects)
    nseconds = float(nseconds)
    
    out_ints = zeros((nrects*4+4,), 'int32')
    out_doubles = zeros((nrects*4+2,), 'float64')
    
    code = """
        sdBestSR(len, (int*)ofs, (int*)size, stats2A, criterion, param1, nrects, nseconds, 
            (int*)out_ints, out_doubles);
        return_val = 1;
    """
    inline(code, arg_names=['len', 'ofs', 'size', 'stats2A',
        'criterion', 'param1', 'nrects', 'nseconds',
        'out_ints', 'out_doubles'
        ], \
        headers=['<SdSRFeature.h>'], libraries=['sdcpp', 'cblas_ext', 'cephesd', 'plat_det'])

    n = int(out_ints[0])
    fc = THaarClassifier(out_ints[4:n+4], out_doubles[2:n+2], float(out_doubles[0]))
    fc.err = float(out_doubles[1])
    return (fc, int(out_ints[1]), int(out_ints[2]), int(out_ints[3]))

def train_refined_SRClassifier(wcd):
    """A training function that takes in a WeightedCDataset and produces a weak BinaryClassifier.
    
    This training function uses train_SRClassifier() to train but with 6 rectangles and 2 seconds.
    Also, the threshold of the weak classifier is refined after training. The function to minimize
    is classification error with lambda = 1.0
    
    :Parameters:
        wcd : WeightedCDataset
            a weighted dataset of vector-integrated patches
    
    :Returns:
        fc: THaarClassifier
            the best feature classifier obtained with
                fc.b being the best threshold after refined
                fc.err is the estimated 'error' corresponding to fc.b
                fc.b2 is the best threshold estimated by train_SRClassifier()
                fc.err2 is its corresponding 'estimated error'
                fc.stats2e is its corresponding 1D statistics

    """
    stats2 = compute_Stats2_integral(wcd, True)
    fc = train_SRClassifier(stats2, 0, 1, 6, 2)[0]

    # project to this newly found direction
    feature_value_array = [project_Haar(x, fc.w, fc.p) for x in wcd.input_data]
    wcd1 = WeightedCDataset(feature_value_array, wcd.weights)

    # save results
    fc.b2 = fc.b
    fc.err2 = fc.err
    fc.stats2e = project_SR_stats(stats2, fc.w, fc.p)

    # refine the fc classifier
    z = thresh_1d(0,1,wcd1,sort_1d(wcd1))
    fc.b = z[0]
    fc.err = z[1]
    
    # store the histograms
    # z = histogram_1d(wcd1, 32)
    # fc.hist = z[0]
    # fc.minValue = z[1][0,0]
    # fc.maxValue = z[1][31,1]

    # tprint("Hist=")
    # tprint(fc.hist)
    # tprint(fc.minValue)
    # tprint(fc.maxValue)
    # tprint(fc.stats2e.A)

    return fc

