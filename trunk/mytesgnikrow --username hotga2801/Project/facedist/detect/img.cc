// Image library that can be used with Tcl code (no Tk dependencies)
//#define UseXpm

#ifdef __GNUC__
#include <string.h>
#endif

#ifdef hpux
#include <math.h>
#endif

#ifndef WINNT
extern "C" {
#endif
#ifndef hpux
#include <math.h>
#endif
#ifndef __GNUC__
#include <string.h>
#endif
#ifdef UseXpm
#include "xpm.h"
#endif
#include <assert.h>
#ifndef WINNT
}
#endif

#include "system.hpp"
#include "img.hpp"
#include "gif.hpp"
#include "fft.hpp"
#ifndef NoTcl
#include "hacks.hpp"
#endif

ImageDataFunction *UnlinkImageDataFunction;
ImageDataFunction *RedrawImageDataFunction;

static Tcl_HashTable ImageTable;

Image::Image(char *n)
{
  if (n==NULL) name=NULL; else
  {
    name=strdup(n);
    Tcl_HashEntry *entry=Tcl_FindHashEntry(&ImageTable,name);
    if (entry!=NULL) 
    {
      delete (Image*)(Tcl_GetHashValue(entry));
    }
    int newentry;
    entry=Tcl_CreateHashEntry(&ImageTable,name,&newentry);
    if (!newentry)
    {
      fprintf(stderr,"Create image: duplicate name '%s'.\n",name);
      exit(1);
    }
    Tcl_SetHashValue(entry,this);
  }
  width=0;
  height=0;
  size=0;
  link=NULL;
}

Image::~Image()
{
  if (link!=NULL) UnlinkImageDataFunction(link);
  if (name!=NULL)
  {
    Tcl_HashEntry *entry=Tcl_FindHashEntry(&ImageTable,name);
    if (entry!=NULL) Tcl_DeleteHashEntry(entry);
    free(name);
  }
}

int Image::GetType()
{
  return NotAnImageType;
}

void Image::ImageToBytes(unsigned char *data, int zoom,
                         double min, double max)
{
}

void Image::Change()
{
  if (link!=NULL) RedrawImageDataFunction(link);
}

// USED
// Create an image object with byte pixels
ByteImage::ByteImage(char *name) : Image(name)
{
  buffer=NULL;
}

// USED
// Discard an image object with byte pixels
ByteImage::~ByteImage()
{
  if (buffer!=NULL) free(buffer);
}

// USED
// Indicate that this image has byte pixels
int ByteImage::GetType()
{
  return ByteImageType;
}

double ByteImage::interpolate(double x, double y)
{
  int left=(int)(floor(x));
  int right=(int)(ceil(x));
  int top=(int)(floor(y));
  int bottom=(int)(ceil(y));
  double fracLeft=x-left;
  double fracTop=y-top;
  double ll, lr, ul, ur;
  if (left<0 || right>=width || top<0 || bottom>=height) return 0;
  ll=data(left,bottom);
  lr=data(right,bottom);
  ul=data(left,top);
  ur=data(right,top);
  return (fracTop*((1.0-fracLeft)*ll+fracLeft*lr)+
          (1.0-fracTop)*((1.0-fracLeft)*ul+fracLeft*ur));
}

double ByteImage::interpolateLogPolar(double x, double y)
{
  x=fmod(x,width);
  if (x<0) x+=width;
  int left=((int)(floor(x))) % width;
  int right=((int)(ceil(x))) % width;
  int top=(int)(floor(y));
  int bottom=(int)(ceil(y));
  double fracLeft=x-left;
  double fracTop=y-top;
  double ll, lr, ul, ur;
  if (top<0 || bottom>=height) return 0;
  ll=data(left,bottom);
  lr=data(right,bottom);
  ul=data(left,top);
  ur=data(right,top);
  return (fracTop*((1.0-fracLeft)*ll+fracLeft*lr)+
          (1.0-fracTop)*((1.0-fracLeft)*ul+fracLeft*ur));
}

double ByteImage::interpolateExtend(double x, double y)
{
  if (x<0) x=0;
  if (y<0) y=0;
  if (x>width-1) x=width-1;
  if (y>height-1) y=height-1;
  int left=(int)(floor(x));
  int right=(int)(ceil(x));
  int top=(int)(floor(y));
  int bottom=(int)(ceil(y));
  double fracLeft=x-left;
  double fracTop=y-top;
  double ll, lr, ul, ur;
  ll=data(left,bottom);
  lr=data(right,bottom);
  ul=data(left,top);
  ur=data(right,top);
  return (fracTop*((1.0-fracLeft)*ll+fracLeft*lr)+
          (1.0-fracTop)*((1.0-fracLeft)*ul+fracLeft*ur));
}

void ByteImage::ImageToBytes(unsigned char *data, int zoom,
                             double min, double max)
{
  if (buffer==NULL) return;
  if (zoom==0) memcpy(data,buffer,size);
  if (zoom>0)
  {
    int block=zoom;
    for (int y=0; y<height; y++)
      for (int yb=0; yb<block; yb++)
      {
        unsigned char *bufferPtr=buffer+y*width;
        for (int x=0; x<width; x++)
        {
          for (int xb=0; xb<block; xb++)
            *(data++)=*bufferPtr;
          bufferPtr++;
        }
      }
  }
  if (zoom<0)
  {
    int block=-zoom;
    for (int y=0; y<height; y+=block)
      for (int x=0; x<width; x+=block)
        *(data++)=this->data(x,y);
  }
}

// USED 
// Initialize the image object, using the given data as the image
void ByteImage::NewBuffer(Byte *buf, int w, int h)
{
  if (buffer!=buf)
  {
    if (buffer!=NULL) free(buffer);
    buffer=buf;
    width=w;
    height=h;
    size=width*height;
  }
  Change();
}

// USED
// Allocate a new buffer for the image, of the given size.  If the 
// buffer size is the same as the current buffer, just use that instead
// of allocating new memory.  This call should be followed by a call to
// ByteImage::NewBuffer() to tell the image to use the new buffer.
Byte *ByteImage::AllocBuffer(int w, int h)
{
  if (width==w && height==h && buffer!=NULL)
    return buffer; else
      return (Byte*)malloc(w*h);
}

void ByteImage::FreeBuffer()
{
  if (buffer!=NULL) free(buffer);
  buffer=NULL;
  size=0;
  width=0;
  height=0;
}

FloatImage::FloatImage(char *name) : Image(name)
{
  buffer=NULL;
}

FloatImage::~FloatImage()
{
  free(buffer);
}

int FloatImage::GetType()
{
  return FloatImageType;
}

double FloatImage::interpolate(double x, double y)
{
  int left=(int)(floor(x));
  int right=(int)(ceil(x));
  int top=(int)(floor(y));
  int bottom=(int)(ceil(y));
  double fracLeft=x-left;
  double fracTop=y-top;
  double ll, lr, ul, ur;
  if (left<0 || right>=width || top<0 || bottom>=height) return 0;
  ll=data(left,bottom);
  lr=data(right,bottom);
  ul=data(left,top);
  ur=data(right,top);
  return (fracTop*((1.0-fracLeft)*ll+fracLeft*lr)+
          (1.0-fracTop)*((1.0-fracLeft)*ul+fracLeft*ur));
}

void FloatImage::ImageToBytes(unsigned char *data, int zoom,
                              double min, double max)
{
  if (buffer==NULL) return;
  if (min==0 && max==0) {
    min=buffer[0], max=buffer[0];
    for (int i=1; i<size; i++)
    {
      double val=buffer[i];
      if (val<min) min=val;
      if (max<val) max=val;
    }
    if (max==min) max=min+1;
  }
  if (zoom==0)
  {
    for (int i=0; i<size; i++)
      data[i]=(int)floor(0.5+255.0*(buffer[i]-min)/(max-min));
  }
  if (zoom>0)
  {
    int block=zoom;
    for (int y=0; y<height; y++)
      for (int yb=0; yb<block; yb++)
      {
        float *bufferPtr=buffer+y*width;
        for (int x=0; x<width; x++)
        {
          unsigned char val=(int)floor(0.5+255.0*(*(bufferPtr++)-min)/
                                       (max-min));
          for (int xb=0; xb<block; xb++) *(data++)=val;
        }
      }
  }
  if (zoom<0)
  {
    int block=-zoom;
    for (int y=0; y<height; y+=block)
      for (int x=0; x<width; x+=block)
        *(data++)=(int)floor(0.5+255.0*(this->data(x,y)-min)/(max-min));
  }
}

void FloatImage::NewBuffer(float *buf, int w, int h)
{
  if (buffer!=NULL) free(buffer);
  buffer=buf;
  width=w;
  height=h;
  size=width*height;
  Change();
}

