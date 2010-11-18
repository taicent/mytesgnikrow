#include "plat_det.h"
#include "cblas_ext.h"

#if defined (HAS_ATLAS)
#include <cblas.h>
#endif

// sum of all elements in X
double cblas_dsum( int N, double *X, int incX )
{
	double sum = 0;
	if(incX == 1)
	{
#if defined (TARGET_COMPILER_GCC) && defined (TARGET_COMPILER_SUPPORT_SSE2)
	    if(hasSSE2())
	    {
            while(N != 0 && (((int)X)&15) != 0) // align it
            {
                sum += X[0];
                ++X;
                --N;
            }
            while(N&7)
            {
                --N;
                sum += X[N];
            }
            if(N > 0)
            {
                asm(
                    "leal	(%0), %%edx;"
                    "xorpd	%%xmm7, %%xmm7;"
                    "movsd	(%1), %%xmm7;"
                    "xorpd	%%xmm6, %%xmm6;"
                    "xorpd	%%xmm5, %%xmm5;"
                    "xorpd	%%xmm4, %%xmm4;"
                    : : "r" (X), "r" (&sum) : "%xmm7", "%xmm6", "%xmm5", "%xmm4", "%edx" );
                N >>= 3;
                do
                {
                    asm(
                        "addpd	(%%edx), %%xmm7;"
                        "addpd	16(%%edx), %%xmm6;"
                        "addpd	32(%%edx), %%xmm5;"
                        "addpd	48(%%edx), %%xmm4;"
                        "addl	$64,%%edx;"
                        : : : "%xmm4", "%xmm5", "%xmm6",  "%xmm7", "%edx" );
                } while(--N > 0);
                asm(
                    "addpd	%%xmm6, %%xmm7;"
                    "addpd	%%xmm4, %%xmm5;"
                    "addpd	%%xmm5, %%xmm7;"
                    "movapd	%%xmm7, %%xmm6;"
                    "shufpd	$0x1, %%xmm5, %%xmm6;"
                    "addsd	%%xmm6, %%xmm7;"
                    "movsd	%%xmm7, (%0);"
                    : : "r" (&sum) : "%xmm7", "%xmm6", "%xmm5", "%xmm4", "%edx" );
            }
            return sum;
	    }
#endif

	    while(--N >= 0) sum += X[N];
	}
	else while(--N >= 0)
	{
		sum += X[0];
		X += incX;
	}
	return sum;
}


// sum of squares of all elements in X
double cblas_dsum2( int N, double *X, int incX )
{
	double sum = 0;
	if(incX == 1)
	{
#if defined (TARGET_COMPILER_GCC) && defined (TARGET_COMPILER_SUPPORT_SSE2)
	    if(hasSSE2())
	    {
            while(N != 0 && (((int)X)&15) != 0) // align it
            {
                sum += X[0]*X[0];
                ++X;
                --N;
            }
            while(N&7)
            {
                --N;
                sum += X[N]*X[N];
            }
            if(N > 0)
            {
                asm(
                    "leal	(%0), %%edx;"
                    "xorpd	%%xmm7, %%xmm7;"
                    "movsd	(%1), %%xmm7;"
                    : : "r" (X), "r" (&sum) : "%xmm7", "%edx" );
                N >>= 3;
                do
                {
                    asm(
                        "movapd	(%%edx), %%xmm6;"
                        "mulpd	%%xmm6, %%xmm6;"
                        "addpd	%%xmm6, %%xmm7;"
                        "movapd	16(%%edx), %%xmm6;"
                        "mulpd	%%xmm6, %%xmm6;"
                        "addpd	%%xmm6, %%xmm7;"
                        "movapd	32(%%edx), %%xmm6;"
                        "mulpd	%%xmm6, %%xmm6;"
                        "addpd	%%xmm6, %%xmm7;"
                        "movapd	48(%%edx), %%xmm6;"
                        "mulpd	%%xmm6, %%xmm6;"
                        "addpd	%%xmm6, %%xmm7;"
                        "addl	$64,%%edx;"
                        : : : "%xmm6", "%xmm7", "%edx" );
                } while(--N > 0);
                asm(
                    "movapd	%%xmm7, %%xmm6;"
                    "shufpd	$0x1, %%xmm5, %%xmm6;"
                    "addsd	%%xmm6, %%xmm7;"
                    "movsd	%%xmm7, (%0);"
                    : : "r" (&sum) : "%xmm7", "%xmm6", "%xmm5" );
            }
            return sum;
	    }
#endif

	    while(--N >= 0) sum += X[N]*X[N];
	}
	else while(--N >= 0)
	{
		sum += X[0]*X[0];
		X += incX;
	}
	return sum;
}


