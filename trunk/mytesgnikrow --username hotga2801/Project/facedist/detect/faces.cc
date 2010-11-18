extern "C"
{
#ifndef NoTcl
#include <tcl.h>
#endif
#if !defined(hpux) && !defined(WINNT)
#include <math.h>
#endif
#ifndef NoVista
#include <vista/VImage.h>
#endif
#include <assert.h>
#ifndef __GNUG__
#include <string.h>
#endif
#ifndef NoTcl
#include "tclvista.h"
#endif
}
#if defined(hpux) || defined(WINNT)
#include <math.h>
#endif
#ifdef __GNUG__
#include <string.h>
#endif

#include "myhebp.hpp"
#include "system.hpp"
#include "mat.hpp"
#include "img.hpp"
#include "faces.hpp"
#include "fft.hpp"
#include "follow.hpp"
#ifndef NoVista
#include "imgops.hpp"
#endif

//#define Test

//#define sqr(num) ((num)*(num))

#ifndef M_PI
#define M_PI (4.0*atan(1))
#endif

struct FloatInt {
  float quality;
  int index;
};

int CompareFloatIntDecreasing(const void *p1, const void *p2)
{
  const FloatInt *fi1=(const FloatInt *)p1;
  const FloatInt *fi2=(const FloatInt *)p2;
  if (fi1->quality>fi2->quality) return -1;
  if (fi1->quality<fi2->quality) return 1;
  return 0;
}

int CompareFloatIntIncreasing(const void *p1, const void *p2)
{
  const FloatInt *fi1=(const FloatInt *)p1;
  const FloatInt *fi2=(const FloatInt *)p2;
  if (fi1->quality>fi2->quality) return 1;
  if (fi1->quality<fi2->quality) return -1;
  return 0;
}

// USED
// Given the alignment data structure and a vector of features for a
// face, return the scale and position and which the face should be
// detected.  Note: the position returned is the center of the face
// (and assumes the window is 20x20 pixels).
void CorrectFaceDetection(Alignment *alignment, double *features,
                          int *x, int *y, int *s, int *angle)
{
  // Compute least squares transformation between the features and the
  // average feature locations in the alignment structure
  Mat mat=Zero(alignment->numFeatures*2,4);
  Vec vec=Zero(alignment->numFeatures*2);
  for (int feature=0; feature<alignment->numFeatures; feature++) {
    double x=features[feature*2];
    double y=features[feature*2+1];
    mat(feature*2,0)=x;
    mat(feature*2,1)=-y;
    mat(feature*2,2)=1;
    mat(feature*2,3)=0;
    mat(feature*2+1,0)=y;
    mat(feature*2+1,1)=x;
    mat(feature*2+1,2)=0;
    mat(feature*2+1,3)=1;
    vec(feature*2)=alignment->average[feature*2];
    vec(feature*2+1)=alignment->average[feature*2+1];
  }
  Vec ans=LUInverse(Transpose(mat)*mat)*(Transpose(mat)*vec);

  // Apply that transformation to the center of the face (10,10)
  Mat xform=Identity(3);
  xform(0,0)=ans(0);
  xform(0,1)=-ans(1);
  xform(0,2)=ans(2);
  xform(1,0)=ans(1);
  xform(1,1)=ans(0);
  xform(1,2)=ans(3);
  xform=LUInverse(xform);
  Vec pt(3);
  pt(0)=10; pt(1)=10; pt(2)=1;
  pt=xform*pt;
  double xx=pt(0);
  double yy=pt(1);

  Vec pt1(3);
  pt1(0)=10; pt1(1)=20; pt1(2)=1;
  pt1=xform*pt1-pt;

  // Compute how the transformation scales the face, convert to level in
  // image pyramid
  double ss=sqrt(xform(0,0)*xform(1,1)-xform(0,1)*xform(1,0));
  ss=log(ss)/log(1.2);

  // Return the results
  *s=(int)floor(0.5+ss);
  if (*s<0) *s=0;
  *x=(int)floor(0.5+xx/pow(1.2,*s));
  *y=(int)floor(0.5+yy/pow(1.2,*s));
  *angle=(int)floor((atan2(pt1(1),pt1(0))-atan2(1,0))*180.0/M_PI+0.5);
  while (*angle<0) *angle+=360;
  *angle=(*angle) % 360;
}

void CorrectFaceDetectionValid(Alignment *alignment, double *features,
                               int *valid,
                               int *x, int *y, int *s, int *angle)
{
  // Compute least squares transformation between the features and the
  // average feature locations in the alignment structure
  
  int numFeatures=0;
  for (int i=0; i<alignment->numFeatures; i++) 
    if (valid[i]) numFeatures++;

  Mat mat=Zero(numFeatures*2,4);
  Vec vec=Zero(numFeatures*2);
  int ptr=0;
  for (int feature=0; feature<alignment->numFeatures; feature++) 
    if (valid[feature]) {
      double x=features[feature*2];
      double y=features[feature*2+1];
      mat(ptr*2,0)=x;
      mat(ptr*2,1)=-y;
      mat(ptr*2,2)=1;
      mat(ptr*2,3)=0;
      mat(ptr*2+1,0)=y;
      mat(ptr*2+1,1)=x;
      mat(ptr*2+1,2)=0;
      mat(ptr*2+1,3)=1;
      vec(ptr*2)=alignment->average[feature*2];
      vec(ptr*2+1)=alignment->average[feature*2+1];
      ptr++;
    }
  Vec ans=LUInverse(Transpose(mat)*mat)*(Transpose(mat)*vec);

  // Apply that transformation to the center of the face (10,10)
  Mat xform=Identity(3);
  xform(0,0)=ans(0);
  xform(0,1)=-ans(1);
  xform(0,2)=ans(2);
  xform(1,0)=ans(1);
  xform(1,1)=ans(0);
  xform(1,2)=ans(3);
  xform=LUInverse(xform);
  Vec pt(3);
  pt(0)=10; pt(1)=10; pt(2)=1;
  pt=xform*pt;
  double xx=pt(0);
  double yy=pt(1);

  Vec pt1(3);
  pt1(0)=10; pt1(1)=20; pt1(2)=1;
  pt1=xform*pt1-pt;

  // Compute how the transformation scales the face, convert to level in
  // image pyramid
  double ss=sqrt(xform(0,0)*xform(1,1)-xform(0,1)*xform(1,0));
  ss=log(ss)/log(1.2);

  // Return the results
  *s=(int)floor(0.5+ss);
  if (*s<0) *s=0;
  *x=(int)floor(0.5+xx/pow(1.2,*s));
  *y=(int)floor(0.5+yy/pow(1.2,*s));
  *angle=(int)floor((atan2(pt1(1),pt1(0))-atan2(1,0))*180.0/M_PI+0.5);
  while (*angle<0) *angle+=360;
  *angle=(*angle) % 360;
}

double GetAlignmentAngle(Alignment *alignment, int face)
{
  double a=alignment->xform[face*4];
  double b=alignment->xform[face*4+1];
  return atan2(b,a)*180.0/M_PI;
}

// USED
// Get X position of a feature of a face, under the current alignment 
// transformation
double GetAlignmentX(Alignment *alignment, int face, int feature)
{
  double x=alignment->data[face*alignment->numFeatures*2+feature*2];
  double y=alignment->data[face*alignment->numFeatures*2+feature*2+1];
  double a=alignment->xform[face*4];
  double b=alignment->xform[face*4+1];
  double c=alignment->xform[face*4+2];
  double d=alignment->xform[face*4+3];
  double xx=a*x-b*y+c;
  double yy=b*x+a*y+d;
  return xx;
}

// USED
// Get Y position of a feature of a face, under the current alignment
// transformation
double GetAlignmentY(Alignment *alignment, int face, int feature)
{
  double x=alignment->data[face*alignment->numFeatures*2+feature*2];
  double y=alignment->data[face*alignment->numFeatures*2+feature*2+1];
  double a=alignment->xform[face*4];
  double b=alignment->xform[face*4+1];
  double c=alignment->xform[face*4+2];
  double d=alignment->xform[face*4+3];
  double xx=a*x-b*y+c;
  double yy=b*x+a*y+d;
  return yy;
}

double GetAlignmentEntry(Alignment *alignment, int face, int component)
{
  return alignment->xform[face*4+component];
}

// USED
// Create data structure used by example alignment code
Alignment *InitAlignment(int numFaces, int numFeatures)
{
  Alignment *alignment=new Alignment;
  alignment->numFaces=numFaces;
  alignment->numFeatures=numFeatures;
  alignment->average=new double[numFeatures*2];
  alignment->data=new double[numFeatures*numFaces*2];
  alignment->xform=new double[numFaces*4];
  alignment->valid=new int[numFeatures*numFaces];
  alignment->avgValid=new int[numFeatures];
  for (int i=0; i<numFeatures*numFaces; i++) alignment->valid[i]=1;
  for (i=0; i<numFeatures*2*numFaces; i++) alignment->data[i]=0;
  for (i=0; i<numFeatures*2; i++) alignment->average[i]=0;
  for (i=0; i<numFeatures; i++) alignment->avgValid[i]=0;
  return alignment;
}

// USED
// Copy average facial feature locations from another alignment structure
void CopyAverageFeatures(Alignment *align, Alignment *other)
{
  for (int i=0; i<align->numFeatures*2; i++) 
    align->average[i]=other->average[i];
  for (i=0; i<align->numFeatures; i++)
    align->avgValid[i]=other->avgValid[i];
}

// USED
// Free up the alignment information
void FreeAlignment(Alignment *alignment)
{
  delete[] alignment->average;
  delete[] alignment->data;
  delete[] alignment->xform;
  delete[] alignment->valid;
  delete[] alignment->avgValid;
  delete alignment;
}

// USED
// Set location of a feature of a face
void SetFaceFeature(Alignment *alignment, int face, int feature,
                    double x, double y, int fixscale)
{
  if (fixscale) x*=640.0/512.0;
  alignment->data[face*alignment->numFeatures*2+feature*2]=x;
  alignment->data[face*alignment->numFeatures*2+feature*2+1]=y;
}

void SetFaceFeatureValid(Alignment *alignment, int face, int feature, 
                         int valid)
{
  alignment->valid[face*alignment->numFeatures+feature]=valid;
}

// USED
// Initialize the alignment transformations to identity
void RunAlignmentInit(Alignment *alignment)
{
  int i;
  for (i=0; i<alignment->numFeatures; i++) 
    alignment->avgValid[i]=alignment->valid[i];
  for (i=0; i<alignment->numFeatures*2; i++) 
    alignment->average[i]=alignment->data[i];
  for (i=0; i<alignment->numFaces; i++) {
    alignment->xform[i*4]=1;
    alignment->xform[i*4]=0;
    alignment->xform[i*4]=0;
    alignment->xform[i*4]=0;
  }
}

// USED
// Compute the average position of each feature over all the faces, 
// using their current alignments
void RunAlignmentAverage(Alignment *alignment)
{
  int i=0; 
  int *samples=new int[alignment->numFeatures];
  for (i=0; i<alignment->numFeatures*2; i++) alignment->average[i]=0;
  for (i=0; i<alignment->numFeatures; i++) samples[i]=0;
  for (int face=0; face<alignment->numFaces; face++) {
    // Get transformation
    double a=alignment->xform[face*4];
    double b=alignment->xform[face*4+1];
    double c=alignment->xform[face*4+2];
    double d=alignment->xform[face*4+3];
    for (int feature=0; feature<alignment->numFeatures; feature++) {
      if (alignment->valid[face*alignment->numFeatures+feature]) {
        double x=alignment->data[face*alignment->numFeatures*2+feature*2];
        double y=alignment->data[face*alignment->numFeatures*2+feature*2+1];
        // Transform each feature coordinate
        double xx=a*x-b*y+c;
        double yy=b*x+a*y+d;
        // Add into average
        alignment->average[feature*2]+=xx;
        alignment->average[feature*2+1]+=yy;
        samples[feature]++;
      }
    }
  }
  for (i=0; i<alignment->numFeatures; i++) {
    if (samples[i]>0) {
      alignment->average[i*2]/=samples[i];
      alignment->average[i*2+1]/=samples[i];
      alignment->avgValid[i]=1;
    } else 
      alignment->avgValid[i]=0;
  }
  delete[] samples;
}

void AlignmentDebug(Alignment *alignment, int face) 
{
}

// USED
// Get average feature X position
double GetAverageX(Alignment *alignment, int feature)
{
  return alignment->average[feature*2];
}

// USED
// Get average feature Y position
double GetAverageY(Alignment *alignment, int feature)
{
  return alignment->average[feature*2+1];
}

// USED
// Translate, rotate, and scale the average feature positions to set the
// positions of two of them to some desired values.  This allows the faces
// to be placed at particular locations in their windows.
void FixAverage(Alignment *alignment,
                int feature1, double x1, double y1,
                int feature2, double x2, double y2)
{
  double fx1=alignment->average[feature1*2];
  double fy1=alignment->average[feature1*2+1];
  double fx2=alignment->average[feature2*2];
  double fy2=alignment->average[feature2*2+1];
  // Set up linear system to solve for transformation
  Mat mat(4,4);
  mat(0,0)=fx1; mat(0,1)=-fy1; mat(0,2)=1; mat(0,3)=0;
  mat(1,0)=fy1; mat(1,1)=fx1; mat(1,2)=0; mat(1,3)=1;
  mat(2,0)=fx2; mat(2,1)=-fy2; mat(2,2)=1; mat(2,3)=0;
  mat(3,0)=fy2; mat(3,1)=fx2; mat(3,2)=0; mat(3,3)=1;
  Vec vec(4);
  vec(0)=x1;
  vec(1)=y1;
  vec(2)=x2;
  vec(3)=y2;
  Vec param=LUInverse(mat)*vec;
  // Transform the average coordinates
  for (int feature=0; feature<alignment->numFeatures; feature++) {
    double fx=alignment->average[feature*2];
    double fy=alignment->average[feature*2+1];
    alignment->average[feature*2]=fx*param(0)-fy*param(1)+param(2);
    alignment->average[feature*2+1]=fx*param(1)+fy*param(0)+param(3);
  }
}

#ifndef NoVista
VImage InputWeightsImage(int net, int from, int to)
{
//  if (to==2) FastDebugNetwork(real_fast_list[net]);
  int width, height, num;
  float *buf=FastInputWeights(real_fast_list[net],from,to,
                              &width,&height,&num);
  VImage result=VCreateImage(num,height,width,VFloatRepn);
  memcpy((char*)result->data,(char*)buf,
         sizeof(float)*width*height*num);
  free((char*)buf);
  return result;
}

VImage OutputWeightsImage(int net, int from, int to)
{
  int width, height, num;
  float *buf=FastOutputWeights(real_fast_list[net],from,to,
                              &width,&height,&num);
  VImage result=VCreateImage(num,height,width,VFloatRepn);
  memcpy((char*)result->data,(char*)buf,
         sizeof(float)*width*height*num);
  free((char*)buf);
  return result;
}

