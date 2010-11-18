// This file defines my functions for interfacing with Koji Doi's 
// skin color matching code

#ifndef WINNT
extern "C" {
#endif
#ifndef NoTcl
#include <tk.h>
#endif
#include <stdlib.h>
#ifndef NoVista
#include <vista/VImage.h>
#endif
#include <math.h>
#ifndef WINNT
}
#endif

extern "C" {
#include "detect.h"
}

#include "img.hpp"
#include "kdoi.hpp"
#include "maincore.h"
#include "tool.h"
#include "ip.h"
#ifndef NoDebug
#include "debug.hpp"
#endif

#define Threshold 0.1

// These parameters define the gaussian hue model
extern double gEx;
extern double gEy; 
extern double gExx;
extern double gEyy;
extern double gExy;
extern double gA11;
extern double gA12;
extern double gA22;
extern double gDet;

#ifndef NoTcl
extern Tcl_Interp *theInterp;
#endif

BOOL BuildModel(char *path);
void ClassfyImage( unsigned long*, int ,int );
unsigned long *Expansion4( unsigned long *pInbuf, int width, int height );
extern int gRegionCnt;
extern RectREC gRect[1000];

// USED
// Load in the images with examples of skin color.  The path parameter
// gives a file name which contains a list of .ppm image files to load.
void KDoiInit(char *path) 
{
  BuildModel(path);
}

#ifndef NoTcl
// USED
// Load in the images with examples of skin color.  The path parameter
// gives a file name which contains a list of .ppm image files to load.
int KDoiInitCmd(ClientData data, Tcl_Interp *interp,
                int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: KDoiInit <path>";
    return TCL_ERROR;
  }
  KDoiInit(argv[1]);
  return TCL_OK;
}
#endif

// USED
// Given a color image as input (3 consecutive bytes per pixel),
// classify each pixel as containing skin color or not.  Then compute
// connected regions of skin color, and find their bounding rectangles.
// Mark those rectangles in the given mask image.
void KDoiColorMask(unsigned char *input, int gXsize, int gYsize,
                   ByteImage *image, int maskValue)
{
  int i, j, tmp, tmp1, tmp2, tmp3, tmp4;
  int delta;
  unsigned int ct, sum, size;
  unsigned char r, g;
  double xEx, yEy;
  unsigned char *ptr;
  unsigned char *ptrct, *endptr;
  unsigned long  *pWorkBuf, *pWorkBuf2;
  int width = (gXsize+1)/2, height = (gYsize+1)/2;
 
  size = width*height;
  pWorkBuf = (unsigned long*)calloc( sizeof(unsigned long), size );

  delta = 3*gXsize;
  ptr = input;
  endptr = ptr+delta*gYsize;
  ptrct = ptr+delta;

  i = 0;
  // Classify each pixel
  while( ptr<endptr ) {
    for( ; ptr<ptrct; ptr+=3*2, i++ ) {
      sum = ( r = *(ptr+0) ) +
        ( g = *(ptr+1) ) +
        *(ptr+2);
      if( sum < 50 )
        continue;
      xEx = ((double)r)/sum - gEx;
      yEy = ((double)g)/sum - gEy;
      if( xEx*(xEx*gA22-yEy*gA12)+yEy*yEy<gDet )
        *(pWorkBuf+i) = 1; else
          *(pWorkBuf+i) = 0;
    }
    ptr = ptrct+delta;
    ptrct = ptr+delta;
  }

  // Expand the skin color regions
  pWorkBuf = Expansion4( pWorkBuf, width, height );

  // Compute connected components, placing result in a global array
  ClassfyImage( pWorkBuf, width, height );
  free(pWorkBuf);
  //  fprintf(stderr,"Region count = %d\n",gRegionCnt);

  // Mark connected rectangles
  for (i=0; i<gRegionCnt; i++) {
    int x1=gRect[i].sx*2;
    int x2=gRect[i].ex*2;
    int y1=gRect[i].sy*2;
    int y2=gRect[i].ey*2;
    if (x1<0) x1=0;
    if (x1>=image->width) x1=image->width-1;
    if (x2<0) x2=0;
    if (x2>=image->width) x2=image->width-1;
    if (y1<0) y1=0;
    if (y1>=image->height) y1=image->height-1;
    if (y2<0) y2=0;
    if (y2>=image->height) y2=image->height-1;
    //    fprintf(stderr,"  (%d,%d) - (%d,%d)\n",x1,y1,x2,y2);
    for (int y=y1; y<=y2; y++) for (int x=x1; x<=x2; x++)
      image->data(x,y) &= maskValue;
  }
  image->Change();
}

