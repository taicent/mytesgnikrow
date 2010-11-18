#include "SdGaussian1D.h"

// ---------------------------------------------------------------------------------
// Functions for 1D normal distribution
// ---------------------------------------------------------------------------------

double sdNorm_entropy(double stddev)
{
	return 0.5*(1+log(PI*2))+log(stddev);
}


// ---------------------------------------------------------------------------------
// Functions dealing with two 1D normal distributions
// ---------------------------------------------------------------------------------
// These sets of functions solve the following problem:
// Given two classes normally distributed, a positive one and a negative one,
// a threshold-based classifier classifies a value x into a positive or a negative
// class: sign(x - \theta). The optimal \theta is chosen based on different criteria:
//  - Minimize the classification error: \lambda * p(pos)*FRR + p(neg)*FAR
//  - Minimize the error without prior: \lambda * FRR + FAR
//  - Minimize FAR with constraint FRR <= maxFRR
//  - Minimize FRR with constraint FAR <= maxFAR
// ---------------------------------------------------------------------------------
// stats = an array of 6 doubles representing:
//  stats[0]: the total weight of the negative distribution
//  stats[1]: the mean value of the negative distribution
//  stats[2]: the standard deviation of the negative distribution
//  stats[3]: the total weight of the positive distribution
//  stats[4]: the mean value of the positive distribution
//  stats[5]: the standard deviation of the positive distribution
// REQUIREMENT: stats[1] <= stats[4]
//
// result = an array of 2 doubles representing:
//  result[0]: the threshold
//  result[1]: the optimized function value at that threshold

void sdGTSolve_MinError(double lambda, double *stats, double *result, bool noprior)
{
    double ss0, ss1, a0, a1;
    double AA, BB, CC, delta;
    double err0, err1, b0, b1;

    ss0 = 1.0 / (stats[2] * stats[2]); // 1/sigma[0]^2
    ss1 = 1.0 / (stats[5] * stats[5]); // 1/sigma[1]^2

    if(noprior)
    {
        a0 = 1.0;
        a1 = lambda;
    }
    else
    {
        delta = stats[0] + stats[3];
        a0 = stats[0] / delta;
        a1 = lambda * stats[3] / delta;
    }

    // Now we have AA * b^2 - 2 * BB * b + CC = 0, where
    AA = ss1 - ss0;
    BB = stats[4]*ss1 - stats[1]*ss0;
    CC = stats[4]*stats[4]*ss1 - stats[1]*stats[1]*ss0
        + 2*log((a0*stats[5])/(a1*stats[2]));

    // Solve it
    err0 = -1; // assume we haven't computed the minimum error
    if(fabs(AA) < 0.00000000001) // degenerate, one point
    {
        if(fabs(BB) < 0.00000000001) // degenerate further
            b0 = stats[1];
        else
            b0 = 0.5*CC/BB;
    }
    else if((delta = BB*BB-AA*CC) >= 0) // got solution
    {
        delta = sqrt(delta);
        b0 = (BB+delta)/AA;
        err0 = a0*sdStdNorm_sf((b0-stats[1])/stats[2]) + a1*sdStdNorm_cdf((b0-stats[4])/stats[5]);
        b1 = (BB-delta)/AA;
        err1 = a0*sdStdNorm_sf((b1-stats[1])/stats[2]) + a1*sdStdNorm_cdf((b1-stats[4])/stats[5]);
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
        err0 = a0*sdStdNorm_sf((b0-stats[1])/stats[2]) + a1*sdStdNorm_cdf((b0-stats[4])/stats[5]);

    result[0] = b0;
    result[1] = err0;
}

void sdGTSolve_MinFARConFRR(double maxFRR, double *stats, double *result)
{
    result[0] = sdStdNorm_ppf(maxFRR)*stats[5] + stats[4];
    result[1] = sdStdNorm_sf((result[0]-stats[1])/stats[2]);
}

void sdGTSolve_MinFRRConFAR(double maxFAR, double *stats, double *result)
{
    result[0] = sdStdNorm_isf(maxFAR)*stats[2] + stats[1];
    result[1] = sdStdNorm_cdf((result[0]-stats[4])/stats[5]);
}

void sdGTSolve(int criterion, double param1, double *stats, double *result)
{
    switch (criterion)
    {
    case 0:
    case 1:
        sdGTSolve_MinError(param1,stats,result,(bool)criterion);
        break;
    case 2:
        sdGTSolve_MinFARConFRR(param1,stats,result);
        break;
    case 3:
        sdGTSolve_MinFRRConFAR(param1,stats,result);
        break;
    }
}

// solve J problems of the above type
// criterion =
//  0: min error
//  1: min error with no prior
//  2: min FAR with FRR constrained
//  3: min FRR with FAR constrained
void sdGTSolveArray(int criterion, double param1, int J, double *stats_array, double *result_array)
{
    int j;
    double *dsrc, *ddst, tmp;
    switch (criterion)
    {
    case 0:
    case 1:
        for( j = 0, dsrc = stats_array, ddst = result_array; j < J; ++j, dsrc += 6, ddst += 2 )
            sdGTSolve_MinError(param1,dsrc,ddst,(bool)criterion);
        break;
    case 2:
        tmp = sdStdNorm_ppf(param1);
        for( j = 0, dsrc = stats_array, ddst = result_array; j < J; ++j, dsrc += 6, ddst += 2 )
        {
            ddst[0] = tmp*dsrc[5] + dsrc[4];
            ddst[1] = sdStdNorm_sf((ddst[0]-dsrc[1])/dsrc[2]);
        }
        break;
    case 3:
        tmp = sdStdNorm_isf(param1);
        for( j = 0, dsrc = stats_array, ddst = result_array; j < J; ++j, dsrc += 6, ddst += 2 )
        {
            ddst[0] = tmp*dsrc[2] + dsrc[1];
            ddst[1] = sdStdNorm_cdf((ddst[0]-dsrc[4])/dsrc[5]);
        }
        break;
    }
}