void FloatImage::FreeBuffer()
{
  if (buffer!=NULL) free(buffer);
  buffer=NULL;
  size=0;
  width=0;
  height=0;
}

// USED
// Look through the list of images trying to find one with the
// given name, and return a pointer to that image.  Return NULL
// if no such image is found.
Image *FindImageWithName(char *name)
{
  Tcl_HashEntry *entry=Tcl_FindHashEntry(&ImageTable,name);
  if (entry==NULL) return NULL;
  return (Image*)(Tcl_GetHashValue(entry));
}

// USED
// Look through the list of images, trying to find one with the given
// name and that also has byte pixels.  Return that image, or NULL if
// no such image is found.
ByteImage *FindByteImageWithName(char *name)
{
  Image *image=FindImageWithName(name);
  if (image==NULL || image->GetType()!=ByteImageType) return NULL;
  return (ByteImage*)image;
}

FloatImage *FindFloatImageWithName(char *name)
{
  Image *image=FindImageWithName(name);
  if (image==NULL || image->GetType()!=FloatImageType) return NULL;
  return (FloatImage*)image;
}

void RotateByteImage2(ByteImage *dest, ByteImage *src, double angle)
{
  double c=cos(angle);
  double s=sin(angle);
  Byte *buf=(Byte*)malloc(src->width*src->height);
  int ptr=0;
  double hx=src->width/2;
  double hy=src->height/2;
  for (int y=0; y<src->height; y++)
    for (int x=0; x<src->width; x++)
      buf[ptr++]=(int)floor(0.5+src->interpolate((x-hx)*c+(y-hy)*s+hx,
                                                 -(x-hx)*s+(y-hy)*c+hy));
  dest->NewBuffer(buf,src->width,src->height);
}

void ZoomByteImage(ByteImage *image, double scale, ByteImage *src)
{
  int newWidth=(int)floor(src->width/scale);
  int newHeight=(int)floor(src->height/scale);
  Byte *newdata=(Byte*)malloc(sizeof(Byte)*newWidth*newHeight);
  for (int x=0; x<newWidth; x++)
    for (int y=0; y<newHeight; y++)
      newdata[x+y*newWidth]=(int)floor(0.5+
                                       src->interpolate(x*scale,y*scale));
  image->NewBuffer(newdata,newWidth,newHeight);
}

// USED
// Fill a rectangular region of a byte image with the given value
void FillRectangle(ByteImage *image, int x1, int y1, int x2, int y2, int color)
{
  for (int y=y1; y<y2; y++)
    if (y>=0 && y<image->height)
      for (int x=x1; x<x2; x++)
        if (x>=0 && x<image->width)
          image->data(x,y)=color;
  image->Change();
}

void SubSampleWithAveraging(ByteImage *dest, ByteImage *src, int scale)
{
  Byte *newdata=(Byte*)malloc((src->width/scale)*(src->height/scale));
  int ptr=0;
  for (int y=0; y<src->height/scale; y++)
    for (int x=0; x<src->width/scale; x++)
    {
      double tot=0;
      for (int i=x*scale; i<(x+1)*scale; i++)
        for (int j=y*scale; j<(y+1)*scale; j++)
          tot+=src->data(i,j);
      newdata[ptr++]=(int)floor(0.5+tot/(scale*scale));
    }
  dest->NewBuffer(newdata,src->width/scale,src->height/scale);
}

#ifdef notdef
void ResampleByteImage(ByteImage *image, ByteImage *src,
                       int newWidth, int newHeight, double scale)
{
  Byte *buf=image->AllocBuffer(newWidth,newHeight);
  for (int i=0; i<newWidth*newHeight; i++)
    buf[i]=255;
  image->NewBuffer(buf,newWidth,newHeight);
  for (int x=0; x<src->width; x++)
    for (int y=0; y<src->height; y++)
      if (!src->data(x,y))
      {
        int nx=(int)(0.5+x*scale);
        int ny=(int)(0.5+y*scale);
        if (nx<image->width && ny<image->height)
          image->data(nx,ny)=0;
      }
  image->Change();
}
#endif

// USED
// Resample an image, according to the following rule: if any of the
// pixels in the source image which coorespond to a pixel in the
// destination image are zero, then set the destination pixel to zero.
// Otherwise, set the pixel to 255.  scale<1 makes the image smaller.
// The idea is that 0 means a pixel location should be scanned, while
// 255 means that a face has already been found and need not be checked
// again.  Destination can be the same as the source.
// The scaling factor is determined by SCALE, and the size of the
// destination image is controlled by newWith and newHeight;
void ResampleByteImage(ByteImage *image, ByteImage *src,
                       int newWidth, int newHeight, double scale)
{
  Byte *buf=image->AllocBuffer(newWidth,newHeight);  // Allocate destination
  Byte *ptr=buf;
  for (int i=0; i<newWidth*newHeight; i++) buf[i]=255;

  for (int y=0; y<newHeight; y++)      // Scan over lower resolution image
    for (int x=0; x<newWidth; x++) {
      int x1=(int)(0.5+x/scale);
      int y1=(int)(0.5+y/scale);
      if (x1>=src->width) x1=src->width-1;
      if (y1>=src->height) y1=src->height-1;
      int x2=(int)(0.5+(x+1)/scale);
      int y2=(int)(0.5+(y+1)/scale);
      if (x2>=src->width) x2=src->width-1;
      if (y2>=src->height) y2=src->height-1;
      int i, j;
      for (j=y1; j<=y2; j++)      // Scan over corresponding pixels in hi res
        for (i=x1; i<=x2; i++)
          if (!src->data(i,j)) {  // If any pixel is zero, make new pixel zero
            *ptr=0;
            goto next;
          }
      next:
      ptr++;
    }
  image->NewBuffer(buf,newWidth,newHeight);
}

// USED
// Scale the given byte image down by a factor of 1.2, putting the result
// in the given destination (which can be the same as the source).  The
// scaling uses bilinear interpolation, implemented by two steps of linear
// interpolation: first scaling in the X direction, then in the Y direction.
void ReduceSize(ByteImage *dest, ByteImage *src)
{
  int width=src->width;
  int height=src->height;
  int newWidth=(int)floor(width/1.2);
  int newHeight=(int)floor(height/1.2);
//  double scaleX=(double)(src->width-1)/(double)(newWidth-1);
//  double scaleY=(double)(src->height-1)/(double)(newHeight-1);
  double scaleX=1.2, scaleY=1.2;

  // First scale in X
  Byte *tmp=(Byte*)malloc(height*newWidth);
  int x, y;
  Byte *inptr=src->buffer;
  for (y=0; y<height; y++) {
    for (x=0; x<newWidth; x++) {
      int pos=(int)(x*scaleX);
      double frac=(x*scaleX)-pos;
      double val=(1.0-frac)*inptr[pos]+frac*inptr[pos+1];
      if (val<0) val=0; 
      if (val>255) val=255;
      tmp[y+x*height]=(int)(val+0.5);
    }
    inptr+=width;
  }

  // Then scale in Y
  Byte *tmp2=dest->AllocBuffer(newWidth,newHeight);
  inptr=tmp;
  for (y=0; y<newWidth; y++) {
    for (x=0; x<newHeight; x++) {
      int pos=(int)(x*scaleY);
      double frac=(x*scaleY)-pos;
      double val=(1.0-frac)*inptr[pos]+frac*inptr[pos+1];
      if (val<0) val=0; 
      if (val>255) val=255;
      tmp2[y+x*newWidth]=(int)(val+0.5);
    }
    inptr+=height;
  }
  dest->NewBuffer(tmp2,newWidth,newHeight);
  free(tmp);
}

