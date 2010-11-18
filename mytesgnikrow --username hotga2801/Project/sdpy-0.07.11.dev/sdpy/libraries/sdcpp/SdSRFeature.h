#ifndef __SD_SPARSE_RECT_FEATURE_H
#define __SD_SPARSE_RECT_FEATURE_H

// Suppose we have 2 classes c: positive and negative.
// Suppose the vector-integrated version of an image of size 'len'-by-'len'
// is a random vector y.
// Let 'stats' be a pointer to the following info (all vectorized and concat'ed):
//   (y | c = 0): total weight, mean, covariance
//   (y | c = 1): total weight, mean, covariance
// Let d = len*len
// ofs = array([0,1,1+d])
// size = array([1,d,d^2])
//
// Now consider the following features called Sparse Rectangular features:
//    H = \sum_{k=1}^{nrects} a_k \pi(x_k,y_k,w_k,h_k)
// This feature can be considered as a len^2-dimensional sparse vector.
// Let ind denotes the set of nind indices of non-zero elements,
// and A denotes their coefficients.
// This function projects the statistics from high-dim to 1D
// using a Sparse Rectangular feature as the direction of projection.
// WARNING: NO GUARRANTEE THAT stats1d[1] < stats1d[4]
//
// Results are stored in an array of 6 doubles
// Including:
//      stats1d[0]: total weight of feature value given class 0
//      stats1d[1]: mean value of feature value given class 0
//      stats1d[2]: standad deviation of feature value given class 0
//      stats1d[3]: total weight of feature value given class 1
//      stats1d[4]: mean value of feature value given class 1
//      stats1d[5]: standad deviation of feature value given class 1
void project_SR_with_stats( int *ofs, int *size, double *stats,
    int nind, int *ind, double *A,
    double *stats1d );

// Suppose we have 2 classes c: positive and negative.
// Suppose the vector-integrated version of an image of size 'len'-by-'len'
// is a random vector y.
// Let 'stats' be a pointer to the following info (all vectorized and concat'ed):
//   (y | c = 0): total weight, mean, covariance
//   (y | c = 1): total weight, mean, covariance
// Consider 'criterion' and 'param1' described in sdGTSolve().
// Let d = len*len
// ofs = array([0,1,1+d])
// size = array([1,d,d^2])
//
// Now consider the following features called Sparse Rectangular features:
//    H = \sum_{k=1}^{nrects} a_k \pi(x_k,y_k,w_k,h_k)
// This function finds the 'best' Sparse Rectangular feature based on
// sdGTSolve()'s criterion, after projecting y to the 1D feature-value space.
// The underlying algorithm is Differential Evolution. It runs for 'nseconds' seconds.
//
// Results are stored in both out_ints (for integers) and out_doubles (for doubles)
// Including:
//      out_ints[0]: the number of non-zero elements in the SR feature (<= 4*nrects)
//      out_ints[1]: the number of generations used
//      out_ints[2]: the number of genomes used
//      out_ints[3]: the number of valid genomes used
//      out_ints[4] and the rest: indices to each non-zero elements, ascending order
//      out_doubles[0]: threshold value of the best genome
//      out_doubles[1]: 'error' of the best genome
//      out_doubles[2] and the rest: coefficients of each non-zero elements
void sdBestSR( int len, int *ofs, int *size, double *stats,
    int criterion, double param1,
    int nrects, double nseconds,
    int *out_ints, double *out_doubles );

#endif
