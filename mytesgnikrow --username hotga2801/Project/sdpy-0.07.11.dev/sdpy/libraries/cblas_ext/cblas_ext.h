/*
 * cblas_ext: An extension of CBLAS
 * Author: Minh-Tri Pham
 * Copyright (2007) Minh-Tri Pham. All rights reserved.
 */
#ifndef CBLAS_EXT_H
#define CBLAS_EXT_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ===========================================================================
 * My CBLAS' extra subroutines: Prototypes for level 1 BLAS routines
 * ===========================================================================
 */

/*
 * Routines with standard 1 prefix (d)
 */

// sum of all elements in X
double cblas_dsum( int N, double *X, int incX );

// sum of squares of all elements in X
double cblas_dsum2( int N, double *X, int incX );



// These are smart routines, they optimize almost all special cases
// Y += a X^2
void cblas_dax2py(int N,
	double alpha, double *X, int incX,
	double *Y, int incY );

#if defined (HAS_ATLAS)
// These are smart routines, they optimize almost all special cases
// Y = a X + b Y + c
void cblas_daxpbypc(int N,
	double alpha, double *X, int incX,
	double beta, double *Y, int incY,
	double gamma );

// These are smart routines, they optimize almost all special cases
// Y = a X + b
void cblas_daxpb(int N,
	double alpha, double *X, int incX,
	double beta, double *Y, int incY );

// These are smart routines, they optimize almost all special cases
// Y += a
void cblas_dadd(int N,
	double alpha, double *Y, int incY );

// These are smart routines, they optimize almost all special cases
// Y = a X
void cblas_dscale(int N,
	double alpha, double *X, int incX,
	double *Y, int incY );


/*
 * ===========================================================================
 * My CBLAS' extra subroutines: Prototypes for level 1 BLAS routines
 * ===========================================================================
 */

/*
 * Routines with standard 4 prefixes (s, d, c, z)
 */

// These are smart routines, they optimize almost all special cases
// Y = aX + bY
void cblas_saxpby(const int N, const float alpha, const float *X,
                  const int incX, const float beta, float *Y, const int incY);
void cblas_daxpby(const int N, const double alpha, const double *X,
                  const int incX, const double beta, double *Y, const int incY);
void cblas_caxpby(const int N, const void *alpha, const void *X,
                  const int incX, const void *beta, void *Y, const int incY);
void cblas_zaxpby(const int N, const void *alpha, const void *X,
                  const int incX, const void *beta, void *Y, const int incY);

// X = a
void cblas_sset
   (const int N, const float alpha, float *X, const int incX);
void cblas_dset
   (const int N, const double alpha, double *X, const int incX);
void cblas_cset
   (const int N, const void *alpha, void *X, const int incX);
void cblas_zset
   (const int N, const void *alpha, void *X, const int incX);
   
#endif

#ifdef __cplusplus
}  // end of extern "C"
#endif


#endif