VImage ExtractFace(Alignment *alignment, int face,
                   ByteImage *image, int sx, int sy, int fixscale)
{
  VImage faceImage=VCreateImage(1,sy,sx,VUByteRepn);
  ByteImage *src=image;
  Mat mat=Identity(3);
  mat(0,0)=alignment->xform[face*4];
  mat(0,1)=-alignment->xform[face*4+1];
  mat(0,2)=alignment->xform[face*4+2];
  mat(1,0)=alignment->xform[face*4+1];
  mat(1,1)=alignment->xform[face*4];
  mat(1,2)=alignment->xform[face*4+3];
  mat=LUInverse(mat);
  if (fixscale) {
    mat(0,0)*=512.0/640.0;
    mat(0,1)*=512.0/640.0;
    mat(0,2)*=512.0/640.0;
  }
  double scale=floor(sqrt(mat(0,0)*mat(0,0)+mat(0,1)*mat(0,1)));
  if (scale>1) {
    src=new ByteImage(NULL);
    SubSampleWithAveraging(src,image,(int)scale);
  } else {
    src=image;
    scale=1;
  }
  mat(0,0)/=scale;
  mat(0,1)/=scale;
  mat(0,2)/=scale;
  mat(1,0)/=scale;
  mat(1,1)/=scale;
  mat(1,2)/=scale;
  for (int y=0; y<sy; y++)
    for (int x=0; x<sx; x++) {
      Vec pt(3);
      pt(0)=x;
      pt(1)=y;
      pt(2)=1;
      pt=mat*pt;
      VPixel(faceImage,0,y,x,VUByte)=(VUByte)src->interpolate(pt(0),pt(1));
    }
  if (src!=image) delete src;
  return faceImage;
}

PointList *PointListForRandomFace(Alignment *align, int face,
                                  int sx, int sy,
                                  int fixscale,
                                  double translationRange,
                                  double angleRange, double distStep,
                                  int flip)
{
  // a -b  c
  // b  a  d
  // 0  0  1
  Mat mat=Identity(3);
  mat(0,0)=align->xform[face*4];
  mat(0,1)=-align->xform[face*4+1];
  mat(0,2)=align->xform[face*4+2];
  mat(1,0)=align->xform[face*4+1];
  mat(1,1)=align->xform[face*4];
  mat(1,2)=align->xform[face*4+3];
  mat=LUInverse(mat);

  // For the Harvard images, convert to the right aspect ratio
  if (fixscale) {
    mat(0,0)*=512.0/640.0;
    mat(0,1)*=512.0/640.0;
    mat(0,2)*=512.0/640.0;
  }

  // Apply a small, random transformation
  double randScale=RandomDouble()*(sqrt(distStep)-1.0/sqrt(distStep))+
    1.0/sqrt(distStep);
  double randAngle=M_PI/180.0*(RandomDouble()*angleRange*2-angleRange);
  double randX=RandomDouble()*translationRange*2-translationRange;
  double randY=RandomDouble()*translationRange*2-translationRange;
  Mat rand=Translate2D(0.5*sx,0.5*sy)*
    Translate2D(randX,randY)*Scale2D(randScale)*Rotate2D(randAngle)*
    Translate2D(-0.5*sx,-0.5*sy);
  if ((flip==1 && RandomDouble()>0.5) || flip==2) {
    Mat f=Identity(3);
    f(0,0)=-1;
    rand=rand*Translate2D(0.5*sx,0)*f*Translate2D(-0.5*sx,0);
  }
  Mat xform=LUInverse(mat*rand);

  PointList *pl=new PointList;
  pl->numPoints=align->numFeatures;
  pl->x=new double[align->numFeatures];
  pl->y=new double[align->numFeatures];
  for (int i=0; i<align->numFeatures; i++) {
    Vec pt(3);
    pt(0)=align->data[face*align->numFeatures*2+i*2];
    pt(1)=align->data[face*align->numFeatures*2+i*2+1];
    pt(2)=1;
    pt=xform*pt;
    pl->x[i]=pt(0);
    pl->y[i]=pt(1);
  }
  return pl;
}

PointList *PointListForFeature(Alignment *align, int feature,
                               int sx, int sy,
                               double distStep, double translationRange)
{
  PointList *pl=new PointList;
  pl->numPoints=8;
  pl->x=new double[8];
  pl->y=new double[8];
  for (int s=0; s<=1; s++)
    for (int y=0; y<=1; y++)
      for (int x=0; x<=1; x++) {
        double randScale=s*(sqrt(distStep)-1.0/sqrt(distStep))+
          1.0/sqrt(distStep);
        double randX=x*translationRange*2-translationRange;
        double randY=y*translationRange*2-translationRange;
        Mat rand=Translate2D(0.5*sx,0.5*sy)*
          Translate2D(randX,randY)*Scale2D(randScale)*
          Translate2D(-0.5*sx,-0.5*sy);
        Vec pt(3);
        pt(0)=align->average[feature*2];
        pt(1)=align->average[feature*2+1];
        pt(2)=1;
        pt=rand*pt;
        pl->x[x*4+y*2+s]=pt(0);
        pl->y[x*4+y*2+s]=pt(1);
      }
  return pl;
}

double PointListX(PointList *pl, int num) 
{
  return pl->x[num];
}

double PointListY(PointList *pl, int num) 
{
  return pl->y[num];
}

double PointListNumPoints(PointList *pl)
{
  return pl->numPoints;
}

void FreePointList(PointList *pl)
{
  delete pl->x;
  delete pl->y;
  delete pl;
}

// USED
// Generate a training example, using the given alignment, and adding
// a small amount of translation, rotation, and scaling for robustness
VImage RandomizeAlignedTrainingImage(Alignment *alignment, int face,
                                     VImage src, VImage background,
                                     int sx, int sy,
                                     int fixscale,
                                     double translationRange,
                                     double angleRange, double distStep,
                                     int flip)
{
  VImage dest=VCreateImage(1,sy,sx,VUByteRepn);

  // a -b  c
  // b  a  d
  // 0  0  1
  Mat mat=Identity(3);
  mat(0,0)=alignment->xform[face*4];
  mat(0,1)=-alignment->xform[face*4+1];
  mat(0,2)=alignment->xform[face*4+2];
  mat(1,0)=alignment->xform[face*4+1];
  mat(1,1)=alignment->xform[face*4];
  mat(1,2)=alignment->xform[face*4+3];
  mat=LUInverse(mat);

  // For the Harvard images, convert to the right aspect ratio
  if (fixscale) {
    mat(0,0)*=512.0/640.0;
    mat(0,1)*=512.0/640.0;
    mat(0,2)*=512.0/640.0;
  }

  // Apply a small, random transformation
  double randScale=RandomDouble()*(sqrt(distStep)-1.0/sqrt(distStep))+
    1.0/sqrt(distStep);
  double randAngle=M_PI/180.0*(RandomDouble()*angleRange*2-angleRange);
  double randX=RandomDouble()*translationRange*2-translationRange;
  double randY=RandomDouble()*translationRange*2-translationRange;
//  Mat rand=Translate2D(0.5*sx,0.5*sy)*
//    Translate2D(randX,randY)*Scale2D(randScale)*Rotate2D(randAngle)*
//    Translate2D(-0.5*sx,-0.5*sy);

  Mat rand=Translate2D(0.5*sx,0.5*sy)*
    Scale2D(randScale)*Rotate2D(randAngle)*Translate2D(randX,randY)*
    Translate2D(-0.5*sx,-0.5*sy);

  if ((flip==1 && RandomDouble()>0.5) || flip==2) {
    Mat f=Identity(3);
    f(0,0)=-1;
    rand=rand*Translate2D(0.5*sx,0)*f*Translate2D(-0.5*sx,0);
  }
  Mat xform=mat*rand;

  // Figure out how much the transformation scales the image, reduce it
  // to an integer scaling, and apply that scaling (averaging blocks of
  // pixels).
  double actualScale=sqrt(fabs(xform(0,0)*xform(1,1)-xform(0,1)*xform(1,0)));
  int reduce=(int)floor(actualScale);
  if (reduce<1) reduce=1;
  VImage synth=src;
  if (background!=NULL) {
    // Create a new background, if we are trying to mask out the 
    // current background.
    synth=MakeRandomBackground(VImageNColumns(src),VImageNRows(src),reduce,-1);
    for (int y=0; y<VImageNRows(src); y++)
      for (int x=0; x<VImageNColumns(src); x++)
        if (x>=VImageNColumns(background) || 
            y>=VImageNRows(background) ||
            !VPixel(background,0,y,x,VUByte)) 
          VPixel(synth,0,y,x,VUByte)=VPixel(src,0,y,x,VUByte);
  }

//  PrintMatrix(xform);
  // Apply the scaling
  if (reduce>1) {
    VImage tmp=VReduceImage(synth,NULL,0,reduce,reduce);
    if (synth!=src) VDestroyImage(synth);
    synth=tmp;
    xform=Scale2D(1.0/reduce)*xform;
  }
 
  // Go through window pixel-by-pixel, getting interpolated pixel values
  // from the original image
  for (int y=0; y<sy; y++) for (int x=0; x<sx; x++) {
    Vec pt(3);
    pt(0)=x+0.5; pt(1)=y+0.5; pt(2)=1.0;
    pt=xform*pt;
    VPixel(dest,0,y,x,VUByte)=(int)floor(0.5+
                                         InterpolatePixel(synth,0,
                                                          pt(0)-0.5,
                                                          pt(1)-0.5));
  }
  if (synth!=src) VDestroyImage(synth);
  return dest;
}
#endif

// USED
// Given the current average position of each feature, compute the best
// (in least squares sense) alignment for each face to those averages.
// Allowed transformations are translation, rotation, and scaling.
void RunAlignmentXform(Alignment *alignment)
{
  for (int face=0; face<alignment->numFaces; face++) {
    int numvalid=0;
    for (int feature=0; feature<alignment->numFeatures; feature++) 
      if (alignment->avgValid[feature] &&
          alignment->valid[face*alignment->numFeatures+feature]) numvalid++;
    assert(numvalid>=2);
    // Set up the over-constrained linear system to solve
    Mat mat=Zero(numvalid*2,4);
    Vec vec=Zero(numvalid*2);
    int ptr=0;
    for (feature=0; feature<alignment->numFeatures; feature++) {
      if (alignment->avgValid[feature] &&
          alignment->valid[face*alignment->numFeatures+feature]) {
        double x=alignment->data[face*alignment->numFeatures*2+feature*2];
        double y=alignment->data[face*alignment->numFeatures*2+feature*2+1];
        mat(ptr*2,0)=x;
        mat(ptr*2,1)=-y;
        mat(ptr*2,2)=1;
        mat(ptr*2,3)=0;
        mat(ptr*2+1,0)=y;
        mat(ptr*2+1,1)=x;
        mat(ptr*2+1,2)=0;
        mat(ptr*2+1,3)=1;
        vec(ptr*2)=alignment->average[feature*2];
        vec(ptr*2+1)=alignment->average[feature*2+1];
        ptr++;
      }
    }
    // Solve for the transformation using pseudo-inverse
    Vec ans=LUInverse(Transpose(mat)*mat)*(Transpose(mat)*vec);
    alignment->xform[face*4]=ans(0);
    alignment->xform[face*4+1]=ans(1);
    alignment->xform[face*4+2]=ans(2);
    alignment->xform[face*4+3]=ans(3);
  }
}

void RunAlignmentXformNoRotation(Alignment *alignment)
{
  for (int face=0; face<alignment->numFaces; face++) {
    int numvalid=0;
    for (int feature=0; feature<alignment->numFeatures; feature++) 
      if (alignment->valid[face*alignment->numFeatures+feature]) numvalid++;
    // Set up the over-constrained linear system to solve
    Mat mat=Zero(numvalid*2,3);
    Vec vec=Zero(numvalid*2);
    int ptr=0;
    for (feature=0; feature<alignment->numFeatures; feature++) {
      if (alignment->valid[face*alignment->numFeatures+feature]) {
        double x=alignment->data[face*alignment->numFeatures*2+feature*2];
        double y=alignment->data[face*alignment->numFeatures*2+feature*2+1];
        mat(ptr*2,0)=x;
        mat(ptr*2,1)=1;
        mat(ptr*2,2)=0;
        mat(ptr*2+1,0)=y;
        mat(ptr*2+1,1)=0;
        mat(ptr*2+1,2)=1;
        vec(ptr*2)=alignment->average[feature*2];
        vec(ptr*2+1)=alignment->average[feature*2+1];
        ptr++;
      }
    }
    // Solve for the transformation using pseudo-inverse
    Vec ans=LUInverse(Transpose(mat)*mat)*(Transpose(mat)*vec);
    alignment->xform[face*4]=ans(0);
    alignment->xform[face*4+1]=0;
    alignment->xform[face*4+2]=ans(1);
    alignment->xform[face*4+3]=ans(2);
  }
}

#ifndef NoVista
#include "eigen.hpp"
#include "sparse.hpp"

void ComputeClosestExamples(VImage src) 
{
  int num=VImageNBands(src);
  VImage dest=VCreateImage(1,num,num,VShortRepn);
  VImage dist=VCreateImage(1,num,num,VFloatRepn);
  FloatInt *dists=new FloatInt[num];
  for (int i=0; i<num; i++) {
    for (int j=0; j<num; j++) {
      dists[j].index=j;
      double tot=0.0;
      for (int y=0; y<VImageNRows(src); y++)
        for (int x=0; x<VImageNColumns(src); x++)
          tot+=sqr(VPixel(src,i,y,x,VUByte)-VPixel(src,j,y,x,VUByte));
      tot=sqrt(tot)/(VImageNRows(src)*VImageNColumns(src));
      dists[j].quality=tot;
    }
    qsort(dists,num,sizeof(FloatInt),CompareFloatIntIncreasing);
    for (j=0; j<num; j++) {
      VPixel(dest,0,j,i,VShort)=dists[j].index;
      VPixel(dist,0,j,i,VFloat)=dists[j].quality;
    }
    fprintf(stderr,"%d/%d\r",i,num);
  }
  delete[] dists;
  WriteVistaImage(dest,"/IUS/har1/har/pearl/nearest.v");
  WriteVistaImage(dist,"/IUS/har1/har/pearl/nearestDistance.v");
  VDestroyImage(dest);
  VDestroyImage(dist);
}

void UpdateImageLocations(VImage xy, VImage nearest, VImage distances,
                          double rate)
{
  int num=VImageNBands(xy);
  double *deltax=new double[num];
  double *deltay=new double[num];
  for (int i=0; i<num; i++) {
    deltax[i]=0;
    deltay[i]=0;
    for (int j=1; j<=4; j++) {
      int index=VPixel(nearest,0,j,i,VShort);
      deltax[i]-=(VPixel(xy,i,0,0,VFloat)-VPixel(xy,index,0,0,VFloat))/
        VPixel(distances,0,j,i,VFloat);
      deltay[i]-=(VPixel(xy,i,0,1,VFloat)-VPixel(xy,index,0,1,VFloat))/
        VPixel(distances,0,j,i,VFloat);
    }
  }
  for (i=0; i<num; i++) 
    if (i!=0 && i!=37 && i!=(38*30)-1 && i!=(38*30)-38) {
      VPixel(xy,i,0,0,VFloat)+=deltax[i]*rate;
      VPixel(xy,i,0,1,VFloat)+=deltay[i]*rate;
    }
}

void GenerateMeshFile(char *filename, VImage heights, VImage normals)
{
  FILE *f=fopen(filename,"w");
  fprintf(f,"CZMESH\n");
  fprintf(f,"%d %d\n",VImageNColumns(heights),VImageNRows(heights));
  for (int y=0; y<VImageNRows(heights); y++)
    for (int x=0; x<VImageNColumns(heights); x++) {
      double mag=sqrt(sqr(VPixel(normals,0,y,x,VFloat))+
                      sqr(VPixel(normals,1,y,x,VFloat))+
                      sqr(VPixel(normals,2,y,x,VFloat)))/255.0;
      //      double mag=VPixel(normals,2,y,x,VFloat)/255.0;
      fprintf(f,"%g %g %g %g %g\n",VPixel(heights,0,y,x,VFloat),
              mag,mag,mag,0.0,1.0);
    }
  fclose(f);
}