// These are smart routines, they optimize almost all special cases
// Y += a X^2
void cblas_dax2py(int N,
	double alpha, double *X, int incX,
	double *Y, int incY )
{
	if(alpha == 0) return;
	if (incX == 1 && incY == 1)
	{
#if defined (TARGET_COMPILER_GCC) && defined (TARGET_COMPILER_SUPPORT_SSE2)
	    if( hasSSE2() && !(((int)X)&7) && !(((int)Y)&7) && !( (((int)X)^((int)Y)) & 8 ) )
	    {
            if( N && (((int)X)&8) ) // align it
            {
                Y[0] += alpha*X[0]*X[0];
                ++X;
                ++Y;
                --N;
            }
            while(N&1)
            {
                --N;
                Y[N] += alpha*X[N]*X[N];
            }
            if(N > 0)
            {
                double aa[2];
                aa[0] = aa[1] = alpha;
                asm("movupd	(%0), %%xmm7;" : : "r" (aa) : "%xmm7" );
                asm("leal	(%0), %%ecx;" : : "r" (X) : "%ecx" );
                asm("leal	(%0), %%edx;" : : "r" (Y) : "%edx" );
                N >>= 1;
                do
                {
                    asm(
                        "movapd	(%%ecx), %%xmm6;"
                        "mulpd	%%xmm6,%%xmm6;"
                        "mulpd	%%xmm7,%%xmm6;"
                        "addl	$16,%%ecx;"
                        "addpd	(%%edx),%%xmm6;"
                        "movapd	%%xmm6,(%%edx);"

                        "addl	$16,%%edx;"
                        : : : "%xmm6",  "%xmm7", "%ecx", "%edx" );
                } while(--N > 0);
            }
            return;
        }
#endif

		while(--N >= 0) Y[N] += alpha*X[N]*X[N];
	}
	else while(--N >= 0)
	{
		Y[0] += alpha*X[0]*X[0];
		X += incX;
		Y += incY;
	}
}



#if defined (HAS_ATLAS)

