#-------------------------------------------------------------------------------
# Module:      gaussian
# Package:     sd.cs.stats
# Purpose:     Subroutines dealing with class-conditional Gaussian ditributions
#
# Author:      minhtri
#
# Created:     04/04/2007
# Copyright:   (c) minhtri 2007
# Licence:     Lesser GPL
#-------------------------------------------------------------------------------
#!/usr/bin/env python

__all__ = ['find_classification_threshold','find_filtering_threshold',
    'find_filtering_threshold2']

from numpy import zeros
from scipy.weave import inline
from .. import Stats2e, stdnorm

#-------------------------------------------------------------------------------
# Subroutines dealing with class-conditional Gaussian ditributions.
#-------------------------------------------------------------------------------
# All these subroutines assume we have two random variables x+ and x-
# Gaussian-distributed. The statistics are stored in a Stats2e.
# A thresholded classifier is of this form:
#   if x >= b then return +1
#   else return -1
# A probabilistic classifier is of this form (MAP):
#   if p( x positive | x ) > p ( x negative | x) then return +1
#   else return -1
#-------------------------------------------------------------------------------

def find_classification_threshold(stats2e, thelambda = 1.0):
    """Compute the estimated error. Here we penalize false negative by sqrt(thelambda)
    and false positive by 1/sqrt(thelambda).
    
    Input:
        stats2e: per-element statistics of 2*N classes
        thelambda: the value of 'lambda'
    Output:
        b: a numpy.array of shape (N,2) to hold the results where
            b[i][0]: a threshold minimizing w.r.t. b
                f(b) = (1/sqrt(thelambda) * w[i*2] * sf((b-mu[i*2])/sigma[i*2]) +
                    sqrt(thelambda) * w[i*2+1] * cdf((b-mu[i*2+1])/sigma[i*2+1]))
                    / (w[i*2]+w[i*2+1])
            where sf(.) and cdf(.) are w.r.t. the normalized gaussian distribution
            b[i][1] = f(b[i][0]), the estimated error
    Requirement:
        mu[i*2] <= mu[i*2+1] for all i = 0..N-1
    """
    tprint("Warning, this function is now obsolete. Please use sdGTSolve() instead")
    N = stats2e.J / 2
    A = stats2e.A
    b = zeros([N,2])
    ld = float(thelambda)
    code = """
        double *dA, *db;
        double AA, BB, CC;
        double b0, b1, a0, a1, ss0, ss1, err0, err1, delta;
        int n;
        
        // Notice: for the normalized Gaussian distribution:
        //   sf(x) = erfc(x) / 2
        //   cdf(x) = 1 - erfc(x)/2 = (erf(x)+1)/2

        for( dA = A, db = b, n = 0; n < N; ++n, dA += 6, db += 2 )
        {
            ss0 = dA[2] * dA[2]; // sigma[0]^2
            ss1 = dA[5] * dA[5]; // sigma[1]^2
            
            a0 = dA[0];
            a1 = ld * dA[3];

            // Now we have AA * b^2 - 2 * BB * b + CC = 0, where
            AA = 1.0/ss1 - 1.0/ss0;
            BB = dA[4]/ss1 - dA[1]/ss0;
            CC = dA[4]*dA[4]/ss1 - dA[1]*dA[1]/ss0
                + 2*log((a0*dA[5])/(a1*dA[2]));
                
            // Solve it
            err0 = -1; // assume we haven't computed the minimum error
            if(fabs(AA) < 0.00000000001) // degenerate, one point
            {
                if(fabs(BB) < 0.00000000001) // degenerate further
                    b0 = dA[1];
                else
                    b0 = 0.5*CC/BB;
            }
            else if(BB*BB >= AA*CC) // got solution
            {
                delta = sqrt(BB*BB-AA*CC);
                b0 = (BB+delta)/AA;
                err0 = 0.5 * (a0*erfc((b0-dA[1])/dA[2]) + a1*(erf((b0-dA[4])/dA[5])+1));
                b1 = (BB-delta)/AA;
                err1 = 0.5 * (a0*erfc((b1-dA[1])/dA[2]) + a1*(erf((b1-dA[4])/dA[5])+1));
                if(err1 < err0) // take the better of the two solutions
                {
                    err0 = err1;
                    b0 = b1;
                }
            }
            else if(a0 < a1) // no solution, check two end points +inf and -inf
            {
                b0 = -100000000;
                err0 = a0;
            }
            else
            {
                b0 = 100000000;
                err0 = a1;
            }
            if(err0 < 0) // haven't computed?
                err0 = 0.5 * (a0*erfc((b0-dA[1])/dA[2]) + a1*(erf((b0-dA[4])/dA[5])+1));
                
            db[0] = b0;
            db[1] = err0 / (sqrt(ld) * (dA[0]+dA[3]));
        }

        return_val = 1;
    """
    inline(code,['N','A','b','ld'],compiler='mingw32')
    return b