void GenerateMeshNormalFile(char *filename, VImage heights, VImage normals)
{
  FILE *f=fopen(filename,"w");
  fprintf(f,"{ LIST\n");
  fprintf(f,"{ CZMESH\n");
  fprintf(f,"%d %d\n",VImageNColumns(heights),VImageNRows(heights));
  for (int y=0; y<VImageNRows(heights); y++)
    for (int x=0; x<VImageNColumns(heights); x++) {
      double mag=sqrt(sqr(VPixel(normals,0,y,x,VFloat))+
                      sqr(VPixel(normals,1,y,x,VFloat))+
                      sqr(VPixel(normals,2,y,x,VFloat)))/255.0;
      //      double mag=VPixel(normals,2,y,x,VFloat)/255.0;
      fprintf(f,"%g %g %g %g %g\n",VPixel(heights,0,y,x,VFloat),
              mag,mag,mag,0.0,1.0);
    }
  fprintf(f,"}\n");
  fprintf(f,"{ VECT\n");
  fprintf(f,"%d %d %d\n",VImageNRows(heights)*VImageNColumns(heights),
          VImageNRows(heights)*VImageNColumns(heights)*2,1);
  for (y=0; y<VImageNRows(heights); y++)
    for (int x=0; x<VImageNColumns(heights); x++) 
      fprintf(f,"%d\n",2);
  for (y=0; y<VImageNRows(heights); y++)
    for (int x=0; x<VImageNColumns(heights); x++) 
      fprintf(f,"%d\n",((x==0 && y==0)?1:0));
  for (y=0; y<VImageNRows(heights); y++)
    for (int x=0; x<VImageNColumns(heights); x++) {
      double mag=sqrt(sqr(VPixel(normals,0,y,x,VFloat))+
                      sqr(VPixel(normals,1,y,x,VFloat))+
                      sqr(VPixel(normals,2,y,x,VFloat)));
      fprintf(f,"%d %d %g %g %g %g\n",
              x,y,VPixel(heights,0,y,x,VFloat),
              x+VPixel(normals,0,y,x,VFloat)/mag,
              y+VPixel(normals,1,y,x,VFloat)/mag,
              VPixel(heights,0,y,x,VFloat)+VPixel(normals,2,y,x,VFloat)/mag);
    }
  fprintf(f,"1 1 1 1\n");
  fprintf(f,"} }\n");
  fclose(f);
}

VImage ShapeFromNormals(VImage normal)
{
  fprintf(stderr,"Building RHS...\n");
  int num=(VImageNRows(normal)-2)*(VImageNColumns(normal)-2)*2+
    (VImageNRows(normal)-2)*2+
    (VImageNColumns(normal)-2)*2;
  BigVector vec(num);
  int ptr=0;
  for (int y=0; y<VImageNRows(normal); y++)
    for (int x=0; x<VImageNColumns(normal); x++) {
      double mag=sqrt(sqr(VPixel(normal,0,y,x,VFloat))+
                      sqr(VPixel(normal,1,y,x,VFloat))+
                      sqr(VPixel(normal,2,y,x,VFloat)));
      if (x>0 && x<VImageNColumns(normal)-1)
        vec(ptr++)=-2.0*VPixel(normal,0,y,x,VFloat)/mag;
      if (y>0 && y<VImageNRows(normal)-1)
        vec(ptr++)=-2.0*VPixel(normal,1,y,x,VFloat)/mag;
    }
  fprintf(stderr,"ptr=%d, num=%d\n",ptr,num);
  fprintf(stderr,"Building LHS...\n");
  SparseMatrix mat(VImageNRows(normal)*VImageNColumns(normal));
  int colPtr=0;
  for (y=0; y<VImageNRows(normal); y++)
    for (int x=0; x<VImageNColumns(normal); x++) {
      BigVector col(num);
      int ptr=0;
      for (int yy=0; yy<VImageNRows(normal); yy++)
        for (int xx=0; xx<VImageNColumns(normal); xx++) {
          //          fprintf(stderr,"ptr=%d, xx=%d, yy=%d, x=%d, y=%d\n",
          //                  ptr,xx,yy,x,y);
          if (abs(x-xx)+abs(y-yy)==1) {
            double mag=sqrt(sqr(VPixel(normal,0,yy,xx,VFloat))+
                            sqr(VPixel(normal,1,yy,xx,VFloat))+
                            sqr(VPixel(normal,2,yy,xx,VFloat)));
            if (xx>0 && xx<VImageNColumns(normal)-1) {
              if (x==xx-1) {
                col(ptr++)=-VPixel(normal,2,yy,xx,VFloat)/mag;
              } else if (x==xx+1) {
                col(ptr++)=VPixel(normal,2,yy,xx,VFloat)/mag;
              } else
                col(ptr++)=0;
            }
            if (yy>0 && yy<VImageNRows(normal)-1) {
              if (y==yy-1) {
                col(ptr++)=-VPixel(normal,2,yy,xx,VFloat)/mag;
              } else if (y==yy+1) {
                col(ptr++)=VPixel(normal,2,yy,xx,VFloat)/mag;
              } else 
                col(ptr++)=0;
            }
          } else {
            if (xx>0 && xx<VImageNColumns(normal)-1) col(ptr++)=0;
            if (yy>0 && yy<VImageNRows(normal)-1) col(ptr++)=0;
          }
          //          fprintf(stderr,"ptr=%d, xx=%d, yy=%d, x=%d, y=%d\n",
          //                  ptr,xx,yy,x,y);
        }
      //      fprintf(stderr,"ptr=%d, num=%d, col=%d\n",ptr,num,colPtr);
      mat.column[colPtr++]=new SparseVector(col);
    }
  fprintf(stderr,"Preparing...\n");
  BigVector vec2=mat*vec;
  fprintf(stderr,"abs(vec)=%g, abs(vec2)=%g\n",abs(vec),abs(vec2));
  SparseMatrix mat2=mat*mat;
  PrintMatrix(mat);
  PrintMatrix(mat2);
  fprintf(stderr,"Solving...\n");
  BigVector solution=ConjugateGradient(mat2,vec2,0);
  fprintf(stderr,"Generating answer image...\n");
  VImage dest=VCreateImage(1,VImageNRows(normal),VImageNColumns(normal),
                           VFloatRepn);
  colPtr=0;
  for (y=0; y<VImageNRows(normal); y++)
    for (int x=0; x<VImageNColumns(normal); x++) 
      VPixel(dest,0,y,x,VFloat)=solution(colPtr++);
  fprintf(stderr,"Done.\n");
  return dest;
}

VImage EigenSpaceNoAverage(VImage src, int num) 
{
  int size=VImageNRows(src)*VImageNColumns(src);
  Mat images(VImageNBands(src),size);
  for (int band=0; band<VImageNBands(src); band++) {
    int ptr=0;
    for (int y=0; y<VImageNRows(src); y++) 
      for (int x=0; x<VImageNColumns(src); x++)
        images(band,ptr++)=VPixel(src,band,y,x,VUByte);
  }
  Mat cov=images*Transpose(images);
  //  PrintMatrix(cov);
  Vec *vecs=new Vec[num];
  double *vals=new double[num];
  FindEigenVectors(cov,vecs,vals,num);
  vecs[0]=Normalize(Transpose(images)*vecs[0]);
  vecs[1]=Normalize(Transpose(images)*vecs[1]);
  vecs[2]=Normalize(Transpose(images)*vecs[2]);
  //  PrintVector(vecs[0]);
  //  PrintVector(vecs[1]);
  //  PrintVector(vecs[2]);
  // Make a matrix of the basis vectors
  Mat eigen(size,num);
  for (int jj=0; jj<size; jj++) 
    for (int ii=0; ii<num; ii++) 
      eigen(jj,ii)=vecs[ii](jj);
  Mat orig=eigen;
  //  PrintMatrix(eigen);
  // Now rectify so the directions are right (roughly speaking)
  Mat j=Zero(3*VImageNBands(src),9);
  Vec e=Zero(3*VImageNBands(src));
  Mat proj=LUInverse(Transpose(eigen)*eigen)*Transpose(eigen)*
    Transpose(images);
  //  PrintMatrix(proj);
  for (band=0; band<VImageNBands(src); band++) {
    Vec dir;
    switch (band) {
#if 0
      case 0: dir=Normalize(Point3D(-10,-10,40)); break;
      case 1: dir=Normalize(Point3D(-10,10,40)); break;
      case 2: dir=Normalize(Point3D(10,10,40)); break;
      case 3: dir=Normalize(Point3D(10,-10,40)); break;
#endif 
#if 1
      case 0: dir=Normalize(Point3D(-0.5,-0.5,0.5)); break;
      case 1: dir=Normalize(Point3D(0,-0.5,0.5)); break;
      case 2: dir=Normalize(Point3D(0.5,-0.5,0.5)); break;
      case 3: dir=Normalize(Point3D(0.5,0,0.5)); break;
      case 4: dir=Normalize(Point3D(0.5,0.5,0.5)); break;
      case 5: dir=Normalize(Point3D(0,0.5,0.5)); break;
      case 6: dir=Normalize(Point3D(-0.5,0.5,0.5)); break;
      case 7: dir=Normalize(Point3D(-0.5,0,0.5)); break;
#endif
    }
    j(band*3,0)=dir(0);
    j(band*3,1)=dir(1);
    j(band*3,2)=dir(2);
    j(band*3+1,3)=dir(0);
    j(band*3+1,4)=dir(1);
    j(band*3+1,5)=dir(2);
    j(band*3+2,6)=dir(0);
    j(band*3+2,7)=dir(1);
    j(band*3+2,8)=dir(2);
    e(band*3)=proj(0,band);
    e(band*3+1)=proj(1,band);
    e(band*3+2)=proj(2,band);
  }
  //  PrintMatrix(Transpose(j)*j);
  //  PrintVector(Transpose(j)*e);
  //  PrintMatrix(LUInverse(Transpose(j)*j));
  Vec transvec=LUInverse(Transpose(j)*j)*Transpose(j)*e;
  Mat transmat(3,3);
  transmat(0,0)=transvec(0);
  transmat(0,1)=transvec(1);
  transmat(0,2)=transvec(2);
  transmat(1,0)=transvec(3);
  transmat(1,1)=transvec(4);
  transmat(1,2)=transvec(5);
  transmat(2,0)=transvec(6);
  transmat(2,1)=transvec(7);
  transmat(2,2)=transvec(8);
  //  PrintMatrix(transmat);
  eigen=eigen*transmat;
  VImage dest=VCreateImage(num,VImageNRows(src),VImageNColumns(src),
                           VFloatRepn);
  int ptr=0;
  for (int y=0; y<VImageNRows(src); y++) 
    for (int x=0; x<VImageNColumns(src); x++) {
      for (band=0; band<num; band++) 
        VPixel(dest,band,y,x,VFloat)=eigen(ptr,band);
      ptr++;
    }
  delete[] vals;
  delete[] vecs;
  return dest;
}

VImage BackgroundMaskColor(VImage rgb, VImage dest)
{
//  VImage dest=VCreateImage(3,VImageNRows(rgb),VImageNColumns(rgb),VUByteRepn);
  VSetBandInterp(dest,VBandInterpNone,1,VBandInterpNone,1,
                 VBandInterpRGB,3,VBandInterpNone,1);
//  VFillImage(dest,VAllBands,0);
  for (int y=0; y<VImageNRows(rgb); y++)
    for (int x=0; x<VImageNRows(rgb); x++) {
      double tot=0;
      for (int band=0; band<3; band++) {
        int v1=VPixel(rgb,band,y,x,VUByte);
        int v2=VPixel(rgb,band+3,y,x,VUByte);
        tot+=(v1-v2)*(v1-v2);
      }
      if (sqrt(tot)<20) {
        VPixel(dest,0,y,x,VUByte)=0; 
        VPixel(dest,1,y,x,VUByte)=255; 
        VPixel(dest,2,y,x,VUByte)=0; 
      }
    }
  return dest;
}

VImage BackgroundMaskBinary(VImage rgb)
{
  VImage dest=VCreateImage(1,VImageNRows(rgb),VImageNColumns(rgb),VUByteRepn);
  VFillImage(dest,VAllBands,0);
  for (int y=0; y<VImageNRows(rgb); y++)
    for (int x=0; x<VImageNRows(rgb); x++) {
      double tot=0;
      for (int band=0; band<3; band++) {
        int v1=VPixel(rgb,band,y,x,VUByte);
        int v2=VPixel(rgb,band+3,y,x,VUByte);
        tot+=(v1-v2)*(v1-v2);
      }
      if (sqrt(tot)<20) {
        VPixel(dest,0,y,x,VUByte)=255; 
      }
    }
  return dest;
}

VImage FaceLightSpaceColorCheck(VImage src, double height)
{
  int cams=5;
  VImage dest=VCreateImage(3*cams,VImageNRows(src),VImageNColumns(src),
                           VUByteRepn);
  VSetBandInterp(dest,VBandInterpNone,cams,VBandInterpNone,1,
                 VBandInterpRGB,3,VBandInterpNone,1);
  for (int band=0; band<cams; band++) {
    Vec dir;
    switch (band) {
//      case 0: dir=Point3D(3,-13,-24); break;
//      case 1: dir=Point3D(15,-11,-28); break;
//      case 2: dir=Point3D(12,8,-30); break;
//      case 3: dir=Point3D(9,8,-30); break;
//      case 4: dir=Point3D(1,5,-28); break;
      case 0: dir=Point3D(-8,7,-36); break;
      case 1: dir=Point3D(9,7,-36); break;
      case 2: dir=Point3D(9,-8,-40); break;
      case 3: dir=Point3D(-8,-8,-40); break;
      case 4: dir=Point3D(0,5,-40); break;
    }
    dir(1)-=height;
    dir=Normalize(dir)/sqr(Abs(dir))*30*30;
    if (band==4) dir=(75.0/60.0)*dir;

    for (int y=0; y<VImageNRows(src); y++) 
      for (int x=0; x<VImageNColumns(src); x++) {
        double val1=dir(0)*VPixel(src,0,y,x,VFloat)+
          dir(1)*VPixel(src,1,y,x,VFloat)+
          dir(2)*VPixel(src,2,y,x,VFloat);
        if (val1<0) val1=0;
        double intensity=val1;
        int r=(int)floor(0.5+intensity*VPixel(src,3,y,x,VFloat));
        int g=(int)floor(0.5+intensity*VPixel(src,4,y,x,VFloat));
        int b=(int)floor(0.5+intensity*VPixel(src,5,y,x,VFloat));
        if (r<0) r=0; if (r>255) r=255;
        if (g<0) g=0; if (g>255) g=255;
        if (b<0) b=0; if (b>255) b=255;
        VPixel(dest,band*3+0,y,x,VUByte)=r;
        VPixel(dest,band*3+1,y,x,VUByte)=g;
        VPixel(dest,band*3+2,y,x,VUByte)=b;
    }
  }
  return dest;
}

