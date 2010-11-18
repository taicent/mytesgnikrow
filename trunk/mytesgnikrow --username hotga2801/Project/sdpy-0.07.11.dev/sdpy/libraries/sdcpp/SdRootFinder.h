#ifndef __SD_ROOT_FINDER_H
#define __SD_ROOT_FINDER_H


// find roots (both real and complex) of the polynomial:
// f(x) = \sum_{k=0}^n a[k]*x^(n-k)
// returns the number of roots found,
// or returns 0 if all but the last coefficient are 0
// or returns -1 if all coefficients are 0
int sdFindRoots( double *a, int n, double *wr, double *wi );


#endif