#ifdef notdef
// Version that actually does smoothing
int ReduceSize(ByteImage *dest, ByteImage *src)
{
  int width=src->width;
  int height=src->height;
  int newWidth=(int)floor(width/1.2);
  int newHeight=(int)floor(height/1.2);
  Byte *tmp=(Byte*)malloc(height*newWidth);
  double *scan=(double*)malloc(width*sizeof(double));
#define size 3
#define len 7
//  int size=3;
//  int len=7;
  double kernel[7]={0.09749,-0.12662,0.14628,0.76569,0.14628,-0.12662,0.09749};
  double div[4];
  int x, y;
  double tot=0;
  for (x=size*2; x>size; x--) tot+=kernel[x];
  for (x=0; x<size; x++)
  {
    tot+=kernel[size-x];
    div[x+1]=tot;
  }
  Byte *inptr=src->buffer;
  for (y=0; y<height; y++)
  {
    double *optr=scan;
    for (x=size; x>=1; x--)
    {
      double tot=0;
      double *kptr=kernel+x;
      Byte *iptr=inptr;
      for (int i=0; i<len-x; i++)
        tot+=(*(kptr++))*(*(iptr++));
      *(optr++)=tot/div[x];
    }
    for (x=size; x<width-size; x++)
    {
      double tot=0;
      double *kptr=kernel;
      Byte *iptr=inptr++;
      for (int i=0; i<len; i++)
        tot+=(*(kptr++))*(*(iptr++));
      *(optr++)=tot;
    }
    for (x=1; x<=size; x++)
    {
      double tot=0;
      double *kptr=kernel;
      Byte *iptr=inptr++;
      for (int i=0; i<len-x; i++)
        tot+=(*(kptr++))*(*(iptr++));
      *(optr++)=tot/div[x];
    }
    inptr+=size;
    for (x=0; x<newWidth; x++)
    {
      int pos=(int)(x*1.2);
      double frac=(x*1.2)-pos;
      double val=(1.0-frac)*scan[pos]+frac*scan[pos+1];
      if (val<0) val=0; 
      if (val>255) val=255;
      tmp[y+x*height]=(int)val;
    }
  }
  free(scan);
  scan=(double*)malloc(height*sizeof(double));
  Byte *tmp2=dest->AllocBuffer(newWidth,newHeight);
  inptr=tmp;
  for (y=0; y<newWidth; y++)
  {
    double *optr=scan;
    for (x=size; x>=1; x--)
    {
      double tot=0;
      double *kptr=kernel+x;
      Byte *iptr=inptr;
      for (int i=0; i<len-x; i++)
        tot+=(*(kptr++))*(*(iptr++));
      *(optr++)=tot/div[x];
    }
    for (x=size; x<height-size; x++)
    {
      double tot=0;
      double *kptr=kernel;
      Byte *iptr=inptr++;
      for (int i=0; i<len; i++)
        tot+=(*(kptr++))*(*(iptr++));
      *(optr++)=tot;
    }
    for (x=1; x<=size; x++)
    {
      double tot=0;
      double *kptr=kernel;
      Byte *iptr=inptr++;
      for (int i=0; i<len-x; i++)
        tot+=(*(kptr++))*(*(iptr++));
      *(optr++)=tot/div[x];
    }
    inptr+=size;
    for (x=0; x<newHeight; x++)
    {
      int pos=(int)(x*1.2);
      double frac=(x*1.2)-pos;
      double val=(1.0-frac)*scan[pos]+frac*scan[pos+1];
      if (val<0) val=0; 
      if (val>255) val=255;
      tmp2[y+x*newWidth]=(int)(val+0.5);
    }
  }
  dest->NewBuffer(tmp2,newWidth,newHeight);
  free(tmp);
  free(scan);
#undef len
#undef size
}
#endif

#ifdef notdef
void SmoothByteImage(ByteImage *image)
{
  Byte *newdata=(Byte*)malloc(image->size);
  int center=3;
  int size=3;
  double kernel[7]={0.1061,
                    -0.1378,
                    0.1592,
                    0.8333,
                    0.1592,
                    -0.1378,
                    0.1061};
  double realtot=1;
  int x, y;
  for (y=0; y<image->height; y++)
    for (x=0; x<image->width; x++)
    {
      double tot=0;
      double div=0;
      for (int i=-size; i<=size; i++)
        if (x+i>=0 && x+i<image->width)
        {
          tot+=kernel[i+center]*image->data(x+i,y);
          div+=kernel[i+center];
        }
      tot=tot*realtot/div;
      if (tot<0) tot=0;
      if (tot>255) tot=255;
      newdata[y*image->width+x]=(int)tot;
    }

  for (y=0; y<image->height; y++)
    for (x=0; x<image->width; x++)
    {
      double tot=0;
      double div=0;
      for (int i=-size; i<=size; i++)
        if (y+i>=0 && y+i<image->height)
        {
          tot+=kernel[i+center]*newdata[(y+i)*image->width+x];
          div+=kernel[i+center];
        }
      tot=tot*realtot/div;
      if (tot<0) tot=0;
      if (tot>255) tot=255;
      image->data(x,y)=(int)tot;
    }
  free(newdata);
  image->Change();
}
#endif

void SmoothByteImage(ByteImage *image)
{
  Byte *newdata=(Byte*)malloc(image->size);
  int center=3;
  int size=3;
  double kernel[7]={0.1061,
                    -0.1378,
                    0.1592,
                    0.8333,
                    0.1592,
                    -0.1378,
                    0.1061};
  double realtot=1;
  int x, y;
  for (y=0; y<image->height; y++)
    for (x=0; x<image->width; x++)
    {
      double tot=0;
      double div=0;
      for (int i=-size; i<=size; i++)
        if (x+i>=0 && x+i<image->width)
        {
          tot+=kernel[i+center]*image->data(x+i,y);
          div+=kernel[i+center];
        }
      tot=tot*realtot/div;
      if (tot<0) tot=0;
      if (tot>255) tot=255;
      newdata[y*image->width+x]=(int)tot;
    }

  for (y=0; y<image->height; y++)
    for (x=0; x<image->width; x++)
    {
      double tot=0;
      double div=0;
      for (int i=-size; i<=size; i++)
        if (y+i>=0 && y+i<image->height)
        {
          tot+=kernel[i+center]*newdata[(y+i)*image->width+x];
          div+=kernel[i+center];
        }
      tot=tot*realtot/div;
      if (tot<0) tot=0;
      if (tot>255) tot=255;
      image->data(x,y)=(int)tot;
    }
  free(newdata);
  image->Change();
}

void SaveByteImagesPpm(ByteImage *red, ByteImage *green, ByteImage *blue,
                       FILE *outf)
{
  fprintf(outf,"P6\n%d %d\n255\n",red->width,red->height);
  unsigned char *buf=(unsigned char *)malloc(red->size*3);
  unsigned char *ptr=buf, *r=red->buffer, *g=green->buffer, *b=blue->buffer;
  for (int i=0; i<red->size; i++)
  {
    *(ptr++)=*(r++);
    *(ptr++)=*(g++);
    *(ptr++)=*(b++);
  }
  fwrite((char*)buf,red->size*3,1,outf);
  free((char*)buf);
}

void SaveByteImagePgm(ByteImage *image, FILE *outf)
{
  fprintf(outf,"P5\n%d %d\n255\n",image->width,image->height);
  fwrite((char*)(image->buffer),image->size,1,outf);
}

// USED
// Given an image and a FILE pointer, load an image in raw pgm format.
// An image buffer of the appropriate size is allocated.
void LoadByteImagePgm(ByteImage *image, FILE *inf)
{
  int tokens=0;
  int dataStart=0;
  // Read the header, ignoring comments
  do {
    char line[80];
    //    dataStart=ftell(inf);
    fgets(line,80,inf);
    char *comment=strchr(line,'#');
    if (comment!=NULL) *comment=0;
    char *token=NULL, *input=&line[0];
    while (tokens<4 && (token=strtok(input," \t\n\r"))!=NULL) {
      switch (tokens) {
        case 0:
          if (strcmp(token,"P5")!=0) {
            fprintf(stderr,"Bad PGM file.\n");
            exit(1);
          }
          break;
        case 1:
          image->width=atoi(token);
          break;
        case 2:
          image->height=atoi(token);
          break;
        case 3:
          dataStart+=(token-&line[0])+strlen(token)+1;
          break;
        default:
          break;
      }
      tokens++;
      input=NULL;
    }
  } while (tokens<4);
//  fseek(inf,dataStart,SEEK_SET);
  // Read the image data itself
  if (image->buffer!=NULL) free(image->buffer);
  image->size=image->width*image->height;
  image->buffer=(Byte*)malloc(image->size);
  fread((char*)image->buffer,1,image->size,inf);
  image->Change();
}

#ifdef WINNT
#include <io.h>

char *myfgets(char *buf, int num, FILE *inf)
{
  int fd=fileno(inf);
  int i=0;
  do {
    read(fd,buf+i,1);
    i++;
  } while (i<num-1 && buf[i-1]!=10);
  buf[i]=0;
  fseek(inf,tell(fd),SEEK_SET);
  return buf;
}

int myfread(char *buf, size_t size, size_t num, FILE *inf)
{
  int fd=fileno(inf);
  fprintf(stderr,"myfread at %d/%d\n",(int)tell(fd),(int)ftell(inf));
  int i;
  for (i=0; i<num; i++)
    if (read(fd,buf+size*i,size)<size) break;
    perror("what happened");
  fseek(inf,tell(fd),SEEK_SET);
  return i;
}
#else
#define myfgets fgets
#define myfread fread
#endif

