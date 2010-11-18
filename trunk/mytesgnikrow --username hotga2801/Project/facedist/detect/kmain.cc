#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <sys/types.h>
#ifndef WINNT
#include <sys/time.h>
#include <unistd.h>
#endif

//#include <dmedia/vl.h>
//#include <dmedia/vl_vino.h>

//#include <X11/X.h>
//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/keysym.h>
//#include <Xm/MwmUtil.h>

//#include <gl/gl.h>

#include "maincore.h"
#include "tool.h"
#include "ip.h"

extern "C" {
#include "detect.h"
}
//VLBuffer buf = NULL;
int gXsize, gYsize;


/* global variables */
unsigned char *gpInbuf;
int gWidth, gHeight;
int gLevel;
unsigned char *gpOutbuf;
int *bbuf;
double gEx;
double gEy; 
double gExx;
double gEyy;
double gExy;
double gA11;
double gA12;
double gA22;
double gDet;
int gCount;
int gMode;

int gRegionCnt;
RectREC gRect[1000];


//void drawFrame( VLServer, VLEvent*, void* );
void DetectFace( unsigned char* );
unsigned long *Expansion4( unsigned long*, int, int );
unsigned char *ExtractRegion( unsigned char*, int );
extern BOOL BuildModel( void );
extern void CreateControlPanel( int, char** );

#ifdef notdef
void main( int argc, char **argv )
{

  if( fork()== 0 )
    CreateControlPanel( argc, argv );

  BuildModel();
 
//  Display *dpy;
  VLControlValue val;
//  int x, y;
//  unsigned int  w, h, bw, d;
//  Window dummyWin;

//  dpy = XOpenDisplay("");

  VLServer svr = vlOpenVideo( "" );

#if 0
  Window vwin = XCreateSimpleWindow( dpy, RootWindow(dpy, 0), 
			 10, 10, 312, 214, 0, 
			 BlackPixel( dpy, DefaultScreen(dpy) ),
		         BlackPixel( dpy, DefaultScreen(dpy) ) );
  XMapWindow( dpy, vwin );
  XFlush( dpy );
#endif

  VLNode src = vlGetNode( svr, VL_SRC, VL_VIDEO, VL_ANY ); 
  VLNode drn = vlGetNode( svr, VL_DRN, VL_MEM, VL_ANY ); 

#if 0
  VLDevList devlist;
  int count = vlGetDeviceList( svr, &devlist ); 
  printf( "count = %d\n", count );
#endif

  VLPath path;
  if((path = vlCreatePath( svr, VL_ANY, src, drn )) < 0) {
    printf( "path = %d\n", path );
    exit(1);
  }

  vlSetupPaths( svr, (VLPathList)&path, 1, VL_SHARE, VL_SHARE );
  
  val.intVal = VL_PACKING_RGB_8;
  vlSetControl( svr, path, drn, VL_PACKING, &val );

  vlGetControl( svr, path, drn, VL_SIZE, &val );
  gXsize = val.xyVal.x;
  gYsize = val.xyVal.y;

  prefsize( gXsize, gYsize );
  long win = winopen( "Sample" );
  RGBmode();
  pixmode( PM_TTOB, 1 );
  gconfig();

printf( "gXsize = %d, gYsize = %d\n", gXsize, gYsize );

#if 0
  val.intVal = vwin;
  vlSetControl( svr, path, drn, VL_WINDOW, &val );

  XTranslateCoordinates( dpy, vwin, RootWindow( dpy, DefaultScreen(dpy) ),
			     0, 0, &x, &y, &dummyWin );

  val.xyVal.x = x;
  val.xyVal.y = y;
  vlSetControl( svr, path, drn, VL_ORIGIN, &val );
  XGetGeometry( dpy, vwin, &dummyWin, &x, &y, &w, &h, &bw, &d );
  val.xyVal.x = w;
  val.xyVal.y = h;
  vlSetControl( svr, path, drn, VL_SIZE, &val );

#endif

  buf = vlCreateBuffer( svr, path, drn, 10);
  vlRegisterBuffer( svr, path, drn, buf );
  int mask = VLTransferCompleteMask | 
                         VLStreamPreemptedMask | VLTransferFailedMask;
  vlSelectEvents( svr, path, mask );
  vlAddCallback( svr, path, mask, drawFrame, NULL );
  
  vlBeginTransfer( svr, path, 0, NULL );

 vlMainLoop();


#if 0
  VLInfoPtr info;
  unsigned char *dataPtr, *lastdataPtr = NULL;

int icount = 0;

  while(1){
    while (info = vlGetLatestValid( svr, buf )) {

	dataPtr = (unsigned char*)vlGetActiveRegion( svr, buf, info );

#if 0
icount++;
if( icount==20 ) {
  unsigned char *buf = (unsigned char*)malloc( gXsize*gYsize*3 );
  for( int i=0; i<gXsize*gYsize; i++ ) {
      *(buf+i*3) = *(dataPtr+i*4+3);
      *(buf+i*3+1) = *(dataPtr+i*4+2);
      *(buf+i*3+2) = *(dataPtr+i*4+1);
  }
  writeim( buf, gXsize, gYsize, 255, "xxx.xxx" ); 
  free( buf );
}
#endif

	DetectFace( dataPtr );

        //	lrectwrite(0, 0, gXsize-1, gYsize-1, (ulong*)dataPtr);
	vlPutFree( svr, buf );

    }
  }
#endif


  vlEndTransfer( svr, path );
  vlDestroyPath( svr, path );
  vlCloseVideo( svr );

}
#endif