VImage FaceLightSpaceColorRobust(VImage rgb, double height, VImage mask) 
{
  int cams=5;
  for (int img=1; img<1+cams; img++) 
    for (int band=0; band<3; band++)
      for (int y=0; y<VImageNRows(rgb); y++)
        for (int x=0; x<VImageNColumns(rgb); x++) {
          int v1=VPixel(rgb,img*3+band,y,x,VUByte);
          int v2=VPixel(rgb,band,y,x,VUByte);
          v1-=v2;
          if (v1<0) v1=0;
          VPixel(rgb,img*3+band,y,x,VUByte)=v1;
        }

  Mat d(cams,3);
  for (int band=0; band<cams; band++) {
    Vec dir;
    switch (band) {
/*      case 0: dir=Normalize(Point3D(-12,-12-height,28)); break;
      case 1: dir=Normalize(Point3D(-10,10-height,36)); break;
      case 2: dir=Normalize(Point3D(10,10-height,36)); break;
      case 3: dir=Normalize(Point3D(0,-12-height,28)); break;*/
      case 0: dir=Point3D(0,-12,-12); break;
      case 1: dir=Point3D(12,-12,-24); break;
      case 2: dir=Point3D(-12,12,-24); break;
      case 3: dir=Point3D(12,12,-24); break;
      case 4: dir=Point3D(-6,12,-24); break;
/*      case 0: dir=Point3D(7,7,-36); break;
      case 1: dir=Point3D(-13,-8,-36); break;
      case 2: dir=Point3D(-13,7,-36); break;
      case 3: dir=Point3D(0,5,-37); break;
      case 4: dir=Point3D(7,-8,-22); break; */
    }
    dir(1)-=height;
    dir=Normalize(dir)/sqr(Abs(dir))*30*30;
    if (band==4) dir=(75.0/60.0)*dir;
    d(band,0)=dir(0);
    d(band,1)=dir(1);
    d(band,2)=dir(2);
  }

  Mat *proj=new Mat[cams];
  for (int skip=0; skip<cams; skip++) {
    int band=0;
    Mat mat(cams-1,3);
    for (int i=0; i<cams-1; i++) {
      if (band==skip) band++;
      mat(i,0)=d(band,0);
      mat(i,1)=d(band,1);
      mat(i,2)=d(band,2);
      band++;
    }
    proj[skip]=LUInverse(Transpose(mat)*mat)*Transpose(mat);
  }

  VImage result=VCreateImage(6,VImageNRows(rgb),VImageNColumns(rgb),
                             VFloatRepn);
  Vec inten(cams);

  for (int y=0; y<VImageNRows(rgb); y++)
    for (int x=0; x<VImageNColumns(rgb); x++) {
      for (int band=1; band<1+cams; band++) {
        double mag=((double)VPixel(rgb,band*3,y,x,VUByte)+
                    (double)VPixel(rgb,band*3+1,y,x,VUByte)+
                    (double)VPixel(rgb,band*3+2,y,x,VUByte))/3.0;
        inten(band-1)=mag;
      }
      int bestSkip=-1;
      double bestSkipQuality=0;
      for (int skip=0; skip<cams; skip++) {
        int band=0;
        Vec intensity(cams-1);
        for (int i=0; i<cams-1; i++) {
          if (band==skip) band++;
          intensity(i)=inten(band);
          band++;
        }
        Vec comp=proj[skip]*intensity;
#if 1
        Vec dir(3);
        dir(0)=d(skip,0);
        dir(1)=d(skip,1);
        dir(2)=d(skip,2);
        double guess=Dot(dir,comp);
        if (guess<0) guess=0;
        double diff=inten(skip)-guess;
        if (diff<0) diff=0;
        if (bestSkip==-1 || diff>bestSkipQuality) {
          bestSkip=skip;
          bestSkipQuality=diff;
        }
#endif
#if 0
        double diff=0;
        band=0;
        Vec dir(3);
        for (i=0; i<cams-1; i++) {
          if (band==skip) band++;
          dir(0)=d(band,0);
          dir(1)=d(band,1);
          dir(2)=d(band,2);
          double guess=Dot(dir,comp);
          if (guess<0) guess=0;
          diff+=fabs(inten(band)-guess);
          band++;
        }
        if (bestSkip==-1 || diff<bestSkipQuality) {
          bestSkip=skip;
          bestSkipQuality=diff;
        }
#endif
//        VPixel(mask,skip,y,x,VFloat)=diff/(cams-1);
      }
      VPixel(mask,bestSkip,y,x,VFloat)=255;
      band=0;
      Vec intensity(cams-1);
      double r=0, g=0, b=0, weight=0;
      for (int i=0; i<cams-1; i++) {
        if (band==bestSkip) band++;
        intensity(i)=inten(band);
        r+=VPixel(rgb,band*3,y,x,VUByte);
        g+=VPixel(rgb,band*3+1,y,x,VUByte);
        b+=VPixel(rgb,band*3+2,y,x,VUByte);
        weight+=intensity(i);
        band++;
      }
      if (weight>0) {
        r/=weight;
        g/=weight;
        b/=weight;
      }
      Vec comp=proj[bestSkip]*intensity;
      weight=Abs(comp);
      comp=Normalize(comp);
      VPixel(result,0,y,x,VFloat)=comp(0);
      VPixel(result,1,y,x,VFloat)=comp(1);
      VPixel(result,2,y,x,VFloat)=comp(2);
      VPixel(result,3,y,x,VFloat)=r*weight;
      VPixel(result,4,y,x,VFloat)=g*weight;
      VPixel(result,5,y,x,VFloat)=b*weight;
    }

  delete[] proj;
  return result;
}

void FixSubtractedImages(VImage rgb) 
{
  int cams=5;

  for (int img=2; img<2+cams; img++) 
    for (int band=0; band<3; band++)
      for (int y=0; y<VImageNRows(rgb); y++)
        for (int x=0; x<VImageNColumns(rgb); x++) {
          int v1=VPixel(rgb,img*3+band,y,x,VUByte);
          int v2=VPixel(rgb,band+3,y,x,VUByte);
          v1+=v2;
          if (v1>255) v1=255;
          VPixel(rgb,img*3+band,y,x,VUByte)=v1;
        }
}

void DoSubtractImages(VImage rgb) 
{
  int cams=5;

  for (int img=2; img<2+cams; img++) 
    for (int band=0; band<3; band++)
      for (int y=0; y<VImageNRows(rgb); y++)
        for (int x=0; x<VImageNColumns(rgb); x++) {
          int v1=VPixel(rgb,img*3+band,y,x,VUByte);
          int v2=VPixel(rgb,band+3,y,x,VUByte);
          v1-=v2;
          if (v1<0) v1=0;
          VPixel(rgb,img*3+band,y,x,VUByte)=v1;
        }
}

VImage FaceLightSpaceColor(VImage rgb, double height) 
{
  int cams=5;

  VImage result=VCreateImage(6,VImageNRows(rgb),VImageNColumns(rgb),
                             VFloatRepn);

  Mat d(cams,3);
  for (int band=0; band<cams; band++) {
    Vec dir;
    switch (band) {
//      case 0: dir=Point3D(3,-13,-24); break;
//      case 1: dir=Point3D(15,-11,-28); break;
//      case 2: dir=Point3D(12,8,-30); break;
//      case 3: dir=Point3D(9,8,-30); break;
//      case 4: dir=Point3D(1,5,-28); break;
      case 0: dir=Point3D(-8,7,-36); break;
      case 1: dir=Point3D(9,7,-36); break;
      case 2: dir=Point3D(9,-8,-40); break;
      case 3: dir=Point3D(-8,-8,-40); break;
      case 4: dir=Point3D(0,5,-40); break;
    }
    dir(1)-=height;
    dir=Normalize(dir)/sqr(Abs(dir))*30*30;
    if (band==4) dir=(75.0/60.0)*dir;
    d(band,0)=dir(0);
    d(band,1)=dir(1);
    d(band,2)=dir(2);
  }

  Mat proj=LUInverse(Transpose(d)*d)*Transpose(d);
  
  Vec inten(cams);

  for (int y=0; y<VImageNRows(rgb); y++)
    for (int x=0; x<VImageNColumns(rgb); x++) {
      double r=0, g=0, b=0, weight=0;
      for (int band=2; band<cams+2; band++) {
        int red=VPixel(rgb,band*3,y,x,VUByte)-VPixel(rgb,3,y,x,VUByte);
        int green=VPixel(rgb,band*3+1,y,x,VUByte)-VPixel(rgb,4,y,x,VUByte);
        int blue=VPixel(rgb,band*3+2,y,x,VUByte)-VPixel(rgb,5,y,x,VUByte);
        if (red<0) red=0;
        if (green<0) green=0;
        if (blue<0) blue=0;
        double mag=(double)(red+green+blue)/3.0;
        inten(band-2)=mag;
        r+=red;
        g+=green;
        b+=blue;
        weight+=mag;
      }
      if (weight==0) weight=1;
      Vec comp=proj*inten;
      weight=Abs(comp)/weight;
      comp=Normalize(comp);
      VPixel(result,0,y,x,VFloat)=comp(0);
      VPixel(result,1,y,x,VFloat)=comp(1);
      VPixel(result,2,y,x,VFloat)=comp(2);
      VPixel(result,3,y,x,VFloat)=r*weight;
      VPixel(result,4,y,x,VFloat)=g*weight;
      VPixel(result,5,y,x,VFloat)=b*weight;
    }

  return result;
}

VImage SynthesizeFaceLightingColor(VImage src, double x1, double y1, double z1,
                                   double x2, double y2, double z2)
{
  VImage dest=VCreateImage(3,VImageNRows(src),VImageNColumns(src),VUByteRepn);
  VSetBandInterp(dest,VBandInterpNone,1,VBandInterpNone,1,
                 VBandInterpRGB,3,VBandInterpNone,1);
  for (int y=0; y<VImageNRows(src); y++) 
    for (int x=0; x<VImageNColumns(src); x++) {
      double val1=x1*VPixel(src,0,y,x,VFloat)+
        y1*VPixel(src,1,y,x,VFloat)+
        z1*VPixel(src,2,y,x,VFloat);
      if (val1<0) val1=0;
      double val2=x2*VPixel(src,0,y,x,VFloat)+
        y2*VPixel(src,1,y,x,VFloat)+
        z2*VPixel(src,2,y,x,VFloat);
      if (val2<0) val2=0;
      double intensity=val1+val2;
      int r=(int)floor(0.5+intensity*VPixel(src,3,y,x,VFloat));
      int g=(int)floor(0.5+intensity*VPixel(src,4,y,x,VFloat));
      int b=(int)floor(0.5+intensity*VPixel(src,5,y,x,VFloat));
      if (r<0) r=0; if (r>255) r=255;
      if (g<0) g=0; if (g>255) g=255;
      if (b<0) b=0; if (b>255) b=255;
      VPixel(dest,0,y,x,VUByte)=r;
      VPixel(dest,1,y,x,VUByte)=g;
      VPixel(dest,2,y,x,VUByte)=b;
    }
  return dest;
}

VImage SynthesizeFaceLighting(VImage src, double x1, double y1, double z1,
                              double x2, double y2, double z2)
{
  VImage dest=VCreateImage(1,VImageNRows(src),VImageNColumns(src),VFloatRepn);
  for (int y=0; y<VImageNRows(src); y++) 
    for (int x=0; x<VImageNColumns(src); x++) {
      double val1=x1*VPixel(src,0,y,x,VFloat)+
        y1*VPixel(src,1,y,x,VFloat)+
        z1*VPixel(src,2,y,x,VFloat);
      if (val1<0) val1=0;
      double val2=x2*VPixel(src,0,y,x,VFloat)+
        y2*VPixel(src,1,y,x,VFloat)+
        z2*VPixel(src,2,y,x,VFloat);
      if (val2<0) val2=0;
      VPixel(dest,0,y,x,VFloat)=val1+val2;
    }
  return dest;
}
#endif

#ifndef NoTcl
int RandFaceRotationAndPositionCmd(ClientData data, Tcl_Interp *interp,
                                   int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: RandFaceRotationAndPosition <dest> <src> <params>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  char *params=argv[3];

  double xe1, ye1, xe2, ye2, xn, yn, xl, yl, xc, yc, xr, yr;
  sscanf(params,"{leye %lf %lf} {reye %lf %lf} {nose %lf %lf} "
         "{lmouth %lf %lf} {cmouth %lf %lf} {rmouth %lf %lf}",
         &xe1, &ye1, &xe2, &ye2, &xn, &yn,
         &xl, &yl, &xc, &yc, &xr, &yr);

  Vec leftEye=Point2D(xe1,ye1);
  Vec rightEye=Point2D(xe2,ye2);
  Vec nose=Point2D(xn,xn);
  Vec leftMouth=Point2D(xl,yl);
  Vec centerMouth=Point2D(xc,yc);
  Vec rightMouth=Point2D(xr,yr);

  Vec eyeCenter=(leftEye+rightEye)*0.5;
  Vec faceCenter=(eyeCenter+centerMouth)*0.5;

  double size=Abs(eyeCenter-centerMouth);
  double newSize=12.0;
  //  double newSize=12.0+2.0*(RandomDouble()-0.5);
  double scale=(newSize/size);
  //  double scale=(newSize/size)*(1.0+(RandomDouble()*0.2-0.1));
  double angle=atan2(ye2-ye1,xe2-xe1);
  int reduce=(int)floor(1.0/scale);
  if (reduce>1) {
    SubSampleWithAveraging(image,src,reduce);
    src=image;
    scale*=reduce;
    faceCenter/=reduce;
  }

  int xs=30;
  int ys=30;
  int flip=(RandomDouble()>0.5)?1:-1;
  Byte *newdata=(Byte*)malloc(xs*ys);
  double xcc=RandomDouble()*10.0+10.0;
  double ycc=RandomDouble()*10.0+10.0;
  double ang=(RandomDouble()*20.0-10.0)*M_PI/180.0;
  //  double ang=RandomDouble()*2.0*M_PI;
  angle-=ang;
  int ptr=0;
  for (int j=0; j<ys; j++)
    for (int i=0; i<xs; i++)
    {
      double x=flip*(i-xcc);
      double y=(j-ycc);
      double rx=(cos(angle)*x-sin(angle)*y)/scale+faceCenter(0);
      double ry=(sin(angle)*x+cos(angle)*y)/scale+faceCenter(1);
      newdata[ptr++]=(int)floor(0.5+src->interpolateExtend(rx,ry));
    }
  image->NewBuffer(newdata,xs,ys);
  angle=flip*ang;
  angle*=180.0/M_PI;
  if (angle>360.0) angle-=360.0;
  if (angle<0.0) angle+=360.0;
  sprintf(interp->result,"%g %g %g",angle,xcc,ycc);
  return TCL_OK;
}

int GenLinearGaussianCmd(ClientData data, Tcl_Interp *interp,
                         int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="GenLinearGaussian <image> <position> <width>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  double position=atof(argv[2]);
  double width=atof(argv[3]);
  for (int i=0; i<image->width; i++)
  {
    int val=(int)(0.5+exp(-(position-i)*(position-i)/(width*width))*254);
    if (val<0) val=0;
    if (val>254) val=254;
    image->data(i)=val;
  }
  image->Change();
  return TCL_OK;
}

int GenCircularGaussianCmd(ClientData data, Tcl_Interp *interp,
                           int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="GenCircularGaussian <image> <angle>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  double angle=atof(argv[2]);
  for (int i=0; i<image->width; i++)
  {
    int val=(int)(0.5+127.0*(1+cos(angle*M_PI/180.0-
                                   (double)i*M_PI*2.0/
                                   (double)image->width)));
    if (val<0) val=0;
    if (val>254) val=254;
    image->data(i)=val;
  }
  image->Change();
  return TCL_OK;
}