unsigned char *LoadPgmStreamToBuffer(FILE *inf, int *width, int *height)
{
  int tokens=0;
  int dataStart=0;
  do {
    char line[80];
    //    dataStart=ftell(inf);
    fgets(line,80,inf);
    char *comment=strchr(line,'#');
    if (comment!=NULL) *comment=0;
    char *token=NULL, *input=&line[0];
    while (tokens<4 && (token=strtok(input," \t\n\r"))!=NULL)
    {
      switch (tokens)
      {
        case 0:
          if (strcmp(token,"P5")!=0)
          {
            fprintf(stderr,"Bad PGM file.\n");
            exit(1);
          }
          break;
        case 1:
          *width=atoi(token);
          break;
        case 2:
          *height=atoi(token);
          break;
        case 3:
          dataStart+=(token-&line[0])+strlen(token)+1;
          break;
        default:
          break;
      }
      tokens++;
      input=NULL;
    }
  } while (tokens<4);
//  fseek(inf,dataStart,SEEK_SET);
  Byte *buffer=(Byte*)malloc((*width)*(*height));
  fread((char*)buffer,1,(*width)*(*height),inf);
  return buffer;
}

unsigned char *LoadPpmStreamToBuffer(FILE *inf, int *width, int *height)
{
  int tokens=0;
  int dataStart=0;
  do {
    char line[80];
    //    dataStart=ftell(inf);
    fgets(line,80,inf);
    char *comment=strchr(line,'#');
    if (comment!=NULL) *comment=0;
    char *token=NULL, *input=&line[0];
    while (tokens<4 && (token=strtok(input," \t\n\r"))!=NULL)
    {
      switch (tokens)
      {
        case 0:
          if (strcmp(token,"P6")!=0)
          {
            fprintf(stderr,"Bad PPM file.\n");
            exit(1);
          }
          break;
        case 1:
          *width=atoi(token);
          break;
        case 2:
          *height=atoi(token);
          break;
        case 3:
          dataStart+=(token-&line[0])+strlen(token)+1;
          break;
        default:
          break;
      }
      tokens++;
      input=NULL;
    }
  } while (tokens<4);
//  fseek(inf,dataStart,SEEK_SET);
  Byte *buffer=(Byte*)malloc((*width)*(*height)*3);
  fread((char*)buffer,1,(*width)*(*height)*3,inf);
  return buffer;
}

unsigned char *LoadPgmToBuffer(char *filename, int *width, int *height)
{
  FILE *inf=fopen(filename,"rb");
  assert(inf!=NULL);
  unsigned char *buffer=LoadPgmStreamToBuffer(inf,width,height);
  fclose(inf);
  return buffer;
}

unsigned char *LoadPpmToBuffer(char *filename, int *width, int *height)
{
  FILE *inf=fopen(filename,"rb");
  assert(inf!=NULL);
  unsigned char *buffer=LoadPpmStreamToBuffer(inf,width,height);
  fclose(inf);
  return buffer;
}

void ByteToFloatImage(ByteImage *bi, FloatImage *fi)
{
  float *buf=(float*)malloc(bi->size*sizeof(float));
  for (int i=0; i<bi->size; i++)
    buf[i]=bi->data(i)/127.0-1.0;
  fi->NewBuffer(buf,bi->width,bi->height);
}

void FFTFloatImage(FloatImage *src1, FloatImage *src2, int forward)
{
  int width=src1->width;
  int height=src1->height;
  Complex *row=new Complex[width];
  Complex *col=new Complex[height];
  for (int j=0; j<height; j++)
  {
    for (int i=0; i<width; i++)
    {
      row[i].a=src1->data(i+j*width);
      row[i].b=src2->data(i+j*width);
    }
    ComputeFFT(row,width,forward);
    for (i=0; i<width; i++)
    {
      src1->data(i+j*width)=row[i].a;
      src2->data(i+j*width)=row[i].b;
    }
  }
  for (int i=0; i<width; i++)
  {
    for (int j=0; j<height; j++)
    {
      col[j].a=src1->data(i+j*width);
      col[j].b=src2->data(i+j*width);
    }
    ComputeFFT(col,height,forward);
    for (j=0; j<height; j++)
    {
      src1->data(i+j*width)=col[j].a;
      src2->data(i+j*width)=col[j].b;
    }
  }
  delete[] row;
  delete[] col;
  src1->Change();
  src2->Change();
}

void PowerSpectrumFloatImage(FloatImage *dest, FloatImage *src)
{
  float *buf=(float*)malloc(src->size*sizeof(float));
  Complex *data=new Complex[src->size];
  Complex *row=new Complex[src->width];
  Complex *col=new Complex[src->height];
  for (int j=0; j<src->height; j++)
  {
    for (int i=0; i<src->width; i++)
      row[i].a=src->data(i+j*src->width);
    ComputeFFT(row,src->width,1);
    for (i=0; i<src->width; i++)
      data[i+j*src->width]=row[i];
  }
  for (int i=0; i<src->width; i++)
  {
    for (int j=0; j<src->height; j++)
      col[j]=data[i+j*src->width];
    ComputeFFT(col,src->height,1);
    for (j=0; j<src->height; j++)
      buf[i+j*src->width]=Abs(col[j]);
  }
  delete[] row;
  delete[] col;
  delete[] data;
  dest->NewBuffer(buf,src->width,src->height);
}

#ifndef NoTcl
int LoadByteImageCmd(ClientData data, Tcl_Interp *interp,
                     int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: LoadByteImage <name> <file> <file|stream>";
    return TCL_ERROR;
  }
  char *name=argv[1];
  char *file=argv[2];
  char *mode=argv[3];
  ByteImage *image=FindByteImageWithName(name);
  if (image==NULL)
  {
    interp->result="Unknown image";
    return TCL_ERROR;
  }
  FILE *inf;
  if (strcmp(mode,"file")==0)
  {
    inf=fopen(file,"rb");
    if (inf==NULL) return TCL_ERROR;
  } else
  {
//    if (Tcl_GetOpenFile(interp,file,0,1,(ClientData*)&inf)!=TCL_OK)
//      return TCL_ERROR;
    inf=GetOpenFile(interp,file,TCL_READABLE);
  }
//  int foo;
//  fscanf(inf,"P5\n%d %d\n%d\n",&foo,&foo,&foo);
//  int prevDataStart=ftell(inf);
//  rewind(inf);
  int tokens=0;
  int dataStart=0;
  do {
    char line[80];
    //    if (strcmp(mode,"file")==0)
    //      dataStart=ftell(inf); else
    //        dataStart=0;
//    if (dataStart==-1) perror("ftell");
//    fprintf(stderr,"datastart=%d\n",dataStart);
    if (fgets(line,80,inf)==NULL) {
      fprintf(stderr,"EOF!!!\n");
    }
//    fprintf(stderr,"line=%s\n",line);
    char *comment=strchr(line,'#');
    if (comment!=NULL) *comment=0;
    char *token=NULL, *input=&line[0];
    while (tokens<4 && (token=strtok(input," \t\n\r"))!=NULL)
    {
//      fprintf(stderr,"token=%s\n",token);
      switch (tokens)
      {
        case 0:
          if (strcmp(token,"P5")!=0)
          {
            fprintf(stderr,"Bad PGM file.\n");
            return TCL_ERROR;
          }
          break;
        case 1:
          image->width=atoi(token);
          break;
        case 2:
          image->height=atoi(token);
          break;
        case 3:
          dataStart+=(token-&line[0])+strlen(token)+1;
          break;
        default:
          break;
      }
      tokens++;
      input=NULL;
    }
  } while (tokens<4);
//  fprintf(stderr,"size=%d %d\n",image->width,image->height);
//  fprintf(stderr,"prevStart=%d dataStart=%d\n",prevDataStart,dataStart);
  //  if (strcmp(mode,"file")==0) fseek(inf,dataStart,SEEK_SET);
  if (image->buffer!=NULL) free(image->buffer);
  image->size=image->width*image->height;
  image->buffer=(Byte*)malloc(image->size);
  int num;
  if ((num=fread((char*)image->buffer,1,image->size,inf))!=image->size)
  {
    fprintf(stderr,"Claims to have read wrong amount: %d/%d\n",
            num,image->size);
  }
  image->Change();
  if (strcmp(mode,"file")==0) fclose(inf); else
    FreeOpenFile(interp,file,inf);
//  sprintf(interp->result,"%d",prevDataStart-dataStart);
  return TCL_OK;
}
#endif

void LoadByteImageRaw(ByteImage *image, int width, int height, FILE *inf)
{
  Byte *newdata=image->AllocBuffer(width,height);
  fread((char*)newdata,width,height,inf);
  image->NewBuffer(newdata,width,height);
}

void LoadByteImageHsi(ByteImage *image, FILE *inf)
{
  int foo;
  fread((char*)&foo,sizeof(int),1,inf);
  fread((char*)&foo,sizeof(int),1,inf);
  fread((char*)&image->height,sizeof(int),1,inf);
  fread((char*)&image->width,sizeof(int),1,inf);
  for (int i=0; i<39; i++)
    fread((char*)&foo,sizeof(int),1,inf);
  if (image->buffer!=NULL) free(image->buffer);
  image->size=image->width*image->height;
  image->buffer=(Byte*)malloc(image->size);
  int num;
  if ((num=fread((char*)image->buffer,1,image->size,inf))!=image->size)
  {
    fprintf(stderr,"Claims to have read wrong amount: %d/%d\n",
            num,image->size);
  }
  image->Change();
}

