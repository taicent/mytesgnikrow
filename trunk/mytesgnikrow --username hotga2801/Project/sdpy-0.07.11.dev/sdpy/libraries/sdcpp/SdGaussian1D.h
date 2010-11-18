#ifndef __SD_GAUSSIAN1D_H
#define __SD_GAUSSIAN1D_H

#include <cephesd.h>

// ---------------------------------------------------------------------------------
// Functions for 1D normal distribution
// ---------------------------------------------------------------------------------

double sdNorm_entropy(double stddev = 1.0); // entropy

#define sdStdNorm_cdf(x) (ndtr(x))
#define sdStdNorm_ppf(x) (ndtri(x))
#define sdStdNorm_sf(x) (1-ndtr(x))
#define sdStdNorm_isf(x) (ndtri(1-(x)))

// pdf = probability density function
// cdf = cumulative distribution function, use cephes' ndtr(x)
// ppf = percent point function (inverse cdf), use cephes' ndtri(x)
// sf = survival function (complementary cdf), use cephes' 1-ndtr(x)
// isf = inverse survival function, use cephes' ndtri(1-x)

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

void sdGTSolve_MinError(double lambda, double *stats, double *result, bool noprior = false);
void sdGTSolve_MinFARConFRR(double maxFRR, double *stats, double *result);
void sdGTSolve_MinFRRConFAR(double maxFAR, double *stats, double *result);
// solve 1 of the 4 problems above
void sdGTSolve(int criterion, double param1, double *stats, double *result);

// solve J problems of the above type
// criterion =
//  0: min error
//  1: min error with no prior
//  2: min FAR with FRR constrained
//  3: min FRR with FAR constrained
void sdGTSolveArray(int criterion, double param1, int J, double *stats_array, double *result_array);

#endif
