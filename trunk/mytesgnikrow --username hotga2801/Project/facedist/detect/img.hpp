#ifndef ImageIncluded
#define ImageIncluded

#ifndef WINNT
extern "C" {
#endif
#ifdef NoTcl
#include "tclHash.h"
#else
#include <tcl.h>
#endif
#ifndef NoVista
#include <vista/VImage.h>
#endif
#ifndef WINNT
};
#endif

#define NotAnImageType 0
#define ByteImageType 1
#define FloatImageType 2

typedef unsigned char Byte;

class Image
{
  public:
  void *link;
  char *name;
  int width, height, size;

  Image(char *name);
  virtual ~Image();
  virtual int GetType();
  virtual void ImageToBytes(unsigned char *data, int zoom,
                            double min, double max);
  virtual void *GetBuffer() { return NULL; }
  void Change();
};

class ByteImage : public Image
{
  public:
  Byte *buffer;

  ByteImage(char *name);
  virtual ~ByteImage();
  virtual void *GetBuffer() { return buffer; }
  virtual int GetType();
  virtual void ImageToBytes(unsigned char *data, int zoom,
                            double min, double max);
  double interpolate(double x, double y);
  double interpolateExtend(double x, double y);
  double interpolateLogPolar(double x, double y);
// USED: return the i'th pixel
  unsigned char & data(int i) { return buffer[i]; }
// USED: return the pixel at (i,j)
  unsigned char & data(int i, int j) { return buffer[i+j*width]; }
  void NewBuffer(Byte *buf, int w, int h);
  Byte *AllocBuffer(int w, int h);
  void FreeBuffer();
  void ZeroBuffer(int w, int h, unsigned char fill=0);
};

class FloatImage : public Image
{
  public:
  float *buffer;

  FloatImage(char *name);
  virtual ~FloatImage();
  virtual int GetType();
  virtual void *GetBuffer() { return buffer; }
  virtual void ImageToBytes(unsigned char *data, int zoom,
                            double min, double max);
  double interpolate(double i, double j);
  float & data(int i) { return buffer[i]; }
  float & data(int i, int j) { return buffer[i+j*width]; }
  void NewBuffer(float *buf, int w, int h);
  void FreeBuffer();
  void ZeroBuffer(int w, int h);
};

typedef void (ImageDataFunction)(void *image);
extern ImageDataFunction *UnlinkImageDataFunction;
extern ImageDataFunction *RedrawImageDataFunction;

Image *FindImageWithName(char *name);
ByteImage *FindByteImageWithName(char *name);
FloatImage *FindFloatImageWithName(char *name);

void RotateByteImage2(ByteImage *dest, ByteImage *src, double angle);
void ZoomByteImage(ByteImage *image, double scale, ByteImage *src);
void FillRectangle(ByteImage *image,
                   int x1, int y1, int x2, int y2, int color);
void SubSampleWithAveraging(ByteImage *dest, ByteImage *src, int scale);
void ResampleByteImage(ByteImage *image, ByteImage *src,
                       int newWidth, int newHeight, double scale);
void ReduceSize(ByteImage *dest, ByteImage *src);
void SmoothByteImage(ByteImage *image);
void SaveByteImagesPpm(ByteImage *red, ByteImage *green, ByteImage *blue,
                       FILE *outf);
void SaveByteImagePgm(ByteImage *image, FILE *outf);
void LoadByteImagePgm(ByteImage *image, FILE *inf);
void LoadByteImageRaw(ByteImage *dest, int width, int height, FILE *inf);
void LoadByteImageHsi(ByteImage *dest, FILE *inf);
void CopyByteImage(ByteImage *dest, ByteImage *src);
void DifferenceImage(ByteImage *dest, ByteImage *src1, ByteImage *src2);
void ExtrantRectangle(ByteImage *dest, ByteImage *src,
                      int x, int y, int width, int height);
void ExtractAndZoom(ByteImage *dest, ByteImage *src, double scale,
                    int minX, int minY, int maxX, int maxY);
void DifferenceFloatImage(ByteImage *dest, ByteImage *src1, FloatImage *src2);
void CentroidImage(ByteImage* image, double *ax, double *ay,
                   double *vx, double *vy);
void AddImages(ByteImage *dest, ByteImage *src1, ByteImage *src2);
void Threshold(ByteImage *image, int threshold, int inv);
void SmartThreshold(ByteImage *image);
void MirrorByteImage(ByteImage *image);
void MirrorFloatImage(FloatImage *image);
double CountAboveZero(ByteImage *image);
void EvolveBackgroundImage(FloatImage *background, ByteImage *dat,
                           double rate);
void FindMotion(ByteImage *dest, ByteImage *src1, FloatImage *src2,
                int threshold, unsigned char maskValue);
void FindMotion(ByteImage *dest, ByteImage *src1, ByteImage *src2,
                int threshold, unsigned char maskValue);
#ifndef NoVista
VImage ByteImageToVista(char *byteimage);
VImage ByteImageToVista(ByteImage *image);
#endif

void Image_Init_NoTcl();
#ifndef NoTcl
int Image_Init(Tcl_Interp *interp);
#endif

extern "C" {
unsigned char *LoadPpmStreamToBuffer(FILE *inf, int *width, int *height);
unsigned char *LoadPgmStreamToBuffer(FILE *inf, int *width, int *height);
unsigned char *LoadPgmToBuffer(char *filename, int *width, int *height);
unsigned char *LoadPpmToBuffer(char *filename, int *width, int *height);
}

void ExtractAndRotate(ByteImage *dest, ByteImage *src, int x, int y,
                      int width, int height, double angle);
void ExtractRectangle(ByteImage *dest, ByteImage *src, int x, int y,
                      int width, int height);

struct WidthHeight {
  int width, height;
};

#endif