#ifndef NoTcl
// Given a color image as input (3 consecutive bytes per pixel),
// classify each pixel as containing skin color or not.  Then compute
// connected regions of skin color, and find their bounding rectangles.
// Mark those rectangles in the given mask image.
int KDoiColorMaskCmd(ClientData data, Tcl_Interp *interp,
                     int argc, char *argv[])
{
  if (argc!=4) {
    interp->result="Usage: KDoiColorMask <photo> <mask> <maskvalue>";
    return TCL_ERROR;
  }
  extern Tcl_Interp *theInterp;
  Tk_PhotoHandle photo=Tk_FindPhoto(theInterp,argv[1]);
  ByteImage *image=FindByteImageWithName(argv[2]);
  unsigned char maskValue=atoi(argv[3]);

  Tk_PhotoImageBlock block;
  Tk_PhotoGetImage(photo,&block);

  int gXsize, gYsize;
  Tk_PhotoGetSize(photo,&gXsize,&gYsize);

  int i, j, tmp, tmp1, tmp2, tmp3, tmp4;
  int delta;
  unsigned int ct, sum, size;
  unsigned char r, g;
  double xEx, yEy;
  unsigned char *ptr;
  unsigned char *ptrct, *endptr;
  unsigned long  *pWorkBuf, *pWorkBuf2;
  int width = (gXsize+1)/2, height = (gYsize+1)/2;
 
  size = width*height;
  pWorkBuf = (unsigned long*)calloc( sizeof(unsigned long), size );

  delta = block.pitch;
  ptr = block.pixelPtr;
  endptr = ptr+delta*gYsize;
  ptrct = ptr+delta;

  i = 0;
  // Classify each pixel
  while( ptr<endptr ) {
    for( ; ptr<ptrct; ptr+=block.pixelSize*2, i++ ) {
      sum = ( r = *(ptr+block.offset[0]) ) +
        ( g = *(ptr+block.offset[1]) ) +
        *(ptr+block.offset[2]);
      if( sum < 50 )
        continue;
      xEx = ((double)r)/sum - gEx;
      yEy = ((double)g)/sum - gEy;
      if( xEx*(xEx*gA22-yEy*gA12)+yEy*yEy<gDet )
        *(pWorkBuf+i) = 1; else
          *(pWorkBuf+i) = 0;
    }
    ptr = ptrct+delta;
    ptrct = ptr+delta;
  }

  // Expand the skin color regions
  pWorkBuf = Expansion4( pWorkBuf, width, height );

  // Compute connected components, placing result in a global array
  ClassfyImage( pWorkBuf, width, height );
  free(pWorkBuf);
  //  fprintf(stderr,"Region count = %d\n",gRegionCnt);

  // Mark connected rectangles
  for (i=0; i<gRegionCnt; i++) {
    int x1=gRect[i].sx*2;
    int x2=gRect[i].ex*2;
    int y1=gRect[i].sy*2;
    int y2=gRect[i].ey*2;
    if (x1<0) x1=0;
    if (x1>=image->width) x1=image->width-1;
    if (x2<0) x2=0;
    if (x2>=image->width) x2=image->width-1;
    if (y1<0) y1=0;
    if (y1>=image->height) y1=image->height-1;
    if (y2<0) y2=0;
    if (y2>=image->height) y2=image->height-1;
    //    fprintf(stderr,"  (%d,%d) - (%d,%d)\n",x1,y1,x2,y2);
    for (int y=y1; y<=y2; y++) for (int x=x1; x<=x2; x++)
      image->data(x,y) &= maskValue;
  }
  image->Change();
  return TCL_OK;
}