void FloatImage::ZeroBuffer(int w, int h)
{
  if (buffer!=NULL) free(buffer);
  width=w;
  height=h;
  size=w*h;
  buffer=(float*)malloc(sizeof(float)*size);
  for (int i=0; i<size; i++) buffer[i]=0.0;
  Change();
}

// USED
// Create data for image, and fill with zeros (or whatever value is passed
// in).  Any previous data for image is discarded.
void ByteImage::ZeroBuffer(int w, int h, unsigned char fill)
{
  if (buffer!=NULL) free(buffer);
  width=w;
  height=h;
  size=w*h;
  buffer=(Byte*)malloc(sizeof(Byte)*size);
  for (int i=0; i<size; i++) buffer[i]=fill;
  Change();
}

void CopyByteImage(ByteImage *dest, ByteImage *src)
{
  Byte *newdata=dest->AllocBuffer(src->width,src->height);
  Byte *to=newdata;
  Byte *from=src->buffer;
  for (int i=0; i<src->size; i++)
    *(to++)=*(from++);
  dest->NewBuffer(newdata,src->width,src->height);
}

void DifferenceImage(ByteImage *dest, ByteImage *src1, ByteImage *src2)
{
  Byte *newdata=dest->AllocBuffer(src1->width,src1->height);
  Byte *ptr1=src1->buffer;
  Byte *ptr2=src2->buffer;
  Byte *ptr=newdata;
  for (int i=0; i<src1->size; i++)
    *(ptr++)=abs((int)(*(ptr1++))-(int)(*(ptr2++)));
  dest->NewBuffer(newdata,src1->width,src1->height);
}

void LaplacianImage(ByteImage *dest, ByteImage *src)
{
  Byte *newdata=(Byte*)malloc(src->width*src->height);
  int ptr=0;
  for (int y=0; y<src->height; y++)
    for (int x=0; x<src->width; x++)
    {
      int cur=src->data(x,y);
      int val=-4*cur;
      if (x>0) val+=src->data(x-1,y); else val+=cur;
      if (y>0) val+=src->data(x,y-1); else val+=cur;
      if (x<src->width-1) val+=src->data(x+1,y); else val+=cur;
      if (y<src->height-1) val+=src->data(x,y+1); else val+=cur;
      newdata[ptr++]=(abs(val)>>2);
    }
  dest->NewBuffer(newdata,src->width,src->height);
}

void ExtractRectangle(ByteImage *dest, ByteImage *src, int x, int y,
                      int width, int height)
{
  Byte *buf=dest->AllocBuffer(width,height);
  dest->NewBuffer(buf,width,height);
  for (int j=y; j<y+height; j++)
  {
    int jj=j; if (jj<0) jj=0; if (jj>=src->height) jj=src->height-1;
    for (int i=x; i<x+width; i++) 
    {
      int ii=i; if (ii<0) ii=0; if (ii>=src->width) ii=src->width-1;
      dest->data(i-x,j-y)=src->data(ii,jj);
    }
  }
  dest->Change();
}

void ExtractAndRotate(ByteImage *dest, ByteImage *src, int x, int y,
                      int width, int height, double angle)
{
  Byte *buf=dest->AllocBuffer(width,height);
  dest->NewBuffer(buf,width,height);
  for (int j=0; j<height; j++)
    for (int i=0; i<width; i++) {
      double rx=x+(i-width/2+0.5)*cos(angle)-(j-height/2)*sin(angle);
      double ry=y+(i-width/2+0.5)*sin(angle)+(j-height/2)*cos(angle);
      int value=(int)floor(src->interpolateExtend(rx,ry)+0.5);
      if (value<0) value=0;
      if (value>255) value=255;
      dest->data(i,j)=value;
    }
  dest->Change();
}

void ExtractAndZoom(ByteImage *dest, ByteImage *src, double scale,
                    int minX, int minY, int maxX, int maxY)
{
  int width=(int)floor(0.5+(maxX-minX)/scale);
  int height=(int)floor(0.5+(maxY-minY)/scale);
  Byte *buf=(Byte*)malloc(width*height);
  int ptr=0;
  if (0) { 
    for (int y=0; y<height; y++) for (int x=0; x<width; x++)
      buf[ptr++]=(int)(src->interpolateExtend(x*scale+minX,y+scale*minY));
  } else {
    for (int y=0; y<height; y++)
    {
      int y1=(int)(0.5+y*scale+minY);
      int y2=(int)(0.5+(y+1)*scale+minY);
      if (y2>=src->height) y2=src->height-1;
      for (int x=0; x<width; x++)
      {
        int x1=(int)(0.5+x*scale+minX);
        int x2=(int)(0.5+(x+1)*scale+minX);
        if (x2>=src->width) x2=src->width-1;
        int num=(x2-x1)*(y2-y1);
        int tot=num>>1;
        for (int j=y1; j<y2; j++) for (int i=x1; i<x2; i++)
          tot+=src->data(i,j);
        tot/=num;
        if (tot>255) tot=255;
        buf[ptr++]=tot;
      }
    }
  }
  dest->NewBuffer(buf,width,height);
}

void DifferenceFloatImage(ByteImage *dest, ByteImage *src1, FloatImage *src2)
{
  Byte *newdata=dest->AllocBuffer(src1->width,src1->height);
  Byte *ptr1=src1->buffer;
  float *ptr2=src2->buffer;
  Byte *ptr=newdata;
  for (int i=0; i<src1->size; i++)
    *(ptr++)=abs((int)(*(ptr1++))-(int)(*(ptr2++)));
  dest->NewBuffer(newdata,src1->width,src1->height);
}

void FindMotion(ByteImage *dest, ByteImage *src1, FloatImage *src2,
                int threshold, unsigned char maskValue)
{
  Byte *ptr1=src1->buffer;
  float *ptr2=src2->buffer;
  Byte *ptr=dest->buffer;
  for (int i=0; i<src1->size; i++)
  {
    if (abs((int)(*(ptr1++))-(int)(*(ptr2++)))>=threshold) 
      *ptr &= maskValue;
    ptr++;
  }
  dest->Change();
}

void FindMotion(ByteImage *dest, ByteImage *src1, ByteImage *src2,
                int threshold, unsigned char maskValue)
{
  Byte *ptr1=src1->buffer;
  Byte *ptr2=src2->buffer;
  Byte *ptr=dest->buffer;
  for (int i=0; i<src1->size; i++)
  {
    if (abs((int)(*(ptr1++))-(int)(*(ptr2++)))>=threshold) 
      *ptr &= maskValue;
    ptr++;
  }
  dest->Change();
}

void CentroidImage(ByteImage *image, double *ax, double *ay, 
                   double *vx, double *vy)
{
  double avgX=0, avgY=0;
  double varX=0, varY=0;
  double tot=0;
  Byte *valPtr=image->buffer;
  for (int y=0; y<image->height; y++)
    for (int x=0; x<image->width; x++)
    {
      double val=*(valPtr++);
      tot+=val;
      avgX+=x*val;
      varX+=x*x*val;
      avgY+=y*val;
      varY+=y*y*val;
    }
  if (tot==0) tot=1;
  avgX/=tot;
  avgY/=tot;
  varX=sqrt(varX/tot-avgX*avgX);
  varY=sqrt(varY/tot-avgY*avgY);
  *ax=avgX;
  *ay=avgY;
  *vx=varX;
  *vy=varY;
}

void AddByteImages(ByteImage *dest, ByteImage *src1, ByteImage *src2)
{
  Byte *newdata=dest->AllocBuffer(src1->width,src1->height);
  for (int i=0; i<src1->size; i++)
    newdata[i]=(src1->data(i) >> 1) + (src2->data(i) >> 1);
  dest->NewBuffer(newdata,src1->width,src1->height);
}

void AddFloatImages(FloatImage *dest, FloatImage *src1, FloatImage *src2)
{
  float *newdata=(float*)malloc(sizeof(float)*src1->width*src1->height);
  for (int i=0; i<src1->size; i++)
    newdata[i]=src1->data(i)+src2->data(i);
  dest->NewBuffer(newdata,src1->width,src1->height);
}

void Threshold(ByteImage *image, int threshold, int inv)
{
  if (inv) 
  {
    for (int i=0; i<image->size; i++)
      if (image->data(i)>=threshold) image->data(i)=0; else
        image->data(i)=255;
  } else
  {
    for (int i=0; i<image->size; i++)
      if (image->data(i)>=threshold) image->data(i)=255; else
        image->data(i)=0;
  }
  image->Change();
}