// USED
// Given profile, half profile, and frontal faces, train a "router" 
// network which distinguishes among these views, turning on one output
// which represents which view is present.  The profile and half profile
// images are presented both in normal and mirrored form, giving a total
// of 5 views.  Other parameters specify the number of epochs, learning
// parameters, and the size of the testing set.
void TrainFaceRouterNetwork(VImage profileImage,
                            VImage halfImage, VImage frontImage,
                            int network, 
                            int numEpochs, int batchSize,
                            double rate, double momentum, double decay,
                            int holdout)
{
  FastMakeNetTrainable(real_fast_list[network],batchSize);
  int width=VImageNColumns(frontImage), height=VImageNRows(frontImage);
  int inputsize=width*height;
  int profileRight=0, halfRight=0, front=0, halfLeft=0, profileLeft=0;
  int numProfile=VImageNFrames(profileImage)-holdout;
  int numHalf=VImageNFrames(halfImage)-holdout;
  int numFront=VImageNFrames(frontImage)-holdout;

  // Have enough batches to see each example about epochs times (this
  // assumes that there are roughly the same number of examples in 
  // each class)
  int batches=(((numProfile*2+numHalf*2+numFront)+batchSize-1)/batchSize)*
    numEpochs;
  if (batches<0) batches=1;

  for (int batch=0; batch<batches; batch++) {
    for (int i=0; i<batchSize; i++) {
      VSByte *ptr;
      int flip;
      int out;
      // Pick example type
      double which=RandomDouble();
      if (which<=0.2) {
        ptr=(VSByte*)VPixelPtr(profileImage,profileLeft,0,0);
        profileLeft=(profileLeft+1) % numProfile;
        flip=1;
        out=0;
      } else if (which<=0.4) {
        ptr=(VSByte*)VPixelPtr(halfImage,halfLeft,0,0);
        halfLeft=(halfLeft+1) % numHalf;
        flip=1;
        out=1;
      } else if (which<=0.6) {
        ptr=(VSByte*)VPixelPtr(frontImage,front,0,0);
        front=(front+1) % numFront;
        flip=(RandomDouble()>0.5);
        out=2;
      } else if (which<=0.8) {
        ptr=(VSByte*)VPixelPtr(halfImage,halfRight,0,0);
        halfRight=(halfRight+1) % numHalf;
        flip=0;
        out=3;
      } else {
        ptr=(VSByte*)VPixelPtr(profileImage,profileRight,0,0);
        profileRight=(profileRight+1) % numProfile;
        flip=0;
        out=4;
      }
      // Copy example into network, mirroring if necessary
      FastForwardUnit *unit=real_fast_list[network]->unitList+
        real_fast_list[network]->firstInput;
      if (!flip) {
        for (int j=0; j<inputsize; j++)
          (unit++)->train->activations[i]=(*(ptr++))/127.0;
      } else {
        for (int y=0; y<height; y++) {
          for (int x=0; x<width; x++)
            (unit++)->train->activations[i]=(*(ptr+width-x-1))/127.0;
          ptr+=width;
        }
      }
      // Set desired outputs: 1 for correct category, -1 for others
      unit=real_fast_list[network]->unitList+
        real_fast_list[network]->firstOutput;
      for (int j=0; j<5; j++)
        if (j==out) 
          (unit++)->train->delta[i]=1.0; else
            (unit++)->train->delta[i]=-1.0;
    }
    // Do the training
    FastTrainBatch(real_fast_list[network],rate,momentum,decay);
  }
}

// USED
// Apply the router network to examples of profiles, half profiles, 
// and frontal faces, and calculate the error rate.  If holdout is
// greater than zero, use the last holdout examples in each group,
// otherwise use all the examples.
// Perhaps it would be better to present the results as a confusion
// matrix?
void TestFaceRouterNetwork(VImage profileImage,
                           VImage halfImage, VImage frontImage,
                           int network, int holdout)
{
  int width=VImageNColumns(frontImage), height=VImageNRows(frontImage);
  int inputsize=width*height;
  int profileRight=0, halfRight=0, front=0, halfLeft=0, profileLeft=0;
  int numProfile=VImageNFrames(profileImage);
  int numHalf=VImageNFrames(halfImage);
  int numFront=VImageNFrames(frontImage);
  if (holdout>0) {
    profileRight=numProfile-holdout;
    profileLeft=numProfile-holdout;
    halfRight=numHalf-holdout;
    halfLeft=numHalf-holdout;
    front=numFront-holdout;
  }
  int which=0;
  int detection[5], falseDetection[5], num[5];
  // detection[i] = number of times view i was detected correctly
  // falseDetection[i] = number of times view i was detected, but another
  // view was presented
  for (int i=0; i<5; i++) {
    detection[i]=falseDetection[i]=num[i]=0;
  }
  while (which<5) {
    VSByte *ptr;
    int flip;
    int out;
    // Select example of the appropriate type
    if (which==0) {
      ptr=(VSByte*)VPixelPtr(profileImage,profileLeft++,0,0);
      if (profileLeft==numProfile) which++;
      flip=1;
      out=0;
    } else if (which==1) {
      ptr=(VSByte*)VPixelPtr(halfImage,halfLeft++,0,0);
      if (halfLeft==numHalf) which++;
      flip=1;
      out=1;
    } else if (which==2) {
      ptr=(VSByte*)VPixelPtr(frontImage,(front++)%numFront,0,0);
      if (front==numFront*2) which++;
      flip=(front>=numFront);
      if (front==numFront && holdout>0) 
        front=numFront*2-holdout;
      out=2;
    } else if (which==3) {
      ptr=(VSByte*)VPixelPtr(halfImage,halfRight++,0,0);
      if (halfRight==numHalf) which++;
      flip=0;
      out=3;
    } else {
      ptr=(VSByte*)VPixelPtr(profileImage,profileRight++,0,0);
      if (profileRight==numProfile) which++;
      flip=0;
      out=4;
    }
    // Put example into network, mirroring image if necessary
    FastForwardUnit *unit=real_fast_list[network]->unitList+
      real_fast_list[network]->firstInput;
    if (!flip) {
      for (int j=0; j<inputsize; j++)
        (unit++)->activation=(*(ptr++))/127.0;
    } else {
      for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++)
          (unit++)->activation=(*(ptr+width-x-1))/127.0;
        ptr+=width;
      }
    }
    // Apply network
    FastForwardPass(real_fast_list[network]);
    unit=real_fast_list[network]->unitList+
      real_fast_list[network]->firstOutput;
    // Evaluate the results
    for (int j=0; j<5; j++) if (j==out) {
      if ((unit++)->activation>0) detection[out]++;
    } else {
      if ((unit++)->activation>0) falseDetection[out]++;
    }
    num[out]++;
  }
  // Print out the results
  int totalDetection=0, totalFalseDetection=0, totalNum=0;
  for (i=0; i<5; i++) {
    fprintf(stderr,"Rates[%d]: Detection: %g (%d/%d)  False: %g (%d/%d)\n",i,
            (double)detection[i]/(double)(num[i]),
            detection[i],num[i],
            (double)falseDetection[i]/(double)(num[i]),
            falseDetection[i],num[i]);
    totalNum+=num[i];
    totalDetection+=detection[i];
    totalFalseDetection+=falseDetection[i];
  }
  fprintf(stderr,"Overall: Detection: %g (%d/%d)  False: %g (%d/%d)\n",
          (double)totalDetection/(double)(totalNum),
          totalDetection,totalNum,
          (double)totalFalseDetection/(double)(totalNum),
          totalFalseDetection,totalNum);
}

// USED
// Run through a set of face images, feeding them into the router 
// network.  Three of the outputs from the network are selected and
// printed as outputs, to give a 3D plot of the distribution of
// router outputs.
void FaceRouterOutputVista(VImage images, int flip, int network, int label,
                           char *filename)
{
  FILE *outf=fopen(filename,"w");
  assert(outf!=NULL);
  fprintf(outf,"create %d\n",label);
  int width=VImageNColumns(images), height=VImageNRows(images);
  int inputsize=width*height;
  int total=0;

  for (int i=0; i<VImageNFrames(images); i++) {
    // Put image into network, mirroring if necessary
    VSByte *ptr=(VSByte*)VPixelPtr(images,i,0,0);
    FastForwardUnit *unit=real_fast_list[network]->unitList+
      real_fast_list[network]->firstInput;
    if (!flip) {
      for (int j=0; j<inputsize; j++)
        (unit++)->activation=(*(ptr++))/127.0;
    } else {
      for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++)
          (unit++)->activation=(*(ptr+width-x-1))/127.0;
        ptr+=width;
      }
    }
    FastForwardPass(real_fast_list[network]);
    unit=real_fast_list[network]->unitList+
      real_fast_list[network]->firstOutput;
#if 0
    fprintf(outf,"%d %g %g %g %g %g\n",label,
            unit[0].activation,
            unit[1].activation,
            unit[2].activation,
            unit[3].activation,
            unit[4].activation);
#else
    // Print the coords
    fprintf(outf,"point %d %g %g %g\nspot %d %d\n",
            total,unit[2].activation,unit[3].activation,unit[4].activation,
            total,label);
#endif
    total++;
  }
  fprintf(outf,"done\n");
  fclose(outf);
}

// USED
// Run through a set of nonface images, feeding them into the router 
// network.  Three of the outputs from the network are selected and
// printed as outputs, to give a 3D plot of the distribution of
// router outputs.  Stop after a given number of points have been 
// collected.
void FaceRouterOutputImage(ByteImage *image, ByteImage *mask,
                           int network, int label, int stopat,
                           char *filename)
{
  assert(image!=NULL);
  FILE *outf=fopen(filename,"w");
  assert(outf!=NULL);
  fprintf(outf,"create %d\n",label);

  int i;
  int ptr=0;
  int *tmp=new int[mask->size];

  int halfW=mask->width/2;
  int halfH=mask->height/2;

  double scale=1.0;
  int total=0;
  while (image->height>=mask->height && image->width>=mask->width &&
         total<stopat) {
    for (int y=0; y<image->height && total<stopat; y++) {
      for (int x=0; x<image->width && total<stopat; x++) {
	// Extract window from image, compute histogram
        int ptr=0;
        int hist[256];
        for (i=0; i<256; i++) hist[i]=0;
        for (int j=-halfH; j<halfH; j++) for (int i=-halfW; i<halfW; i++) 
        {
          int ii=i+x;
          int jj=j+y;
          if (ii<0) ii=0;
          if (ii>=image->width) ii=image->width-1;
          if (jj<0) jj=0;
          if (jj>=image->height) jj=image->height-1;
          int data=((int)image->data(ii,jj));
          tmp[ptr++]=data;
          hist[data]++;
        }

	// Compute cummulative histogram
        int map[256];
        int num=0;
        for (i=0; i<256; i++) {
          map[i]=num;
          num+=hist[i];
        }
        int tot=num;
        for (i=255; i>=0; i--) {
          map[i]+=num;
          num-=hist[i];
        }

	// Feed into NN
        double scaleFactor=1.0/tot;
        FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;
        FastForwardPass(real_fast_list[0]);
        unit=real_fast_list[network]->unitList+
          real_fast_list[network]->firstOutput;
#if 0
        fprintf(outf,"%d %g %g %g %g %g\n",label,
                unit[0].activation,
                unit[1].activation,
                unit[2].activation,
                unit[3].activation,
                unit[4].activation);
#else
	// Record output
        fprintf(outf,"point %d %g %g %g\nspot %d %d\n",
                total,unit[2].activation,unit[3].activation,unit[4].activation,
                total,label);
#endif
        total++;
      }
    }
    // Go to next level in pyramid
    ZoomByteImage(image,1.2,image);
    scale*=1.2;
  }
  delete[] tmp;
  fprintf(outf,"done\n");
  fclose(outf);
}

#ifndef NoHEBP
int TrainFaceRotationCmd(ClientData data, Tcl_Interp *interp,
                         int argc, char *argv[])
{
  if (argc!=5)
  {
    interp->result="Usage: TrainFaceRotation <examples> <answers> <epochsperbatch> <batches>";
    return TCL_ERROR;
  }
  char *exampleFile=argv[1];
  char *answerFile=argv[2];
  int epochsPerBatch=atoi(argv[3]);
  int batches=atoi(argv[4]);

  int exampleSize=900+20+20+18;
  int numExamples=FileSize(exampleFile)/exampleSize;
  //  numExamples=4000;
  signed char *dat=(signed char *)malloc(numExamples*exampleSize);
  fprintf(stderr,"Loading %d examples...\n",numExamples);
  FILE *inf=fopen(exampleFile,"r");
  fread((char*)dat,numExamples,exampleSize,inf);
  fclose(inf);

  fprintf(stderr,"Loading answers...\n");
  float *answerAngle=new float[numExamples];
  float *answerX=new float[numExamples];
  float *answerY=new float[numExamples];
  inf=fopen(answerFile,"r");
  for (int i=0; i<numExamples; i++)
    fscanf(inf,"%g%g%g",&answerAngle[i],&answerX[i],&answerY[i]);
  fclose(inf);

  fprintf(stderr,"Permuting examples...\n");
  for (i=0; i<numExamples; i++)
  {
    int dest=random() % numExamples;
    for (int j=0; j<900+20+20+18; j++)
    {
      signed char tmp=dat[i*(900+20+20+18)+j];
      dat[i*(900+20+20+18)+j]=dat[dest*(900+20+20+18)+j];
      dat[dest*(900+20+20+18)+j]=tmp;
    }
    float tmp=answerAngle[i]; answerAngle[i]=answerAngle[dest];
    answerAngle[dest]=tmp;
    tmp=answerX[i]; answerX[i]=answerX[dest]; answerX[dest]=tmp;
    tmp=answerY[i]; answerY[i]=answerY[dest]; answerY[dest]=tmp;
  }

  int useExamples=1000;
  //  int useExamples=numExamples;
  for (int batch=-1; batch<batches; batch++)
  {
    if (batch>-1)
    {
      real_net->checkpoint_interval=epochsPerBatch*(useExamples+99)/100;
      fprintf(stderr,"Batch %d...\n",batch);
      for (int epoch=0; epoch<epochsPerBatch; epoch++)
      {
        fprintf(stderr,"Epoch %d...\n",epoch);
        for (int pat=0; pat<useExamples; pat+=100)
        {
          int max=pat+99;
          if (max>=useExamples) max=useExamples-1;
          fprintf(stderr,".");
          //          fprintf(stderr,"Patterns %d-%d...\n",pat,max);
          for (int i=pat; i<=max; i++)
          {
            unit_type *unit=&(real_net[0].unit[real_net[0].input_index[1]]);
            for (int j=i*exampleSize; j<i*exampleSize+900; j++, unit++)
              unit->output[i-pat]=dat[j]/127.0;
            unit=&(real_net[0].unit[real_net[0].output_index[0]]);
            for (j=i*exampleSize+900; j<(i+1)*exampleSize; j++, unit++)
              unit->target[i-pat]=dat[j]/127.0;
          }
          HEBP_do_epoch(real_net,0,max-pat+1,
                        1, /* !checkpoint */ 
                        0 /* !verbose */ );
          fflush(stdout);
        }
        fprintf(stderr,"\n");
      }
    }
    fprintf(stderr,"Testing network...\n");
    if (real_fast!=NULL) delete real_fast;
    real_fast=FastConstructForward(real_net);
    signed char *ptr=dat;
    double distErr=0;
    double angleErr=0;
    for (i=0; i<useExamples; i++)
    {
      for (int j=1; j<=900; j++)
        real_fast->unitList[j].activation=(*(ptr++))/127.0;
      //      for (j=0; j<58; j++)
      //        real_fast->unitList[j+real_fast->firstOutput].activation=
      //          (*(ptr++))/127.0;
      ptr+=20+20+18;
      FastForwardPass(real_fast);
      double x=0, tot=0;
      for (j=0; j<20; j++)
      {
        double act=1.0+
          real_fast->unitList[real_fast->firstOutput+j].activation;
        tot+=act;
        x+=act*(j+5);
      }
      x/=tot;
      double y=0;
      tot=0;
      for (j=0; j<20; j++)
      {
        double act=1.0+
          real_fast->unitList[real_fast->firstOutput+j+20].activation;
        tot+=act;
        y+=act*(j+5);
      }
      y/=tot;
      double a1=0, a2=0;
      for (j=0; j<18; j++)
      {
        double act=1.0+
          real_fast->unitList[real_fast->firstOutput+j+40].activation;
        a1+=act*cos(j*M_PI/9);
        a2+=act*sin(j*M_PI/9);
      }
      double a=atan2(a2,a1);
      distErr+=sqrt((x-answerX[i])*(x-answerX[i])+
                    (y-answerY[i])*(y-answerY[i]));
      angleErr+=acos(cos(a-answerAngle[i]*M_PI/180.0));
    }
    distErr/=useExamples;
    angleErr/=useExamples;
    fprintf(stderr,"Average distange: %g   Average angle: %g\n",
            distErr,angleErr*180.0/M_PI);
    if (distErr<1.0 && angleErr<10.0*M_PI/180.0)
    {
      useExamples+=1000;
      if (useExamples>numExamples) useExamples=numExamples;
      fprintf(stderr,"Permuting examples...\n");
      for (i=0; i<useExamples; i++)
      {
        int dest=random() % useExamples;
        for (int j=0; j<900+20+20+18; j++)
        {
          signed char tmp=dat[i*(900+20+20+18)+j];
          dat[i*(900+20+20+18)+j]=dat[dest*(900+20+20+18)+j];
          dat[dest*(900+20+20+18)+j]=tmp;
        }
        float tmp=answerAngle[i]; answerAngle[i]=answerAngle[dest];
        answerAngle[dest]=tmp;
        tmp=answerX[i]; answerX[i]=answerX[dest]; answerX[dest]=tmp;
        tmp=answerY[i]; answerY[i]=answerY[dest]; answerY[dest]=tmp;
      }
    }
    fprintf(stderr,"Using %d examples...\n",useExamples);
    Sbrk("Memory usage");
  }

  free(dat);
  delete[] answerAngle;
  delete[] answerX;
  delete[] answerY;
  return TCL_OK;
}
#endif