def find_filtering_threshold(stats2e, minDR = 0.002):
    """Use the thresholded classifier as above. If we wish detection rate >= minDR, what is the largest threshold?

    Input:
        stats2e: per-element statistics of 2*N classes
        minDR: the value of 'minDR'
    Output:
        b: a numpy.array of shape (N,2) to hold the results where
            b[i][0]: a threshold that
                minimize FAR(b) = sf((b-mu[i*2])/sigma[i*2])
                w.r.t. b
                subject to DR(b) = sf((b-mu[i*2+1])/sigma[i*2+1]) >= minDR
            where sf(.) is w.r.t. the normalized gaussian distribution
            b[i][1] = FAR(b[i][0]), the estimated false acceptance rate
    Requirement:
        mu[i*2] <= mu[i*2+1] for all i = 0..N-1
    """
    tprint("Warning, this function is now obsolete. Please use sdGTSolve() instead")
    b0 = stdnorm.isf(minDR)
    N = stats2e.J / 2
    A = stats2e.A.reshape(N,6)
    b = zeros([N,2])
    b[:,0] = A[:,5]*b0 + A[:,4]
    b[:,1] = stdnorm.sf((b[:,0]-A[:,1])/A[:,2])
    return b

def find_filtering_threshold2(stats2e, maxFAR = 0.002):
    """Use the thresholded classifier as above. If we wish false acceptance rate <= maxFAR, what is the smallest threshold?

    Input:
        stats2e: per-element statistics of 2*N classes
        maxFAR: the value of 'maxFAR'
    Output:
        b: a numpy.array of shape (N,2) to hold the results where
            b[i][0]: a threshold that
                maximize DR(b) = sf((b-mu[i*2+1])/sigma[i*2+1])
                w.r.t. b
                subject to FAR(b) = sf((b-mu[i*2])/sigma[i*2]) <= maxFAR
            where sf(.) is w.r.t. the normalized gaussian distribution
            b[i][1] = 1-DR(b[i][0]), the estimated false rejection rate
    Requirement:
        mu[i*2] <= mu[i*2+1] for all i = 0..N-1
    """
    tprint("Warning, this function is now obsolete. Please use sdGTSolve() instead")
    b0 = stdnorm.isf(maxFAR)
    N = stats2e.J / 2
    A = stats2e.A.reshape(N,6)
    b = zeros([N,2])
    b[:,0] = A[:,2]*b0 + A[:,1]
    b[:,1] = stdnorm.cdf((b[:,0]-A[:,4])/A[:,5])
    return b

def main(): # seems ok for now
    class Test:
        pass

    a = zeros([4,3])
    a[0,0] = 1
    a[0,1] = 1
    a[0,2] = 1
    a[1,0] = 1
    a[1,1] = 2
    a[1,2] = 1
    a[2,0] = 10
    a[2,1] = 10
    a[2,2] = 10
    a[3,0] = 10
    a[3,1] = 20
    a[3,2] = 10
    b = Test()
    b.J = 4
    b.A = a
    return a, b, find_filtering_threshold2(b,0.01)

if __name__ == '__main__':
    main()