void SmartThreshold(ByteImage *image)
{

  double hist[256];
  double total=image->size;
  for (int i=0; i<256; i++) hist[i]=0;
  for (i=0; i<image->size; i++)
    hist[image->data(i)]++;
  int bestk=0;
  int min=0, max=0;
  for (i=0; i<256; i++) if (hist[i]>0) max=i;
  for (i=256; i>=0; i--) if (hist[i]>0) min=i;
  if (min==max) bestk=max-1; else
  {
    double average=0.0;
    for (i=0; i<256; i++)
    {
      average+=i*hist[i];
      hist[i]/=total;
    }
    average/=total;
    double measure=0.0;
    bestk=-1;
    double classProb=0.0;
    double classAverage=0.0;
    for (int k=min; k<max; k++)
    {
      classProb+=hist[k];
      classAverage+=k*hist[k];
      double m=(average*classProb-classAverage)*
        (average*classProb-classAverage)/(classProb*(1.0-classProb));
      if (bestk==-1 || m>measure)
      {
	bestk=k;
	measure=m;
      }
    }
  }
//  fprintf(stderr,"threshold=%d\n",bestk);
  for (i=0; i<image->size; i++)
    if (image->data(i)>bestk) image->data(i)=255; else
      image->data(i)=0;
  image->Change();
}

void MirrorByteImage(ByteImage *image)
{
  for (int y=0; y<image->height; y++)
  {
    for (int x=0; x<image->width/2; x++)
    {
      Byte tmp=image->data(x,y);
      image->data(x,y)=image->data(image->width-x-1,y);
      image->data(image->width-x-1,y)=tmp;
    }
  }
  image->Change();
}

void MirrorFloatImage(FloatImage *image)
{
  for (int y=0; y<image->height; y++)
  {
    for (int x=0; x<image->width/2; x++)
    {
      float tmp=image->data(x,y);
      image->data(x,y)=image->data(image->width-x-1,y);
      image->data(image->width-x-1,y)=tmp;
    }
  }
  image->Change();
}

double CountAboveZero(ByteImage *image)
{
  int tot=0;
  for (int i=0; i<image->size; i++)
    if (image->data(i)>0) tot++;
  return (double)tot/(double)image->size;
}

void EvolveBackgroundImage(FloatImage *background, ByteImage *dat,
                           double rate)
{
  for (int i=0; i<background->size; i++)
    background->data(i)=background->data(i)*(1-rate)+dat->data(i)*rate;
  background->Change();
}

void Image_Init_NoTcl()
{
  static int initialized=0;
  if (!initialized) {
    Tcl_InitHashTable(&ImageTable,TCL_STRING_KEYS);
    initialized=1;
  }
}

////////////////////////////////////
// Tcl related wrappers and stuff //
////////////////////////////////////

#ifndef NoTcl
int RotateByteImage2Cmd(ClientData data, Tcl_Interp *interp,
                        int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: RotateByteImage2 <dest> <src> <angle>";
    return TCL_ERROR;
  }
  ByteImage *dest=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  double angle=atof(argv[3])*3.1415926/180.0;
  RotateByteImage2(dest,src,angle);
  return TCL_OK;
}

int WidthImageCmd(ClientData data, Tcl_Interp *interp,
                  int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: WidthImage <name>";
    return TCL_ERROR;
  } 
  char *name=argv[1];
  Image *image=FindImageWithName(name);
  if (image==NULL)
  {
    interp->result="Unknown image.";
    return TCL_ERROR;
  }
  sprintf(interp->result,"%d",image->width);
  return TCL_OK;
}

int HeightImageCmd(ClientData data, Tcl_Interp *interp,
                   int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: WidthHeight <name>";
    return TCL_ERROR;
  } 
  char *name=argv[1];
  Image *image=FindImageWithName(name);
  if (image==NULL)
  {
    interp->result="Unknown image.";
    return TCL_ERROR;
  }
  sprintf(interp->result,"%d",image->height);
  return TCL_OK;
}

int CreateByteImageCmd(ClientData data, Tcl_Interp *interp,
                       int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: CreateByteImage <name>";
    return TCL_ERROR;
  }
  char *name=argv[1];
  if (FindByteImageWithName(name)==NULL)
    new ByteImage(name);
  return TCL_OK;
}

int DeleteByteImageCmd(ClientData data, Tcl_Interp *interp,
                       int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: DeleteByteImage <name>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  if (image!=NULL)
    delete image;
  return TCL_OK;
}

int ZoomByteImageCmd(ClientData data, Tcl_Interp *interp,
                     int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: ZoomByteImage <dest> <scale> <src>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  double scale=atof(argv[2]);
  ByteImage *src=FindByteImageWithName(argv[3]);
  ZoomByteImage(image,scale,src);
  return TCL_OK;
}

int SubSampleWithAveragingCmd(ClientData data, Tcl_Interp *interp,
                              int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: SubSampleWithAveraging <dest> <src> <scale>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  int scale=atoi(argv[3]);
  SubSampleWithAveraging(image,src,scale);
  return TCL_OK;
}

int FillRectangleCmd(ClientData data, Tcl_Interp *interp,
                     int argc, char *argv[])
{
  if (argc!=7)
  {
    interp->result="Usage: FillRectangle <image> <x1> <y1> <x2> <y2> <color>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  int x1=atoi(argv[2]);
  int y1=atoi(argv[3]);
  int x2=atoi(argv[4]);
  int y2=atoi(argv[5]);
  int color=atoi(argv[6]);
  FillRectangle(image,x1,y1,x2,y2,color);
  return TCL_OK;
}

int GrowRegionCmd(ClientData data, Tcl_Interp *interp,
                  int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: GrowRegion <image> <dist>";
    return TCL_ERROR;
  }
//  ByteImage *image=FindByteImageWithName(argv[1]);
//  int dist=atoi(argv[1]);
  
  return TCL_OK;
}

int ResampleByteImageCmd(ClientData data, Tcl_Interp *interp,
                         int argc, char *argv[])
{
  if (argc!=6)
  {
    interp->result="Usage: ResampleByteImage <dest> <src> <width> <height> <scale>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  int width=atoi(argv[3]);
  int height=atoi(argv[4]);
  double scale=atof(argv[5]);
  ResampleByteImage(image,src,width,height,scale);
  return TCL_OK;
}

int ReduceSizeCmd(ClientData data, Tcl_Interp *interp,
                  int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: ReduceSize <to> <from>";
    return TCL_ERROR;
  }
  ByteImage *dest=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  ReduceSize(dest,src);
  return TCL_OK;
}

int SmoothByteImageCmd(ClientData data, Tcl_Interp *interp,
                       int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: SmoothByteImage <dest>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  SmoothByteImage(image);
  return TCL_OK;
}

int LoadByteImagesPpmCmd(ClientData data, Tcl_Interp *interp,
                         int argc, char *argv[])
{
  if (argc!=5) {
    interp->result="LoadByteImagesPpm <red> <green> <blue> <filename>";
    return TCL_ERROR;
  }
  ByteImage *red=FindByteImageWithName(argv[1]);
  ByteImage *green=FindByteImageWithName(argv[2]);
  ByteImage *blue=FindByteImageWithName(argv[3]);
  FILE *inf=fopen(argv[4],"rb");
  int tokens=0;
  int dataStart=0;
  int width=0, height=0;
  do {
    char line[80];
    //    dataStart=ftell(inf);
    fgets(line,80,inf);
    char *comment=strchr(line,'#');
    if (comment!=NULL) *comment=0;
    char *token=NULL, *input=&line[0];
    while (tokens<4 && (token=strtok(input," \t\n\r"))!=NULL)
    {
      switch (tokens)
      {
        case 0:
          if (strcmp(token,"P6")!=0)
          {
            fprintf(stderr,"Bad PGM file.\n");
            exit(1);
          }
          break;
        case 1:
          width=atoi(token);
          break;
        case 2:
          height=atoi(token);
          break;
        case 3:
          dataStart+=(token-&line[0])+strlen(token)+1;
          break;
        default:
          break;
      }
      tokens++;
      input=NULL;
    }
  } while (tokens<4);
//  fseek(inf,dataStart,SEEK_SET);
  unsigned char *rbuf=red->AllocBuffer(width,height);
  unsigned char *gbuf=green->AllocBuffer(width,height);
  unsigned char *bbuf=blue->AllocBuffer(width,height);
  unsigned char *buf=(unsigned char*)malloc(width*height*3);
  fread((char*)buf,1,width*height*3,inf);
  for (int i=0; i<width*height; i++) {
    rbuf[i]=buf[i*3];
    gbuf[i]=buf[i*3+1];
    bbuf[i]=buf[i*3+2];
  }
  free((char*)buf);
  red->NewBuffer(rbuf,width,height);
  green->NewBuffer(gbuf,width,height);
  blue->NewBuffer(bbuf,width,height);
  return TCL_OK;
}