int TrainFacePositionCmd(ClientData data, Tcl_Interp *interp,
                         int argc, char *argv[])
{
  if (argc!=5)
  {
    interp->result="Usage: TrainFacePosition <examples> <answers> <epochsperbatch> <batches>";
    return TCL_ERROR;
  }
  char *exampleFile=argv[1];
  char *answerFile=argv[2];
  int epochsPerBatch=atoi(argv[3]);
  int batches=atoi(argv[4]);

  int exampleSize=900+20+20+18;
  int numExamples=FileSize(exampleFile)/exampleSize;
  //  numExamples=4000;
  signed char *dat=(signed char *)malloc(numExamples*exampleSize);
  fprintf(stderr,"Loading %d examples...\n",numExamples);
  FILE *inf=fopen(exampleFile,"r");
  fread((char*)dat,numExamples,exampleSize,inf);
  fclose(inf);

  FastMakeNetTrainable(real_fast_list[0],100);

  fprintf(stderr,"Loading answers...\n");
  float *answerAngle=new float[numExamples];
  float *answerX=new float[numExamples];
  float *answerY=new float[numExamples];
  inf=fopen(answerFile,"r");
  for (int i=0; i<numExamples; i++)
    fscanf(inf,"%g%g%g",&answerAngle[i],&answerX[i],&answerY[i]);
  fclose(inf);

  fprintf(stderr,"Permuting examples...\n");
  for (i=0; i<numExamples; i++)
  {
    int dest=random() % numExamples;
    for (int j=0; j<900+20+20+18; j++)
    {
      signed char tmp=dat[i*(900+20+20+18)+j];
      dat[i*(900+20+20+18)+j]=dat[dest*(900+20+20+18)+j];
      dat[dest*(900+20+20+18)+j]=tmp;
    }
    float tmp=answerAngle[i]; answerAngle[i]=answerAngle[dest];
    answerAngle[dest]=tmp;
    tmp=answerX[i]; answerX[i]=answerX[dest]; answerX[dest]=tmp;
    tmp=answerY[i]; answerY[i]=answerY[dest]; answerY[dest]=tmp;
  }

//  int useExamples=1000;
  int useExamples=numExamples;
  for (int batch=-1; batch<batches; batch++)
  {
    if (batch>-1)
    {
/*      real_net->checkpoint_interval=epochsPerBatch*(useExamples+99)/100; */
      fprintf(stderr,"Batch %d...\n",batch);
      for (int epoch=0; epoch<epochsPerBatch; epoch++)
      {
        fprintf(stderr,"Epoch %d...\n",epoch);
        for (int pat=0; pat<useExamples; pat+=100)
        {
          int max=pat+99;
          if (max>=useExamples) max=useExamples-1;
          fprintf(stderr,".");
          //          fprintf(stderr,"Patterns %d-%d...\n",pat,max);
          for (int i=pat; i<=max; i++)
          {
            FastForwardUnit *unit=real_fast_list[0]->unitList+1;
            for (int j=i*exampleSize; j<i*exampleSize+900; j++, unit++)
              unit->train->activations[i-pat]=dat[j]/127.0;
            unit=real_fast_list[0]->unitList+real_fast_list[0]->firstOutput;
            for (j=i*exampleSize+900; j<(i+1)*exampleSize-18; j++, unit++)
              unit->train->delta[i-pat]=dat[j]/127.0;
          }
        }
        FastTrainBatch(real_fast_list[0],0.01,0.01,0.0);
        fprintf(stderr,"\n");
      }
    }
    fprintf(stderr,"Testing network...\n");
    signed char *ptr=dat;
    double distErr=0;
    double angleErr=0;
    for (i=0; i<useExamples; i++)
    {
      for (int j=1; j<=900; j++)
        real_fast_list[0]->unitList[j].activation=(*(ptr++))/127.0;
      ptr+=20+20+18;
      FastForwardPass(real_fast_list[0]);
      double x=0, tot=0;
      for (j=0; j<20; j++)
      {
        double act=1.0+
          real_fast_list[0]->unitList[real_fast_list[0]->firstOutput+j].
          activation;
        tot+=act;
        x+=act*(j+5);
      }
      x/=tot;
      double y=0;
      tot=0;
      for (j=0; j<20; j++)
      {
        double act=1.0+
          real_fast_list[0]->unitList[real_fast_list[0]->firstOutput+j+20].
          activation;
        tot+=act;
        y+=act*(j+5);
      }
      y/=tot;
      distErr+=sqrt((x-answerX[i])*(x-answerX[i])+
                    (y-answerY[i])*(y-answerY[i]));
    }
    distErr/=useExamples;
    fprintf(stderr,"Average distange: %g\n",
            distErr);
    if (distErr<1.0)
    {
      useExamples+=1000;
      if (useExamples>numExamples) useExamples=numExamples;
      fprintf(stderr,"Permuting examples...\n");
      for (i=0; i<useExamples; i++)
      {
        int dest=random() % useExamples;
        for (int j=0; j<900+20+20+18; j++)
        {
          signed char tmp=dat[i*(900+20+20+18)+j];
          dat[i*(900+20+20+18)+j]=dat[dest*(900+20+20+18)+j];
          dat[dest*(900+20+20+18)+j]=tmp;
        }
        float tmp=answerAngle[i]; answerAngle[i]=answerAngle[dest];
        answerAngle[dest]=tmp;
        tmp=answerX[i]; answerX[i]=answerX[dest]; answerX[dest]=tmp;
        tmp=answerY[i]; answerY[i]=answerY[dest]; answerY[dest]=tmp;
      }
    }
    fprintf(stderr,"Using %d examples...\n",useExamples);
    Sbrk("Memory usage");
  }

  free(dat);
  delete[] answerAngle;
  delete[] answerX;
  delete[] answerY;
  return TCL_OK;
}

int GetFaceLogPolarCmd(ClientData data, Tcl_Interp *interp,
                       int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: RandFaceRotationAndPosition <dest> <src> <params>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  char *params=argv[3];

  double xe1, ye1, xe2, ye2, xn, yn, xl, yl, xc, yc, xr, yr;
  sscanf(params,"{leye %lf %lf} {reye %lf %lf} {nose %lf %lf} "
         "{lmouth %lf %lf} {cmouth %lf %lf} {rmouth %lf %lf}",
         &xe1, &ye1, &xe2, &ye2, &xn, &yn,
         &xl, &yl, &xc, &yc, &xr, &yr);

  Vec leftEye=Point2D(xe1,ye1);
  Vec rightEye=Point2D(xe2,ye2);
  Vec nose=Point2D(xn,xn);
  Vec leftMouth=Point2D(xl,yl);
  Vec centerMouth=Point2D(xc,yc);
  Vec rightMouth=Point2D(xr,yr);

  Vec eyeCenter=(leftEye+rightEye)*0.5;
  Vec faceCenter=(eyeCenter+centerMouth)*0.5;

  double size=Abs(eyeCenter-centerMouth);
  double newSize=12.0;
  double scale=(newSize/size)*(1.0+(RandomDouble()*0.2-0.1));
  int reduce=(int)floor(1.0/scale);
  if (reduce>1) {
    SubSampleWithAveraging(image,src,reduce);
    src=image;
    scale*=reduce;
    faceCenter/=reduce;
  }

  int xs=32;
  int ys=16;
  int flip=(RandomDouble()>0.5)?1:-1;
  Byte *newdata=(Byte*)malloc(xs*ys);
  double xcc=16;
  double ycc=16;
  int ptr=0;
  for (int j=0; j<ys; j++)
    for (int i=0; i<xs; i++)
    {
      double ang=i*M_PI*2.0/(double)xs;
      double x=flip*j*cos(ang)/scale+faceCenter(0);
      double y=j*sin(ang)/scale+faceCenter(1);
      newdata[ptr++]=(int)floor(0.5+src->interpolateExtend(x,y));
    }
  image->NewBuffer(newdata,xs,ys);
  return TCL_OK;
}

int UndoLogPolarCmd(ClientData data, Tcl_Interp *interp,
                    int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="UndoLogPolar <dest> <src>";
    return TCL_ERROR;
  }
  ByteImage *dest=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  int size=src->height*2;
  unsigned char *newdata=(unsigned char *)malloc(size*size);
  int ptr=0;
  for (int y=0; y<size; y++)
    for (int x=0; x<size; x++)
    {
      double ang=atan2(y-0.5*size+0.5,x-0.5*size+0.5);
      if (ang<0) ang+=2.0*M_PI;
      ang=fmod(ang*src->width/(2*M_PI),src->width);
      double rad=sqrt((x-0.5*size+0.5)*(x-0.5*size+0.5)+
                      (y-0.5*size+0.5)*(y-0.5*size+0.5));
      newdata[ptr++]=(int)floor(0.5+src->interpolateLogPolar(ang,rad));
    }
  dest->NewBuffer(newdata,size,size);
  return TCL_OK;
}
#endif

void UnpackAndFFTFast(int width, int height, signed char *bytes, 
                      FastForwardStruct *net)
{
  Complex *data=new Complex[width*height];
  Complex *row=new Complex[width];
  Complex *col=new Complex[height];
  for (int j=0; j<height; j++) {
    for (int i=0; i<width; i++) row[i]=Complex((*(bytes++))/127.0,0.0);
    ComputeFFT(row,width,1);
    for (i=0; i<width; i++) data[i+j*width]=row[i];
  }
  for (int i=0; i<width; i++) {
    for (int j=0; j<height; j++) col[j]=data[i+j*width];
    ComputeFFT(col,height,1);
    for (j=0; j<height; j++) net->unitList[1+i+j*width].activation=Abs(col[j]);
  }
  delete[] row;
  delete[] col;
  delete[] data;
}

void ImageToFFTFast(int width, int height, int *img, int *map, double scale,
                    FastForwardStruct *net)
{
  Complex *data=new Complex[width*height];
  Complex *row=new Complex[width];
  Complex *col=new Complex[height];
  for (int j=0; j<height; j++) {
    for (int i=0; i<width; i++) row[i]=Complex(map[*(img++)]*scale-1.0,0.0);
    ComputeFFT(row,width,1);
    for (i=0; i<width; i++) data[i+j*width]=row[i];
  }
  for (int i=0; i<width; i++) {
    for (int j=0; j<height; j++) col[j]=data[i+j*width];
    ComputeFFT(col,height,1);
    for (j=0; j<height; j++) net->unitList[1+i+j*width].activation=Abs(col[j]);
  }
  delete[] row;
  delete[] col;
  delete[] data;
}

#ifndef NoHEBP
void UnpackAndFFTHEBP(int width, int height, signed char *bytes, 
                      unit_type *unit, int pattern)
{
  Complex *data=new Complex[width*height];
  Complex *row=new Complex[width];
  Complex *col=new Complex[height];
  for (int j=0; j<height; j++) {
    for (int i=0; i<width; i++) row[i]=Complex((*(bytes++))/127.0,0.0);
    ComputeFFT(row,width,1);
    for (i=0; i<width; i++) data[i+j*width]=row[i];
  }
  for (int i=0; i<width; i++) {
    for (int j=0; j<height; j++) col[j]=data[i+j*width];
    ComputeFFT(col,height,1);
    for (j=0; j<height; j++) (unit++)->output[pattern]=Abs(col[j]);
  }
  delete[] row;
  delete[] col;
  delete[] data;
}
#endif

