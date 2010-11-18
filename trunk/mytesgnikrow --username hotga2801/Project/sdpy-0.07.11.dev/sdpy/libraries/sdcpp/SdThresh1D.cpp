#include <cephesd.h>
#include "cblas_ext.h"
#include "SdThresh1D.h"


// ---------------------------------------------------------------------------------
// Functions dealing with two 1D distributions
// ---------------------------------------------------------------------------------
// Draw a histogram from a set of N input values with optional weights.
// 'nBins' of bins are uniformly defined on the inteval [minValue,maxValue]
// The resulting histograms are normalized --> approximating true distribution
void sdHistogram1D( int N, double *input_array,
    double minValue, double maxValue,
    int nBins, double *hist_array,
    double *weights )
{
    int i, j;
    double res = (maxValue-minValue) / nBins;
    for( i = 0; i < nBins; ++i ) hist_array[i] = 0;

    if(weights)
    {
        double sum = 0;
        for( i = 0; i < N; ++i )
        {
            j = (int) floor((input_array[i] - minValue)/res);
            sum += weights[i];
            if(j >= 0 && j < nBins) hist_array[j] += weights[i];
        }
        for( j = 0; j < nBins; ++j ) hist_array[j] /= sum;
    }
    else
    {
        for( i = 0; i < N; ++i )
        {
            j = (int) floor((input_array[i] - minValue)/res);
            if(j >= 0 && j < nBins) hist_array[j] += 1.0;
        }
        for( j = 0; j < nBins; ++j ) hist_array[j] /= N;
    }
}



inline double get_value( int criterion, double param1, double *we, double *tw  )
{
    switch (criterion)
    {
    case 0:
        return (we[0] + param1 * we[1]) / (tw[0] + tw[1]);
    case 1:
        return we[0] / tw[0] + param1 * we[1] / tw[1];
    case 2:
        if(we[1] <= param1 * tw[1]) return we[0] / tw[0];
        break;
    case 3:
        if(we[0] <= param1 * tw[0]) return we[1] / tw[1];
        break;
    }
    return INFINITY;
}

inline void update_weight( int j, int i, double *we, double **weight )
{
    if(weight)
        if(j) we[1] += weight[j][i];
        else we[0] -= weight[j][i];
    else
        if(j) we[1] += 1;
        else we[0] -= 1;
}


// ---------------------------------------------------------------------------------
// Functions dealing with two 1D distributions
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
void sdTSolve(int criterion, double param1,
    int *nspc, double **input, int *sort_id,
    double *result, double **weight)
{
    int i, j, N = nspc[0] + nspc[1];
    double we[2], tw[2];
    double val, bval, bthresh, pos1, pos2;

    // get total weights
    if(weight)
    {
        tw[0] = cblas_dsum(nspc[0],weight[0],1);
        tw[1] = cblas_dsum(nspc[1],weight[1],1);
    }
    else
    {
        tw[0] = nspc[0];
        tw[1] = nspc[1];
    }

    // initalize everything from the left most
    we[0] = tw[0]; we[1] = 0;
    j = sort_id[0];
    i = sort_id[1];
    pos2 = input[j][i];
    bthresh = pos2 - 1;
    bval = get_value(criterion,param1,we,tw);

    // run to the right most
    while(--N > 0)
    {
        update_weight(j,i,we,weight);
        val = get_value(criterion,param1,we,tw);

        // move to the right
        pos1 = pos2;
        j = *(sort_id += 2);
        i = sort_id[1];
        pos2 = input[j][i];

        // check if better
        if(val < bval)
        {
            bval = val;
            bthresh = 0.5 * (pos1 + pos2);
        }
    }

    update_weight(j,i,we,weight);
    val = get_value(criterion,param1,we,tw);

    // check if better
    if(val < bval)
    {
        bval = val;
        bthresh = pos2 + 1;
    }

    result[0] = bthresh;
    result[1] = bval;
}