// These are smart routines, they optimize almost all special cases
// Y = a X + b Y + c
void cblas_daxpbypc(int N,
	double alpha, double *X, int incX,
	double beta, double *Y, int incY,
	double gamma )
{
	if(alpha == 0) cblas_daxpb( N, beta, Y, incY, gamma, Y, incY );
	else if(beta == 0) cblas_daxpb( N, alpha, X, incX, gamma, Y, incY );
	else if(gamma == 0) cblas_dscale( N, alpha, X, incX, Y, incY );
	else if (incX == 1 && incY == 1)
	{
#if defined (TARGET_COMPILER_GCC) && defined (TARGET_COMPILER_SUPPORT_SSE2)
		if( hasSSE2() && !(((int)X)&7) && !(((int)Y)&7) && !( (((int)X)^((int)Y)) & 8 ) )
		{
			if( N && (((int)X)&8) ) // align it
			{
				Y[0] = alpha*X[0]+beta*Y[0]+gamma;
				++X;
				++Y;
				--N;
			}
			while(N&1)
			{
				--N;
				Y[N] = alpha*X[N]+beta*Y[0]+gamma;
			}
			if(N > 0)
			{
				double aa[2], bb[2], cc[2];
				aa[0] = aa[1] = alpha;
				bb[0] = bb[1] = beta;
				cc[0] = cc[1] = gamma;
				asm("movupd	(%0), %%xmm5;" : : "r" (aa) : "%xmm5" );
				asm("movupd	(%0), %%xmm6;" : : "r" (bb) : "%xmm6" );
				asm("movupd	(%0), %%xmm7;" : : "r" (cc) : "%xmm7" );
				asm("leal	(%0), %%ecx;" : : "r" (X) : "%ecx" );
				asm("leal	(%0), %%edx;" : : "r" (Y) : "%edx" );
				N >>= 1;
				do
				{
					asm(
						"movapd	(%%ecx), %%xmm4;"
						"movapd	(%%edx), %%xmm3;"
						"addl	$16,%%ecx;"
						"mulpd	%%xmm5,%%xmm4;"
						"mulpd	%%xmm6,%%xmm3;"
						"addpd	%%xmm7,%%xmm4;"
						"addpd	%%xmm4,%%xmm3;"
						"movapd	%%xmm3,(%%edx);"

						"addl	$16,%%edx;"
						: : : "%xmm3", "%xmm4", "%xmm5", "%xmm6",  "%xmm7", "%ecx", "%edx" );
				} while(--N > 0);
			}
			return;
		}
#endif

		while(--N >= 0) Y[N] = alpha*X[N]+beta*Y[N]+gamma;
	}
	else while(--N >= 0)
	{
		Y[0] = alpha*X[0]+beta*Y[N]+gamma;
		X += incX;
		Y += incY;
	}
}


// These are smart routines, they optimize almost all special cases
// Y = a X + b
void cblas_daxpb(int N,
	double alpha, double *X, int incX,
	double beta, double *Y, int incY )
{
	if(alpha == 0) cblas_dset( N, beta, Y, incY );
	else if(beta == 0) cblas_dscale( N, alpha, X, incX, Y, incY );
	else if (incX == 1 && incY == 1)
	{
#if defined (TARGET_COMPILER_GCC) && defined (TARGET_COMPILER_SUPPORT_SSE2)
		if( hasSSE2() && !(((int)X)&7) && !(((int)Y)&7) && !( (((int)X)^((int)Y)) & 8 ) )
		{
			if( N && (((int)X)&8) ) // align it
			{
				Y[0] = alpha*X[0]+beta;
				++X;
				++Y;
				--N;
			}
			while(N&7)
			{
				--N;
				Y[N] = alpha*X[N]+beta;
			}
			if(N > 0)
			{
				double aa[2], bb[2];
				aa[0] = aa[1] = alpha;
				bb[0] = bb[1] = beta;
				asm(
					"movupd	(%3), %%xmm7;"
					"movupd	(%2), %%xmm6;"
					"leal	(%1), %%edx;"
					"leal	(%0), %%ecx;"
					: : "r" (X), "r" (Y), "r" (aa), "r" (bb) : "%xmm7", "%xmm6", "%ecx", "%edx" );
				N >>= 3;
				do
				{
					asm(
						"movapd	(%%ecx), %%xmm5;"
						"movapd	16(%%ecx), %%xmm4;"
						"movapd	32(%%ecx), %%xmm3;"
						"movapd	48(%%ecx), %%xmm2;"

						"mulpd	%%xmm6,%%xmm5;"
						"mulpd	%%xmm6,%%xmm4;"
						"mulpd	%%xmm6,%%xmm3;"
						"mulpd	%%xmm6,%%xmm2;"

						"addpd	%%xmm7,%%xmm5;"
						"addpd	%%xmm7,%%xmm4;"
						"addpd	%%xmm7,%%xmm3;"
						"addpd	%%xmm7,%%xmm2;"

						"movapd	%%xmm5,(%%edx);"
						"movapd	%%xmm4,16(%%edx);"
						"movapd	%%xmm3,32(%%edx);"
						"movapd	%%xmm2,48(%%edx);"

						"addl	$64,%%ecx;"
						"addl	$64,%%edx;"
						: : : "%xmm2", "%xmm3", "%xmm4", "%xmm5", "%xmm6",  "%xmm7", "%ecx", "%edx" );
				} while(--N > 0);
			}
			return;
		}
#endif

		while(--N >= 0) Y[N] = alpha*X[N]+beta;
	}
	else while(--N >= 0)
	{
		Y[0] = alpha*X[0]+beta;
		X += incX;
		Y += incY;
	}
}


