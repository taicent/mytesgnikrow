#ifndef __SD_THRESH1D_H
#define __SD_THRESH1D_H


// ---------------------------------------------------------------------------------
// Functions dealing with two 1D distributions
// ---------------------------------------------------------------------------------
// Draw a histogram from a set of input points with optional weights.
// 'nBins' of bins are uniformly defined on the inteval [minValue,maxValue]
// The resulting histograms are normalized --> approximating true distribution
void sdHistogram1D( int N, double *input_array,
    double minValue, double maxValue,
    int nBins, double *hist_array,
    double *weights = 0 );


// ---------------------------------------------------------------------------------
// Functions dealing with two 1D normal distributions
// ---------------------------------------------------------------------------------
// The function solves the following problem:
// Given two sets of samples of two classes, a positive one and a negative one,
// a threshold-based classifier classifies a value x into a positive or a negative
// class: sign(x - \theta). The optimal \theta is chosen based on different criteria:
//  - Minimize the classification error: \lambda * p(pos)*FRR + p(neg)*FAR
//  - Minimize the error without prior: \lambda * FRR + FAR
//  - Minimize FAR with constraint FRR <= maxFRR
//  - Minimize FRR with constraint FAR <= maxFAR
// ---------------------------------------------------------------------------------
// nspc[2]: number of samples per class
// input[j]: array of input samples of class j
// sort_id: array of (j,index) sorting the joint samples in ascending order
// weight[j]: array of weights for the input samples of class j
//
// result = an array of 2 doubles representing:
//  result[0]: the threshold
//  result[1]: the optimized function value at that threshold

// solve 1 of the 4 problems above
void sdTSolve(int criterion, double param1,
    int *nspc, double **input, int *sort_id,
    double *result, double **weight = 0);

#endif