int SaveByteImagesPpmCmd(ClientData data, Tcl_Interp *interp,
                         int argc, char *argv[])
{
  if (argc!=6)
  {
    interp->result="SaveByteImagesPpm <red> <green> <blue> <name> <file|stream>";
    return TCL_ERROR;
  }
  ByteImage *red=FindByteImageWithName(argv[1]);
  ByteImage *green=FindByteImageWithName(argv[2]);
  ByteImage *blue=FindByteImageWithName(argv[3]);
  char *file=argv[4];
  char *mode=argv[5];
  if (strcmp(mode,"stream")==0)
  {
    FILE *outf=GetOpenFile(interp,file,TCL_WRITABLE);;
    //    if (Tcl_GetOpenFile(interp,file,1,1,&outf)!=TCL_OK) return TCL_ERROR;
    SaveByteImagesPpm(red,green,blue,outf);
    FreeOpenFile(interp,file,outf);
  } else
  {
    FILE *outf=fopen(file,"wb");
    if (outf==NULL) return TCL_ERROR;
    SaveByteImagesPpm(red,green,blue,outf);
    fclose(outf);
  }
  return TCL_OK;
}

int SaveByteImagePgmCmd(ClientData data, Tcl_Interp *interp,
                        int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="SaveByteImagePgm <image> <name> <file|stream>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  char *file=argv[2];
  char *mode=argv[3];
  if (strcmp(mode,"stream")==0)
  {
    FILE *outf=GetOpenFile(interp,file,TCL_WRITABLE);
    //    if (Tcl_GetOpenFile(interp,file,1,1,&outf)!=TCL_OK) return TCL_ERROR;
    SaveByteImagePgm(image,outf);
    FreeOpenFile(interp,file,outf);
  } else
  {
    FILE *outf=fopen(file,"wb");
    if (outf==NULL) return TCL_ERROR;
    SaveByteImagePgm(image,outf);
    fclose(outf);
  }
  return TCL_OK;
}

int LoadByteImageGifCmd(ClientData data, Tcl_Interp *interp,
                        int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: LoadByteImageGif <image> <filename>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  char *file=argv[2];
  LoadByteImageGif(image,file);
  return TCL_OK;
}

int LoadByteImageRawCmd(ClientData data, Tcl_Interp *interp,
                        int argc, char *argv[])
{
  if (argc!=5)
  {
    interp->result="Usage: LoadByteImageRaw <name> <file> <width> <height>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  char *file=argv[2];
  int width=atoi(argv[3]);
  int height=atoi(argv[4]);
  FILE *inf=fopen(file,"rb");
  assert(inf!=NULL);
  LoadByteImageRaw(image,width,height,inf);
  fclose(inf);
  return TCL_OK;
}

int LoadByteImageHsiCmd(ClientData data, Tcl_Interp *interp,
                        int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: LoadByteImageHsi <name> <file> <file|stream>";
    return TCL_ERROR;
  }
  char *name=argv[1];
  char *file=argv[2];
  char *mode=argv[3];
  ByteImage *image=FindByteImageWithName(name);
  if (image==NULL)
  {
    interp->result="Unknown image";
    return TCL_ERROR;
  }
  FILE *inf;
  if (strcmp(mode,"file")==0)
  {
    inf=fopen(file,"rb");
    if (inf==NULL) return TCL_ERROR;
  } else
  {
    inf=GetOpenFile(interp,file,TCL_READABLE);
    //    if (Tcl_GetOpenFile(interp,file,0,1,&inf)!=TCL_OK) return TCL_ERROR;
  }
  LoadByteImageHsi(image,inf);
  if (strcmp(mode,"file")==0) fclose(inf); else FreeOpenFile(interp,file,inf);
  return TCL_OK;
}

#ifdef UseXpm
int LoadByteImageXpmCmd(ClientData data, Tcl_Interp *interp,
                        int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: LoadByteImage <name> <file>";
    return TCL_ERROR;
  }
  char *name=argv[1];
  char *file=argv[2];
  ByteImage *image=FindByteImageWithName(name);
  if (image==NULL)
  {
    interp->result="Unknown image";
    return TCL_ERROR;
  }
  XpmImage xpmImage;
  XpmInfo xpmInfo;
  if (XpmReadFileToXpmImage(file,&xpmImage,&xpmInfo)!=XpmSuccess)
  {
    interp->result="Bad file";
    return TCL_ERROR;
  }
  XpmFreeXpmImage(&xpmImage);
  XpmFreeXpmInfo(&xpmInfo);
  image->width=xpmImage.width;
  image->height=xpmImage.height;
  image->size=image->width*image->height;
  if (image->buffer!=NULL) free(image->buffer);
  image->buffer=(Byte*)malloc(image->size);
/*  XParseColor(display,colormap,name,&xcolor);*/
  for (int i=0; i<image->size; i++)
    image->data(i)=xpmImage.data[i];
  image->Change();
  return TCL_OK;
}
#endif

int CreateFloatImageCmd(ClientData data, Tcl_Interp *interp,
                        int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: CreateFloatImage <name>";
    return TCL_ERROR;
  }
  char *name=argv[1];
  new FloatImage(name);
  return TCL_OK;
}

int DeleteFloatImageCmd(ClientData data, Tcl_Interp *interp,
                        int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: DeleteFloatImage <name>";
    return TCL_ERROR;
  }
  FloatImage *image=FindFloatImageWithName(argv[1]);
  if (image!=NULL)
    delete image;
  return TCL_OK;
}

int ZeroFloatImageCmd(ClientData data, Tcl_Interp *interp,
                      int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: ZeroFloatImage <name> <width> <height>";
    return TCL_ERROR;
  }
  FloatImage *image=FindFloatImageWithName(argv[1]);
  int width=atoi(argv[2]);
  int height=atoi(argv[3]);
  float *newdata=(float*)malloc(sizeof(float)*width*height);
  for (int i=0; i<width*height; i++)
    newdata[i]=0.0;
  image->NewBuffer(newdata,width,height);
  return TCL_OK;
}

int ZeroByteImageCmd(ClientData data, Tcl_Interp *interp,
                     int argc, char *argv[])
{
  if (argc!=4 && argc!=5)
  {
    interp->result="Usage: ZeroByteImage <name> <width> <height> <fill>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  int width=atoi(argv[2]);
  int height=atoi(argv[3]);
  unsigned char fill=(argc==5)?atoi(argv[4]):0;
  image->ZeroBuffer(width,height,fill);
  return TCL_OK;
}

int CopyByteImageCmd(ClientData data, Tcl_Interp *interp,
                     int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: CopyByteImage <dest> <src>";
    return TCL_ERROR;
  }
  ByteImage *dest=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  CopyByteImage(dest,src);
  return TCL_OK;
}

int DifferenceImageCmd(ClientData data, Tcl_Interp *interp,
                       int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: DifferenceImage <dest> <src1> <src2>";
    return TCL_ERROR;
  }
  ByteImage *dest=FindByteImageWithName(argv[1]);
  ByteImage *src1=FindByteImageWithName(argv[2]);
  ByteImage *src2=FindByteImageWithName(argv[3]);
  DifferenceImage(dest,src1,src2);
  return TCL_OK;
}

int ExtractRectangleCmd(ClientData data, Tcl_Interp *interp,
                        int argc, char *argv[])
{
  if (argc!=7)
  {
    interp->result="Usage: ExtractRectangle <dest> <src> <x> <y> <width> <height>";
    return TCL_ERROR;
  }
  ByteImage *dest=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  int x=atoi(argv[3]);
  int y=atoi(argv[4]);
  int width=atoi(argv[5]);
  int height=atoi(argv[6]);
  ExtractRectangle(dest,src,x,y,width,height);
  return TCL_OK;
}

int ExtractAndRotateCmd(ClientData data, Tcl_Interp *interp,
                        int argc, char *argv[])
{
  if (argc!=8)
  {
    interp->result="Usage: ExtractAndRotate <dest> <src> <x> <y> <width> <height> <angle>";
    return TCL_ERROR;
  }
  ByteImage *dest=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  int x=atoi(argv[3]);
  int y=atoi(argv[4]);
  int width=atoi(argv[5]);
  int height=atoi(argv[6]);
  double angle=atof(argv[7]);
  ExtractAndRotate(dest,src,x,y,width,height,angle);
  return TCL_OK;
}

int DifferenceFloatImageCmd(ClientData data, Tcl_Interp *interp,
                            int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: DifferenceFloatImage <dest> <src1> <src2>";
    return TCL_ERROR;
  }
  ByteImage *dest=FindByteImageWithName(argv[1]);
  ByteImage *src1=FindByteImageWithName(argv[2]);
  FloatImage *src2=FindFloatImageWithName(argv[3]);
  DifferenceFloatImage(dest,src1,src2);
  return TCL_OK;
}