// These are smart routines, they optimize almost all special cases
// Y += a
void cblas_dadd(int N,
	double alpha, double *Y, int incY )
{
	if(alpha == 0) return;
	if (incY == 1)
	{
#if defined (TARGET_COMPILER_GCC) && defined (TARGET_COMPILER_SUPPORT_SSE2)
		if( hasSSE2() && !(((int)Y)&7) )
		{
			if( N && (((int)Y)&8) ) // align it
			{
				Y[0] += alpha;
				++Y;
				--N;
			}
			while(N&7)
			{
				--N;
				Y[N] += alpha;
			}
			if(N > 0)
			{
				double aa[2];
				aa[0] = aa[1] = alpha;
				asm(
					"movupd	(%1), %%xmm7;"
					"leal	(%0), %%edx;"
					: : "r" (Y), "r" (aa) : "%xmm7", "%edx" );
				N >>= 3;
				do
				{
					asm(
						"movapd	(%%edx), %%xmm6;"
						"movapd	16(%%edx), %%xmm5;"
						"movapd	32(%%edx), %%xmm4;"
						"movapd	48(%%edx), %%xmm3;"

						"addpd	%%xmm7,%%xmm6;"
						"addpd	%%xmm7,%%xmm5;"
						"addpd	%%xmm7,%%xmm4;"
						"addpd	%%xmm7,%%xmm3;"

						"movapd	%%xmm6,(%%edx);"
						"movapd	%%xmm5,16(%%edx);"
						"movapd	%%xmm4,32(%%edx);"
						"movapd	%%xmm3,48(%%edx);"

						"addl	$64,%%edx;"
						: : : "%xmm3", "%xmm4", "%xmm5", "%xmm6",  "%xmm7", "%ecx", "%edx" );
				} while(--N > 0);
			}
			return;
		}
#endif

		while(--N >= 0) Y[N] += alpha;
	}
	else while(--N >= 0)
	{
		Y[0] += alpha;
		Y += incY;
	}
}


// These are smart routines, they optimize almost all special cases
// Y = aX
void cblas_dscale(int N,
	double alpha, double *X, int incX,
	double *Y, int incY )
{
	if(X == Y) cblas_dscal( N, alpha, Y, incY );
	else cblas_daxpby( N, alpha, X, incX, 0, Y, incY );
}


// These are smart routines, they optimize almost all special cases
// Y = aX + bY
void cblas_saxpby(const int N, const float alpha, const float *X,
                  const int incX, const float beta, float *Y, const int incY)
{
	catlas_saxpby( N, alpha, X, incX, beta, Y, incY );
}

void cblas_daxpby(const int N, const double alpha, const double *X,
                  const int incX, const double beta, double *Y, const int incY)
{
	catlas_daxpby( N, alpha, X, incX, beta, Y, incY );
}

void cblas_caxpby(const int N, const void *alpha, const void *X,
                  const int incX, const void *beta, void *Y, const int incY)
{
	catlas_caxpby( N, alpha, X, incX, beta, Y, incY );
}

void cblas_zaxpby(const int N, const void *alpha, const void *X,
                  const int incX, const void *beta, void *Y, const int incY)
{
	catlas_zaxpby( N, alpha, X, incX, beta, Y, incY );
}


// X = a
void cblas_sset
   (const int N, const float alpha, float *X, const int incX)
{
	catlas_sset( N, alpha, X, incX );
}

void cblas_dset
   (const int N, const double alpha, double *X, const int incX)
{
	catlas_dset( N, alpha, X, incX );
}

void cblas_cset
   (const int N, const void *alpha, void *X, const int incX)
{
	catlas_cset( N, alpha, X, incX );
}

void cblas_zset
   (const int N, const void *alpha, void *X, const int incX)
{
	catlas_zset( N, alpha, X, incX );
}

#endif