void SelectWork( void )
{
} 

#ifdef sparc
#ifndef solaris
#ifndef WINNT
extern "C" {
int gettimeofday(struct timeval *tp, struct timezone *tzp);
}
#endif
#endif
#endif

#ifndef WINNT
struct timeval *StartTimer( void )
{
  struct timeval *stimer = (struct timeval*)malloc( sizeof(struct timeval) );
  gettimeofday( stimer ,NULL);
  return stimer;
}

void StopTimer( char *str, struct timeval *stime )
{
  struct timeval thistime;
  gettimeofday(&thistime,NULL);
  double diff=1e-6*(thistime.tv_usec-stime->tv_usec)+
    (thistime.tv_sec-stime->tv_sec);
  //  fprintf( stderr, str );
  //  fprintf(stderr,"     FPS=%.3f  SPF=%.3f\n",1.0/diff,diff);
  free( stime );
}
#endif

#ifdef notdef
void drawFrame( VLServer svr, VLEvent *ev, void *data )
{
  static int count = 0;

  VLInfoPtr info;
  char *dataPtr, *lastdataPtr = NULL;

  if( gMode&MOD_EXIT )
    exit(1);


  switch( ev->reason )
  {
	case VLTransferComplete:
           while (info = vlGetLatestValid( svr, buf )) {
	     dataPtr = (unsigned char*)vlGetActiveRegion( svr, buf, info );
	     DetectFace( dataPtr );
             //	     lrectwrite(0, 0, gXsize-1, gYsize-1, (ulong*)dataPtr);

#if 0
linewidth( 10 );

     for( int i=0; i<gRegionCnt; i++ ) {
        int x1 = gRect[i].sx*2;
        int x2 = gRect[i].ex*2;
        int y1 = gRect[i].sy*2;
        int y2 = gRect[i].ey*2;


printf( "%d  %d  %d  %d\n", x1, x2, y1, y2 );

	int vector1[2] = { x1, y1 };
	int vector2[2] = { x2, y1 };
	int vector3[2] = { x2, y2 };
	int vector4[2] = { x1, y2 };

	bgnclosedline();
	v2i( vector1 );
	v2i( vector2 );
	v2i( vector3 );
	v2i( vector4 );
	endclosedline();
    }
#endif

#if 0
	   count++;
	   if( count > 10 )
	     exit(2);
#endif
	     vlPutFree( svr, buf );
	   }
	break;
   }
}	
#endif