#ifndef NoTcl
#ifndef NoHEBP
int TrainNN2FFTCmd(ClientData data, Tcl_Interp *interp,
                int argc, char *argv[])
{
  if (argc!=5)
  {
    interp->result="Usage: TrainNN2FFT <positive-file> <negative-file> <epochs> <mask>";
    return TCL_ERROR;
  }

  int numFaces, numNonFaces;
  char *faceFile=argv[1];
  char *nonFaceFile=argv[2];
  int epochs=atoi(argv[3]);
  ByteImage *mask=FindByteImageWithName(argv[4]);
  
  numFaces=FileSize(faceFile)/mask->size;
  numNonFaces=FileSize(nonFaceFile)/mask->size;
  signed char *dat=(signed char *)malloc(mask->size*(numFaces+numNonFaces));
  fprintf(stderr,"Loading %d faces...\n",numFaces);
  FILE *inf=fopen(faceFile,"r");
  fread((char*)dat,sizeof(signed char)*mask->size,numFaces,inf);
  fclose(inf);
  fprintf(stderr,"Loading %d non-faces...\n",numNonFaces);
  inf=fopen(nonFaceFile,"r");
  fread((char*)(dat+mask->size*numFaces),sizeof(signed char)*mask->size,
        numNonFaces,inf);
  fclose(inf);

{
  signed char *ptr=dat;
  int pos=0, neg=0;
  for (int i=0; i<numFaces; i++)
  {
    UnpackAndFFTFast(mask->width,mask->height,ptr,real_fast);
    ptr+=mask->size;
    if (FastForwardPass(real_fast)>0.0) pos++; else neg++;
  }
  fprintf(stderr,"Before: Positive: %d   Negative: %d\n",pos,neg);
  pos=0; neg=0;
  for (i=0; i<numNonFaces; i++)
  {
    UnpackAndFFTFast(mask->width,mask->height,ptr,real_fast);
    ptr+=mask->size;
    if (FastForwardPass(real_fast)>0.0) pos++; else neg++;
  }
  fprintf(stderr,"Before: Positive: %d   Negative: %d\n",pos,neg); 
}
  
  for (int i=0; i<numFaces; i++)
  {
    int dest=random() % numFaces;
    for (int j=0; j<mask->size; j++)
    {
      signed char tmp=dat[i*mask->size+j];
      dat[i*mask->size+j]=dat[dest*mask->size+j];
      dat[dest*mask->size+j]=tmp;
    }
  }

  for (i=numFaces; i<numFaces+numNonFaces; i++)
  {
    int dest=(random() % numNonFaces)+numFaces;
    for (int j=0; j<mask->size; j++)
    {
      signed char tmp=dat[i*mask->size+j];
      dat[i*mask->size+j]=dat[dest*mask->size+j];
      dat[dest*mask->size+j]=tmp;
    }
  }

  int face=0;
  int nonface=0;
  int numIters=epochs*(numFaces+numFaces)/100;
  if (numFaces<numNonFaces)
    numIters=epochs*(numNonFaces+numNonFaces)/100;
  for (int iter=0; iter<numIters; iter++)
  {
//    fprintf(stderr,"Iteration %d out of %d...\n",iter,numIters);
    for (i=0; i<100; i++)
    {
      int pat;
      if ((random() % 2) == 0)
      {
        pat=face;
        face=(face+1) % numFaces;
      } else
      {
        pat=nonface+numFaces;
        nonface=(nonface+1) % numNonFaces;
      }
      unit_type *unit=&(real_net[0].unit[real_net[0].input_index[1]]);
      UnpackAndFFTHEBP(mask->width,mask->height,dat+pat*mask->size,unit,i);
      real_net[0].unit[real_net[0].output_index[0]].target[i]=
        (pat>=numFaces)?-1:1;
    }
    HEBP_do_epoch(real_net,0,100,0 /* !checkpoint */ ,0 /* !verbose */ );
    fflush(stdout);
  }

  if (real_fast!=NULL) delete real_fast;
  real_fast=FastConstructForward(real_net);
  signed char *ptr=dat;
  int pos=0, neg=0;
  for (i=0; i<numFaces; i++)
  {
    UnpackAndFFTFast(mask->width,mask->height,ptr,real_fast);
    ptr+=mask->size;
    if (FastForwardPass(real_fast)>0.0) pos++; else neg++;
  }
  fprintf(stderr,"Positive: %d   Negative: %d\n",pos,neg);
  pos=0; neg=0;
  for (i=0; i<numNonFaces; i++)
  {
    UnpackAndFFTFast(mask->width,mask->height,ptr,real_fast);
    ptr+=mask->size;
    if (FastForwardPass(real_fast)>0.0) pos++; else neg++;
  }
  fprintf(stderr,"Positive: %d   Negative: %d\n",pos,neg);

  free(dat);
  return TCL_OK;
}
#endif