int KDoiColorMask2Cmd(ClientData data, Tcl_Interp *interp,
                     int argc, char *argv[])
{
  if (argc!=4) {
    interp->result="Usage: KDoiColorMask2 <photo> <mask> <maskvalue>";
    return TCL_ERROR;
  }
  extern Tcl_Interp *theInterp;
  Tk_PhotoHandle photo=Tk_FindPhoto(theInterp,argv[1]);
  ByteImage *image=FindByteImageWithName(argv[2]);
  unsigned char maskValue=atoi(argv[3]);

  Tk_PhotoImageBlock block;
  Tk_PhotoGetImage(photo,&block);

  int gXsize, gYsize;
  Tk_PhotoGetSize(photo,&gXsize,&gYsize);

  int x, y;
  int delta;
  double xEx, yEy;
  unsigned char *ptr;

  delta = block.pitch;
  ptr = block.pixelPtr;

  unsigned char *optr=image->buffer;

  // Classify each pixel
  for (y=0; y<gYsize; y+=2) {
    for (x=0; x<gXsize; x+=2) {
      int r=(*(ptr+block.offset[0])+
             *(ptr+block.offset[0]+block.pixelSize)+
             *(ptr+block.offset[0]+delta)+
             *(ptr+block.offset[0]+delta+block.pixelSize))/4;
      int g=(*(ptr+block.offset[1])+
             *(ptr+block.offset[1]+block.pixelSize)+
             *(ptr+block.offset[1]+delta)+
             *(ptr+block.offset[1]+delta+block.pixelSize))/4;
      int b=(*(ptr+block.offset[2])+
             *(ptr+block.offset[2]+block.pixelSize)+
             *(ptr+block.offset[2]+delta)+
             *(ptr+block.offset[2]+delta+block.pixelSize))/4;
      if (r+g+b>=50) {
        xEx=(double)r/(double)(r+g+b)-gEx;
        yEy=(double)g/(double)(r+g+b)-gEy;
        if (xEx*(xEx*gA22-yEy*gA12)+yEy*yEy<gDet*Threshold) {
          *(optr) &= maskValue;
          *(optr+1) &= maskValue;
          *(optr+gXsize) &= maskValue;
          *(optr+gXsize+1) &= maskValue;
        }
      } 
      optr+=2;
      ptr+=2*block.pixelSize;
    }
    ptr+=delta;
    optr+=gXsize;
  }

  image->Change();
  return TCL_OK;
}
#endif

void InvertColorCovariance()
{
  gA11 = gExx-gEx*gEx;
  gA12 = gExy-gEx*gEy;
  gA22 = gEyy-gEy*gEy;
  gDet = 2*(gA11*gA22-gA12*gA12);
    
  gA12 = 2*gA12/gA11;
  gA22 /= gA11;
  gDet /= gA11;
}

#ifndef NoVista
void SetColorMask(VImage rgb, int x1, int y1, int x2, int y2) 
{
  double u=0, v=0, uu=0, vv=0, uv=0, tot=0;
  for (int j=y1; j<=y2; j++) if (j>=0 && j<VImageNRows(rgb))
    for (int i=x1; i<=x2; i++) if (i>=0 && i<=VImageNColumns(rgb)) {
      int r=VPixel(rgb,0,j,i,VUByte);
      int g=VPixel(rgb,1,j,i,VUByte);
      int b=VPixel(rgb,2,j,i,VUByte);
      int sum=r+g+b;
      if (sum>=50) {
        double thisu=r/(double)sum;
        double thisv=g/(double)sum;
        u+=thisu;
        v+=thisv;
        uu+=thisu*thisu;
        vv+=thisv*thisv;
        uv+=thisu*thisv;
        tot++;
      }
    }
  if (tot==0) tot=1;

  gEx = u/tot;
  gEy = v/tot;
  gExx = uu/tot;
  gEyy = vv/tot;
  gExy = uv/tot;

  InvertColorCovariance();
}