void DetectFace( unsigned char *pInbuf )
{
#ifndef WINNT
    struct timeval *timer0 = StartTimer();
#endif

    int i, j, tmp, tmp1, tmp2, tmp3, tmp4;
    int delta;
    unsigned int ct, sum, size;
    unsigned char r, g;
    double xEx, yEy;
    unsigned char *ptr;
    unsigned char *ptrct, *endptr;
    unsigned long  *pWorkBuf, *pWorkBuf2;
    int width = (gXsize+1)/2, height = (gYsize+1)/2;
//    int width = gXsize, height = gYsize, level;
    char name[STRSIZE];
 
//    if( (pInbuf = readim( &width, &height, &level, pFname )) == NULL )
//        return FALSE;

    size = width*height;
    pWorkBuf = (unsigned long*)calloc( sizeof(unsigned long), size );

//    pWorkBuf2 = (unsigned long*)calloc( sizeof(unsigned long), size );

    delta = gXsize*4;
    ptr = pInbuf;
    endptr = ptr+delta*gYsize;
    ptrct = ptr+delta;

#if 0
static double Ex=0;
static double Ey=0; 
static double Exx=0;
static double Eyy=0;
static double Exy=0;
static int count=0;
#endif 




#ifndef WINNT
    struct timeval *timer = StartTimer();
#endif

    i = 0;
    while( ptr<endptr ) {
       for( ; ptr<ptrct; ptr+=8, i++ ) {
            sum = ( r = *(ptr+3) ) + ( g = *(ptr+2) ) + *(ptr+1);
            if( sum < 50 )
                continue;
            xEx = ((double)r)/sum - gEx;
            yEy = ((double)g)/sum - gEy;
            if( xEx*(xEx*gA22-yEy*gA12)+yEy*yEy<gDet ) {
                *(pWorkBuf+i) = 1;

#if 0
		  double dr = ((double)r)/sum;
		  double dg = ((double)g)/sum;
		  Ex += dr;
		  Ey += dg;
		  Exx += dr*dr;
		  Eyy += dg*dg;
		  Exy += dr*dg;
		  count++;
#endif
	      }
	  }
       ptr = ptrct+delta;
       ptrct = ptr+delta;
    }
#ifndef WINNT
    StopTimer( "\nSkinColor", timer );
#endif
                                                // adjust covariance
#if 0
    if( count > 50000 ) {
#if 1
      gEx = gEx*2/3 + Ex/count/3;
      gEy = gEy*2/3 + Ey/count/3;
      gExx = gExx*2/3 + Exx/count/3;
      gEyy =gEyy*2/3 + Eyy/count/3;
      gExy =gExy*2/3 + Exy/count/3;
      gA11 = gExx-gEx*gEx;
      gA12 = gExy-gEx*gEy;
      gA22 = gEyy-gEy*gEy;
      gDet = 2*(gA11*gA22-gA12*gA12);
#else
      gEx = Ex/count;
      gEy = Ey/count;
      gExx = Exx/count;
      gEyy = Eyy/count;
      gExy = Exy/count;
      gA11 = gExx-gEx*gEx;
      gA12 = gExy-gEx*gEy;
      gA22 = gEyy-gEy*gEy;
      gDet = 2*(gA11*gA22-gA12*gA12);
#endif
      gA12 = 2*gA12/gA11;
      gA22 /= gA11;
      gDet /= gA11;
    printf( "count = %d\n", count );
    printf( "gEx = %f\n", gEx );
    printf( "gEy = %f\n", gEy );
    printf( "gExx = %f\n", gExx );
    printf( "gEyy = %f\n", gEyy );
    printf( "gExy = %f\n",gExy );
      Ex=0;
      Ey=0; 
      Exx=0;
      Eyy=0;
      Exy=0;
      count=0;
    }
#endif

    pWorkBuf = Expansion4( pWorkBuf, width, height );

    ClassfyImage( pWorkBuf, width, height );

#if 0
  int numFaces   ;
  struct FaceLocation *locArray;
  unsigned char *pBuf = (unsigned char*)malloc( gXsize*gYsize );
  for( i=0; i<gXsize*gYsize; i++ ) {
    unsigned char *tmp = pInbuf+i*4+1;
    *(pBuf+i) = 0.299*(*(tmp+2))+0.587*(*(tmp+1))+0.114*(*tmp);
  }
  free( pBuf );			      
    //  numFaces = Xavier_FindAllFaces( gXsize, gYsize, pBuf, &locArray );
  if(numFaces==0) 
    fprintf(stderr,"No faces found.\n");
  else
  {
    fprintf(stderr,"%d faces found:\n",numFaces);
    for (i=0; i<numFaces; i++) {
      fprintf(stderr,"  (%d,%d)-(%d,%d)\n",
              locArray[i].x1,locArray[i].y1,
              locArray[i].x2,locArray[i].y2);
    }
    free( (char*)locArray );
  }
#endif

#if 1
    int numFaces;
    struct FaceLocation *locArray=NULL;
    for( i=0; i<gRegionCnt; i++ ) {
      unsigned char *pBuf = ExtractRegion( pInbuf, i );
      int xsize = (gRect[i].ex-gRect[i].sx+1)*2;
      int ysize = (gRect[i].ey-gRect[i].sy+1)*2;

#ifndef WINNT
      struct timeval *timer = StartTimer();
#endif
      numFaces = Xavier_FindAllFaces( xsize, ysize, pBuf, &locArray );
#ifndef WINNT
      StopTimer( "NeuralNetwork", timer );
#endif

      fprintf(stderr,"%d faces found:\n",numFaces);
      for (int ii=0; ii<numFaces; ii++) {
	fprintf(stderr,"  (%d,%d)-(%d,%d)\n",
		locArray[ii].x1,locArray[ii].y1,
		locArray[ii].x2,locArray[ii].y2);
      }

      int tmp0 = gRect[i].sy*2*gXsize + gRect[i].sx*2;
      for( int k=0; k<numFaces; k++ ) { 
	tmp1 = tmp0 + locArray[k].y1*gXsize + locArray[k].x1;
	tmp2 = tmp0 + locArray[k].y1*gXsize + locArray[k].x2;
	tmp3 = tmp0 + locArray[k].y2*gXsize + locArray[k].x1;
	tmp4 = tmp0 + locArray[k].y2*gXsize + locArray[k].x2;

	unsigned char *pTmp;
	for( pTmp=pInbuf+tmp1*4; pTmp<=pInbuf+tmp2*4; pTmp+=4 ) {
	  *(pTmp+1) = 0;
	  *(pTmp+2) = 0xFF;
	  *(pTmp+3) = 0;
	}
	for( pTmp=pInbuf+tmp3*4; pTmp<=pInbuf+tmp4*4; pTmp+=4 ) {
	  *(pTmp+1) = 0;
	  *(pTmp+2) = 0xFF;
	  *(pTmp+3) = 0;
	}
	for( pTmp=pInbuf+tmp1*4; pTmp<=pInbuf+tmp3*4; pTmp+=(4*gXsize) ) {
	  *(pTmp+1) = 0;
	  *(pTmp+2) = 0xFF;
	  *(pTmp+3) = 0;
	}
	for( pTmp=pInbuf+tmp2*4; pTmp<=pInbuf+tmp4*4; pTmp+=(4*gXsize) ) {
	  *(pTmp+1) = 0;
	  *(pTmp+2) = 0xFF;
	  *(pTmp+3) = 0;
	}
      }
      if( locArray ) {
	free( (char*)locArray );
	locArray = NULL;
      }
      free( pBuf );
    }
#endif

#if 1
    for( i=0; i<gRegionCnt; i++ ) {
        tmp1 = gRect[i].sy*gXsize*2 + gRect[i].sx*2;
        tmp2 = gRect[i].sy*gXsize*2 + gRect[i].ex*2;
        tmp3 = gRect[i].ey*gXsize*2 + gRect[i].sx*2;
        tmp4 = gRect[i].ey*gXsize*2 + gRect[i].ex*2;

	unsigned char *pTmp;
	for( pTmp=pInbuf+tmp1*4; pTmp<=pInbuf+tmp2*4; pTmp+=4 ) {
	    *(pTmp+1) = 0;
	    *(pTmp+3) = 0xFF;
            *(pTmp+2) = 0;
	}
	for( pTmp=pInbuf+tmp3*4; pTmp<=pInbuf+tmp4*4; pTmp+=4 ) {
	    *(pTmp+1) = 0;
	    *(pTmp+3) = 0xFF;
            *(pTmp+2) = 0;
	}
	for( pTmp=pInbuf+tmp1*4; pTmp<=pInbuf+tmp3*4; pTmp+=(4*gXsize) ) {
	    *(pTmp+1) = 0;
	    *(pTmp+3) = 0xFF;
            *(pTmp+2) = 0;
        }
	for( pTmp=pInbuf+tmp2*4; pTmp<=pInbuf+tmp4*4; pTmp+=(4*gXsize) ) {
	    *(pTmp+1) = 0;
	    *(pTmp+3) = 0xFF;
            *(pTmp+2) = 0;
        }
     }
#endif

//  free( pInbuf );
    free( pWorkBuf );
//  free( pWorkBuf2 );

#ifndef WINNT
    StopTimer( "DetectFace", timer0 );
#endif
}

