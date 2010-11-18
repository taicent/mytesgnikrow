#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

#include "maincore.h"
#include "ip.h"
#include "tool.h"

extern int *bbuf;
extern double gEx;
extern double gEy; 
extern double gExx;
extern double gEyy;
extern double gExy;
extern double gA11;
extern double gA12;
extern double gA22;
extern double gDet;
extern int gCount;
extern int gXsize;
extern int gYsize;
extern int gMode;
extern char gCovFileName[STRSIZE];
extern char gRGBFileName[STRSIZE];
extern char gMLFileName[STRSIZE];
extern int gRegionCnt;
extern RectREC gRect[100];

// USED
// Read the file with the given name, which lists image files to load
// to build a model of skin color. 
BOOL BuildModel(char *path)
{
    char *str, fname[STRSIZE];
    int i=0, ct;
    FILE *file;
    double h, v, c;
    int num;
    unsigned char *inbuf;

    gEx = 0;
    gEy = 0;
    gExx = 0;
    gEyy = 0;
    gExy = 0;
    gCount = 0;

    /* calculate covariance */ 
    file = fopen( path, "r" );
    if( file != NULL ) {
        while(1) {
           if( fgets( fname, STRSIZE, file ) ) {
                ct = strlen( fname );
                fname[ct-1] = 0;
                calcCov( fname );
            }
            else {
               fclose( file ); 
               break;
	    }
	}
    }

    if( !gCount )
        return FALSE;

    gEx /= gCount;
    gEy /= gCount;
    gExx /= gCount;
    gEyy /= gCount;
    gExy /= gCount;

    gA11 = gExx-gEx*gEx;
    gA12 = gExy-gEx*gEy;
    gA22 = gEyy-gEy*gEy;
    gDet = 2*(gA11*gA22-gA12*gA12);
    
    gA12 = 2*gA12/gA11;
    gA22 /= gA11;
    gDet /= gA11;

    //    printf( "gCount = %d\n", gCount );
    //    printf( "gEx = %f\n", gEx );
    //    printf( "gEy = %f\n", gEy );
    //    printf( "gExx = %f\n", gExx );
    //    printf( "gEyy = %f\n", gEyy );
    //    printf( "gExy = %f\n",gExy );

    return TRUE;
}

// USED
// Given an image file, load that file and add it into the skin color model
BOOL calcCov( char *fname )
{
    int i, j, ct, sum;
    double h, v, c;
    int width, height, level;
    unsigned char *inbuf;
    //    FILE *pFile;
    double nred, ngreen;

    //    printf( "%s\n", fname );

    // Read the image
    if( (inbuf = readim( &width, &height, &level, fname )) == NULL )
        return FALSE;

    // For each pixel
    for( i=0; i<height; i++ ) {
        for( j=0; j<width; j++ ) {
            ct = (width*i+j)*3;

	    // If the pixel is too dark, discard it
            sum = inbuf[ct] + inbuf[ct+1] + inbuf[ct+2];
            if( sum < 50 )
                continue;

	    // Otherwise, compute the normalized color
            nred = (double)inbuf[ct]/sum;
            ngreen = (double)inbuf[ct+1]/sum;

	    //            if( gMode == EMD_COV )
	    //                fprintf( pFile, "%f %f\n", nred, ngreen );
 
	    // Add it into the Gaussian color model
            gEx += nred;
            gEy += ngreen;
            gExx += nred*nred;
            gEyy += ngreen*ngreen;
            gExy += nred*ngreen;
            gCount++;
	}
    }
 
    free( inbuf );
    return TRUE;
}

BOOL DrawVariance( void )
{
    double i, j;
    double xEx, yEy, tmp;
    double a11 = gExx-gEx*gEx;
    double a12 = gExy-gEx*gEy;
    double a22 = gEyy-gEy*gEy;
    double inv = a11*a22-a12*a12;
    int tmp2;

    FILE *file;

    if( (file = fopen( "decfile", "w" )) == NULL ) {
        printf( "I can't open decfile.\n" );
        return FALSE;
    }
    for( i=0; i<1; i+=0.001 ) {
        for( j=0; j<1-i; j+=0.001 ) {
            xEx = j - gEx;
            yEy = i - gEy;
            tmp = (xEx*(xEx*a22-yEy*a12)+yEy*(-xEx*a12+yEy*a11))/inv;
            if( (tmp>0.49 && tmp<0.51) || (tmp>0.99 && tmp<1.01) 
                  || (tmp>1.49 && tmp<1.51) || (tmp>1.99 && tmp<2.01)
                  || (tmp>2.49 && tmp<2.51) || (tmp>2.99 && tmp<3.01)
                  || (tmp>3.49 && tmp<3.51) || (tmp>3.99 && tmp<4.01) )
                      fprintf( file, "%f %f\n", j, i );                
          

/*
            tmp2 = tmp + 0.5;
            tmp = (tmp<tmp2)?(tmp2-tmp):(tmp-tmp2);
            if(  tmp<4 && ((tmp2>0 && tmp<0.01) || (0.5-tmp<0.01)) )
                fprintf( file, "%f %f\n", j, i );                
*/
       }
    }       
    fclose( file );
    return TRUE;
}