VImage ColorMaskVista(VImage rgb)
{
  VImage mask=VCreateImage(1,VImageNRows(rgb),VImageNColumns(rgb),VUByteRepn);

  for (int y=0; y<VImageNRows(rgb); y++)
    for (int x=0; x<VImageNColumns(rgb); x++) {
      int r=VPixel(rgb,0,y,x,VUByte);
      int g=VPixel(rgb,1,y,x,VUByte);
      int b=VPixel(rgb,2,y,x,VUByte);
      int sum=r+g+b;
      if (sum<0) VPixel(mask,0,y,x,VUByte)=0; else {
        double xEx = ((double)r)/sum - gEx;
        double yEy = ((double)g)/sum - gEy;
        double prob=exp(-(xEx*(xEx*gExx-yEy*gExy)+
                          yEy*(-xEx*gExy+yEy*gEyy))/(gExx*gEyy-gExy*gExy))/
          sqrt((gExx*gEyy-gExy*gExy)*2.0*M_PI);
        VPixel(mask,0,y,x,VUByte)=(int)(255*prob);
//          VPixel(mask,0,y,x,VUByte)=255; else
//            VPixel(mask,0,y,x,VUByte)=0; 
      }
    }

  return mask;
}

VImage ColorMaskVista2(VImage rgb)
{
  VImage mask=VCreateImage(1,VImageNRows(rgb),VImageNColumns(rgb),VUByteRepn);

  for (int y=0; y<VImageNRows(rgb)-1; y++)
    for (int x=0; x<VImageNColumns(rgb)-1; x++) {
      int r=VPixel(rgb,0,y,x,VUByte)+VPixel(rgb,0,y+1,x,VUByte)+
        VPixel(rgb,0,y,x+1,VUByte)+VPixel(rgb,0,y+1,x+1,VUByte);
      int g=VPixel(rgb,1,y,x,VUByte)+VPixel(rgb,1,y+1,x,VUByte)+
        VPixel(rgb,1,y,x+1,VUByte)+VPixel(rgb,1,y+1,x+1,VUByte);
      int b=VPixel(rgb,2,y,x,VUByte)+VPixel(rgb,2,y+1,x,VUByte)+
        VPixel(rgb,2,y,x+1,VUByte)+VPixel(rgb,2,y+1,x+1,VUByte);
      int sum=r+g+b;
      if (sum<0) VPixel(mask,0,y,x,VUByte)=0; else {
        double xEx = ((double)r)/sum - gEx;
        double yEy = ((double)g)/sum - gEy;
        double prob=exp(-(xEx*(xEx*gExx-yEy*gExy)+
                          yEy*(-xEx*gExy+yEy*gEyy))/(gExx*gEyy-gExy*gExy))/
          sqrt((gExx*gEyy-gExy*gExy)*2.0*M_PI);
        VPixel(mask,0,y,x,VUByte)=(int)(255*prob);
//          VPixel(mask,0,y,x,VUByte)=255; else
//            VPixel(mask,0,y,x,VUByte)=0; 
      }
    }

  return mask;
}
#endif

void EmptyColorMask()
{
  gEx=gEy=0.25;
  gExx=gEyy=0.625;
  gExy=0;
  InvertColorCovariance();
}

void BroadenColorMask(double amount)
{
  double u=0.25, v=0.25;
  double uu=0.625, uv=0, vv=0.625;
  gEx = gEx*(1-amount)+u*amount;
  gEy = gEy*(1-amount)+v*amount;
  gExx = gExx*(1-amount)+uu*amount;
  gExy = gExy*(1-amount)+uv*amount;
  gEyy = gEyy*(1-amount)+vv*amount;
  InvertColorCovariance();
}