unsigned char *ExtractRegion( unsigned char *pInbuf, int num )
{
#ifndef WINNT
  struct timeval *timer = StartTimer();
#endif

  int xsize = (gRect[num].ex-gRect[num].sx+1)*2;
  int ysize = (gRect[num].ey-gRect[num].sy+1)*2;

  unsigned char *pBuf = (unsigned char*)malloc( xsize*ysize );
  unsigned char *ptr;
  int tmp = gRect[num].sy*2*gXsize + gRect[num].sx*2;
  int delta = (gXsize-xsize)*4;
  int i=0;
  for( ptr=pInbuf+tmp*4; i<xsize*ysize; ptr+=delta ) {
    for( int j=0; j<xsize; i++, j++, ptr+=4 ) {
	*(pBuf+i) = (unsigned char)
          (0.299*(*(ptr+3))+0.587*(*(ptr+2))+0.114*(*(ptr+1)));
//	*(pBuf+i) = *(ptr+2);
      }
  }

#ifndef WINNT
  StopTimer( "ExtractRecgion", timer );
#endif
  return pBuf;
}

// USED
// Given a binary image, return a new image such that for each active
// pixel in the input, that pixel's 8 neighbors are also active in the
// new image.
unsigned long *Expansion4( unsigned long *pInbuf, int width, int height )
{
#ifndef WINNT
  struct timeval *timer = StartTimer();
#endif

    int i, j;
    unsigned long *pBuf, *pCur = pInbuf;
    unsigned long *pPtr1, *pPtr2, *pPtr3;

    pBuf = (unsigned long*)calloc( sizeof(unsigned long), width*height );

    pPtr2 = pBuf;
    pPtr3 = pPtr2 + width;

    if( *pCur ) {
        *pPtr2 = 1;
	*pPtr3 = 1;
        *( pPtr2 + 1 )= 1;
	*( pPtr3 + 1 )= 1;
    }
    for( j=1; j<width-1; j++ ) {
        if( *(++pCur) ) {
           *pPtr2 = 1;
	   *pPtr3 = 1;
	   *(++pPtr2) = 1;
	   *(++pPtr3) = 1;
	   *( pPtr2 + 1 )= 1;
	   *( pPtr3 + 1 )= 1;
	}
	else {
	  pPtr2++;
	  pPtr3++;
	}
    }
    if( *(++pCur) ) {
        *pPtr2 = 1;
	*pPtr3 = 1;
        *(++pPtr2) = 1;
	*(++pPtr3) = 1;
	pPtr2++;
	pPtr3++;
    }
    else {
	pPtr2 += 2;
	pPtr3 += 2;
    }
	
    pPtr1 = pBuf;
    for( i=1; i<height-1; i++ ) {
        if( *(++pCur) ) {
	    *pPtr1 = 1;
	    *pPtr2 = 1;
	    *pPtr3 = 1;
	    *( pPtr1 + 1 )= 1;
	    *( pPtr2 + 1 )= 1;
	    *( pPtr3 + 1 )= 1;
 	}
        for( j=1; j<width-1; j++ ) {
	    if( *(++pCur) ) {
	      *pPtr1 = 1;
	      *pPtr2 = 1;
	      *pPtr3 = 1;
	      *(++pPtr1) = 1;
	      *(++pPtr2) = 1;
	      *(++pPtr3) = 1;
	      *( pPtr1 + 1 )= 1;
	      *( pPtr2 + 1 )= 1;
	      *( pPtr3 + 1 )= 1;
	    }
	    else {
	      pPtr1++;
	      pPtr2++;
	      pPtr3++;
	    }
	}
	if( *(++pCur) ) {
	    *pPtr1 = 1;
	    *pPtr2 = 1;
	    *pPtr3 = 1;
	    *(++pPtr1) = 1;
	    *(++pPtr2) = 1;
	    *(++pPtr3) = 1;
	    pPtr1++;
	    pPtr2++;
	    pPtr3++;
	}
	else {
	  pPtr1 += 2;
	  pPtr2 += 2;
	  pPtr3 += 2;
	}
    }

    if( *(++pCur) ) {
        *pPtr1 = 1;
	*pPtr2 = 1;
        *( pPtr1 + 1 )= 1;
	*( pPtr2 + 1 )= 1;
    }
    for( j=1; j<width-1; j++ ) {
        if( *(++pCur) ) {
           *pPtr1 = 1;
	   *pPtr2 = 1;
	   *(++pPtr1) = 1;
	   *(++pPtr2) = 1;
	   *( pPtr1 + 1 )= 1;
	   *( pPtr2 + 1 )= 1;
	}
	else {
	  pPtr1++;
	  pPtr2++;
	}
    }
    if( *(++pCur) ) {
        *pPtr1 = 1;
	*pPtr2 = 1;
        *(++pPtr1) = 1;
	*(++pPtr2) = 1;
    }

    free( pInbuf );

#ifndef WINNT
    StopTimer( "Expansion", timer );
#endif
    return pBuf;
}