int CentroidImageCmd(ClientData data, Tcl_Interp *interp,
                     int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: CentroidImage <image>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  double avgX,avgY,varX,varY;
  CentroidImage(image,&avgX,&avgY,&varX,&varY);
  sprintf(interp->result,"%g %g %g %g",avgX,avgY,varX,varY);
  return TCL_OK;
}

int AddByteImagesCmd(ClientData data, Tcl_Interp *interp,
                 int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: AddByteImages <dest> <src1> <src2>";
    return TCL_ERROR;
  }
  ByteImage *dest=FindByteImageWithName(argv[1]);
  ByteImage *src1=FindByteImageWithName(argv[2]);
  ByteImage *src2=FindByteImageWithName(argv[3]);
  AddByteImages(dest,src1,src2);
  return TCL_OK;
}

int AddFloatImagesCmd(ClientData data, Tcl_Interp *interp,
                 int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: AddFloatImages <dest> <src1> <src2>";
    return TCL_ERROR;
  }
  FloatImage *dest=FindFloatImageWithName(argv[1]);
  FloatImage *src1=FindFloatImageWithName(argv[2]);
  FloatImage *src2=FindFloatImageWithName(argv[3]);
  AddFloatImages(dest,src1,src2);
  return TCL_OK;
}

int ThresholdCmd(ClientData data, Tcl_Interp *interp,
                 int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: Threshold <image> <threshold> <inv>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  int threshold=atoi(argv[2]);
  int inv=atoi(argv[3]);
  Threshold(image,threshold,inv);
  return TCL_OK;
}

int SmartThresholdCmd(ClientData data, Tcl_Interp *interp,
		      int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: SmartThreshold <image>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  SmartThreshold(image);
  return TCL_OK;
}

int MirrorByteImageCmd(ClientData data, Tcl_Interp *interp,
                   int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: MirrorByteImage <image>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  MirrorByteImage(image);
  return TCL_OK;
}

int MirrorFloatImageCmd(ClientData data, Tcl_Interp *interp,
                        int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: MirrorFloatImage <image>";
    return TCL_ERROR;
  }
  FloatImage *image=FindFloatImageWithName(argv[1]);
  MirrorFloatImage(image);
  return TCL_OK;
}

int CountAboveZeroCmd(ClientData data, Tcl_Interp *interp,
                      int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: CountAboveZero <image>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  sprintf(interp->result,"%g",CountAboveZero(image));
  return TCL_OK;
}

int EvolveBackgroundImageCmd(ClientData data, Tcl_Interp *interp,
                             int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: EvolveBackgroundImage <background> <new> <rate>";
    return TCL_ERROR;
  }
  FloatImage *background=FindFloatImageWithName(argv[1]);
  ByteImage *dat=FindByteImageWithName(argv[2]);
  double rate=atof(argv[3]);
  EvolveBackgroundImage(background,dat,rate);
  return TCL_OK;
}

int ByteToFloatImageCmd(ClientData data, Tcl_Interp *interp,
                        int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: ByteToFloatImage <byteimage> <floatimage>";
    return TCL_ERROR;
  }
  ByteImage *bi=FindByteImageWithName(argv[1]);
  FloatImage *fi=FindFloatImageWithName(argv[2]);
  ByteToFloatImage(bi,fi);
  return TCL_OK;
}

int PowerSpectrumFloatImageCmd(ClientData data, Tcl_Interp *interp,
                               int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: PowerSpectrumFloatImage <dest> <src>";
    return TCL_ERROR;
  }
  FloatImage *dest=FindFloatImageWithName(argv[1]);
  FloatImage *src=FindFloatImageWithName(argv[2]);
  PowerSpectrumFloatImage(dest,src);
  return TCL_OK;
}

int FFTFloatImageCmd(ClientData data, Tcl_Interp *interp,
                     int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: FFTFloatImage <src1> <src2> <forward>";
    return TCL_ERROR;
  }
  FloatImage *src1=FindFloatImageWithName(argv[1]);
  FloatImage *src2=FindFloatImageWithName(argv[2]);
  int forward=atoi(argv[3]);
  FFTFloatImage(src1,src2,forward);
  return TCL_OK;
}

int FindMotionCmd(ClientData data, Tcl_Interp *interp,
                  int argc, char *argv[])
{
  if (argc!=6)
  {
    interp->result="Usage: FindMotion <dest> <src1> <src2> <threshold> <mask>";
    return TCL_ERROR;
  }
  ByteImage *dest=FindByteImageWithName(argv[1]);
  ByteImage *src1=FindByteImageWithName(argv[2]);
  FloatImage *src2=FindFloatImageWithName(argv[3]);
  int threshold=atoi(argv[4]);
  unsigned char maskValue=atoi(argv[5]);
  FindMotion(dest,src1,src2,threshold,maskValue);
  return TCL_OK;
}

int LaplacianImageCmd(ClientData data, Tcl_Interp *interp,
                      int argc, char *argv[])
{
  if (argc!=3) {
    interp->result="Usage: LaplacianImage <dest> <src>";
    return TCL_ERROR;
  }
  ByteImage *dest=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  LaplacianImage(dest,src);
  return TCL_OK;
}

int SaveByteImageBinUnsignedCmd(ClientData data, Tcl_Interp *interp,
                                int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: SaveByteImageBinUnsigned <image> <file>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  char *file=argv[2];
  FILE *outf=fopen(file,"ab");
  fwrite((char*)image->buffer,image->size,1,outf);
  fclose(outf);
  return TCL_OK;
}

int Image_Init(Tcl_Interp *interp)
{
  Image_Init_NoTcl();
  Tcl_CreateCommand(interp,"SaveByteImageBinUnsigned",
                    SaveByteImageBinUnsignedCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"LaplacianImage",LaplacianImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"CountAboveZero",CountAboveZeroCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"CreateByteImage",CreateByteImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"DeleteByteImage",DeleteByteImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"LoadByteImage",LoadByteImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"LoadByteImageGif",LoadByteImageGifCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"LoadByteImageHsi",LoadByteImageHsiCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"CreateFloatImage",CreateFloatImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"DeleteFloatImage",DeleteFloatImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"WidthImage",WidthImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"HeightImage",HeightImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"ZeroFloatImage",ZeroFloatImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"ZeroByteImage",ZeroByteImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"CopyByteImage",CopyByteImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SmoothByteImage",SmoothByteImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"ZoomByteImage",ZoomByteImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"ResampleByteImage",ResampleByteImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SaveByteImagePgm",SaveByteImagePgmCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SaveByteImagesPpm",SaveByteImagesPpmCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"DifferenceImage",DifferenceImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"AddByteImages",AddByteImagesCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"AddFloatImages",AddFloatImagesCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"CentroidImage",CentroidImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SmartThreshold",SmartThresholdCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"Threshold",ThresholdCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"ReduceSize",ReduceSizeCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"LoadByteImageRaw",LoadByteImageRawCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"MirrorByteImage",MirrorByteImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"MirrorFloatImage",MirrorFloatImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"FillRectangle",FillRectangleCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"GrowRegion",GrowRegionCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"DifferenceFloatImage",DifferenceFloatImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"EvolveBackgroundImage",EvolveBackgroundImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL); 
  Tcl_CreateCommand(interp,"ExtractRectangle",ExtractRectangleCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL); 
  Tcl_CreateCommand(interp,"ExtractAndRotate",ExtractAndRotateCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"RotateByteImage2",RotateByteImage2Cmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SubSampleWithAveraging",SubSampleWithAveragingCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"PowerSpectrumFloatImage",
                    PowerSpectrumFloatImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"ByteToFloatImage",ByteToFloatImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"FFTFloatImage",FFTFloatImageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"FindMotion",FindMotionCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"LoadByteImagesPpm",LoadByteImagesPpmCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  return TCL_OK;
}
#endif

#include "img.hpp"

#ifndef NoVista

#include <vista/VImage.h>

VImage ByteImageToVista(char *byteimage)
{
  ByteImage *image=FindByteImageWithName(byteimage);
  VImage dest=VCreateImage(1,image->height,image->width,VUByteRepn);
  memcpy(dest->data,image->buffer,image->size);
  return dest;
}

VImage ByteImageToVista(ByteImage *image)
{
  VImage dest=VCreateImage(1,image->height,image->width,VUByteRepn);
  memcpy(dest->data,image->buffer,image->size);
  return dest;
}

void VistaToByteImage(char *byteimage, VImage src)
{
  ByteImage *image=FindByteImageWithName(byteimage);
  memcpy(image->buffer,src->data,image->size);
  if (VPixelRepn(src)==VSByteRepn) {
    for (int i=0; i<image->size; i++)
      image->buffer[i]+=127;
  }
}
#endif