#ifndef NoTcl
void UpdateColorMask(char *photoname, int x1, int y1, int x2, int y2,
                     double amount) 
{
  Tk_PhotoHandle photo=Tk_FindPhoto(theInterp,photoname);
  Tk_PhotoImageBlock block;
  Tk_PhotoGetImage(photo,&block);
  int width, height;
  Tk_PhotoGetSize(photo,&width,&height);
  double u=0, v=0, uu=0, vv=0, uv=0, tot=0;
  for (int j=y1; j<=y2; j++) if (j>=0 && j<height)
    for (int i=x1; i<=x2; i++) if (i>=0 && i<width) {
      int r=*(block.pixelPtr+i*block.pixelSize+j*block.pitch+block.offset[0]);
      int g=*(block.pixelPtr+i*block.pixelSize+j*block.pitch+block.offset[1]);
      int b=*(block.pixelPtr+i*block.pixelSize+j*block.pitch+block.offset[2]);
      int sum=r+g+b;
      if (sum>=50) {
        double thisu=r/(double)sum;
        double thisv=g/(double)sum;
        u+=thisu;
        v+=thisv;
        uu+=thisu*thisu;
        vv+=thisv*thisv;
        uv+=thisu*thisv;
        tot++;
      }
    }
  if (tot==0) tot=1;
  u/=tot;
  v/=tot;
  uu/=tot;
  uv/=tot;
  vv/=tot;

  gEx = gEx*(1-amount)+u*amount;
  gEy = gEy*(1-amount)+v*amount;
  gExx = gExx*(1-amount)+uu*amount;
  gExy = gExy*(1-amount)+uv*amount;
  gEyy = gEyy*(1-amount)+vv*amount;

  InvertColorCovariance();
#ifdef UsePosterLogging
  if (EnablePosterLogging) {
    int w=2*(1+(y2-y1)/2);
    int h=2*(1+(x2-x1)/2);
    VImage vimg=VCreateImage(3,h*2,w*2,VUByteRepn);
    for (int j=y1-h/2; j<y2+h/2; j++)
      for (int i=x1-w/2; i<x2+w/2; i++)
        if (i>=0 && i<width && j>=0 && j<height) {
          int r=*(block.pixelPtr+i*block.pixelSize+j*block.pitch+
                  block.offset[0]);
          int g=*(block.pixelPtr+i*block.pixelSize+j*block.pitch+
                  block.offset[1]);
          int b=*(block.pixelPtr+i*block.pixelSize+j*block.pitch+
                  block.offset[2]);
          VPixel(vimg,0,j-y1+h/2,i-x1+w/2,VUByte)=r;
          VPixel(vimg,1,j-y1+h/2,i-x1+w/2,VUByte)=g;
          VPixel(vimg,2,j-y1+h/2,i-x1+w/2,VUByte)=b;
        } else {
          VPixel(vimg,0,j-y1+h/2,i-x1+w/2,VUByte)=0;
          VPixel(vimg,1,j-y1+h/2,i-x1+w/2,VUByte)=0;
          VPixel(vimg,2,j-y1+h/2,i-x1+w/2,VUByte)=0;
        }
    WriteVistaImageLogging(vimg,"colorupdate");
    VDestroyImage(vimg);
  }
#endif
}
#endif

void UpdateColorMaskFromBuffer(int width, int height,
                               unsigned char *buffer,
                               int x1, int y1, int x2, int y2,
                               double amount)
{
  double u=0, v=0, uu=0, vv=0, uv=0, tot=0;
  for (int j=y1; j<=y2; j++) if (j>=0 && j<height)
    for (int i=x1; i<=x2; i++) if (i>=0 && i<=width) {
      int r=buffer[j*width*3+i];
      int g=buffer[j*width*3+i+1];
      int b=buffer[j*width*3+i+2];
      int sum=r+g+b;
      if (sum>=50) {
        double thisu=r/(double)sum;
        double thisv=g/(double)sum;
        u+=thisu;
        v+=thisv;
        uu+=thisu*thisu;
        vv+=thisv*thisv;
        uv+=thisu*thisv;
        tot++;
      }
    }
  if (tot==0) tot=1;
  u/=tot;
  v/=tot;
  uu/=tot;
  uv/=tot;
  vv/=tot;

  gEx = gEx*(1-amount)+u*amount;
  gEy = gEy*(1-amount)+v*amount;
  gExx = gExx*(1-amount)+uu*amount;
  gExy = gExy*(1-amount)+uv*amount;
  gEyy = gEyy*(1-amount)+vv*amount;

  InvertColorCovariance();
}

#ifndef NoTcl
void KDoi_Init(Tcl_Interp *interp)
{
  Tcl_CreateCommand(interp,"KDoiInit",KDoiInitCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"KDoiColorMask",KDoiColorMaskCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"KDoiColorMask2",KDoiColorMask2Cmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
}
#endif