int RandFaceCenterFeatureCmd(ClientData data, Tcl_Interp *interp,
                             int argc, char *argv[])
{
  if (argc!=10)
  {
    interp->result="Usage: RandFaceCenterFeature <dest> <src> <params> <feature> <xs> <ys> <xfc> <xfc> <scale>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  char *params=argv[3];
  int feature=atoi(argv[4]);
  int xs=atoi(argv[5]);
  int ys=atoi(argv[6]);
  int xfc=atoi(argv[7]);
  int yfc=atoi(argv[8]);
  double extraScale=atof(argv[9]);

  double xe1, ye1, xe2, ye2, xn, yn, xl, yl, xc, yc, xr, yr;
  sscanf(params,"{leye %lf %lf} {reye %lf %lf} {nose %lf %lf} "
         "{lmouth %lf %lf} {cmouth %lf %lf} {rmouth %lf %lf}",
         &xe1, &ye1, &xe2, &ye2, &xn, &yn,
         &xl, &yl, &xc, &yc, &xr, &yr);

  Vec leftEye=Point2D(xe1,ye1);
  Vec rightEye=Point2D(xe2,ye2);
  Vec nose=Point2D(xn,xn);
  Vec leftMouth=Point2D(xl,yl);
  Vec centerMouth=Point2D(xc,yc);
  Vec rightMouth=Point2D(xr,yr);

  Vec eyeCenter=(leftEye+rightEye)*0.5;
  Vec faceCenter=(eyeCenter+centerMouth)*0.5;

  Vec featureCenter;
  switch (feature) {
    case -1:
      featureCenter=leftEye;
      break;
    case 1:
      featureCenter=rightEye;
      break;
    default:
    case 0:
      featureCenter=eyeCenter;
      break;
  }

  double size=Abs(eyeCenter-centerMouth);
  double newSize=12.0*extraScale;
  //  double newSize=12.0+2.0*(RandomDouble()-0.5);
  double scale=(newSize/size);
  //  double scale=(newSize/size)*(1.0+(RandomDouble()*0.2-0.1));
  //  double angle=atan2(ye2-ye1,xe2-xe1);
  double angle=0;
  int reduce=(int)floor(1.0/scale);
  if (reduce>1) {
    SubSampleWithAveraging(image,src,reduce);
    src=image;
    scale*=reduce;
    faceCenter/=reduce;
    featureCenter/=reduce;
  }

  //  int flip=(RandomDouble()>0.5)?1:-1;
  int flip=1;
  Byte *newdata=(Byte*)malloc(xs*ys);
  //  double xcc=RandomDouble()*10.0+10.0;
  //  double ycc=RandomDouble()*10.0+10.0;
  double ang=(RandomDouble()*20.0-10.0)*M_PI/180.0;
  double xcc=xfc;
  double ycc=yfc;
  //  double ang=RandomDouble()*2.0*M_PI;
  angle-=ang;
  int ptr=0;
  for (int j=0; j<ys; j++)
    for (int i=0; i<xs; i++)
    {
      double x=flip*(i-xcc);
      double y=(j-ycc);
      double rx=(cos(angle)*x-sin(angle)*y)/scale+featureCenter(0);
      double ry=(sin(angle)*x+cos(angle)*y)/scale+featureCenter(1);
      newdata[ptr++]=(int)floor(0.5+src->interpolateExtend(rx,ry));
    }
  image->NewBuffer(newdata,xs,ys);
  angle=flip*ang;
  angle*=180.0/M_PI;
  if (angle>360.0) angle-=360.0;
  if (angle<0.0) angle+=360.0;
  sprintf(interp->result,"%g %g %g",angle,xcc,ycc);
  return TCL_OK;
}

int RandFaceFixEyesCmd(ClientData data, Tcl_Interp *interp,
                       int argc, char *argv[])
{
  if (argc!=5)
  {
    interp->result="Usage: RandFaceFixEyes <dest> <src> <params> <fixscale>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  char *params=argv[3];
  int fixscale=atoi(argv[4]);
  int xs=20;
  int ys=20;

  double xe1, ye1, xe2, ye2, xn, yn, xl, yl, xc, yc, xr, yr;
  sscanf(params,"{leye %lf %lf} {reye %lf %lf} {nose %lf %lf} "
         "{lmouth %lf %lf} {cmouth %lf %lf} {rmouth %lf %lf}",
         &xe1, &ye1, &xe2, &ye2, &xn, &yn,
         &xl, &yl, &xc, &yc, &xr, &yr);

  Vec leftEye=Point2D(xe1,ye1);
  Vec rightEye=Point2D(xe2,ye2);
  Vec nose=Point2D(xn,xn);
  Vec leftMouth=Point2D(xl,yl);
  Vec centerMouth=Point2D(xc,yc);
  Vec rightMouth=Point2D(xr,yr);

  double xoff=3;
  double yoff=3;
  double xrange=2;
  double yrange=2;
  Vec newLeftEye=Point2D(RandomDouble()*xrange+xoff,
                         RandomDouble()*yrange+yoff);
  Vec newRightEye=Point2D(-RandomDouble()*xrange+xs-xoff,
                          RandomDouble()*yrange+yoff);

  if (fixscale) {
    leftEye(0)*=640.0/512.0;
    rightEye(0)*=640.0/512.0;
  }

  int reduce=(int)floor(Abs(leftEye-rightEye)/Abs(newLeftEye-newRightEye));
  if (reduce>1) {
    SubSampleWithAveraging(image,src,reduce);
    src=image;
    leftEye/=reduce;
    rightEye/=reduce;
  }

  Mat mat=Identity(3);
  mat(0,2)=-newLeftEye(0);
  mat(1,2)=-newLeftEye(1);
  Vec eyeVec=Normalize(newRightEye-newLeftEye)/Abs(newRightEye-newLeftEye);
  Mat mat2=Identity(3);
  mat2(0,0)=eyeVec(0);  mat2(0,1)=eyeVec(1);
  mat2(1,0)=-eyeVec(1); mat2(1,1)=eyeVec(0);
  mat=mat2*mat;
  eyeVec=rightEye-leftEye;
  mat2(0,0)=eyeVec(0); mat2(0,1)=-eyeVec(1);
  mat2(1,0)=eyeVec(1); mat2(1,1)=eyeVec(0);
  mat=mat2*mat;
  mat2=Identity(3);
  mat2(0,2)=leftEye(0);
  mat2(1,2)=leftEye(1);
  mat=mat2*mat;

  int flip=(RandomDouble()>0.5);

  Byte *newdata=(Byte*)malloc(xs*ys);
  int ptr=0;
  double factor=1.0;
  if (fixscale) factor=512.0/640.0;
  for (int j=0; j<ys; j++)
    for (int i=0; i<xs; i++)
    {
      Vec pt(3);
      if (flip) pt(0)=xs-1-i; else pt(0)=i;
      pt(1)=j;
      pt(2)=1.0;
      pt=mat*pt;
      
      newdata[ptr++]=(int)floor(0.5+src->interpolateExtend(pt(0)*factor,
                                                           pt(1)));
    }
  image->NewBuffer(newdata,xs,ys);
  return TCL_OK;
}

int RandFaceCenterFeatureFERETCmd(ClientData data, Tcl_Interp *interp,
                                  int argc, char *argv[])
{
  if (argc!=9)
  {
    interp->result="Usage: RandFaceCenterFeatureFERET <dest> <src> <params> <xs> <ys> <xfc> <xfc> <scale>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  char *params=argv[3];
  int xs=atoi(argv[4]);
  int ys=atoi(argv[5]);
  int xfc=atoi(argv[6]);
  int yfc=atoi(argv[7]);
  double extraScale=atof(argv[8]);

  double xnose, ynose, xchin, ychin;
  sscanf(params,"{nosetip %lf %lf} {chin %lf %lf}",
         &xnose, &ynose, &xchin, &ychin);

  Vec nose=Point2D(xnose,ynose);
  Vec chin=Point2D(xchin,ychin);
  //  fprintf(stderr,"nose=(%g,%g) chin=(%g,%g)\n",xnose,ynose,xchin,ychin);
  Vec featureCenter=nose;
  //PrintVector(nose);
  //PrintVector(chin);
  //PrintVector(nose-chin);
  double size=Abs(nose-chin);
  double newSize=10.0*extraScale;
  //  double newSize=12.0+2.0*(RandomDouble()-0.5);
  double scale=newSize/size;
  //  double scale=(newSize/size)*(1.0+(RandomDouble()*0.2-0.1));
  //  double angle=atan2(ye2-ye1,xe2-xe1);
  //  fprintf(stderr,"size=%g newsize=%g scale=%g\n",size,newSize,scale);
  double angle=0;
  int reduce=(int)floor(1.0/scale);
  if (reduce>1) {
    SubSampleWithAveraging(image,src,reduce);
    src=image;
    scale*=reduce;
    nose/=reduce;
    chin/=reduce;
    featureCenter/=reduce;
  }

  //  int flip=(RandomDouble()>0.5)?1:-1;
  int flip=1;
  Byte *newdata=(Byte*)malloc(xs*ys);
  //  double xcc=RandomDouble()*10.0+10.0;
  //  double ycc=RandomDouble()*10.0+10.0;
  double ang=(RandomDouble()*20.0-10.0)*M_PI/180.0;
  double xcc=xfc;
  double ycc=yfc;
  //  double ang=RandomDouble()*2.0*M_PI;
  angle-=ang;
  int ptr=0;
  for (int j=0; j<ys; j++)
    for (int i=0; i<xs; i++)
    {
      double x=flip*(i-xcc);
      double y=(j-ycc);
      double rx=(cos(angle)*x-sin(angle)*y)/scale+featureCenter(0);
      double ry=(sin(angle)*x+cos(angle)*y)/scale+featureCenter(1);
      newdata[ptr++]=(int)floor(0.5+src->interpolateExtend(rx,ry));
    }
  image->NewBuffer(newdata,xs,ys);
  angle=flip*ang;
  angle*=180.0/M_PI;
  if (angle>360.0) angle-=360.0;
  if (angle<0.0) angle+=360.0;
  sprintf(interp->result,"%g %g %g",angle,xcc,ycc);
  return TCL_OK;
}

#ifndef Test
// Train the network on some examples.
// The "NoisyOr" part of the name is for historical reasons; this code
// applies to all NN architectures and node types.
int TrainNN2NoisyOrCmd(ClientData data, Tcl_Interp *interp,
                       int argc, char *argv[])
{
  if (argc!=5)
  {
    interp->result="Usage: TrainNN2NoisyOr <positive-file> <negative-file> <epochs> <mask>";
    return TCL_ERROR;
  }

  int numFaces, numNonFaces;
  char *faceFile=argv[1];
  char *nonFaceFile=argv[2];
  int epochs=atoi(argv[3]);
  ByteImage *mask=FindByteImageWithName(argv[4]);
  
  numFaces=FileSize(faceFile)/mask->size;
  numNonFaces=FileSize(nonFaceFile)/mask->size;
  signed char *dat=(signed char *)malloc(mask->size*(numFaces+numNonFaces));
  fprintf(stderr,"Loading %d faces...\n",numFaces);
  FILE *inf=fopen(faceFile,"r");
  fread((char*)dat,sizeof(signed char)*mask->size,numFaces,inf);
  fclose(inf);
  fprintf(stderr,"Loading %d non-faces...\n",numNonFaces);
  inf=fopen(nonFaceFile,"r");
  fread((char*)(dat+mask->size*numFaces),sizeof(signed char)*mask->size,
        numNonFaces,inf);
  fclose(inf);
  StartFrameTimer();
  {
  signed char *ptr=dat;
  int pos=0, neg=0;
  for (int i=0; i<numFaces; i++)
  {
    for (int j=1; j<mask->size+1; j++)
      real_fast_list[0]->unitList[j].activation=(*(ptr++))/127.0;
    if (FastForwardPass(real_fast_list[0])>0.0) pos++; else neg++;
  }
  fprintf(stderr,"Before: Positive: %d   Negative: %d\n",pos,neg);
  pos=0; neg=0;
  for (i=0; i<numNonFaces; i++)
  {
    for (int j=1; j<mask->size+1; j++)
      real_fast_list[0]->unitList[j].activation=(*(ptr++))/127.0;
    if (FastForwardPass(real_fast_list[0])>0.0) pos++; else neg++;
  }
  fprintf(stderr,"Before: Positive: %d   Negative: %d\n",pos,neg);
  }
  
  for (int i=0; i<numFaces; i++)
  {
    int dest=random() % numFaces;
    for (int j=0; j<mask->size; j++)
    {
      signed char tmp=dat[i*mask->size+j];
      dat[i*mask->size+j]=dat[dest*mask->size+j];
      dat[dest*mask->size+j]=tmp;
    }
  }

  for (i=numFaces; i<numFaces+numNonFaces; i++)
  {
    int dest=(random() % numNonFaces)+numFaces;
    for (int j=0; j<mask->size; j++)
    {
      signed char tmp=dat[i*mask->size+j];
      dat[i*mask->size+j]=dat[dest*mask->size+j];
      dat[dest*mask->size+j]=tmp;
    }
  }
  if (epochs>0) FastMakeNetTrainable(real_fast_list[0],100);
  int face=0;
  int nonface=0;
  int numIters=epochs*(numFaces+numFaces)/100;
  if (numFaces<numNonFaces)
    numIters=epochs*(numNonFaces+numNonFaces)/100;
  for (int iter=0; iter<numIters; iter++)
  {
    for (i=0; i<100; i++)
    {
      int pat;
      if ((random() % 2) == 0)
      {
        pat=face;
        face=(face+1) % numFaces;
      } else
      {
        pat=nonface+numFaces;
        nonface=(nonface+1) % numNonFaces;
      }
      FastForwardUnit *unit=real_fast_list[0]->unitList+
        real_fast_list[0]->firstInput;
      for (int j=0; j<mask->size; j++)
        (unit++)->train->activations[i]=dat[pat*mask->size+j]/127.0;
      real_fast_list[0]->unitList[real_fast_list[0]->firstOutput].train->
        delta[i]=(pat>=numFaces)?-1:1;
    }
    FastTrainBatch(real_fast_list[0],0.01,0.1,0.0);
  }

  signed char *ptr=dat;
  int pos=0, neg=0;
  for (i=0; i<numFaces; i++)
  {
    for (int j=1; j<mask->size+1; j++)
      real_fast_list[0]->unitList[j].activation=(*(ptr++))/127.0;
    if (FastForwardPass(real_fast_list[0])>0.0) pos++; else neg++;
  }
  fprintf(stderr,"Positive: %d   Negative: %d\n",pos,neg);
  pos=0; neg=0;
  for (i=0; i<numNonFaces; i++)
  {
    for (int j=1; j<mask->size+1; j++)
      real_fast_list[0]->unitList[j].activation=(*(ptr++))/127.0;
    if (FastForwardPass(real_fast_list[0])>0.0) pos++; else neg++;
  }
  fprintf(stderr,"Positive: %d   Negative: %d\n",pos,neg);
  StopFrameTimer();
  free(dat);
  return TCL_OK;
}
#endif

// USED
// Train the network on some examples, using a vista image for the 
// positive examples.
// The "NoisyOr" part of the name is for historical reasons; this code
// applies to all NN architectures and node types.
int TrainNN2NoisyOrVista(VImage posFaceImage, char *nonFaceFile,
                         int epochs, char *maskname, int parallel,
                         int batchSize)
{
  int numFaces, numNonFaces;
  ByteImage *mask=FindByteImageWithName(maskname);
  
  numFaces=VImageNBands(posFaceImage);
  numNonFaces=FileSize(nonFaceFile)/mask->size;
  signed char *faceData=(signed char *)VPixelPtr(posFaceImage,0,0,0);
  signed char *nonFaceData=(signed char *)malloc(mask->size*numNonFaces);
  fprintf(stderr,"Loading %d non-faces...\n",numNonFaces);
  FILE *inf=fopen(nonFaceFile,"r");
  assert(inf!=NULL);
  fread((char*)nonFaceData,sizeof(signed char)*mask->size,numNonFaces,inf);
  fclose(inf);

  {
  // Apply NN to positive training images, record number of +- classifications
  signed char *ptr=faceData;
  int pos=0, neg=0;
  for (int i=0; i<numFaces; i++)
  {
    for (int j=1; j<mask->size+1; j++)
      real_fast_list[0]->unitList[j].activation=(*(ptr++))/127.0;
    if (FastForwardPass(real_fast_list[0])>0.0) pos++; else neg++;
  }
  fprintf(stderr,"Before: Positive: %d   Negative: %d\n",pos,neg);

  // Apply NN to negative training images, record number of +- classifications
  ptr=nonFaceData;
  pos=0; neg=0;
  for (i=0; i<numNonFaces; i++)
  {
    for (int j=1; j<mask->size+1; j++)
      real_fast_list[0]->unitList[j].activation=(*(ptr++))/127.0;
    if (FastForwardPass(real_fast_list[0])>0.0) pos++; else neg++;
  }
  fprintf(stderr,"Before: Positive: %d   Negative: %d\n",pos,neg);
  } 

  // Permute the face examples
  for (int i=0; i<numFaces; i++) {
    int dest=random() % numFaces;
    for (int j=0; j<mask->size; j++) {
      signed char tmp=faceData[i*mask->size+j];
      faceData[i*mask->size+j]=faceData[dest*mask->size+j];
      faceData[dest*mask->size+j]=tmp;
    }
  }

  // Permute the non-face examples
  for (i=0; i<numNonFaces; i++) {
    int dest=random() % numNonFaces;
    for (int j=0; j<mask->size; j++) {
      signed char tmp=nonFaceData[i*mask->size+j];
      nonFaceData[i*mask->size+j]=nonFaceData[dest*mask->size+j];
      nonFaceData[dest*mask->size+j]=tmp;
    }
  }

  // Create training data buffers
  if (epochs>0) FastMakeNetTrainable(real_fast_list[0],batchSize);

  // Figure out how many batches are needed so that each example should
  // be processed epochs times.
  int numIters=epochs*(numFaces+numFaces)/batchSize;
  if (numFaces<numNonFaces)
    numIters=epochs*(numNonFaces+numNonFaces)/batchSize;

  int face=0;
  int nonface=0;
  StartFrameTimer();              // Start the clock 

  for (int iter=0; iter<numIters; iter++) {
//    for (i=0; i<batchSize; i++) {
    for (i=0; i<batchSize; i++) {
      signed char *ptr;
      double target;
      // First decide whether to present a positive or negative example
      if ((random() % 2) == 0) {
        ptr=faceData+mask->size*face;
        target=1.0;
        face=(face+1) % numFaces;
      } else {
        ptr=nonFaceData+mask->size*nonface;
        target=-1.0;
        nonface=(nonface+1) % numNonFaces;
      }
      FastForwardUnit *unit=real_fast_list[0]->unitList+
        real_fast_list[0]->firstInput;
      // Write example image into network
      for (int j=0; j<mask->size; j++)
        (unit++)->train->activations[i]=*(ptr++)/127.0;
      // Write target output into "delta" field
      real_fast_list[0]->unitList[real_fast_list[0]->firstOutput].train->
	delta[i]=target;
    }
    //    FastTrainBatch(real_fast_list[0],0.01,0.1,0.0);
    //    FastTrainBatch(real_fast_list[0],0.005,0.05,0.0);
#ifdef sgi
    if (parallel==0) 
#endif
      // Do the training
      FastTrainBatch(real_fast_list[0],0.01,0.1,0.0); 
#ifdef sgi
    else {
//      FastTrainBatchParallelCombine(real_fast_list[0],0.01,0.1,0.0,parallel);
    }
#endif
  }

  StopFrameTimer();          // Stop the clock

  // Apply NN to positive training images, record number of +- classifications
  signed char *ptr=faceData;
  int pos=0, neg=0;
  for (i=0; i<numFaces; i++)
  {
    for (int j=1; j<mask->size+1; j++)
      real_fast_list[0]->unitList[j].activation=(*(ptr++))/127.0;
    if (FastForwardPass(real_fast_list[0])>0.0) pos++; else neg++;
  }
  fprintf(stderr,"Positive: %d   Negative: %d\n",pos,neg);

  // Apply NN to negative training images, record number of +- classifications
  ptr=nonFaceData;
  pos=0; neg=0;
  for (i=0; i<numNonFaces; i++)
  {
    for (int j=1; j<mask->size+1; j++)
      real_fast_list[0]->unitList[j].activation=(*(ptr++))/127.0;
    if (FastForwardPass(real_fast_list[0])>0.0) pos++; else neg++;
  }
  fprintf(stderr,"Positive: %d   Negative: %d\n",pos,neg);
  
  // Clean up
  free((char*)nonFaceData);
  return TCL_OK;
}

int TestNN2NoisyOrVista(VImage posFaceImage, char *nonFaceFile,
                        char *maskname)
{
  int numFaces, numNonFaces;
  ByteImage *mask=FindByteImageWithName(maskname);
  
  numFaces=VImageNBands(posFaceImage);
  numNonFaces=FileSize(nonFaceFile)/mask->size;
  signed char *faceData=(signed char *)VPixelPtr(posFaceImage,0,0,0);
  signed char *nonFaceData=(signed char *)malloc(mask->size*numNonFaces);
  fprintf(stderr,"Loading %d non-faces...\n",numNonFaces);
  FILE *inf=fopen(nonFaceFile,"r");
  assert(inf!=NULL);
  fread((char*)nonFaceData,sizeof(signed char)*mask->size,numNonFaces,inf);
  fclose(inf);

  // Apply NN to positive training images, record number of +- classifications
  signed char *ptr=faceData;
  int pos=0, neg=0;
  for (int i=0; i<numFaces; i++)
  {
    for (int j=1; j<mask->size+1; j++)
      real_fast_list[0]->unitList[j].activation=(*(ptr++))/127.0;
    if (FastForwardPass(real_fast_list[0])>0.0) pos++; else neg++;
  }
  fprintf(stderr,"Before: Positive: %d   Negative: %d\n",pos,neg);

  // Apply NN to negative training images, record number of +- classifications
  ptr=nonFaceData;
  pos=0; neg=0;
  for (i=0; i<numNonFaces; i++)
  {
    for (int j=1; j<mask->size+1; j++)
      real_fast_list[0]->unitList[j].activation=(*(ptr++))/127.0;
    if (FastForwardPass(real_fast_list[0])>0.0) pos++; else neg++;
  }
  fprintf(stderr,"Before: Positive: %d   Negative: %d\n",pos,neg);
  
  // Clean up
  free((char*)nonFaceData);
  return TCL_OK;
}


// USED
// Apply router network to image, returning a vista image with different
// bands for different levels in the pyramid.
VImage ApplyRouter(ByteImage *image, ByteImage *mask)
{
  int i;
  int ptr=0;
  int *tmp=new int[mask->size];

  int halfW=mask->width/2;
  int halfH=mask->height/2;
  int level=0;
  double scale=1.0;
  int levels=0;
  int temW=image->width, temH=image->height;
  while (temW>=mask->width && temH>=mask->height) {
    levels++;
    temW=(int)(temW/1.2);
    temH=(int)(temH/1.2);
  }

  VImage output=VCreateImage(levels,image->width,image->height,VUByteRepn);
  VFillImage(output,VAllBands,0);
  while (image->height>=mask->height && image->width>=mask->width) {
    for (int y=0; y<image->height; y++) {
      for (int x=0; x<image->width; x++) {
	// Extract window from image, and compute histogram
        int ptr=0;
        int hist[256];
        for (i=0; i<256; i++) hist[i]=0;
        for (int j=-halfH; j<halfH; j++) for (int i=-halfW; i<halfW; i++) {
          int ii=i+x;
          int jj=j+y;
          if (ii<0) ii=0;
          if (ii>=image->width) ii=image->width-1;
          if (jj<0) jj=0;
          if (jj>=image->height) jj=image->height-1;
          int data=((int)image->data(ii,jj));
          tmp[ptr++]=data;
          hist[data]++;
        }
	// Compute cummulative histogram
        int map[256];
        int num=0;
        for (i=0; i<256; i++) {
          map[i]=num;
          num+=hist[i];
        }
        int tot=num;
        for (i=255; i>=0; i--) {
          map[i]+=num;
          num-=hist[i];
        }
	// Feed image into network, applying histogram equalization
        double scaleFactor=1.0/tot;
        FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;
        FastForwardPass(real_fast_list[0]);
        int maxIndex=0;
        float max=real_fast_list[0]->unitList[real_fast_list[0]->firstOutput].
          activation;
	// Check the router outputs, recording the maximum one
        for (i=1; i<5; i++) {
          float output=real_fast_list[0]->unitList[
            real_fast_list[0]->firstOutput+i].activation;
          if (output>max) {
            max=output;
            maxIndex=i;
          }
        }
        if (max<=0) VPixel(output,level,y,x,VUByte)=255; else
          VPixel(output,level,y,x,VUByte)=maxIndex;
      }
    }
    ZoomByteImage(image,1.2,image);
    scale*=1.2;
    level++;
  }
  // Clean up
  delete[] tmp;
  return output;
}

VImage DisplayRouterOutput(VImage rgb, VImage router, int level) 
{
  if (rgb==NULL) {
    rgb=VCreateImage(3,VImageNRows(router),VImageNColumns(router),VUByteRepn);
    VSetBandInterp(rgb,VBandInterpNone,1,VBandInterpNone,1,
                   VBandInterpRGB,3,VBandInterpNone,1);
  }
  for (int y=0; y<VImageNRows(router); y++)
    for (int x=0; x<VImageNColumns(router); x++) {
      switch (VPixel(router,level,y,x,VUByte)) {
        case 0: 
          VPixel(rgb,0,y,x,VUByte)=1;
          VPixel(rgb,1,y,x,VUByte)=0;
          VPixel(rgb,2,y,x,VUByte)=0;
          break;
        case 1:
          VPixel(rgb,0,y,x,VUByte)=1;
          VPixel(rgb,1,y,x,VUByte)=1;
          VPixel(rgb,2,y,x,VUByte)=0;
          break;
        case 2:
          VPixel(rgb,0,y,x,VUByte)=0;
          VPixel(rgb,1,y,x,VUByte)=1;
          VPixel(rgb,2,y,x,VUByte)=0;
          break;
        case 3:
          VPixel(rgb,0,y,x,VUByte)=0;
          VPixel(rgb,1,y,x,VUByte)=1;
          VPixel(rgb,2,y,x,VUByte)=1;
          break;
        case 4:
          VPixel(rgb,0,y,x,VUByte)=0;
          VPixel(rgb,1,y,x,VUByte)=0;
          VPixel(rgb,2,y,x,VUByte)=1;
          break;
        default: 
          VPixel(rgb,0,y,x,VUByte)=0;
          VPixel(rgb,1,y,x,VUByte)=0;
          VPixel(rgb,2,y,x,VUByte)=0;
          break;
      }
    }
  return rgb;
}

double NNApplyToImage(int netnum, VImage img, VBand band)
{
  struct FastForwardStruct *net=real_fast_list[netnum];
  int ptr=1;
  for (int y=0; y<VImageNRows(img); y++)
    for (int x=0; x<VImageNColumns(img); x++)
      net->unitList[ptr++].activation=VGetPixel(img,band,y,x)/127.0;
  return FastForwardPass(net);
}

void Faces_Init(Tcl_Interp *interp)
{
  Tcl_CreateCommand(interp,"RandFaceRotationAndPosition",
                    RandFaceRotationAndPositionCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"RandFaceCenterFeature",
                    RandFaceCenterFeatureCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"RandFaceCenterFeatureFERET",
                    RandFaceCenterFeatureFERETCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"GenLinearGaussian",GenLinearGaussianCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"GenCircularGaussian",GenCircularGaussianCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#ifndef NoHEBP
  Tcl_CreateCommand(interp,"TrainFaceRotation",TrainFaceRotationCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#endif
  Tcl_CreateCommand(interp,"TrainFacePosition",TrainFacePositionCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"GetFaceLogPolar",GetFaceLogPolarCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"UndoLogPolar",UndoLogPolarCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#ifndef NoHEBP
  Tcl_CreateCommand(interp,"TrainNN2FFT",TrainNN2FFTCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#endif
  Tcl_CreateCommand(interp,"RandFaceFixEyes",RandFaceFixEyesCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#ifndef Test
  Tcl_CreateCommand(interp,"TrainNN2NoisyOr",TrainNN2NoisyOrCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#endif
}
#endif