// USED
// Given an input image in which each pixel is classified as either 
// skin colored or not, compute the connected components of each 
// region, and compute the bounding rectangle of each region.  Elongated
// regions are discarded.
void ClassfyImage( unsigned long *pBuf, int width, int height )
{
#ifndef WINNT
  struct timeval *timer = StartTimer();
#endif

    int i, j, k, ct, dummy;
    int maxValue=1, count[100000];          /* temp */
    int table[100000];
    int svalue, lvalue;

    int table0[4] = { -1, width, 1, -width };
    int table1[4] = { 3, 0, 1, 2 };
    int start0, start1, a0, a1, index, cur, x, y, sx, sy, ex, ey;
    
    table[0] = 0;
    table[1] = 1;

    for( i=0; i<width; i++ )
      *(pBuf+i) = 0;

    for( i=width*(height-1); i<width*height; i++ )
      *(pBuf+i) = 0;

    for (i=0; i<width*height; i+=width)
      *(pBuf+i) = *(pBuf+i+width-1) = 0;

  /* Operation for the first line. But I think it is not necessary to 
     consider first line, because it won't affect the whole.*/

    for( i=1; i<height; i++ ) {
        ct = width*i; 
        for( j=1; j<width; j++ ) {
            ct++;
            if( pBuf[ct] ) {
                svalue = table[ pBuf[ct-1] ];
                lvalue = table[ pBuf[ct-width] ];
	        if( svalue > lvalue ) {
                    dummy = svalue;
                    svalue = lvalue;
		    lvalue = dummy;
	        }

                if( svalue ) {
                    pBuf[ct] = svalue;
		    if( lvalue > svalue ) {
                        table[lvalue] = svalue;
                        for( k=lvalue; k<maxValue; k++ )
                            table[k] = table[ table[k] ];
                    }       
                }
                else if( lvalue ) {
                    pBuf[ct] = lvalue;
                }
                else {
                    pBuf[ct] = maxValue;
                    maxValue++;
                    table[maxValue] = maxValue;
 		}
	    }
        }
    }

#if 0
                     /* Arrange the table. Is this routine necessary? */
/*    for( i=0; i<maxValue; i++ ) {
        count[i] = 0;
        if( table[i] != i )
           table[i] = table[ table[i] ];
    }
*/
#else
    for( i=0; i<maxValue; i++ )
        count[i] = 0;
#endif
                             /* labeling */
    for( i=0; i<width*height; i++ ) {
        if( !pBuf[i] )
	    continue;
        pBuf[i] = table[ pBuf[i] ];
        count[ pBuf[i] ]++;
    }

     /* Preprocessing for eliminating the area whose size is small. */
    for( i=0; i<maxValue; i++ ) {
        if( count[i] < 100 )
            table[i] = 0;     
    }

                    /* eliminate small size area. */    
    for( i=0; i<width*height; i++ ) 
        pBuf[i] = table[ pBuf[i] ];           


#ifndef WINNT
    StopTimer( "Labeling", timer );
#endif

#if 1
#ifndef WINNT
    timer = StartTimer();
#endif
    gRegionCnt = 0;
    for( i=0; i<width*height; i++ ) {
        if( !table[ pBuf[i] ] )
	    continue;

        start0 = i;
	sx = width;
	sy = start0/width;
//fprintf(stderr,"start0 (%d,%d)\n",sx,sy);
	ex = 0;
	ey = 0;
        if( pBuf[start0+width] ){
	    start1 = start0+width;
	    index = 0;
	}
	else if( pBuf[start0+1] ){
	    start1 = start0+1;
	    index = 1;
	}
	else {
	  printf( "XXXXx\n" );
          exit(1);
	}
	
//fprintf(stderr,"start1 (%d,%d)\n",start1%width,start1/width);
	cur = a1 = start1;
	do {
	    while( pBuf[cur+table0[index]] == 0 ) {
	        index++;
	        index &= 3;
	    }
  	    cur += table0[index];
    	    y = cur/width;
	    x = cur%width;
//fprintf(stderr,"(%d,%d) ",x,y);
	    if( x < sx )
	        sx = x;
	    if( x > ex )
	        ex = x;
	    if( y > ey )
	        ey = y;
	    index = table1[index];
	    a0 = a1;
	    a1 = cur;
        }while( (a0!=start0) || (a1!=start1) ); 
//fprintf(stderr,"End\n");

	table[ pBuf[i] ] = 0;

        if( (ex-sx)>2.5*(ey-sy) || (ey-sy)>2.5*(ex-sx)  )
          continue;

	gRect[gRegionCnt].sx = sx;

	if( sy-10>0 )
	  sy -= 10;
	gRect[gRegionCnt].sy = sy;
	gRect[gRegionCnt].ex = ex;
	gRect[gRegionCnt].ey = ey;
	gRegionCnt++;

//printf( "count = %d\n", gRegionCnt );
//printf( "%d, %d, %d, %d\n", sx, sy, ex, ey );

    }
#endif



#if 0
	gRect[0].sx = 0;
	gRect[0].sy = 0;
	gRect[0].ex = gXsize-1;
	gRect[0].ey = gYsize-1;
	gRegionCnt = 1;
#endif

#ifndef WINNT
	StopTimer( "MinimumRectangle", timer );
#endif
}





