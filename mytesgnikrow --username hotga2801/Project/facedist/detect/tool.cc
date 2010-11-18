#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

#include "maincore.h"
#include "tool.h"

// USED
// Read a .ppm file into memory, and return the data.  
unsigned char *readim( int *width, int *height, int *level, char *fname )
{
    char str[STRSIZE];
    int i, j, ct;
    FILE *file;
    unsigned char *buf;

    file = fopen( fname, "rb" );
    if( !file )
        return FALSE;

    fgets( str, STRSIZE, file );
    if( strncmp( str, "P6", 2 ) ) {
        return FALSE; 
    }
    do {
        fgets( str, STRSIZE, file );
    } while( str[0] == '#' );
    sscanf( str, "%d%d", width, height );
    do {
        fgets( str, STRSIZE, file );
    } while( str[0] == '#' );
    sscanf( str, "%d", level );
    buf = (unsigned char*)
             malloc( sizeof(unsigned char) * (*width) * (*height) * 3 );

    fread( buf, sizeof(unsigned char), (*width) * (*height) * 3, file );
    return buf;
}

BOOL writeim( unsigned char *buf, int width, 
                    int height, int level, char *fname )
{
    int i;
    FILE *file;

    file = fopen( fname, "w" );
    if( file == NULL )
        return FALSE;

    fprintf( file, "P5\n%d %d\n%d\n", width, height, level );
    for( i=0; i<width*height*3; i++ )
        fprintf( file, "%c", buf[i] );
    fclose( file );

    return TRUE;
}

void rgbtohvc( double r, double g, double b, double *h, double *v, double *c )
{
  double x, y, z, xc, zc;
  double h1, h2, h3, m1, m2, m3;
  double s1, s2;
  double costh, sinth, tmp;
  double aa = 8.880, bb = 0.966, cc = 8.025, dd = 2.558;

/* Adams Color System (L*a*b* ULCS) */
  x = 0.608 * r + 0.174 * g + 0.200 * b;
  y = 0.299 * r + 0.587 * g + 0.144 * b;
  z =             0.066 * g + 1.112 * b;
  xc = 1.020 * x;
  zc = 0.847 * z;
  h3 = V(y);
  h1 = V(xc) - h3;
  h2 = V(zc) - h3;
  m1 = h1;
  m2 = 0.4 * h2;
/*  m3 = 0.23 * h3; */
/* MTM correction */
  costh = m1 / (tmp = sqrt(m1 * m1 + m2 * m2));
  sinth = m2 / tmp;
  s1 = (aa + bb * costh) * m1;
  s2 = (cc + dd * sinth) * m2;
  m3 = V(y);
/*
fprintf(stderr, "xyz=%f %f %f\n", x, y, z);
fprintf(stderr, "s1 s2 m3=%f %f %f\n", s1, s2, m3);
fprintf(stderr, "h1 h2 h3=%f %f %f\n", h1, h2, h3);
fprintf(stderr, "xc zc=%f %f\n", xc, zc);
fprintf(stderr, "V(xc) V(zc)=%f %f\n", V(xc), V(zc));
*/
  if (h)
    *h = atan2(s2, s1);
  if (v)
    *v = m3;
  if (c)
    *c = sqrt(s1 * s1 + s2 * s2);
}

void hvctorgb( double h, double v, double c, double *r, double *g, double *b )
{
  double s1, s2;
  double h1, h2, h3;
  double s1t, s2t, ds1, ds2;
  double dh1, dh2;
  double A, B, C, D;
  double x, y, z;
  double tmp;

  s1 = c * cos(h);
  s2 = c * sin(h);
/* newton method */
  h1 = 1.0;
  h2 = 1.0; /* initial values */
  h3 = v;
  s1t = h1 * (8.88 + 0.966 * h1 / (tmp = sqrt(h1 * h1 + 0.16 * h2 * h2)));
  s2t = h2 * (3.21 + 0.4092 * h2 / tmp);
  do {
    ds1 = s1t - s1;
    ds2 = s2t - s2;
    A = 8.88 + 0.966 * h1 / (tmp = sqrt(h1 * h1 + 0.16 * h2 * h2))
      + h1 * (-0.966 * h1 * h1 / (tmp * (h1 * h1 + 0.16 * h2 * h2))
	      + 0.966 / tmp);
    B = -0.155456 * h1 * h1 * h2 / (tmp * (h1 * h1 + 0.16 * h2 * h2));
    C = -0.4092 * h1 * h2 * h2 / (tmp * (h1 * h1 + 0.16 * h2 * h2));
    D = 3.21 + 0.4092 * h2 / tmp
      + h2 * (-0.065472 * h2 * h2 / (tmp * (h1 * h1 + 0.16 * h2 * h2))
	      + 0.4092 / tmp);
    if ((tmp = A * D - B * C) == 0.0) {
      fprintf(stderr, "cannot calculate inverse of matrix.\n");
      exit(1);
    }
    dh1 = (ds1 * D - ds2 * B) / tmp;
    dh2 = (-ds1 * C + ds2 * A) / tmp;
    h1 -= dh1;
    h2 -= dh2;
    s1t = h1 * (8.88 + 0.966 * h1 / (tmp = sqrt(h1 * h1 + 0.16 * h2 * h2)));
    s2t = h2 * (3.21 + 0.4092 * h2 / tmp);
    tmp = ((s1 - s1t) * (s1 - s1t) + (s2 - s2t) * (s2 - s2t));
  } while (tmp > E);
  x = 0.98 * FINV(h1 + h3);
  y = FINV(h3);
  z = 1.181 * FINV(h2 + h3);
/*
fprintf(stderr, "xyz=%f %f %f\n", x, y, z);
fprintf(stderr, "s1 s2 h3=%f %f %f\n", s1, s2, h3);
fprintf(stderr, "h1 h2 h3=%f %f %f\n", h1, h2, h3);
fprintf(stderr, "V(xc) V(zc)=%f %f\n", h1 + h3, h2 + h3);
fprintf(stderr, "xc zc=%f %f\n", FINV(h1 + h3), FINV(h2 + h3));
*/
  if (r)
    *r =  1.907 * x - 0.533 * y - 0.288 * z;
  if (g)
    *g = -0.983 * x + 1.998 * y - 0.0281 * z;
  if (b)
    *b =  0.0583 * x - 0.1186 * y + 0.901 * z;
}


