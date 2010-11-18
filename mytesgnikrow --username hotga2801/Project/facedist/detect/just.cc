extern "C" {
#if !defined(hpux) && !defined(WINNT)
#include <math.h>
#endif
#ifndef NoVista
#include <vista/VImage.h>
#endif
#ifndef NoTcl
#include <tcl.h>
#endif
#ifndef NoTcl
#ifndef NoVista
#include <tclvista.h>
#endif
#endif
#include <assert.h>
}
#if defined(hpux) || defined(WINNT)
#include <math.h>
#endif

#include "follow.hpp"
#include "myhebp.hpp"
#ifndef NoVista
#include "imgops.hpp"
#endif
#include "system.hpp"
#include "fast2.hpp"
#include "mat.hpp"
#include "faces.hpp"
#include "search.hpp"
#include "vote.hpp"
#ifndef NoDebug
#include "debug.hpp"
#endif
#include "just.hpp"
#ifndef NoTcl
#include "dist.hpp"
#endif

#ifndef M_PI
#define M_PI (atan(1)*4)
#endif

#ifndef NoTcl
extern Tcl_Interp *theInterp;
#endif

#define UseAngleCache
#define QuickOverlapZap
#define OrientationMerging
#define RotClassMerging

struct RandomTransform {
  double angle, x, y, scale;
  int flip;
  double *features;
};

#ifndef NoVista
// Generate a training example, using the given alignment, and adding
// a small amount of translation, rotation, and scaling for robustness
RandomTransform *RandomizeAlignedTrainingImageRecord(VImage dest,
                                                     Alignment *alignment,
                                                     int face,
                                                     VImage src, 
                                                     VImage background,
                                                     int sx, int sy,
                                                     int fixscale,
                                                     double translationRange,
                                                     double angleRange, 
                                                     double distStep,
                                                     int flip)
{
//  VImage dest=VCreateImage(1,sy,sx,VUByteRepn);

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

  if (flip==3) {
    mat(0,0)*=-1;
    mat(0,1)*=-1;
    mat(0,2)*=-1;
  }

  // Apply a small, random transformation
  double randScale=RandomDouble()*(sqrt(distStep)-1.0/sqrt(distStep))+
    1.0/sqrt(distStep);
  double randAngle=M_PI/180.0*(RandomDouble()*angleRange*2-angleRange);
  double randX=RandomDouble()*translationRange*2-translationRange;
  double randY=RandomDouble()*translationRange*2-translationRange;
  RandomTransform *randomtransform=new RandomTransform;
  randomtransform->angle=randAngle;
  randomtransform->x=randX;
  randomtransform->y=randY;
  randomtransform->scale=randScale;
  Mat rand=Translate2D(0.5*sx,0.5*sy)*
    Scale2D(randScale)*Rotate2D(randAngle)*Translate2D(randX,randY)*
    Translate2D(-0.5*sx,-0.5*sy);
  if ((flip==1 && RandomDouble()>0.5) || flip==2) {
    Mat f=Identity(3);
    f(0,0)=-1;
    rand=rand*Translate2D(0.5*sx,0)*f*Translate2D(-0.5*sx,0);
    randomtransform->flip=1;
  } else {
    randomtransform->flip=0;
  }    
  Mat xform=mat*rand;

  randomtransform->features=new double[alignment->numFeatures*2];
  for (int i=0; i<alignment->numFeatures; i++) {
    Vec pt(3);
    pt(0)=alignment->data[face*alignment->numFeatures*2+i*2];
    pt(1)=alignment->data[face*alignment->numFeatures*2+i*2+1];
    pt(2)=1;
    if (fixscale) pt(0)*=512.0/640.0;
    pt=LUInverse(xform)*pt;
    randomtransform->features[i*2]=pt(0);
    randomtransform->features[i*2+1]=pt(1);
  }

/*
  Vec pt(3);
  pt(0)=0.5*sx; pt(1)=0.5*sy; pt(2)=1;
  pt=rand*pt;
  randomtransform->x=pt(0)-0.5*sx;
  randomtransform->y=pt(1)-0.5*sy;
  fprintf(stderr,"computed: %g %g   input: %g %g\n",
          pt(0)-15,pt(1)-15,randX,randY);
*/

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
        if (x<VImageNColumns(background) &&
            y<VImageNRows(background) &&
            !VPixel(background,0,y,x,VUByte)) 
          VPixel(synth,0,y,x,VUByte)=VPixel(src,0,y,x,VUByte);
  }
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
  return randomtransform;
}
#endif

#ifdef UseAngleCache
static int AngleCacheInitialized=0;
static int *AngleCache=NULL;
static double *LookupCos=NULL;
static double *LookupSin=NULL;

void InitializeAngleCache()
{
  AngleCache=new int[360*20*20*2];
  int *ptr=AngleCache;
  for (int a=0; a<360; a++)
    for (int y=-10; y<10; y++) 
      for (int x=-10; x<10; x++) {
        int xx=(int)floor((x+0.5)*cos(a*M_PI/180.0)-(y+0.5)*sin(a*M_PI/180.0));
        int yy=(int)floor((x+0.5)*sin(a*M_PI/180.0)+(y+0.5)*cos(a*M_PI/180.0));
        *(ptr++)=xx;
        *(ptr++)=yy;
      }
  LookupSin=new double[36];
  LookupCos=new double[36];
  for (int i=0; i<36; i++) {
    LookupCos[i]=cos(i*M_PI/18.0);
    LookupSin[i]=sin(i*M_PI/18.0);
  }
  AngleCacheInitialized=1;
}
#endif

#ifndef NoVista
extern double NNApplyToImage(int netnum, VImage img, VBand band);
extern VImage NormalizeLightingHistogram(VImage src, VImage facemask);
extern VImage NormalizeHistogram(VImage src);

double TestFaceRotationVistaImage(VImage example)
{
  NNApplyToImage(0,example,0);
  FastForwardStruct *network=real_fast_list[0];
  FastForwardUnit *unit=network->unitList+network->firstOutput;
  double a1=0, a2=0;
  for (int j=0; j<36; j++) {
    double act=(unit++)->activation;
    a1+=act*cos(j*M_PI/18);
    a2+=act*sin(j*M_PI/18);
  }
  return -atan2(a2,a1);
}

// Generate a training example, using the given alignment, and adding
// a small amount of translation, rotation, and scaling for robustness
void RandomizeAlignedTrainingImageDerotate(VImage dest,
                                           Alignment *alignment,
                                           int face,
                                           VImage src, 
                                           VImage background,
                                           int sx, int sy,
                                           int fixscale,
                                           double translationRange,
                                           double angleRange, 
                                           double distStep,
                                           int flip)
{
//  VImage dest=VCreateImage(1,sy,sx,VUByteRepn);

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

  if (flip==3) {
    mat(0,0)*=-1;
    mat(0,1)*=-1;
    mat(0,2)*=-1;
  }

  // Apply a small, random transformation
  double randScale=RandomDouble()*(sqrt(distStep)-1.0/sqrt(distStep))+
    1.0/sqrt(distStep);
  double randAngle=M_PI/180.0*(RandomDouble()*angleRange*2-angleRange);
  double randX=RandomDouble()*translationRange*2-translationRange;
  double randY=RandomDouble()*translationRange*2-translationRange;
  Mat rand=Translate2D(0.5*sx,0.5*sy)*
    Scale2D(randScale)*Rotate2D(randAngle)*Translate2D(randX,randY)*
    Translate2D(-0.5*sx,-0.5*sy);
  if ((flip==1 && RandomDouble()>0.5) || flip==2) {
    Mat f=Identity(3);
    f(0,0)=-1;
    rand=rand*Translate2D(0.5*sx,0)*f*Translate2D(-0.5*sx,0);
  }
  Mat xform=mat*rand;

  for (int i=0; i<alignment->numFeatures; i++) {
    Vec pt(3);
    pt(0)=alignment->data[face*alignment->numFeatures*2+i*2];
    pt(1)=alignment->data[face*alignment->numFeatures*2+i*2+1];
    pt(2)=1;
    if (fixscale) pt(0)*=512.0/640.0;
    pt=LUInverse(xform)*pt;
  }

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
        if (x<VImageNColumns(background) &&
            y<VImageNRows(background) &&
            !VPixel(background,0,y,x,VUByte)) 
          VPixel(synth,0,y,x,VUByte)=VPixel(src,0,y,x,VUByte);
  }
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

  VImage norm=NormalizeHistogram(dest);
  double derotateAngle=TestFaceRotationVistaImage(norm);
  VDestroyImage(norm);
  xform=xform*Translate2D(0.5*sx,0.5*sy)*
    Rotate2D(derotateAngle)*Translate2D(-0.5*sx,-0.5*sy);

  fprintf(stderr,"rand=%g, derot=%g\n",
          randAngle*180.0/M_PI,derotateAngle*180.0/M_PI);

  for (y=0; y<sy; y++) for (int x=0; x<sx; x++) {
    Vec pt(3);
    pt(0)=x+0.5; pt(1)=y+0.5; pt(2)=1.0;
    pt=xform*pt;
    VPixel(dest,0,y,x,VUByte)=(int)floor(0.5+
                                         InterpolatePixel(synth,0,
                                                          pt(0)-0.5,
                                                          pt(1)-0.5));
  }

  if (synth!=src) VDestroyImage(synth);
}
#endif

#ifndef NoVista
void ExtractDetectedFaces(char *filename, VImage src, 
			  int sx, int sy, double scale,
			  List<Detection> *detections)
{
  int counter=0;
  for (Detection *d=detections->first; d!=NULL; d=d->next) {
    VImage dest=VCreateImage(1,sy,sx,VUByteRepn);
    
    Mat xform=Scale2D(pow(1.2,d->s))*
      Translate2D(d->x,d->y)*
      Rotate2D(d->orient*M_PI/180.0)*
      Translate2D(-10,-10)*Scale2D(1.0/scale);
    
    // Figure out how much the transformation scales the image, reduce it
    // to an integer scaling, and apply that scaling (averaging blocks of
    // pixels).
    double actualScale=sqrt(fabs(xform(0,0)*xform(1,1)-xform(0,1)*xform(1,0)));
    int reduce=(int)floor(actualScale);
    if (reduce<1) reduce=1;
    VImage synth=src;
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
    char destname[1024];
    sprintf(destname,filename,counter++);
    FILE *outf=fopen(destname,"w");
    fprintf(outf,"P5\n%d %d\n%d\n",sx,sy,255);
    fwrite(dest->data,sx*sy,1,outf);
    fclose(outf);
    VDestroyImage(dest);
  }
}

// Generate a training example, using the given alignment, and adding
// a small amount of translation, rotation, and scaling for robustness
VImage ExtractAlignedTrainingImage(VImage dest,
                                   Alignment *alignment,
                                   int face,
                                   VImage src, 
                                   VImage prescaled,
                                   VImage background,
                                   int sx, int sy,
                                   int fixscale,
                                   double randX, double randY, 
                                   double randAngle, double randScale,
                                   int flip, int method)
{
//  VImage dest=VCreateImage(1,sy,sx,VUByteRepn);

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
//  randAngle=M_PI/180.0*randAngle;
//  RandomTransform *randomtransform=new RandomTransform;
//  randomtransform->angle=randAngle;
//  randomtransform->x=randX;
//  randomtransform->y=randY;
//  randomtransform->scale=randScale;
//  randomtransform->flip=flip;
  Mat rand=Translate2D(0.5*sx,0.5*sy)*
    Scale2D(randScale)*Rotate2D(randAngle)*Translate2D(randX,randY)*
    Translate2D(-0.5*sx,-0.5*sy);
  if (flip) {
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
        if (!VPixel(background,0,y,x,VUByte)) 
          VPixel(synth,0,y,x,VUByte)=VPixel(src,0,y,x,VUByte);
  }
  // Apply the scaling
  if (reduce>1) {
    if (prescaled==NULL) prescaled=VReduceImage(synth,NULL,0,reduce,reduce);
    if (synth!=src) VDestroyImage(synth);
    synth=prescaled;
    xform=Scale2D(1.0/reduce)*xform;
  }
 
  // Go through window pixel-by-pixel, getting interpolated pixel values
  // from the original image
  for (int y=0; y<sy; y++) for (int x=0; x<sx; x++) {
    Vec pt(3);
    switch (method) {
      case 0:
        pt(0)=x+0.5; pt(1)=y+0.5; pt(2)=1.0;
        pt=xform*pt;
        VPixel(dest,0,y,x,VUByte)=(int)floor(0.5+
                                             InterpolatePixel(synth,0,
                                                              pt(0)-0.5,
                                                              pt(1)-0.5));
        break;
      case 1:
        pt(0)=x; pt(1)=y; pt(2)=1.0;
        pt=xform*pt;
        VPixel(dest,0,y,x,VUByte)=(int)floor(0.5+
                                             InterpolatePixel(synth,0,
                                                              pt(0)-0.5,
                                                              pt(1)-0.5));
        break;
      case 2:
        pt(0)=x; pt(1)=y; pt(2)=1.0;
        pt=xform*pt;
        VPixel(dest,0,y,x,VUByte)=(int)floor(0.5+
                                             InterpolatePixel(synth,0,
                                                              pt(0),
                                                              pt(1)));
        break;
      case 3:
        pt(0)=x+0.5; pt(1)=y+0.5; pt(2)=1.0;
        pt=xform*pt;
        VPixel(dest,0,y,x,VUByte)=(int)floor(0.5+
                                             InterpolatePixel(synth,0,
                                                              pt(0),
                                                              pt(1)));
        break;
    }
  }
  if (synth!=src && synth!=prescaled) VDestroyImage(synth);
  return prescaled;
}
#endif

void FreeRandomTransform(RandomTransform *rt)
{
  delete[] rt->features;
  delete rt;
}

#ifndef NoVista
double InterpretCircularGaussianVista(VImage image, VBand band)
{
  double a1=0, a2=0;
  for (int j=0; j<36; j++) {
    double act=VPixel(image,band,0,j,VSByte);
    a1+=act*cos(j*M_PI/18);
    a2+=act*sin(j*M_PI/18);
  }
  return atan2(a2,a1);
}
 
double GenCircularGaussianVista(VImage image, VBand band, double angle)
{
  double a1=0, a2=0;
  for (int i=0; i<VImageNColumns(image); i++) {
    int val=(int)floor(0.5+127.0*(cos(angle-
                                      (double)i*M_PI*2.0/
                                      (double)VImageNColumns(image))));
    if (val<-127) val=-127;
    if (val>127) val=127;
    VPixel(image,band,0,i,VSByte)=val;
    a1+=val*cos(i*M_PI/18)/127.0;
    a2+=val*sin(i*M_PI/18)/127.0;
  }
  double newangle=atan2(a2,a1);
  return acos(cos(angle-newangle));
}

double InterpretLinearGaussianVista(VImage image, VBand band, int row)
{
  double avg=0, tot=0;
  for (int j=0; j<VImageNColumns(image); j++) {
    double act=VPixel(image,band,row,j,VSByte);
    act+=127;
    avg+=j*act;
    tot+=act;
  }
  return avg/tot;
}
 
double GenLinearGaussianVista(VImage image, VBand band, int row,
                              double position, double width)
{
  double avg=0, tot=0;
  for (int i=0; i<VImageNColumns(image); i++)
  {
    int val=(int)(0.5+exp(-(position-i)*(position-i)/(width*width))*254);
    if (val<0) val=0;
    if (val>254) val=254;
    VPixel(image,band,row,i,VSByte)=val-127;
    avg+=i*val;
    tot+=val;
  }
  return avg/tot;
}

void TestFaceRotationVista(VImage examples, VImage answers, 
                           int holdout)
{
  FastForwardStruct *network=real_fast_list[0];
  if (holdout==0) holdout=VImageNBands(examples);
  double angleErr=0;
  double ssdErr=0;
  for (int i=VImageNBands(examples)-holdout; i<VImageNBands(examples); i++) {
    FastForwardUnit *unit=network->unitList+network->firstInput;
    VSByte *ptr=(VSByte*)VPixelPtr(examples,i,0,0);
    for (int j=0; j<VImageNRows(examples)*VImageNColumns(examples); j++) 
      (unit++)->activation=*(ptr++)/127.0;
    FastForwardPass(network);
    unit=network->unitList+network->firstOutput;
    double a1=0, a2=0;
    double da1=0, da2=0;
    double ssd=0.0;
    for (j=0; j<36; j++) {
      double act=(unit++)->activation;
      a1+=act*cos(j*M_PI/18);
      a2+=act*sin(j*M_PI/18);
      double dact=VPixel(answers,i,0,j,VSByte)/127.0;
      da1+=dact*cos(j*M_PI/18);
      da2+=dact*sin(j*M_PI/18);
      ssd+=(dact-act)*(dact-act);
    }
    ssdErr+=ssd;
    double a=atan2(a2,a1);
    double da=atan2(da2,da1);
    angleErr+=acos(cos(a-da));
  }
  angleErr/=holdout;
  ssdErr/=holdout;
  fprintf(stderr,"Average angle: %g     SSD: %g\n",angleErr*180.0/M_PI,ssdErr);
}

void TestFacePositionVista(VImage examples, VImage answers, 
                           int holdout)
{
  FastForwardStruct *network=real_fast_list[0];
  if (holdout==0) holdout=VImageNBands(examples);
  double *error=new double[VImageNRows(answers)];
  for (int i=0; i<VImageNRows(answers); i++) error[i]=0;
  for (i=VImageNBands(examples)-holdout; i<VImageNBands(examples); i++) {
    FastForwardUnit *unit=network->unitList+network->firstInput;
    VSByte *ptr=(VSByte*)VPixelPtr(examples,i,0,0);
    for (int j=0; j<VImageNRows(examples)*VImageNColumns(examples); j++) 
      (unit++)->activation=*(ptr++)/127.0;
    FastForwardPass(network);
    unit=network->unitList+network->firstOutput;
    for (j=0; j<VImageNRows(answers); j++) {
      double dsum=0, sum=0, dtot=0, tot=0;
      for (int k=0; k<VImageNColumns(answers); k++) {
        double dval=VGetPixel(answers,i,j,k), val=(unit++)->activation;
        dsum+=(dval+127)*k;
        dtot+=(dval+127);
        sum+=(val+1)*k;
        tot+=(val+1);
      }
      double davg=dsum/dtot;
      double avg=sum/tot;
      error[j]+=fabs(davg-avg);
    }
  }
  fprintf(stderr,"Average errors:");
  for (i=0; i<VImageNRows(answers); i++) 
    fprintf(stderr," %g",error[i]/holdout);
  fprintf(stderr,"\n");
  delete[] error;
}

void TestFacePositionVistaHistogram(VImage examples, VImage answers)
{
  FastForwardStruct *network=real_fast_list[0];
  int error[21][21];
  for (int i=0; i<21; i++) for (int j=0; j<21; j++) 
    error[i][j]=0;
  for (i=0; i<VImageNBands(examples); i++) {
    FastForwardUnit *unit=network->unitList+network->firstInput;
    VSByte *ptr=(VSByte*)VPixelPtr(examples,i,0,0);
    for (int j=0; j<VImageNRows(examples)*VImageNColumns(examples); j++) 
      (unit++)->activation=*(ptr++)/127.0;
    FastForwardPass(network);
    unit=network->unitList+network->firstOutput;

    double dsum=0, sum=0, dtot=0, tot=0;
    for (int k=0; k<VImageNColumns(answers); k++) {
      double dval=VGetPixel(answers,i,0,k), val=(unit++)->activation;
      dsum+=(dval+127)*k;
      dtot+=(dval+127);
      sum+=(val+1)*k;
      tot+=(val+1);
    }
    double dx=dsum/dtot;
    double x=sum/tot;
    
    dsum=0; sum=0; dtot=0; tot=0;
    for (k=0; k<VImageNColumns(answers); k++) {
      double dval=VGetPixel(answers,i,1,k), val=(unit++)->activation;
      dsum+=(dval+127)*k;
      dtot+=(dval+127);
      sum+=(val+1)*k;
      tot+=(val+1);
    }
    double dy=dsum/dtot;
    double y=sum/tot;
      
    int ex=10+(int)floor(dx-x+0.5);
    int ey=10+(int)floor(dy-y+0.5);
    error[ex][ey]++;
  }
  for (int j=0; j<21; j++) {
    for (i=0; i<21; i++) fprintf(stderr," %d",error[i][j]);
    fprintf(stderr,"\n");
  }
}

void TestFaceAngleVistaHistogram(VImage examples, VImage answers)
{
  FastForwardStruct *network=real_fast_list[0];
  int error[36];
  for (int i=0; i<36; i++) error[i]=0;
  for (i=0; i<VImageNBands(examples); i++) {
    FastForwardUnit *unit=network->unitList+network->firstInput;
    VSByte *ptr=(VSByte*)VPixelPtr(examples,i,0,0);
    for (int j=0; j<VImageNRows(examples)*VImageNColumns(examples); j++) 
      (unit++)->activation=*(ptr++)/127.0;
    FastForwardPass(network);
    unit=network->unitList+network->firstOutput+40;

    double a1=0, a2=0;
    double da1=0, da2=0;
    double ssd=0.0;
    for (j=0; j<36; j++) {
      double act=(unit++)->activation;
      a1+=act*cos(j*M_PI/18);
      a2+=act*sin(j*M_PI/18);
      double dact=VPixel(answers,i,0,j,VSByte)/127.0;
      da1+=dact*cos(j*M_PI/18);
      da2+=dact*sin(j*M_PI/18);
      ssd+=(dact-act)*(dact-act);
    }
    double a=atan2(a2,a1);
    double da=atan2(da2,da1);
    int err=(int)floor(((da-a)*180.0/M_PI+5)/10);
    while (err<0) err+=36;
    while (err>=36) err-=36;
    error[err]++;
  }
  for (i=0; i<=36; i++) fprintf(stderr,"%d\n",error[(i+18)%36]);
}

void TestFacePositionSensitivityHistogram(VImage examples, VImage answers,
                                          VImage facemask)
{
  FastForwardStruct *network=real_fast_list[0];
  int error[21][21], total[21][21];
  for (int i=0; i<21; i++) for (int j=0; j<21; j++) 
    error[i][j]=total[i][j]=0;
  for (i=0; i<VImageNBands(examples); i++) {
    VImage window=VCropImage(examples,NULL,i,5,5,20,20);
    VImage norm=NormalizeLightingHistogram(window,facemask);

    FastForwardUnit *unit=network->unitList+network->firstInput;
    VSByte *ptr=(VSByte*)VPixelPtr(norm,0,0,0);
    for (int j=0; j<VImageNRows(norm)*VImageNColumns(norm); j++) 
      (unit++)->activation=*(ptr++)/127.0;
    VDestroyImage(window);
    VDestroyImage(norm);

    double dsum=0, dtot=0;
    for (int k=0; k<VImageNColumns(answers); k++) {
      double dval=VGetPixel(answers,i,0,k), val=(unit++)->activation;
      dsum+=(dval+127)*k;
      dtot+=(dval+127);
    }
    double dx=dsum/dtot;
    
    dsum=0; dtot=0;
    for (k=0; k<VImageNColumns(answers); k++) {
      double dval=VGetPixel(answers,i,1,k), val=(unit++)->activation;
      dsum+=(dval+127)*k;
      dtot+=(dval+127);
    }
    double dy=dsum/dtot;
      
    int ex=10+(int)floor(dx-10+0.5);
    int ey=10+(int)floor(dy-10+0.5);

    if (FastForwardPass(network)>0) 
      error[ex][ey]++;
    total[ex][ey]++;
  }
  for (int j=0; j<21; j++) {
    for (i=0; i<21; i++) {
      if (total[i][j]==0) total[i][j]=1;
      fprintf(stderr," %g",(double)error[i][j]/(double)total[i][j]);
    }
    fprintf(stderr,"\n");
  }
}

void TrainNetworkVista(VImage inputs, VImage outputs,
                       int network,
                       int numEpochs, int batchSize,
                       double rate, double momentum, double decay,
                       int holdout)
{
  int incomp=VImageNComponents(inputs);
  int outcomp=VImageNComponents(outputs);
  int inputsize=incomp*VImageNRows(inputs)*VImageNColumns(inputs);
  int outputsize=outcomp*VImageNRows(outputs)*VImageNColumns(outputs);
  int pattern=0;
  int numPatterns=VImageNFrames(inputs);
  FastMakeNetTrainable(real_fast_list[network],batchSize);
  int batches=((numPatterns-holdout+batchSize-1)/batchSize)*numEpochs;
  if (batches<0) batches=1;
  for (int batch=0; batch<batches; batch++) {
    for (int i=0; i<batchSize; i++) {
      FastForwardUnit *unit=real_fast_list[network]->unitList+
        real_fast_list[network]->firstInput;
      VSByte *ptr=(VSByte*)VPixelPtr(inputs,incomp*pattern,0,0);
      for (int j=0; j<inputsize; j++)
        (unit++)->train->activations[i]=*(ptr++)/127.0;
      unit=real_fast_list[network]->unitList+
        real_fast_list[network]->firstOutput;
      ptr=(VSByte*)VPixelPtr(outputs,outcomp*pattern,0,0);
      for (j=0; j<outputsize; j++)
        (unit++)->train->delta[i]=*(ptr++)/127.0;
      pattern=(pattern+1)%(numPatterns-holdout);
    }
    FastTrainBatch(real_fast_list[network],rate,momentum,decay);
  }
}

void PermuteExamplesRotAndPos(VImage inputs, VImage outputs, VImage outputs2)
{
  int incomp=VImageNComponents(inputs);
  int outcomp=VImageNComponents(outputs);
  int outcomp2=VImageNComponents(outputs2);
  VImage tmpin=VCreateImage(1,
                            VImageNRows(inputs),
                            VImageNColumns(inputs),
                            VPixelRepn(inputs));
  VImage tmpout=VCreateImage(1,
                             VImageNRows(outputs),
                             VImageNColumns(outputs),
                             VPixelRepn(outputs));
  VImage tmpout2=VCreateImage(1,
                             VImageNRows(outputs2),
                             VImageNColumns(outputs2),
                             VPixelRepn(outputs2));
  for (int i=0; i<VImageNFrames(inputs); i++) {
    int j=random() % VImageNFrames(inputs);
    for (int c=0; c<incomp; c++) {
      VCopyBand(inputs,i*incomp+c,tmpin,0);
      VCopyBand(inputs,j*incomp+c,inputs,i*incomp+c);
      VCopyBand(tmpin,0,inputs,j*incomp+c);
    }
    for (c=0; c<outcomp; c++) {
      VCopyBand(outputs,i*outcomp+c,tmpout,0);
      VCopyBand(outputs,j*outcomp+c,outputs,i*outcomp+c);
      VCopyBand(tmpout,0,outputs,j*outcomp+c);
    }
    for (c=0; c<outcomp2; c++) {
      VCopyBand(outputs2,i*outcomp2+c,tmpout2,0);
      VCopyBand(outputs2,j*outcomp2+c,outputs2,i*outcomp2+c);
      VCopyBand(tmpout2,0,outputs2,j*outcomp2+c);
    }
  }
  VDestroyImage(tmpin);
  VDestroyImage(tmpout);
  VDestroyImage(tmpout2);
}

void TrainNetworkRotAndPosVista(VImage inputs, VImage outputs, VImage outputs2,
                                int network,
                                int numEpochs, int batchSize,
                                double rate, double momentum, double decay,
                                int holdout)
{
  int incomp=VImageNComponents(inputs);
  int outcomp=VImageNComponents(outputs);
  int outcomp2=VImageNComponents(outputs2);
  int inputsize=incomp*VImageNRows(inputs)*VImageNColumns(inputs);
  int outputsize=outcomp*VImageNRows(outputs)*VImageNColumns(outputs);
  int outputsize2=outcomp2*VImageNRows(outputs2)*VImageNColumns(outputs2);
  int pattern=0;
  int numPatterns=VImageNFrames(inputs);
  FastMakeNetTrainable(real_fast_list[network],batchSize);
  int batches=((numPatterns-holdout+batchSize-1)/batchSize)*numEpochs;
  if (batches<0) batches=1;
  for (int batch=0; batch<batches; batch++) {
    for (int i=0; i<batchSize; i++) {
      FastForwardUnit *unit=real_fast_list[network]->unitList+
        real_fast_list[network]->firstInput;
      VSByte *ptr=(VSByte*)VPixelPtr(inputs,incomp*pattern,0,0);
      for (int j=0; j<inputsize; j++)
        (unit++)->train->activations[i]=*(ptr++)/127.0;
      unit=real_fast_list[network]->unitList+
        real_fast_list[network]->firstOutput;
      ptr=(VSByte*)VPixelPtr(outputs,outcomp*pattern,0,0);
      for (j=0; j<outputsize; j++)
        (unit++)->train->delta[i]=*(ptr++)/127.0;
      ptr=(VSByte*)VPixelPtr(outputs2,outcomp2*pattern,0,0);
      for (j=0; j<outputsize2; j++)
        (unit++)->train->delta[i]=*(ptr++)/127.0;
      pattern=(pattern+1)%(numPatterns-holdout);
    }
    FastTrainBatch(real_fast_list[network],rate,momentum,decay);
  }
}

void TestFaceRotAndPosVista(VImage examples, VImage positions, 
                            VImage angles, int holdout)
{
  FastForwardStruct *network=real_fast_list[0];
  if (holdout==0) holdout=VImageNBands(examples);
  double xErr=0;
  double yErr=0;
  double angleErr=0;
  double ssdErr=0;
  for (int i=VImageNBands(examples)-holdout; i<VImageNBands(examples); i++) {
    FastForwardUnit *unit=network->unitList+network->firstInput;
    VSByte *ptr=(VSByte*)VPixelPtr(examples,i,0,0);
    for (int j=0; j<VImageNRows(examples)*VImageNColumns(examples); j++) 
      (unit++)->activation=*(ptr++)/127.0;
    FastForwardPass(network);
    unit=network->unitList+network->firstOutput;
    double ssd=0.0;
    double avg=0, tot=0, davg=0, dtot=0;
    for (j=0; j<20; j++) {
      double act=(unit++)->activation+1.0;
      avg+=act*j;
      tot+=act;
      double dact=((int)(VPixel(positions,i,0,j,VSByte))+127)/127.0;
      davg+=dact*j;
      dtot+=dact;
      ssd+=(dact-act)*(dact-act);
    }
    double x=avg/tot, dx=davg/dtot;
    avg=0;
    tot=0;
    davg=0;
    dtot=0;
    for (j=0; j<20; j++) {
      double act=(unit++)->activation+1.0;
      avg+=act*j;
      tot+=act;
      double dact=((int)(VPixel(positions,i,1,j,VSByte))+127)/127.0;
      davg+=dact*j;
      dtot+=dact;
      ssd+=(dact-act)*(dact-act);
    }
    double y=avg/tot, dy=davg/dtot;
    double a1=0, a2=0;
    double da1=0, da2=0;
    for (j=0; j<36; j++) {
      double act=(unit++)->activation;
      a1+=act*cos(j*M_PI/18);
      a2+=act*sin(j*M_PI/18);
      double dact=VPixel(angles,i,0,j,VSByte)/127.0;
      da1+=dact*cos(j*M_PI/18);
      da2+=dact*sin(j*M_PI/18);
      ssd+=(dact-act)*(dact-act);
    }
    ssdErr+=ssd;
    double a=atan2(a2,a1);
    double da=atan2(da2,da1);
    double adiff=a-da;
    while (adiff<-M_PI) adiff+=2*M_PI;
    while (adiff>M_PI) adiff-=2*M_PI;
    angleErr+=fabs(adiff);
    xErr+=(x-dx)*(x-dx);
    yErr+=(y-dy)*(y-dy);
  }
  angleErr/=holdout;
  xErr=sqrt(xErr/holdout);
  yErr=sqrt(yErr/holdout);
  ssdErr/=holdout;
  fprintf(stderr,"Errors a: %g  x: %g  y: %g  SSD: %g\n",
          angleErr*180.0/M_PI,xErr,yErr,ssdErr);
}

void TestDistFaceRotAndPosVista(VImage examples, VImage positions, 
                                VImage angles, int holdout)
{
  double xhist[73];
  double yhist[73];
  double ahist[73];

  for (int i=0; i<73; i++) 
    xhist[i]=yhist[i]=ahist[i]=0;

  FastForwardStruct *network=real_fast_list[0];
  if (holdout==0) holdout=VImageNBands(examples);
  double xErr=0;
  double yErr=0;
  double angleErr=0;
  double ssdErr=0;
  for (i=VImageNBands(examples)-holdout; i<VImageNBands(examples); i++) {
    FastForwardUnit *unit=network->unitList+network->firstInput;
    VSByte *ptr=(VSByte*)VPixelPtr(examples,i,0,0);
    for (int j=0; j<VImageNRows(examples)*VImageNColumns(examples); j++) 
      (unit++)->activation=*(ptr++)/127.0;
    FastForwardPass(network);
    unit=network->unitList+network->firstOutput;
    double ssd=0.0;
    double avg=0, tot=0, davg=0, dtot=0;
    for (j=0; j<20; j++) {
      double act=(unit++)->activation+1.0;
      avg+=act*j;
      tot+=act;
      double dact=((int)(VPixel(positions,i,0,j,VSByte))+127)/127.0;
      davg+=dact*j;
      dtot+=dact;
      ssd+=(dact-act)*(dact-act);
    }
    double x=avg/tot, dx=davg/dtot;
    avg=0;
    tot=0;
    davg=0;
    dtot=0;
    for (j=0; j<20; j++) {
      double act=(unit++)->activation+1.0;
      avg+=act*j;
      tot+=act;
      double dact=((int)(VPixel(positions,i,1,j,VSByte))+127)/127.0;
      davg+=dact*j;
      dtot+=dact;
      ssd+=(dact-act)*(dact-act);
    }
    double y=avg/tot, dy=davg/dtot;
    double a1=0, a2=0;
    double da1=0, da2=0;
    for (j=0; j<36; j++) {
      double act=(unit++)->activation;
      a1+=act*cos(j*M_PI/18);
      a2+=act*sin(j*M_PI/18);
      double dact=VPixel(angles,i,0,j,VSByte)/127.0;
      da1+=dact*cos(j*M_PI/18);
      da2+=dact*sin(j*M_PI/18);
      ssd+=(dact-act)*(dact-act);
    }
    ssdErr+=ssd;
    double a=atan2(a2,a1);
    double da=atan2(da2,da1);
    double adiff=a-da;
    while (adiff<-M_PI) adiff+=2*M_PI;
    while (adiff>M_PI) adiff-=2*M_PI;
    angleErr+=fabs(adiff);
    xErr+=(x-dx)*(x-dx);
    yErr+=(y-dy)*(y-dy);
    
    xhist[36+(int)floor(0.5+x-dx)]++;
    yhist[36+(int)floor(0.5+y-dy)]++;
    ahist[36+(int)floor(0.5+18.0/M_PI*adiff)]++;
  }
  angleErr/=holdout;
  xErr=sqrt(xErr/holdout);
  yErr=sqrt(yErr/holdout);
  ssdErr/=holdout;
  fprintf(stderr,"Errors a: %g  x: %g  y: %g  SSD: %g\n",
          angleErr*180.0/M_PI,xErr,yErr,ssdErr);
  for (i=0; i<73; i++) {
    fprintf(stderr,"%d %g %g %g\n",i-36,
            xhist[i]/holdout,
            yhist[i]/holdout,
            ahist[i]/holdout);
  }
}
#endif

double *CreateAngularHistogram(int num)
{
  double *hist=new double[num];
  for (int i=0; i<num; i++)
    hist[i]=0;
  return hist;
}

void DestroyAngularHistogram(double *hist)
{
  delete[] hist;
}

void PrintAngularHistogram(double *hist, int num, char *filename)
{
  FILE *outf=fopen(filename,"w");
  for (int i=0; i<num; i++) {
    fprintf(outf,"%d %g\n",i-180,hist[i]);
  }
  fprintf(outf,"%d %g\n",num-180,hist[0]);
  fclose(outf);
}

void NormalizeAngularHistogram(double *hist, int num, double val)
{
  if (val==0) {
    for (int i=0; i<num; i++) 
      if (fabs(hist[i])>val) val=fabs(hist[i]);
    if (val==0) val=1;
  }
  for (int i=0; i<num; i++)
    hist[i]/=val;
}

#ifndef NoVista
void DetectorPositionSensitivity(VImage hist,
                                 Alignment *align,
                                 VImage src, VImage facemask,
                                 int face,
                                 int fixscale,
                                 int method)
{
  VImage dest=VCreateImage(1,20,20,VUByteRepn);
  VImage prescaled=NULL;
  for (int flip=0; flip<=1; flip++) 
    for (int y=0; y<=20; y++) 
      for (int x=0; x<=20; x++) {
        prescaled=ExtractAlignedTrainingImage(dest,align,face,src,prescaled,
                                              NULL,20,20,
                                              fixscale,x*0.5-5,y*0.5-5,0,1,
                                              flip,method);
        VImage norm=NormalizeLightingHistogram(dest,facemask);
        VPixel(hist,0,y,x,VFloat)+=NNApplyToImage(0,norm,0);
        VDestroyImage(norm);
      }
  if (prescaled!=NULL) VDestroyImage(prescaled);
  VDestroyImage(dest);
}

void DetectorAngularSensitivity(double *hist1,
                                Alignment *align,
                                VImage src, VImage facemask,
                                int face,
                                int fixscale, int method)
{
  VImage dest=VCreateImage(1,20,20,VUByteRepn);
  VImage prescaled=NULL;
  for (int i=-180; i<180; i++) {
    prescaled=ExtractAlignedTrainingImage(dest,align,face,src,prescaled,
                                          NULL,20,20,
                                          fixscale,0,0,i,1,0,method);
    VImage norm=NormalizeLightingHistogram(dest,facemask);
    hist1[i+180]+=NNApplyToImage(0,norm,0);
    VDestroyImage(norm);
  }
  if (prescaled!=NULL) VDestroyImage(prescaled);
  VDestroyImage(dest);
}

void DerotationAccuracy(double *hist1,
                        double *hist2,
                        double *hist3,
                        double *hist4,
                        double *draccuracy,
                        double *drangular,
                        Alignment *align,
                        VImage src, VImage facemask,
                        int face, int fixscale)
{
  VImage dest=VCreateImage(1,20,20,VUByteRepn);
  VImage prescaled=NULL;
  for (int i=-30; i<=30; i++) {
    prescaled=ExtractAlignedTrainingImage(dest,align,face,src,prescaled,
                                          NULL,20,20,
                                          fixscale,0,0,i,1,0,0);
    VImage norm=NormalizeHistogram(dest);
    NNApplyToImage(4,norm,0);
    VDestroyImage(norm);
    norm=NormalizeLightingHistogram(dest,facemask);
    hist1[i+180]+=(NNApplyToImage(0,norm,0)>0)?1:0;
    hist2[i+180]+=(NNApplyToImage(1,norm,0)>0)?1:0;
    hist3[i+180]+=(NNApplyToImage(2,norm,0)>0)?1:0;
    hist4[i+180]+=(NNApplyToImage(3,norm,0)>0)?1:0;
//    hist1[i+180]+=NNApplyToImage(0,norm,0);
//    hist2[i+180]+=NNApplyToImage(1,norm,0);
//    hist3[i+180]+=NNApplyToImage(2,norm,0);
//    hist4[i+180]+=NNApplyToImage(3,norm,0);
    VDestroyImage(norm);
    double a1=0, a2=0;
    FastForwardUnit *unit=real_fast_list[4]->unitList+
      real_fast_list[4]->firstOutput;
    for (int j=0; j<36; j++) {
      double act=(unit++)->activation;
      a1+=act*cos(j*M_PI/18);
      a2+=act*sin(j*M_PI/18);
    }
    int angle=i-(int)floor(atan2(a2,a1)*180.0/M_PI+0.5);
    while (angle<-180) angle+=360;
    while (angle>=180) angle-=360;
    drangular[i+180]+=abs(angle);
    draccuracy[angle+180]++;
  }
  if (prescaled!=NULL) VDestroyImage(prescaled);
  VDestroyImage(dest);
}

void DerotationAccuracyImage(VImage hist,
                             Alignment *align,
                             VImage src, VImage facemask,
                             int face, int fixscale,
                             int method)
{
  VImage dest=VCreateImage(1,20,20,VUByteRepn);
  VImage prescaled=NULL;
  for (int i=-180; i<180; i++) {
    prescaled=ExtractAlignedTrainingImage(dest,align,face,src,prescaled,
                                          NULL,20,20,
                                          fixscale,0,0,i,1,0,method);
    VImage norm=NormalizeHistogram(dest);
    NNApplyToImage(0,norm,0);
    VDestroyImage(norm);
    double a1=0, a2=0;
    FastForwardUnit *unit=real_fast_list[0]->unitList+
      real_fast_list[0]->firstOutput;
    for (int j=0; j<36; j++) {
      double act=(unit++)->activation;
      a1+=act*cos(j*M_PI/18);
      a2+=act*sin(j*M_PI/18);
    }
    int angle=(int)floor(atan2(a2,a1)*180.0/M_PI+0.5);
    while (angle<-180) angle+=360;
    while (angle>=180) angle-=360;
    VPixel(hist,0,angle+180,i+180,VFloat)++;
  }
  if (prescaled!=NULL) VDestroyImage(prescaled);
  VDestroyImage(dest);
}

void DerotationAccuracyVista(double *hist, VImage input, VImage output,
                             char *filename)
{
  FILE *outf=fopen(filename,"w");
//  int tot[360];
//  for (int j=0; j<360; j++) tot[j]=0;
  for (int i=0; i<VImageNBands(input); i++) {
    NNApplyToImage(0,input,i);
    double a1=0, a2=0;
    double da1=0, da2=0;
    FastForwardUnit *unit=real_fast_list[0]->unitList+
      real_fast_list[0]->firstOutput;
    for (int j=0; j<36; j++) {
      double act=(unit++)->activation;
      a1+=act*cos(j*M_PI/18);
      a2+=act*sin(j*M_PI/18);
      act=VPixel(output,i,0,j,VSByte);
      da1+=act*cos(j*M_PI/18);
      da2+=act*sin(j*M_PI/18);
    }
    int angle=(int)floor(atan2(a2,a1)*180.0/M_PI+0.5);
//    while (angle<-180) angle+=360;
//    if (angle>=180) angle-=360;
    int dangle=(int)floor(atan2(da2,da1)*180.0/M_PI+0.5);
//    while (dangle<-180) dangle+=360;
//    if (dangle>=180) dangle-=360;
//    hist[angle+180]+=dangle;
//    tot[angle+180]++;
    fprintf(outf,"%d %d\n",dangle,angle);
  }
//  for (j=0; j<360; j++) hist[j]/=tot[j];
  fclose(outf);
}

void DerotationAccuracyVistaImage(VImage hist, VImage input, VImage output)
{
  for (int i=0; i<VImageNBands(input); i++) {
    NNApplyToImage(0,input,i);
    double a1=0, a2=0;
    double da1=0, da2=0;
    FastForwardUnit *unit=real_fast_list[0]->unitList+
      real_fast_list[0]->firstOutput;
    for (int j=0; j<36; j++) {
      double act=(unit++)->activation;
      a1+=act*cos(j*M_PI/18);
      a2+=act*sin(j*M_PI/18);
      act=VPixel(output,i,0,j,VSByte);
      da1+=act*cos(j*M_PI/18);
      da2+=act*sin(j*M_PI/18);
    }
    int angle=(int)floor(atan2(a2,a1)*180.0/M_PI+0.5);
    while (angle<-180) angle+=360;
    while (angle>=180) angle-=360;
    int dangle=(int)floor(atan2(da2,da1)*180.0/M_PI+0.5);
    while (dangle<-180) dangle+=360;
    while (dangle>=180) dangle-=360;
    VPixel(hist,0,angle+180,dangle+180,VFloat)++;
  }
}

void DerotationDataAngularDistribution(double *hist, VImage output)
{
  for (int i=0; i<VImageNBands(output); i++) {
    double a1=0, a2=0;
    for (int j=0; j<36; j++) {
      double act=VPixel(output,i,0,j,VSByte);
      a1+=act*cos(j*M_PI/18);
      a2+=act*sin(j*M_PI/18);
    }
    int angle=(int)floor(atan2(a2,a1)*180.0/M_PI+0.5);
    while (angle<-180) angle+=360;
    while (angle>=180) angle-=360;
    hist[angle+180]++;
  }
} 
#endif

void SearchMultiAllOrientation(ByteImage *image, int numNetworks,
                               SearchCallback callback, ClientData data[],
                               int oneOrientation)
{
#ifdef UseAngleCache
  if (!AngleCacheInitialized) InitializeAngleCache();
#endif

  int ptr=0;
  Mat mat=Zero(3,3);
  ByteImage *mask=FindByteImageWithName("facemask");
  int halfW=mask->width/2;
  int halfH=mask->height/2;

  for (int j=-halfH; j<halfH; j++) for (int i=-halfW; i<halfW; i++) 
    if (mask->data(ptr++)) {
      mat(0,0)+=i*i;
      mat(0,1)+=i*j;
      mat(0,2)+=i;
      mat(1,0)+=i*j;
      mat(1,1)+=j*j;
      mat(1,2)+=j;
      mat(2,0)+=i;
      mat(2,1)+=j;
      mat(2,2)+=1;
    }
  mat=LUInverse(mat);

  int *tmp=new int[mask->size];

  Vec vec(3);
  double v0=0, v1=0, v2=0;
  double scale=1.0;
  int level=0;
  int hist[512];
  int map[512];

  int firstOrientation=oneOrientation;
  int lastOrientation=oneOrientation+20;

  if (oneOrientation==-1) {
    firstOrientation=0;
    lastOrientation=360;
  }

  while (image->height>=mask->height && image->width>=mask->width) {
    for (int y=0; y<image->height; y++) {
      for (int x=0; x<image->width; x++) {
        for (int angle=firstOrientation; angle<lastOrientation; angle+=20) {
          int ptr=0;
          v0=0; v1=0; v2=0;

#ifdef UseAngleCache
          int *aptr=AngleCache+(angle%360)*(20*20*2);
          for (int j=-halfH; j<halfH; j++) {
            for (int i=-halfW; i<halfW; i++) {
              int ii=x+(*(aptr++));
              int jj=y+(*(aptr++));
              if (ii<0) ii=0;
              if (jj<0) jj=0;
              if (ii>=image->width) ii=image->width-1;
              if (jj>=image->height) jj=image->height-1;
              int data=image->data(ii,jj);
              tmp[ptr]=data;
              if (mask->data(ptr++)) {
                v0+=i*data;
                v1+=j*data;
                v2+=data;
              }
            } 
          }
#else
          double a=angle*M_PI/180.0;
          for (int j=-halfH; j<halfH; j++) {
            for (int i=-halfW; i<halfW; i++) {
              double ii=x+(i+0.5)*cos(a)-(j+0.5)*sin(a)-0.5;
              double jj=y+(i+0.5)*sin(a)+(j+0.5)*cos(a)-0.5;
              int data=(int)(0.5+image->interpolateExtend(ii,jj));
#ifdef DEBUG
              if (x==DebugX && y==DebugY && level==DebugS) {
                int i2=i+x;
                if (i2<0) i2=0; if (i2>=image->width) i2=image->width-1;
                int j2=j+y;
                if (j2<0) j2=0; if (j2>=image->height) j2=image->height-1;
                fprintf(stderr,"i,j=%d,%d data=%d int=%d\n",i+x,j+y,data,
                        image->data(i2,j2));
              }
#endif
              tmp[ptr]=data;
              if (mask->data(ptr++)) {
                v0+=i*data;
                v1+=j*data;
                v2+=data;
              }
            } 
          }
#endif
          vec(0)=v0; vec(1)=v1; vec(2)=v2;
          vec=mat*vec;
          v0=vec(0); v1=vec(1); v2=vec(2);
          for (int i=0; i<512; i++) hist[i]=0;
          ptr=0;
          for (j=-halfH; j<halfH; j++) for (i=-halfW; i<halfW; i++) {
            int val=tmp[ptr]-(int)(i*v0+j*v1+v2-256.5);
            if (val<0) val=0;
            if (val>=512) val=511;
            if (mask->data(ptr)) hist[val]++;
            tmp[ptr++]=val;
          }
          int *to=map;
          int *from=hist;
          int total=0;
          for (i=0; i<512; i++) {
            int old=total;
            total+=*(from++);
            *(to++)=old+total;
          }
          double scaleFactor=1.0/total;
          FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
          for (i=0; i<mask->size; i++)
            (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;
          
          for (i=0; i<numNetworks; i++) {
            if (i>0) {
              FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
              FastForwardUnit *unit1=&(real_fast_list[i]->unitList[1]);
              for (int j=0; j<mask->size; j++)
                (unit1++)->activation=(unit++)->activation;
            }
            double output=FastForwardPass(real_fast_list[i]);
#ifdef DEBUG
            if (x==DebugX && y==DebugY && level==DebugS) {
              fprintf(stderr,"output=%g\n",output);
              exit(1);
            }
#endif
            if (output>0) {
              callback(data[i],image,x-halfW,y-halfH,mask->width,mask->height,
                       level,scale,output,angle);
            }
          }
        }
      }
    }
    ReduceSize(image,image);
    scale*=1.2;
    level++;
  }
  delete[] tmp;
}

void SearchMultiOrientation(ByteImage *image, int numNetworks, int rotationNet,
			    SearchCallback callback, ClientData data[],
                            double threshold)
{
#ifdef UseAngleCache
  if (!AngleCacheInitialized) InitializeAngleCache();
#endif

  int ptr=0;
  Mat mat=Zero(3,3);
  ByteImage *mask=FindByteImageWithName("facemask");
  int halfW=mask->width/2;
  int halfH=mask->height/2;

  for (int j=-halfH; j<halfH; j++) for (int i=-halfW; i<halfW; i++) 
    if (mask->data(ptr++)) {
      mat(0,0)+=i*i;
      mat(0,1)+=i*j;
      mat(0,2)+=i;
      mat(1,0)+=i*j;
      mat(1,1)+=j*j;
      mat(1,2)+=j;
      mat(2,0)+=i;
      mat(2,1)+=j;
      mat(2,2)+=1;
    }
  mat=LUInverse(mat);

  int *tmp=new int[mask->size];

  Vec vec(3);
  double v0=0, v1=0, v2=0;
  double scale=1.0;
  int level=0;
  while (image->height>=mask->height && image->width>=mask->width) {
    for (int y=0; y<image->height; y++) {
      for (int x=0; x<image->width; x++) {
        int ptr=0;
        v0=0; v1=0; v2=0;
        int hist[512];
        for (int i=0; i<256; i++) hist[i]=0;
        if (x-halfW>=0 && y-halfH>=0 &&
            x+halfW<=image->width && y+halfH<=image->height) {
          for (int iy=y-halfH; iy<y+halfH; iy++)
            for (int ix=x-halfW; ix<x+halfW; ix++) {
              int val=image->data(ix,iy);
              tmp[ptr++]=val;
	      hist[val]++;
            }
        } else {
          for (int iy=y-halfH; iy<y+halfH; iy++)
            for (int ix=x-halfW; ix<x+halfW; ix++) {
              int ii=ix; if (ii<0) ii=0;
              if (ii>=image->width) ii=image->width-1;
              int jj=iy; if (jj<0) jj=0;
              if (jj>=image->height) jj=image->height-1;
              int val=image->data(ii,jj);
              tmp[ptr++]=val;
	      hist[val]++;
            }
        }
        int map[512];
	int *to=map;
	int *from=hist;
	int tot=0;
	for (i=0; i<256; i++) {
	  int old=tot;
	  tot+=*(from++);
	  *(to++)=old+tot;
	}
        double scaleFactor=1.0/tot;
        FastForwardUnit *unit=&(real_fast_list[rotationNet]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;
	FastForwardPass(real_fast_list[rotationNet]);
	unit=real_fast_list[rotationNet]->unitList+
	  real_fast_list[rotationNet]->firstOutput;
	double a1=0, a2=0;
	for (j=0; j<36; j++) {
	  double act=(unit++)->activation;
#ifdef UseAngleCache
	  a1+=act*LookupCos[j];
	  a2+=act*LookupSin[j];
#else
	  a1+=act*cos(j*M_PI/18);
	  a2+=act*sin(j*M_PI/18);
#endif
	}
	double a=-atan2(a2,a1);

	ptr=0;
        v0=0; v1=0; v2=0;
#ifdef UseAngleCache
        int ia=((int)(a*180.0/M_PI+360))%360;
        int *aptr=AngleCache+(ia*20*20*2);
        for (int j=-halfH; j<halfH; j++) {
          for (int i=-halfW; i<halfW; i++) {
            int ii=x+(*(aptr++));
            int jj=y+(*(aptr++));
            if (ii<0) ii=0;
            if (jj<0) jj=0;
            if (ii>=image->width) ii=image->width-1;
            if (jj>=image->height) jj=image->height-1;
            int data=image->data(ii,jj);
            tmp[ptr]=data;
            if (mask->data(ptr++)) {
              v0+=i*data;
              v1+=j*data;
              v2+=data;
            }
          } 
        }
#else
        for (int j=-halfH; j<halfH; j++) {
          for (int i=-halfW; i<halfW; i++) {
            double ii=x+(i+0.5)*cos(a)-(j+0.5)*sin(a)-0.5;
            double jj=y+(i+0.5)*sin(a)+(j+0.5)*cos(a)-0.5;
	    int data=(int)(0.5+image->interpolateExtend(ii,jj));
	    tmp[ptr]=data;
	    if (mask->data(ptr++)) {
	      v0+=i*data;
	      v1+=j*data;
	      v2+=data;
	    }
	  } 
	}
#endif
        vec(0)=v0; vec(1)=v1; vec(2)=v2;
        vec=mat*vec;
        v0=vec(0); v1=vec(1); v2=vec(2);
        for (i=0; i<512; i++) hist[i]=0;
        ptr=0;
        for (j=-halfH; j<halfH; j++) for (i=-halfW; i<halfW; i++) {
          int val=tmp[ptr]-(int)(i*v0+j*v1+v2-256.5);
          if (val<0) val=0;
          if (val>=512) val=511;
          if (mask->data(ptr)) hist[val]++;
          tmp[ptr++]=val;
        }
        to=map;
        from=hist;
        int total=0;
        for (i=0; i<512; i++) {
          int old=total;
          total+=*(from++);
          *(to++)=old+total;
        }
        scaleFactor=1.0/total;
        unit=&(real_fast_list[0]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;
	
        for (i=0; i<numNetworks; i++) {
          if (i>0) {
            FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
            FastForwardUnit *unit1=&(real_fast_list[i]->unitList[1]);
            for (int j=0; j<mask->size; j++)
              (unit1++)->activation=(unit++)->activation;
          }
          double output=FastForwardPass(real_fast_list[i]);
          if (output>threshold) {
            callback(data[i],image,x-halfW,y-halfH,mask->width,mask->height,
                     level,scale,output,(int)floor(0.5+a*180.0/M_PI));
            //            fprintf(stderr,"x=%d,y=%d:\n",x,y);
            //            FastDebugNetwork(real_fast_list[i]);
            //            exit(0);
          }
        }
      }
    }
    ReduceSize(image,image);
    scale*=1.2;
    level++;
  }
  delete[] tmp;
}

double ApplyDerotationDetection(ByteImage *image, int net, int x, int y, 
                                double a)
{
  if (!AngleCacheInitialized) InitializeAngleCache();

  int ptr=0;
  Mat mat=Zero(3,3);
  ByteImage *mask=FindByteImageWithName("facemask");
  int halfW=mask->width/2;
  int halfH=mask->height/2;

  for (int j=-halfH; j<halfH; j++) for (int i=-halfW; i<halfW; i++) 
    if (mask->data(ptr++)) {
      mat(0,0)+=i*i;
      mat(0,1)+=i*j;
      mat(0,2)+=i;
      mat(1,0)+=i*j;
      mat(1,1)+=j*j;
      mat(1,2)+=j;
      mat(2,0)+=i;
      mat(2,1)+=j;
      mat(2,2)+=1;
    }
  mat=LUInverse(mat);

  int *tmp=new int[mask->size];

  Vec vec(3);
  double v0=0, v1=0, v2=0;

  int hist[512];
  for (int i=0; i<512; i++) hist[i]=0;

  int ia=((int)(a+360.5))%360;
  int *aptr=AngleCache+(ia*20*20*2);
  ptr=0;
  for (j=-halfH; j<halfH; j++) {
    for (int i=-halfW; i<halfW; i++) {
      int ii=x+(*(aptr++));
      int jj=y+(*(aptr++));
      if (ii<0) ii=0;
      if (jj<0) jj=0;
      if (ii>=image->width) ii=image->width-1;
      if (jj>=image->height) jj=image->height-1;
      int data=image->data(ii,jj);
      tmp[ptr]=data;
      if (mask->data(ptr++)) {
        v0+=i*data;
        v1+=j*data;
        v2+=data;
      }
    } 
  }
  vec(0)=v0; vec(1)=v1; vec(2)=v2;
  vec=mat*vec;
  v0=vec(0); v1=vec(1); v2=vec(2);
  ptr=0;
  for (j=-halfH; j<halfH; j++) for (i=-halfW; i<halfW; i++) {
    int val=tmp[ptr]-(int)(i*v0+j*v1+v2-256.5);
    if (val<0) val=0;
    if (val>=512) val=511;
    if (mask->data(ptr)) hist[val]++;
    tmp[ptr++]=val;
  }
  int map[512];
  int *to=map;
  int *from=hist;
  int total=0;
  for (i=0; i<512; i++) {
    int old=total;
    total+=*(from++);
    *(to++)=old+total;
  }
  double scaleFactor=1.0/total;
  FastForwardUnit *unit=&(real_fast_list[net]->unitList[1]);
  for (i=0; i<mask->size; i++)
    (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;
	
  double output=FastForwardPass(real_fast_list[net]);
  delete[] tmp;
  return output;
}

#ifndef NoVista
void FigureSearchMultiOrientation(char *byteImageName, 
                                  int x, int y,
                                  int numNetworks, 
                                  int rotationNet,
                                  VImage window,
                                  VImage angle)
{
  ByteImage *image=FindByteImageWithName(byteImageName);
  int ptr=0;
  Mat mat=Zero(3,3);
  ByteImage *mask=FindByteImageWithName("facemask");
  int halfW=mask->width/2;
  int halfH=mask->height/2;

  for (int j=-halfH; j<halfH; j++) for (int i=-halfW; i<halfW; i++) 
    if (mask->data(ptr++)) {
      mat(0,0)+=i*i;
      mat(0,1)+=i*j;
      mat(0,2)+=i;
      mat(1,0)+=i*j;
      mat(1,1)+=j*j;
      mat(1,2)+=j;
      mat(2,0)+=i;
      mat(2,1)+=j;
      mat(2,2)+=1;
    }
  mat=LUInverse(mat);

  int *tmp=new int[mask->size];

  Vec vec(3);
  double v0=0, v1=0, v2=0;

  ptr=0;
  v0=0; v1=0; v2=0;
  int hist[512];
  for (int i=0; i<256; i++) hist[i]=0;
  float *pixelptr=(float*)VPixelPtr(window,0,0,0);
  for (int iy=y-halfH; iy<y+halfH; iy++)
    for (int ix=x-halfW; ix<x+halfW; ix++) {
      int ii=ix; if (ii<0) ii=0;
      if (ii>=image->width) ii=image->width-1;
      int jj=iy; if (jj<0) jj=0;
      if (jj>=image->height) jj=image->height-1;
      int val=image->data(ii,jj);
      tmp[ptr++]=val;
      *(pixelptr++)=val;
      hist[val]++;
    }
  int map[512];
  int *to=map;
  int *from=hist;
  int tot=0;
  for (i=0; i<256; i++) {
    int old=tot;
    tot+=*(from++);
    *(to++)=old+tot;
  }
  double scaleFactor=1.0/tot;
  FastForwardUnit *unit=&(real_fast_list[rotationNet]->unitList[1]);
  for (i=0; i<mask->size; i++) {
    unit->activation=map[tmp[i]]*scaleFactor-1.0;
    *(pixelptr++)=unit->activation;
    unit++;
  }
  FastForwardPass(real_fast_list[rotationNet]);
  unit=real_fast_list[rotationNet]->unitList+
    real_fast_list[rotationNet]->firstOutput;
  double a1=0, a2=0;
  for (j=0; j<36; j++) {
    double act=(unit++)->activation;
    VSetPixel(angle,0,0,j,act);
    a1+=act*cos(j*M_PI/18);
    a2+=act*sin(j*M_PI/18);
  }
  double a=-atan2(a2,a1);
  
  ptr=0;
  v0=0; v1=0; v2=0;
  for (j=-halfH; j<halfH; j++) {
    for (int i=-halfW; i<halfW; i++) {
      double ii=x+(i+0.5)*cos(a)-(j+0.5)*sin(a)-0.5;
      double jj=y+(i+0.5)*sin(a)+(j+0.5)*cos(a)-0.5;
      int data=(int)(0.5+image->interpolateExtend(ii,jj));
      *(pixelptr++)=data;
      tmp[ptr]=data;
      if (mask->data(ptr++)) {
        v0+=i*data;
        v1+=j*data;
        v2+=data;
      }
    } 
  }
  vec(0)=v0; vec(1)=v1; vec(2)=v2;
  vec=mat*vec;
  v0=vec(0); v1=vec(1); v2=vec(2);
  for (i=0; i<512; i++) hist[i]=0;
  ptr=0;
  for (j=-halfH; j<halfH; j++) for (i=-halfW; i<halfW; i++) {
    int val=tmp[ptr]-(int)(i*v0+j*v1+v2-256.5);
    if (val<0) val=0;
    if (val>=512) val=511;
    *(pixelptr++)=val;
    if (mask->data(ptr)) hist[val]++;
    tmp[ptr++]=val;
  }
  to=map;
  from=hist;
  int total=0;
  for (i=0; i<512; i++) {
    int old=total;
    total+=*(from++);
    *(to++)=old+total;
  }
  scaleFactor=1.0/total;
  unit=&(real_fast_list[0]->unitList[1]);
  for (i=0; i<mask->size; i++) {
    unit->activation=map[tmp[i]]*scaleFactor-1.0;
    *(pixelptr++)=unit->activation;
    unit++;
  }

  for (i=0; i<numNetworks; i++) {
    if (i>0) {
      FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
      FastForwardUnit *unit1=&(real_fast_list[i]->unitList[1]);
      for (int j=0; j<mask->size; j++)
        (unit1++)->activation=(unit++)->activation;
    }
    double output=FastForwardPass(real_fast_list[i]);
    fprintf(stderr,"Network %d gives output %g\n",i,output);
  }
  delete[] tmp;
}
#endif

#ifndef NoTcl
int SearchMultiOrientationCmd(ClientData data, Tcl_Interp *interp, 
			      int argc, char *argv[])
{
  if (argc!=4) {
    interp->result="Usage: SearchMultiOrientation <image> <rotnet> {filelist}";
    return TCL_ERROR;
  }
  char **fileList;
  int numFiles;
  ByteImage *image=FindByteImageWithName(argv[1]);
  int rotationNet=atoi(argv[2]);
  Tcl_SplitList(interp,argv[3],&numFiles,&fileList);
  int i;
  FILE **openFileList=new FILE*[numFiles];
  for (i=0; i<numFiles; i++) {
    openFileList[i]=fopen(fileList[i],"w");
    assert(openFileList[i]!=NULL);
  }
  free((char*)fileList);
  SearchMultiOrientation(image,numFiles,rotationNet,SearchMultiCallbackSave,
			 (ClientData*)openFileList);
  for (i=0; i<numFiles; i++)
    fclose(openFileList[i]);
  delete[] openFileList;
  return TCL_OK;
}

int SearchMultiAllOrientationCmd(ClientData data, Tcl_Interp *interp, 
                                 int argc, char *argv[])
{
  if (argc!=4) {
    interp->result="Usage: SearchMultiAllOrientation <image> {filelist} <oneOrientation>";
    return TCL_ERROR;
  }
  char **fileList;
  int numFiles;
  ByteImage *image=FindByteImageWithName(argv[1]);
  Tcl_SplitList(interp,argv[2],&numFiles,&fileList);
  int oneOrientation=atoi(argv[3]);
  int i;
  FILE **openFileList=new FILE*[numFiles];
  for (i=0; i<numFiles; i++) {
    openFileList[i]=fopen(fileList[i],"w");
    assert(openFileList[i]!=NULL);
  }
  free((char*)fileList);
  SearchMultiAllOrientation(image,numFiles,SearchMultiCallbackSave,
                            (ClientData*)openFileList,oneOrientation);
  for (i=0; i<numFiles; i++)
    fclose(openFileList[i]);
  delete[] openFileList;
  return TCL_OK;
}
#endif

CheckResultsStruct *CheckResultsDirect2(char *canvas,
                                        ByteImage *image, char *imageFile, 
                                        List<Detection> *detections, 
                                        Tcl_HashTable *answers, 
                                        ByteImage *mask, 
                                        int angleLimit)
{
  int lw=3;
  int temWidth=image->width, temHeight=image->height;
  int numScales=0, examples=0;
  while (temWidth>=mask->width && temHeight>=mask->height)
  {
    examples+=(temWidth-mask->width+1)*(temHeight-mask->height+1);
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }

  Tcl_HashEntry *entry=Tcl_FindHashEntry(answers,imageFile);
  List<Detection> *answerList=NULL;

  int *faceFound=NULL;
  int numFaces=0;
  if (entry!=NULL) {
    answerList=(List<Detection>*)Tcl_GetHashValue(entry);
    numFaces=answerList->length;
    faceFound=new int[numFaces];
    int ptr=0;
    for (Detection *d=answerList->first; d!=NULL; d=d->next) {
      // Hack to preserve behavior of the old CheckResultDirect,
      // not really necessary any more.
//      LimitScale(0,numScales-1,&d->s,&d->x,&d->y);
      faceFound[ptr++]=0;
      int s=d->s;
      int xx=d->x;
      int yy=d->y;
      double xcenter=xx*pow(1.2,s);
      double ycenter=yy*pow(1.2,s);
#ifndef NoTcl
      char cmd[512];
      sprintf(cmd,"%s create oval %g %g %g %g -fill blue -outline blue"
              " -tag real",
              canvas,xcenter-1,ycenter-1,xcenter+1,ycenter+1);
//      Tcl_Eval(theInterp,cmd);
#endif
    }
  }

  int falsePos=0;
  for (Detection *detect=detections->first; detect!=NULL; 
       detect=detect->next) {
    int found=0;
    if (answerList!=NULL) {
      int ptr=0;
      for (Detection *answer=answerList->first; answer!=NULL && !found;
           answer=answer->next, ptr++) {
        int dx=(int)floor(0.5+(detect->x+mask->width/2)*
                          pow(1.2,detect->s-answer->s));
        int dy=(int)floor(0.5+(detect->y+mask->height/2)*
                          pow(1.2,detect->s-answer->s));
        int angle=detect->orient-answer->orient;
        if (angle<0) angle+=360*(-angle/360+1);
        angle=angle % 360;
        if (angle>180) angle=360-angle;

//        fprintf(stderr,"x = %d\n",abs(dx-answer->x));
//        fprintf(stderr,"y = %d\n",abs(dy-answer->y));
//        fprintf(stderr,"s = %d\n",abs(detect->s-answer->s));
//        fprintf(stderr,"angle = %d  ( %d %d )\n",angle,detect->orient,answer->orient);
        if (abs(dx-answer->x)<=4 &&
            abs(dy-answer->y)<=4 &&
            abs(detect->s-answer->s)<=4 &&
//            detect->s>=answerScale-2 && detect->s<=answerScale+2 &&
            angle<30) {
//          fprintf(stderr,"Found!!!\n");
          found=1;
          faceFound[ptr]=1;
        }
      }
    }
    if (!found) falsePos++;

#ifndef NoTcl
    int x=detect->x, y=detect->y, s=detect->s;
    char cmd[1024];
    double angle=M_PI/180.0*(detect->orient-90);
    double size=mask->width*pow(1.2,s);
    double xc=size/2+x*pow(1.2,s);
    double yc=size/2+y*pow(1.2,s);
    double upX=size/2*cos(angle);
    double upY=size/2*sin(angle);
    double rightX=upY;
    double rightY=-upX;
      
    double x1=(xc-rightX-upX), y1=(yc-rightY-upY);
    double x2=(xc+rightX-upX), y2=(yc+rightY-upY);
    double x3=(xc+rightX+upX), y3=(yc+rightY+upY);
    double x4=(xc-rightX+upX), y4=(yc-rightY+upY);

//      fprintf(stderr,"%g,%g -> %g,%g x %g,%g x %g,%g x %g,%g\n",
//              xc,yc,x1,y1,x2,y2,x3,y3,x4,y4);

    if (found) {
      sprintf(cmd,
              "%s create line %g %g %g %g %g %g %g %g %g %g -fill green -tag right -width %d",
              canvas,x1,y1,x2,y2,x3,y3,x4,y4,x1,y1,lw);
      Tcl_Eval(theInterp,cmd);
      sprintf(cmd,"%s create oval %g %g %g %g -outline green -fill {} -tag right -width %d",
              canvas,
              xc+0.5*(rightX+upX)-0.15*size,yc+0.5*(rightY+upY)-0.15*size,
              xc+0.5*(rightX+upX)+0.15*size,yc+0.5*(rightY+upY)+0.15*size,
              lw);
      Tcl_Eval(theInterp,cmd);
      sprintf(cmd,
              "%s create oval %g %g %g %g -outline green -fill {} -tag right -width %d",
              canvas,
              xc+0.5*(-rightX+upX)-0.15*size,yc+0.5*(-rightY+upY)-0.15*size,
              xc+0.5*(-rightX+upX)+0.15*size,yc+0.5*(-rightY+upY)+0.15*size,
              lw);
      Tcl_Eval(theInterp,cmd);
    } else {
      sprintf(cmd,
              "%s create line %g %g %g %g %g %g %g %g %g %g -fill red -tag wrong -width %d",
              canvas,x1,y1,x2,y2,x3,y3,x4,y4,x1,y1,lw);
      Tcl_Eval(theInterp,cmd);
      sprintf(cmd,
              "%s create oval %g %g %g %g -outline red -fill {} -tag wrong -width %d",
              canvas,
              xc+0.5*(rightX+upX)-0.15*size,yc+0.5*(rightY+upY)-0.15*size,
              xc+0.5*(rightX+upX)+0.15*size,yc+0.5*(rightY+upY)+0.15*size,
              lw);
      Tcl_Eval(theInterp,cmd);
      sprintf(cmd,
              "%s create oval %g %g %g %g -outline red -fill {} -tag wrong -width %d",
              canvas,
              xc+0.5*(-rightX+upX)-0.15*size,yc+0.5*(-rightY+upY)-0.15*size,
              xc+0.5*(-rightX+upX)+0.15*size,yc+0.5*(-rightY+upY)+0.15*size,
              lw);
      Tcl_Eval(theInterp,cmd);
    }
#endif
  }

  int falseNeg=0, detectableFaces=0;
  if (faceFound!=NULL) {
    Detection *answer=answerList->first;
    for (int i=0; i<numFaces; i++, answer=answer->next) {
      if (angleLimit!=-1) {
        int angle=answer->orient;
        while (angle<-180) angle+=360;
        while (angle>=180) angle-=360;
        if (abs(angle)>angleLimit) continue;
      }
      if (!faceFound[i]) {
        falseNeg++;

#ifndef NoTcl
        int x=answer->x, y=answer->y, s=answer->s;
        char cmd[1024];
        double angle=M_PI/180.0*(answer->orient-90);
        double size=mask->width*pow(1.2,s);
        double xc=x*pow(1.2,s);
        double yc=y*pow(1.2,s);
        double upX=size/2*cos(angle);
        double upY=size/2*sin(angle);
        double rightX=upY;
        double rightY=-upX;
        
        double x1=(xc-rightX-upX), y1=(yc-rightY-upY);
        double x2=(xc+rightX-upX), y2=(yc+rightY-upY);
        double x3=(xc+rightX+upX), y3=(yc+rightY+upY);
        double x4=(xc-rightX+upX), y4=(yc-rightY+upY);

        sprintf(cmd,
                "%s create line %g %g %g %g %g %g %g %g %g %g -fill yellow -tag right -width %d",
                canvas,x1,y1,x2,y2,x3,y3,x4,y4,x1,y1,lw);
        Tcl_Eval(theInterp,cmd);
        sprintf(cmd,
                "%s create oval %g %g %g %g -outline yellow -fill {} -tag right -width %d",
                canvas,
                xc+0.5*(rightX+upX)-0.15*size,yc+0.5*(rightY+upY)-0.15*size,
                xc+0.5*(rightX+upX)+0.15*size,yc+0.5*(rightY+upY)+0.15*size,
                lw);
        Tcl_Eval(theInterp,cmd);
        sprintf(cmd,
                "%s create oval %g %g %g %g -outline yellow -fill {} -tag right -width %d",
                canvas,
                xc+0.5*(-rightX+upX)-0.15*size,yc+0.5*(-rightY+upY)-0.15*size,
                xc+0.5*(-rightX+upX)+0.15*size,yc+0.5*(-rightY+upY)+0.15*size,
                lw);
        Tcl_Eval(theInterp,cmd);
#endif
      }
      detectableFaces++;
    }
    delete[] faceFound;
  }
              
  CheckResultsStruct *results=new CheckResultsStruct;
  results->falseNeg=falseNeg;
  results->numFaces=detectableFaces;
  results->falsePos=falsePos;
  results->examples=examples;
  return results;
}

void CheckSearchInstrumentedDirect2(int numScales, char *imageFile,
                                    Tcl_HashTable *answers, ByteImage *mask,
                                    char *tradeName)
{
  FILE *outf=fopen(tradeName,"w");

  ByteImage **outputScale=new ByteImage*[numScales];
  ByteImage **inputScale=new ByteImage*[numScales];
  for (int i=0; i<numScales; i++) {
    char name[256];
    sprintf(name,"output%d",i);
    outputScale[i]=FindByteImageWithName(name);
//    FILE *img=fopen(name,"w");
//    SaveByteImagePgm(outputScale[i],img);
//    fclose(img);
    sprintf(name,"input%d",i);
    inputScale[i]=new ByteImage(name);
    inputScale[i]->ZeroBuffer(outputScale[i]->width,
                              outputScale[i]->height);
  }
  int numFaces=0;

  int facesFound[201];
  int falseAlarms[201];
  int numVal[201];
  for (int t=0; t<=200; t++) {
    facesFound[t]=0;
    falseAlarms[t]=0;
    numVal[t]=0;
  }

  Tcl_HashEntry *entry=Tcl_FindHashEntry(answers,imageFile);
  if (entry!=NULL) {
    List<Detection> *answerList=(List<Detection>*)Tcl_GetHashValue(entry);
    for (Detection *d=answerList->first; d!=NULL; d=d->next) {
      int angle=d->orient;
      while (angle<-180) angle+=360;
      while (angle>=180) angle-=360;
      if (abs(angle)>40) continue;
      numFaces++;
      int min=255;
//      fprintf(stderr,"Trying to look at %d,%d,%d\n",
//              d->x,d->y,d->s);
      for (int scale=d->s-4; scale<=d->s+4; scale++) {
        if (scale>=0 && scale<numScales) {
          int x1=(int)floor((d->x-4.5)*pow(1.2,d->s-scale));
          int x2=(int)ceil((d->x+4.5)*pow(1.2,scale-d->s));
          int y1=(int)floor((d->y-4.5)*pow(1.2,d->s-scale));
          int y2=(int)ceil((d->y+4.5)*pow(1.2,scale-d->s));
          for (int y=y1; y<=y2; y++) for (int x=x1; x<=x2; x++) {
//            fprintf(stderr,"%d,%d,%d -> %d,%d,%d\n",
//                    d->x,d->y,d->s,x,y,scale);
            int dx=(int)floor(0.5+x*pow(1.2,scale-d->s));
            int dy=(int)floor(0.5+y*pow(1.2,scale-d->s));
            if (abs(dx-d->x)<=4 && abs(dy-d->y)<=4) {
              if (x>=0 && y>=0 && 
                  x<outputScale[scale]->width && 
                  y<outputScale[scale]->height) {
//                fprintf(stderr,"%d,%d,%d -> %d,%d,%d\n",
//                        d->x,d->y,d->s,x,y,scale);
                if (outputScale[scale]->data(x,y)<min) 
                  min=outputScale[scale]->data(x,y);
                inputScale[scale]->data(x,y)=1;
              }
            }
          }
        }
      }
      if (min==255) {
        fprintf(stderr,"(%d,%d,%d)=%d\n",
                d->x,d->y,d->s,
                min);
        for (int scale=0; scale<numScales; scale++) {
          char name[256];
          sprintf(name,"trash%d",scale);
          FILE *img=fopen(name,"w");
          assert(img!=NULL);
          SaveByteImagePgm(outputScale[scale],img);
          fclose(img);
        }
        exit(1);
      }
      for (int t=0; t<=min; t++) 
        facesFound[t]++;
    }
  }

  for (int scale=0; scale<numScales; scale++) 
    for (int y=10; y<outputScale[scale]->height-9; y++) 
      for (int x=10; x<outputScale[scale]->width-9; x++) {
//        fprintf(stderr,"%d,%d,%d %d %d\n",x,y,scale,outputScale[scale]->width,outputScale[scale]->height);
        int val=outputScale[scale]->data(x,y);
        if (val>=200) {
          FILE *img=fopen("/tmp/trash","w");
          assert(img!=NULL);
          SaveByteImagePgm(outputScale[scale],img);
          fprintf(stderr,"Yes, I gneerated it !!!!!!\n");
          fprintf(stderr,"error code for close=%d\n",fclose(img));
          fprintf(stderr,"%d,%d,%d=%d\n",x,y,scale,val);
        }
        numVal[val]++;
        if (inputScale[scale]->data(x,y)==0) 
          for (int t=0; t<=outputScale[scale]->data(x,y); t++) 
            falseAlarms[t]++;
      }

  for (t=200; t>=0; t--) {
    fprintf(outf,"%.2f %d %d %d\n",
            (t-100)/100.0,numVal[t],falseAlarms[t],numFaces-facesFound[t]);
  }
  for (i=0; i<numScales; i++) {
    delete inputScale[i];
    delete outputScale[i];
  }
  delete[] inputScale;
  delete[] outputScale;
  fclose(outf);
}

// Set a value in the detection pyramid; if that location does not exist,
// then create it.  Also, add that location to the list of locations with
// that value.  
static void FuzzyVoteAccumInc2(Tcl_HashTable *accum, 
                               int x, int y, int s, int d, int p)
{
  int loc[5];
#ifdef OrientationMerging
  while (d<0) d+=36; d=d%36;
#endif
  loc[0]=x; loc[1]=y; loc[2]=s; loc[3]=d; loc[4]=p;
  Tcl_HashEntry *entry=Tcl_FindHashEntry(accum,(char*)loc);
  if (entry==NULL) {
    int newentry;
    entry=Tcl_CreateHashEntry(accum,(char*)loc,&newentry);
    Tcl_SetHashValue(entry,(char*)1);
  } else {
    int val=1+(int)Tcl_GetHashValue(entry);
    Tcl_SetHashValue(entry,(char*)val);
  }
}

// Set a value in the detection pyramid; if that location does not exist,
// then create it.  Also, add that location to the list of locations with
// that value.  
static void FuzzyVoteAccumZero2(Tcl_HashTable *accum, 
                                int x, int y, int s, int d, int p)
{
  int loc[5];
#ifdef OrientationMerging
  while (d<0) d+=36; d=d%36;
#endif
  loc[0]=x; loc[1]=y; loc[2]=s; loc[3]=d; loc[4]=p;
  Tcl_HashEntry *entry=Tcl_FindHashEntry(accum,(char*)loc);
  if (entry!=NULL) {
    Tcl_SetHashValue(entry,(char*)0);
  }
}

// Get the value of some location in the detection pyramid.  If that location
// does not exist, then return zero (but do not allocate that location).
static int FuzzyVoteAccumGet2(Tcl_HashTable *accum, 
                                int x, int y, int s, int d, int p)
{
  int loc[5];
#ifdef OrientationMerging
  while (d<0) d+=36; d=d%36;
#endif
  loc[0]=x; loc[1]=y; loc[2]=s; loc[3]=d; loc[4]=p;
  Tcl_HashEntry *entry=Tcl_FindHashEntry(accum,(char*)loc);
  if (entry==NULL) return 0;
  return (int)Tcl_GetHashValue(entry);
}

// Given some point in the detection pyramid, locate all 6-connection 
// locations with a value greater than or equal to the specified amount,
// and find their centroid.  The locations in the centroid are set to zero.
// Centroid in scale is computed by averaging the pyramid levels at which
// the faces are detected.
static void FindCentroidAccum2(Tcl_HashTable *accum,
                               int s, int x, int y, int d, int p,
                               int minVal, 
                               double *totalS, double *totalX, double *totalY,
                               double *total)
{
  int value=FuzzyVoteAccumGet2(accum,x,y,s,d,p);
//  fprintf(stderr,"val=%d, min=%d\n",value,minVal);
  if (value>=minVal) {
    fprintf(stderr,"adding %d,%d,%d,%d,%d\n",x,y,s,d,p);
    FuzzyVoteAccumZero2(accum,x,y,s,d,p);
    double rvalue=value;
    (*total)+=rvalue;
    (*totalS)+=s*rvalue;
    (*totalX)+=x*pow(1.2,s)*rvalue;
    (*totalY)+=y*pow(1.2,s)*rvalue;
    FindCentroidAccum2(accum,s,x,y,d,p-1,minVal,
                       totalS,totalX,totalY,total);
    FindCentroidAccum2(accum,s,x,y,d,p+1,minVal,
                       totalS,totalX,totalY,total);
    FindCentroidAccum2(accum,s,x,y,d-1,p,minVal,
                       totalS,totalX,totalY,total);
    FindCentroidAccum2(accum,s,x,y,d+1,p,minVal,
                       totalS,totalX,totalY,total);
    FindCentroidAccum2(accum,s,x+1,y,d,p,minVal,
                       totalS,totalX,totalY,total);
    FindCentroidAccum2(accum,s,x-1,y,d,p,minVal,
                       totalS,totalX,totalY,total);
    FindCentroidAccum2(accum,s,x,y+1,d,p,minVal,
                       totalS,totalX,totalY,total);
    FindCentroidAccum2(accum,s,x,y-1,d,p,minVal,
                       totalS,totalX,totalY,total);
    FindCentroidAccum2(accum,
                       s-1,(int)floor(0.5+x*1.2),(int)floor(0.5+y*1.2),d,p,
                       minVal,totalS,totalX,totalY,total);
    FindCentroidAccum2(accum,
                       s+1,(int)floor(0.5+x/1.2),(int)floor(0.5+y/1.2),d,p,
                       minVal,totalS,totalX,totalY,total);
  }
}

// New arbitration procedure.  Slower and takes more memory than (say)
// FuzzyVote2Direct, but is useful for the router networks because it
// keeps track of which view each detection is associated with, and
// counts that as part of the position of the detection.  Thus, only
// detections of the same view at the same location reinforce one another.
// (However, overlap removal will remove detections which overlap with each
// other but have different views.)
void NewFuzzyVote2(List<Detection> *outputList, 
                   List<Detection> *inputList, int numInputLists,
                   int width, int height, int maskWidth, int maskHeight,
                   int spread, int threshold, int overlap)
{
  // Hash table represents which locations/scales in the detection are
  // filled in, and with what values.  Any missing entries are assumed
  // to be zero.
  Tcl_HashTable accum;
  Tcl_InitHashTable(&accum,5);

  // Compute number of levels in input pyramid
  int temWidth=width, temHeight=height;
  int numScales=0;
  while (temWidth>=maskWidth && temHeight>=maskHeight) {
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }

  // For each detection list given as input (all detections are treated
  // equally)

  for (int num=0; num<numInputLists; num++) {
    // For each detection
    for (Detection *detect=inputList[num].first; detect!=NULL;
         detect=detect->next) {
//      fprintf(stderr,"I");
      int xc=detect->x, yc=detect->y, s=detect->s, d=detect->orient;
#ifdef OrientationMerging
      d=(int)floor(0.5+0.1*d);
#endif
      // Spread out the detection in both scale and location by 
      // "spread" levels or pixels, incrementing the value of each
      // location in the detection pyramid
      for (int si=-spread; si<=spread; si++) {
        if (si+s<0 || si+s>=numScales) continue;
        int xci=(int)floor(0.5+xc*exp(log(1.2)*-si));
        int yci=(int)floor(0.5+yc*exp(log(1.2)*-si));
        for (int xx=xci-spread; xx<=xci+spread; xx++)
          for (int yy=yci-spread; yy<=yci+spread; yy++)
            for (int o=d-spread; o<=d+spread; o++)
              FuzzyVoteAccumInc2(&accum,xx,yy,s+si,o,0);
      }
    }
  }
//  fprintf(stderr,"\n");

  for (num=0; num<numInputLists; num++) {
    // For each detection
    for (Detection *detect=inputList[num].first; detect!=NULL;
         detect=detect->next) {
//      fprintf(stderr,"M");
      int xc=detect->x, yc=detect->y, s=detect->s, d=detect->orient;
#ifdef OrientationMerging
      d=(int)floor(0.5+0.1*d);
#endif
      detect->output=0;
      // Spread out the detection in both scale and location by 
      // "spread" levels or pixels, incrementing the value of each
      // location in the detection pyramid
      for (int si=-spread; si<=spread; si++) {
        if (si+s<0 || si+s>=numScales) continue;
        int xci=(int)floor(0.5+xc*exp(log(1.2)*-si));
        int yci=(int)floor(0.5+yc*exp(log(1.2)*-si));
        for (int xx=xci-spread; xx<=xci+spread; xx++)
          for (int yy=yci-spread; yy<=yci+spread; yy++)
            for (int o=d-spread; o<=d+spread; o++) {
              int conf=FuzzyVoteAccumGet2(&accum,xx,yy,s+si,o,0);
              if (conf>detect->output) {
                detect->output=conf+0.5;
                detect->x=xx;
                detect->y=yy;
                detect->s=s+si;
                detect->orient=o;
              }
            }
      }
    }
  }
//  fprintf(stderr,"\n");

  for (num=1; num<numInputLists; num++)
    inputList[0].append(&(inputList[num]));
//  fprintf(stderr,"S(%d)",inputList[0].length);
  inputList[0].sort(CompareDetections);
//  fprintf(stderr,"\n");

  outputList->deleteNodes();

  // Scan through the detection pyramid from highest to lowest value
  while (!inputList[0].empty() && inputList[0].first->output>=threshold) {
    Detection *detect=inputList[0].first;
//    fprintf(stderr,"Z(%g)",detect->output);
//    fprintf(stderr,"%d,%d s %d o %d c %g\n",
//            detect->x,detect->y,detect->s,detect->orient,detect->output);
    // Get the detection
    int x=detect->x;
    int y=detect->y;
    int s=detect->s;
    int d=detect->orient;
    int cs, cx, cy;
    double total=0.0;
    double totalS=0, totalX=0, totalY=0;
// Find the centroid of detections around this location
    FindCentroidAccum2(&accum,s,x,y,d,0,(int)floor(detect->output),
                       &totalS,&totalX,&totalY,&total);
//    fprintf(stderr,"centroid %g %g %g / %g\n",
//            totalX,totalY,totalS,total);
    cs=(int)floor(0.5+totalS/total);
    cx=(int)floor(0.5+totalX/total*pow(1.2,-cs));
    cy=(int)floor(0.5+totalY/total*pow(1.2,-cs));
//    fprintf(stderr,"centroid %d %d %d\n",cx,cy,cs);
//    cs=s;
//    cx=x;
//    cy=y;
    // If we are to remove overlapping detections, scan through
    // the detection pyramid, removing all possible overlaps
    if (overlap) {
      Detection *elem=inputList[0].first;
      while (elem!=NULL) {
        Detection *next=elem->next;
        int xpos=(int)floor(0.5+cx*pow(1.2,cs-elem->s));
        int ypos=(int)floor(0.5+cy*pow(1.2,cs-elem->s));
        int sizex=maskWidth/2+
          (int)floor(0.5+maskWidth/2*pow(1.2,cs-elem->s));
        int sizey=maskHeight/2+
          (int)floor(0.5+maskHeight/2*pow(1.2,cs-elem->s));
//        if (elem==detect) {
//          fprintf(stderr,"match %d,%d %dx%d %d,%d %d,%d,%d\n",
//                  xpos,ypos,sizex,sizey,cx,cy,elem->x,elem->y,elem->s);
//        }
        if (elem->x>=xpos-sizex && elem->x<=xpos+sizex &&
            elem->y>=ypos-sizey && elem->y<=ypos+sizey) {
//          fprintf(stderr,"Deleted\n");
          inputList[0].deleteNode(elem);
        }
        elem=next;
      }
    }
    
    // Record the detection
#ifdef OrientationMerging
    outputList->addLast(new Detection(cx,cy,cs,1,d*10));
#else
    outputList->addLast(new Detection(cx,cy,cs,1,d));
#endif
  }
//  fprintf(stderr,"\n");
  // Clean up
  Tcl_DeleteHashTable(&accum);
}

// New arbitration procedure.  Slower and takes more memory than (say)
// FuzzyVote2Direct, but is useful for the router networks because it
// keeps track of which view each detection is associated with, and
// counts that as part of the position of the detection.  Thus, only
// detections of the same view at the same location reinforce one another.
// (However, overlap removal will remove detections which overlap with each
// other but have different views.)
void NewFuzzyVote3(List<Detection> *outputList, 
                   List<Detection> *inputList, int numInputLists,
                   int width, int height, int maskWidth, int maskHeight,
                   int spread, int threshold, int overlap)
{
  // Hash table represents which locations/scales in the detection are
  // filled in, and with what values.  Any missing entries are assumed
  // to be zero.
  Tcl_HashTable accum;
  Tcl_InitHashTable(&accum,5);

  // Compute number of levels in input pyramid
  int temWidth=width, temHeight=height;
  int numScales=0;
  while (temWidth>=maskWidth && temHeight>=maskHeight) {
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }

  // For each detection list given as input (all detections are treated
  // equally)

  for (int num=0; num<numInputLists; num++) {
    // For each detection
    for (Detection *detect=inputList[num].first; detect!=NULL;
         detect=detect->next) {
//      fprintf(stderr,"I");
      int xc=detect->x, yc=detect->y, s=detect->s, d=detect->orient;
#ifdef OrientationMerging
      d=(int)floor(0.5+0.1*d);
#endif
      FuzzyVoteAccumInc2(&accum,xc,yc,s,d,detect->profile);
    }
  }
//  fprintf(stderr,"\n");

  for (num=0; num<numInputLists; num++) {
    // For each detection
    for (Detection *detect=inputList[num].first; detect!=NULL;
         detect=detect->next) {
//      fprintf(stderr,"M");
      int xc=detect->x, yc=detect->y, s=detect->s, d=detect->orient, 
        p=detect->profile;
//      fprintf(stderr,"Before: %d",d);
#ifdef OrientationMerging
      d=(int)floor(0.5+0.1*d);
#endif
      detect->output=0;
      // Spread out the detection in both scale and location by 
      // "spread" levels or pixels, incrementing the value of each
      // location in the detection pyramid
      double tx=0, ty=0, ts=0, tax=0, tay=0, tot=0, tp=0;
      for (int si=-spread; si<=spread; si++) {
        if (si+s<0 || si+s>=numScales) continue;
        int xci=(int)floor(0.5+xc*exp(log(1.2)*-si));
        int yci=(int)floor(0.5+yc*exp(log(1.2)*-si));
        for (int xx=xci-spread; xx<=xci+spread; xx++)
          for (int yy=yci-spread; yy<=yci+spread; yy++)
            for (int o=d-spread; o<=d+spread; o++) {
              for (int pp=p-1; pp<=p+1; pp++) {
                double val=FuzzyVoteAccumGet2(&accum,xx,yy,s+si,o,pp);
                if (val>0) {
                  tot+=val;
                  tx+=val*xx*pow(1.2,s+si);
                  ty+=val*yy*pow(1.2,s+si);
                  ts+=val*s;
                  tax+=val*cos(o*10.0*M_PI/180.0);
                  tay+=val*sin(o*10.0*M_PI/180.0);
                  tp+=val*pp;
                }
              }
            }
      }
      detect->output=tot;
      detect->s=(int)floor(0.5+ts/tot);
      detect->x=(int)floor(0.5+tx/tot*pow(1.2,-detect->s));
      detect->y=(int)floor(0.5+ty/tot*pow(1.2,-detect->s));
      detect->orient=(int)floor(0.5+180.0/M_PI*atan2(tay,tax));
      detect->profile=(int)floor(0.5+tp/tot);
//      fprintf(stderr,"   After: %d\n",detect->orient);
    }
  }
//  fprintf(stderr,"\n");

  for (num=1; num<numInputLists; num++)
    inputList[0].append(&(inputList[num]));
//  fprintf(stderr,"S(%d)",inputList[0].length);
  inputList[0].sort(CompareDetections);
//  fprintf(stderr,"\n");

  outputList->deleteNodes();

  // Scan through the detection pyramid from highest to lowest value
  while (!inputList[0].empty() && inputList[0].first->output>=threshold) {
    Detection *detect=inputList[0].first;
//    fprintf(stderr,"Z(%g)",detect->output);
//    fprintf(stderr,"%d,%d s %d o %d c %g\n",
//            detect->x,detect->y,detect->s,detect->orient,detect->output);
    // Get the detection
    int x=detect->x;
    int y=detect->y;
    int s=detect->s;
    int d=detect->orient;
    int p=detect->profile;
    double conf=detect->output;
#ifdef OrientationMerging
    d=(int)floor(0.5+0.1*d);
#endif
    int cs, cx, cy;
    double total=0.0;
    double totalS=0, totalX=0, totalY=0;
// Find the centroid of detections around this location
//    FindCentroidAccum2(&accum,s,x,y,d,1,
//                       &totalS,&totalX,&totalY,&total);
//    fprintf(stderr,"centroid %g %g %g / %g\n",
//            totalX,totalY,totalS,total);
//    cs=(int)floor(0.5+totalS/total);
//    cx=(int)floor(0.5+totalX/total*pow(1.2,-cs));
//    cy=(int)floor(0.5+totalY/total*pow(1.2,-cs));
//    fprintf(stderr,"centroid %d %d %d\n",cx,cy,cs);
    cs=s;
    cx=x;
    cy=y;
    // If we are to remove overlapping detections, scan through
    // the detection pyramid, removing all possible overlaps
    if (overlap) {
      Detection *elem=inputList[0].first;
      while (elem!=NULL) {
        Detection *next=elem->next;
        int xpos=(int)floor(0.5+cx*pow(1.2,cs-elem->s));
        int ypos=(int)floor(0.5+cy*pow(1.2,cs-elem->s));
        int sizex=maskWidth/2+
          (int)floor(0.5+maskWidth/2*pow(1.2,cs-elem->s));
        int sizey=maskHeight/2+
          (int)floor(0.5+maskHeight/2*pow(1.2,cs-elem->s));
//        if (elem==detect) {
//          fprintf(stderr,"match %d,%d %dx%d %d,%d %d,%d,%d\n",
//                  xpos,ypos,sizex,sizey,cx,cy,elem->x,elem->y,elem->s);
//        }
        if (elem->x>=xpos-sizex && elem->x<=xpos+sizex &&
            elem->y>=ypos-sizey && elem->y<=ypos+sizey) {
//          fprintf(stderr,"Deleted\n");
          inputList[0].deleteNode(elem);
        }
        elem=next;
      }
    } else {
      inputList[0].deleteNode(detect);
    }
    
    // Record the detection
#ifdef OrientationMerging
    outputList->addLast(new Detection(cx,cy,cs,conf,d*10,p));
#else
    outputList->addLast(new Detection(cx,cy,cs,conf,d,p));
#endif
  }
//  fprintf(stderr,"\n");
  // Clean up
  Tcl_DeleteHashTable(&accum);
}

#ifndef NoTcl
// Look at one column of pixels in the image (in some random order), and
// see if any false alarms occur.  Record up to maxExamples of them.
void SearchColumnOrientation(ByteImage *image, ByteImage *mask,
                             int maxExamples, int correctLight, int column,
                             int *rowIndex)
{
  int halfW=mask->width/2;
  int halfH=mask->height/2;
  int restW=mask->width-halfW;
  int restH=mask->height-halfH;

  int ptr=0;
  int i, j;

  int rotationNet=1;

  // Compute the lighting correction matrix if this is the first time
  // through.
  if (!correctLightingInitialized) {
    correctLighting=Zero(3,3);
    for (j=-halfH; j<restH; j++) for (i=-halfW; i<restW; i++) 
      if (mask->data(ptr++)) {
        correctLighting(0,0)+=i*i;
        correctLighting(0,1)+=i*j;
        correctLighting(0,2)+=i;
        correctLighting(1,0)+=i*j;
        correctLighting(1,1)+=j*j;
        correctLighting(1,2)+=j;
        correctLighting(2,0)+=i;
        correctLighting(2,1)+=j;
        correctLighting(2,2)+=1;
      }
    correctLighting=LUInverse(correctLighting);
    if (!correctLight) correctLighting=Zero(3,3);
    correctLightingInitialized=1;
  }

  int *tmp=new int[mask->size];           // Holds current window

  Vec vec(3);
  int x=column;
  for (int yInd=0; yInd<image->height; yInd++) {
    int y=rowIndex[yInd];                // Look up which Y position to scan
    int ptr=0;
    double v0=0, v1=0, v2=0;
    // Extract window from image, do histogram equalization.
    // Edge pixels are replicated for window pixels outside of the image
    int hist[512];
    for (int i=0; i<256; i++) hist[i]=0;
    if (x-halfW>=0 && y-halfH>=0 &&
        x+halfW<=image->width && y+halfH<=image->height) {
      for (int iy=y-halfH; iy<y+halfH; iy++)
        for (int ix=x-halfW; ix<x+halfW; ix++) {
          int val=image->data(ix,iy);
          tmp[ptr++]=val;
          hist[val]++;
        }
    } else {
      for (int iy=y-halfH; iy<y+halfH; iy++)
        for (int ix=x-halfW; ix<x+halfW; ix++) {
          int ii=ix; if (ii<0) ii=0;
          if (ii>=image->width) ii=image->width-1;
          int jj=iy; if (jj<0) jj=0;
          if (jj>=image->height) jj=image->height-1;
          int val=image->data(ii,jj);
          tmp[ptr++]=val;
	      hist[val]++;
        }
    }
    int map[512];
    int *to=map;
    int *from=hist;
    int tot=0;
    for (i=0; i<256; i++) {
      int old=tot;
      tot+=*(from++);
      *(to++)=old+tot;
    }
    double scaleFactor=1.0/tot;
    FastForwardUnit *unit=&(real_fast_list[rotationNet]->unitList[1]);
    for (i=0; i<mask->size; i++)
      (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;
    FastForwardPass(real_fast_list[rotationNet]);
    unit=real_fast_list[rotationNet]->unitList+
      real_fast_list[rotationNet]->firstOutput;
    double a1=0, a2=0;
    for (j=0; j<36; j++) {
      double act=(unit++)->activation;
      a1+=act*cos(j*M_PI/18);
      a2+=act*sin(j*M_PI/18);
    }
    double a=-atan2(a2,a1);

    ptr=0;
    v0=0; v1=0; v2=0;
    for (int j=-halfH; j<halfH; j++) {
      for (int i=-halfW; i<halfW; i++) {
        double ii=x+(i+0.5)*cos(a)-(j+0.5)*sin(a)-0.5;
        double jj=y+(i+0.5)*sin(a)+(j+0.5)*cos(a)-0.5;
        int data=(int)(0.5+image->interpolateExtend(ii,jj));
        tmp[ptr]=data;
        if (mask->data(ptr++)) {
          v0+=i*data;
          v1+=j*data;
          v2+=data;
        }
      } 
    }
    vec(0)=v0; vec(1)=v1; vec(2)=v2;
    vec=correctLighting*vec;
    v0=vec(0); v1=vec(1); v2=vec(2);
    for (i=0; i<512; i++) hist[i]=0;
    ptr=0;
    for (j=-halfH; j<halfH; j++) for (i=-halfW; i<halfW; i++) {
      int val=tmp[ptr]-(int)(i*v0+j*v1+v2-256.5);
      if (val<0) val=0;
      if (val>=512) val=511;
      if (mask->data(ptr)) hist[val]++;
      tmp[ptr++]=val;
    }
    to=map;
    from=hist;
    int total=0;
    for (i=0; i<512; i++) {
      int old=total;
      total+=*(from++);
      *(to++)=old+total;
    }
    scaleFactor=1.0/total;
    unit=&(real_fast_list[0]->unitList[1]);
    for (i=0; i<mask->size; i++)
      (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;

    numTried++;

    int mistake=0;

    // Apply network
    double result=FastForwardPass(real_fast_list[0]);
    if (result>SearchThreshold) {
      // If network gave false alarm, then record it
      for (i=0; i<mask->size; i++)
        patterns[i+mask->size*numExamples]=
          (int)floor(0.5+127.0*real_fast_list[0]->unitList[i+1].activation);
      numExamples++;
      if (numExamples==maxExamples) {
	// If we have enough false alarms, just clean up and return
        delete[] tmp;
        return;
      }
    }
  }
  // Clean up and return
  delete[] tmp;
}
#endif

#ifndef NoTcl
int SearchPreciseOrientationCmd(ClientData data, Tcl_Interp *interp, 
                                int argc, char *argv[])
{
#ifdef UseAngleCache
  if (!AngleCacheInitialized) InitializeAngleCache();
#endif
  if (argc!=5 && argc!=7)
  {
    interp->result="Usage: SearchPreciseOrientation <image> <scale> <mask> <precise-net> [searchx searchy]";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  double scale=atof(argv[2]);
  ByteImage *mainmask=FindByteImageWithName(argv[3]);
  int preciseNet=atoi(argv[4]);
  
  int fixedsearch=(argc==7);
  int searchx=0, searchy=0;
  if (fixedsearch) {
    searchx=atoi(argv[5]);
    searchy=atoi(argv[6]);
  }

  Tcl_DString ds;
  Tcl_DStringInit(&ds);

  int candidatesFound=0, candidatesTested=0, candidatesPossible=0;

  int tmp[900];
  int hist[256];
//  for (y=0; y<image->height; y+=10) {
//    for (x=0; x<image->width; x+=10) {
  int startx=0, endx=image->width, starty=0, endy=image->height;
  if (fixedsearch) {
    startx=searchx; endx=searchx+1;
    starty=searchy; endy=searchy+1;
  }

#ifdef UsePosterLogging
  VImage log=NULL;
  if (EnablePosterLogging) log=ByteImageToVista(image);
#endif

  for (int y=starty; y<endy; y+=10) {
    for (int x=startx; x<endx; x+=10) {
      candidatesPossible++;

      int maskx1=(int)floor(x/scale+0.5);
      int maskx2=(int)floor((x+10)/scale+0.5);
      int masky1=(int)floor(y/scale+0.5);
      int masky2=(int)floor((y+10)/scale+0.5);
      if (maskx1<0) maskx1=0;
      if (masky1<0) masky1=0;
      if (maskx2>mainmask->width) maskx2=mainmask->width;
      if (masky2>mainmask->height) masky2=mainmask->height;
      
      int totalSet=0;
      int totalChecked=(maskx2-maskx1)*(masky2-masky1);
      for (int my=masky1; my<masky2; my++)
        for (int mx=maskx1; mx<maskx2; mx++) 
          if (mainmask->data(mx,my)==0) 
            totalSet++;
      if ((double)totalSet/(double)totalChecked<0.10) {
#ifdef UsePosterLogging
        if (EnablePosterLogging) {
          for (int yy=y; yy<y+10; yy++) 
            for (int xx=x; xx<x+10; xx++) 
              if (xx>=0 && yy>=0 && xx<image->width && yy<image->height)
                VPixel(log,0,yy,xx,VUByte)=0;
        }
#endif
        continue;
      }        

      candidatesTested++;
      int i, j;

      int *to=hist;
      for (i=0; i<256; i++) *(to++)=0;
      to=tmp;

      if (x>=10 && y>=10 && x+20<=image->width && y+20<=image->height) {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) {
          int val=image->data(i,j);
          hist[val]++;
          (*(to++))=val;
        }
      } else {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) {
          int ii=i; if (ii<0) ii=0; if (ii>=image->width) ii=image->width-1;
          int jj=j; if (jj<0) jj=0; if (jj>=image->height) jj=image->height-1;
          int val=image->data(ii,jj);
          hist[val]++;
          (*(to++))=val;
        }
      }
      float map[256];
      float *mapto=map;
      int *from=hist;
      int tot=0;
      for (i=0; i<256; i++) {
        int old=tot;
        tot+=*(from++);
        (*(mapto++))=(old+tot)/900.0-1.0;
      }
      from=tmp;
      FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
      for (i=0; i<900; i++)
        (unit++)->activation=map[*(from++)];
#ifdef UsePosterLogging
      if (EnablePosterLogging) {
        VImage cand=VCreateImage(1,30,30,VUByteRepn);
        SetCandidate(x,y);
        for (int y=0; y<30; y++) for (int x=0; x<30; x++) 
          VSetPixel(cand,0,y,x,
                    real_fast_list[0]->unitList[1+y*30+x].activation*127+127);
        WriteVistaImageLogging(cand,"cand1");
        SetNoCandidate();
        VDestroyImage(cand);
      }
#endif
      if (FastForwardPass(real_fast_list[0])>0) {
        candidatesFound++;
        FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
        FastForwardUnit *unit2=&(real_fast_list[preciseNet]->unitList[1]);
        for (i=0; i<900; i++)
          (unit2++)->activation=(unit++)->activation;
        FastForwardPass(real_fast_list[preciseNet]);
        double xx=0, tot=0;
        unit=&(real_fast_list[preciseNet]->unitList
               [real_fast_list[preciseNet]->firstOutput]);
        for (j=0; j<20; j++) {
          double act=1.0+(unit++)->activation;
          tot+=act;
          xx+=act*(j-10);
        }
        xx/=tot;
        double yy=0;
        tot=0;
        for (j=0; j<20; j++) {
          double act=1.0+(unit++)->activation;
          tot+=act;
          yy+=act*(j-10);
        }
        yy/=tot;

        double a1=0, a2=0;
        for (j=0; j<36; j++) {
          double act=(unit++)->activation;
#ifdef UseAngleCache
	  a1+=act*LookupCos[j];
	  a2+=act*LookupSin[j];
#else
	  a1+=act*cos(j*M_PI/18);
	  a2+=act*sin(j*M_PI/18);
#endif
        }
        double a=-atan2(a2,a1);

        char tem[256];
        sprintf(tem,"{ %g %g %g %d %d } ",
                x+5-xx,
                y+5-yy,
                a,x,y);
        Tcl_DStringAppend(&ds,tem,-1);
      }
    }
  }
  fprintf(stderr,"%d of %d of %d candidates.\n",
          candidatesFound,candidatesTested,candidatesPossible);
  Tcl_DStringResult(interp,&ds);

#ifdef UsePosterLogging
  if (EnablePosterLogging) WriteVistaImageLogging(log,"maskregions");
#endif

  return TCL_OK;
}
#endif

#if 0
int SearchPreciseOrientationCmd(ClientData data, Tcl_Interp *interp, 
                                int argc, char *argv[])
{
#ifdef UseAngleCache
  if (!AngleCacheInitialized) InitializeAngleCache();
#endif
  if (argc!=4 && argc!=6)
  {
    interp->result="Usage: SearchPreciseOrientation <image> <levelmask> <precise-net> [searchx searchy]";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *levelmask=FindByteImageWithName(argv[2]);
  int preciseNet=atoi(argv[3]);
  int fixedsearch=(argc==6);
  int searchx=0, searchy=0;
  if (fixedsearch) {
    searchx=atoi(argv[4]);
    searchy=atoi(argv[5]);
  }

  Tcl_DString ds;
  Tcl_DStringInit(&ds);

  int candidatesFound=0, candidatesTested=0, candidatesPossible=0;

  int x, y, i, j;
  int xp=(image->width+9)/10;
  int yp=(image->height+9)/10;
  for (y=0; y<yp; y++)
    for (x=0; x<xp; x++) {
      int tot=0, num=0;
      for (j=y*10; j<y*10+10; j++)
        for (i=x*10; i<x*10+10; i++)
          if (i<levelmask->width && j<levelmask->height) {
            tot++;
            if (!levelmask->data(i,j)) num++;
          }
      if (num<=5)
        levelmask->data(x*10,y*10)=255; else
          levelmask->data(x*10,y*10)=0;
    }

  int tmp[900];
  int hist[256];
//  for (y=0; y<image->height; y+=10) {
//    for (x=0; x<image->width; x+=10) {
  int startx=0, endx=image->width, starty=0, endy=image->height;
  if (fixedsearch) {
    startx=searchx; endx=searchx+1;
    starty=searchy; endy=searchy+1;
  }
  for (y=starty; y<endy; y+=10) {
    for (x=startx; x<endx; x+=10) {
      candidatesPossible++;
      if (levelmask->data(x,y))
        continue;
      candidatesTested++;
      int i, j;

      int *to=hist;
      for (i=0; i<256; i++) *(to++)=0;
      to=tmp;

      if (x>=10 && y>=10 && x+20<=image->width && y+20<=image->height) {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) {
          int val=image->data(i,j);
          hist[val]++;
          (*(to++))=val;
        }
      } else {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) {
          int ii=i; if (ii<0) ii=0; if (ii>=image->width) ii=image->width-1;
          int jj=j; if (jj<0) jj=0; if (jj>=image->height) jj=image->height-1;
          int val=image->data(ii,jj);
          hist[val]++;
          (*(to++))=val;
        }
      }
      float map[256];
      float *mapto=map;
      int *from=hist;
      int tot=0;
      for (i=0; i<256; i++) {
        int old=tot;
        tot+=*(from++);
        (*(mapto++))=(old+tot)/900.0-1.0;
      }
      from=tmp;
      FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
      for (i=0; i<900; i++)
        (unit++)->activation=map[*(from++)];
      if (FastForwardPass(real_fast_list[0])>0) {
//      if (1) {
        candidatesFound++;
        FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
        FastForwardUnit *unit2=&(real_fast_list[preciseNet]->unitList[1]);
        for (i=0; i<900; i++)
          (unit2++)->activation=(unit++)->activation;
        FastForwardPass(real_fast_list[preciseNet]);
        double xx=0, tot=0;
        unit=&(real_fast_list[preciseNet]->unitList
               [real_fast_list[preciseNet]->firstOutput]);
        for (j=0; j<20; j++) {
          double act=1.0+(unit++)->activation;
          tot+=act;
          xx+=act*(j-10);
        }
        xx/=tot;
        double yy=0;
        tot=0;
        for (j=0; j<20; j++) {
          double act=1.0+(unit++)->activation;
          tot+=act;
          yy+=act*(j-10);
        }
        yy/=tot;

        double a1=0, a2=0;
        for (j=0; j<36; j++) {
          double act=(unit++)->activation;
#ifdef UseAngleCache
	  a1+=act*LookupCos[j];
	  a2+=act*LookupSin[j];
#else
	  a1+=act*cos(j*M_PI/18);
	  a2+=act*sin(j*M_PI/18);
#endif
        }
        double a=-atan2(a2,a1);

        char tem[256];
        sprintf(tem,"{ %g %g %g } ",
                x+5-xx,
                y+5-yy,
                a*180.0/M_PI);
        Tcl_DStringAppend(&ds,tem,-1);
      }
    }
  }
  fprintf(stderr,"%d of %d of %d candidates.\n",
          candidatesFound,candidatesTested,candidatesPossible);
  Tcl_DStringResult(interp,&ds);
  return TCL_OK;
}
#endif

int FindNewLocationOrientation(ByteImage *image, ByteImage *mask,
                               double x, double y, double a, 
                               double *newX, double *newY, double *newA)
{
  if (!LightingCorrectionMatrixInitialized)
    InitializeLightingCorrectionMatrix();
#ifdef UseAngleCache
  if (!AngleCacheInitialized) InitializeAngleCache();
#endif

  Vec vec(3);
  int hist[512];
  int map[512];
  int *tmp=new int[mask->size];

  int found=1;

  int halfH=mask->height/2;
  int halfW=mask->width/2;

#if 0
  int incx=1;
  int incy=1;
  double inca=10.0*M_PI/180.0;
  int startx=0;
  int starty=0;
  double starta=-10.0*M_PI/180.0;
  int stepsx=1;
  int stepsy=1;
  int stepsa=3;
#endif
#if 1
  int incx=1;
  int incy=1;
  double inca=20.0*M_PI/180.0;
  int startx=-1;
  int starty=-1;
  double starta=-40.0*M_PI/180.0;
  int stepsx=3;
  int stepsy=3;
  int stepsa=5;
#endif
#if 0
  int incx=1;
  int incy=1;
  double inca=20.0*M_PI/180.0;
  int startx=-1;
  int starty=-1;
  double starta=0;
  int stepsx=3;
  int stepsy=3;
  int stepsa=1;
#endif

  double totX=0, totY=0, totDirX=0, totDirY=0;
  int tot=0;

//  ByteImage *history0=FindByteImageWithName("face0");

  for (int stepa=0; stepa<stepsa; stepa++)
    for (int stepy=0; stepy<stepsy; stepy++)
      for (int stepx=0; stepx<stepsx; stepx++) {
        int rx=(int)(x+startx+incx*stepx);
        int ry=(int)(y+starty+incy*stepy);
        double ra=a+starta+inca*stepa;
        int ptr=0;
        double v0=0, v1=0, v2=0;

#ifdef UseAngleCache
        int ia=((int)(ra*180.0/M_PI+360))%360;
        int *aptr=AngleCache+(ia*20*20*2);
        for (int j=-halfH; j<halfH; j++) {
          for (int i=-halfW; i<halfW; i++) {
            int ii=rx+(*(aptr++));
            int jj=ry+(*(aptr++));
            if (ii<0) ii=0;
            if (jj<0) jj=0;
            if (ii>=image->width) ii=image->width-1;
            if (jj>=image->height) jj=image->height-1;
            int data=image->data(ii,jj);
            tmp[ptr]=data;
            if (mask->data(ptr++)) {
              v0+=i*data;
              v1+=j*data;
              v2+=data;
            }
          } 
        }
#else
        for (int j=-halfH; j<halfH; j++) {
          for (int i=-halfW; i<halfW; i++) {
            double ii=rx+(i+0.5)*cos(ra)-(j+0.5)*sin(ra)-0.5;
            double jj=ry+(i+0.5)*sin(ra)+(j+0.5)*cos(ra)-0.5;
            int data=(int)(0.5+image->interpolateExtend(ii,jj));
            tmp[ptr]=data;
            if (mask->data(ptr++)) {
              v0+=i*data;
              v1+=j*data;
              v2+=data;
            }
//            history0->data(i+10,j+10)=data;
          } 
        }
#endif

        vec(0)=v0; vec(1)=v1; vec(2)=v2;
        vec=LightingCorrectionMatrix*vec;
        v0=vec(0); v1=vec(1); v2=vec(2);
        for (int i=0; i<512; i++) hist[i]=0;
        ptr=0;
        for (j=-halfH; j<halfH; j++) for (i=-halfW; i<halfW; i++) {
          int val=tmp[ptr]-(int)(i*v0+j*v1+v2-256.5);
          if (val<0) val=0;
          if (val>=512) val=511;
          if (mask->data(ptr)) hist[val]++;
          tmp[ptr++]=val;
        }
        int *to=map;
        int *from=hist;
        int total=0;
        for (i=0; i<512; i++) {
          int old=total;
          total+=*(from++);
          *(to++)=old+total;
        }
        double scaleFactor=1.0/total;
        FastForwardUnit *unit=&(real_fast_list[1]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;
  
        int found=1;
        for (i=1; i<=2 && found; i++) {
          if (i>1) {
            FastForwardUnit *unit=&(real_fast_list[1]->unitList[1]);
            FastForwardUnit *unit1=&(real_fast_list[i]->unitList[1]);
            for (int j=0; j<mask->size; j++)
              (unit1++)->activation=(unit++)->activation;
          }
          double output=FastForwardPass(real_fast_list[i]);
//    fprintf(stderr,"net[%d]=%g\n",i,output);
          if (output<=0) found=0;
        }
#ifdef UsePosterLogging
        if (EnablePosterLogging) {
          VImage cand=VCreateImage(1,20,20,VUByteRepn);
          for (int y=0; y<20; y++) for (int x=0; x<20; x++) 
            VSetPixel(cand,0,y,x,
                      real_fast_list[1]->unitList[1+y*20+x].activation*127+
                      127);
          if (stepa==2 && stepx==1 && stepy==1)
            WriteVistaImageLogging(cand,"candc3"); else
              WriteVistaImageLogging(cand,"cand3");
          VDestroyImage(cand);
        }
#endif

        if (found) {
#ifdef UsePosterLogging
          if (EnablePosterLogging) {
            VImage cand=VCreateImage(1,20,20,VUByteRepn);
            for (int y=0; y<20; y++) for (int x=0; x<20; x++) 
              VSetPixel(cand,0,y,x,
                        real_fast_list[1]->unitList[1+y*20+x].activation*127+
                        127);
            WriteVistaImageLogging(cand,"cand4");
            VDestroyImage(cand);
          }
#endif
#if 0          
          *newX=rx;
          *newY=ry;
          *newA=ra;
//          history0->Change();
          delete[] tmp;
          return found;
#endif
          totX+=rx;
          totY+=ry;
          totDirX+=cos(ra);
          totDirY+=sin(ra);
          tot++;
        } 
      }
//  history0->Change();

  delete[] tmp;
  if (tot>0) { 
    *newX=totX/tot;
    *newY=totY/tot;
    *newA=atan2(totDirY,totDirX);
    return 1;
  }
    
  return 0;
}

int DuplicateFaceOrientation(double x1, double y1, double s1, double a1,
                             double x2, double y2, double s2, double a2)
{
  int dup=0;
  if (fabs(s1-s2)<=2) {
    x1/=pow(1.2,s2-s1);
    y1/=pow(1.2,s2-s1);
    if (sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))<=8) {
      double ang=(a1-a2)*180.0/M_PI;
      while (ang<-180) ang+=360;
      while (ang>180) ang-=360;
      if (fabs(ang)<30) 
        dup=1;
    }
  }
  return dup;
}

#ifndef NoVista
void TestFaceLightCorrection(VImage input, VImage output,
                             int net, int holdout)
{
  double err=0;
  int first=0;
  if (holdout!=0) first=VImageNBands(input)-holdout;
  for (int p=first; p<VImageNBands(input); p++) {
    double tot1=0, totsqr1=0, tot2=0, totsqr2=0;
    for (int y=0; y<VImageNRows(input); y++)
      for (int x=0; x<VImageNColumns(input); x++) {
        int val=VPixel(input,p,y,x,VUByte);
        tot1+=val;
        totsqr1+=val*val;
        val=VPixel(output,p,y,x,VUByte);
        tot2+=val;
        totsqr2+=val*val;
      }
    double avg1=tot1/(VImageNRows(input)*VImageNColumns(input));
    double dev1=sqrt(totsqr1/(VImageNRows(input)*VImageNColumns(input))-
                     avg1*avg1);
    double avg2=tot2/(VImageNRows(input)*VImageNColumns(input));
    double dev2=sqrt(totsqr2/(VImageNRows(input)*VImageNColumns(input))-
                     avg2*avg2);
    int ptr=0;
    for (y=0; y<VImageNRows(input); y++)
      for (int x=0; x<VImageNColumns(input); x++) 
        real_fast_list[net]->unitList[1+(ptr++)].activation=
          (VPixel(input,p,y,x,VUByte)-avg1)/dev1;
    FastForwardPass(real_fast_list[net]);
    ptr=0;
    for (y=0; y<VImageNRows(input); y++)
      for (int x=0; x<VImageNColumns(input); x++) {
        double out=
          real_fast_list[net]->unitList[1+ptr].activation+
          real_fast_list[net]->unitList[real_fast_list[net]->firstOutput+ptr].
          activation;
        double desired=(VPixel(output,p,y,x,VUByte)-avg2)/dev2;
        double diff=desired-out;
        err+=diff*diff;
        ptr++;
      }
  }
  fprintf(stderr,"average pixel error is %g\n",
          sqrt(err/(VImageNRows(input)*VImageNColumns(input)*
                    (VImageNBands(input)-first))));
}

VImage TestFaceLightCorrectionExample(VImage input, VImage output,
                                      VBand p, int net)
{
  VImage dest=VCreateImage(3,VImageNRows(input),VImageNColumns(input),
                           VFloatRepn);
  double tot1=0, totsqr1=0, tot2=0, totsqr2=0;
  for (int y=0; y<VImageNRows(input); y++)
    for (int x=0; x<VImageNColumns(input); x++) {
      int val=VPixel(input,p,y,x,VUByte);
      tot1+=val;
      totsqr1+=val*val;
      val=VPixel(output,p,y,x,VUByte);
      tot2+=val;
      totsqr2+=val*val;
    }
  double avg1=tot1/(VImageNRows(input)*VImageNColumns(input));
  double dev1=sqrt(totsqr1/(VImageNRows(input)*VImageNColumns(input))-
                   avg1*avg1);
  double avg2=tot2/(VImageNRows(input)*VImageNColumns(input));
  double dev2=sqrt(totsqr2/(VImageNRows(input)*VImageNColumns(input))-
                   avg2*avg2);
  int ptr=0;
  for (y=0; y<VImageNRows(input); y++)
    for (int x=0; x<VImageNColumns(input); x++) {
      float val=(VPixel(input,p,y,x,VUByte)-avg1)/dev1;
      real_fast_list[net]->unitList[1+(ptr++)].activation=val;
      VPixel(dest,0,y,x,VFloat)=val;
    }
  FastForwardPass(real_fast_list[net]);
  ptr=0;
  for (y=0; y<VImageNRows(input); y++)
    for (int x=0; x<VImageNColumns(input); x++) {
      float out=
        real_fast_list[net]->unitList[1+ptr].activation+
        real_fast_list[net]->unitList[real_fast_list[net]->firstOutput+ptr].
        activation;
      float desired=(VPixel(output,p,y,x,VUByte)-avg2)/dev2;
//      out=desired-real_fast_list[net]->unitList[ptr+1].activation;
      VPixel(dest,1,y,x,VFloat)=out;
      VPixel(dest,2,y,x,VFloat)=desired;
      ptr++;
    }
  return dest;
}

void TestFaceLightCorrection2(VImage input, VImage output,
                              int net, int holdout)
{
  double realerr=0, realerrsqr=0;
  int first=0;
  if (holdout!=0) first=VImageNBands(input)-holdout;
  for (int p=first; p<VImageNBands(input); p++) {
    int ptr=0;

    double tot2=0, totsqr2=0;
    for (int y=0; y<VImageNRows(input); y++)
      for (int x=0; x<VImageNColumns(input); x++) {
        double val=VPixel(output,p,y,x,VUByte);
          tot2+=val;
          totsqr2+=val*val;
      }
    double avg2=tot2/(VImageNRows(input)*VImageNColumns(input));
    double dev2=sqrt(totsqr2/(VImageNRows(input)*VImageNColumns(input))-
                     avg2*avg2);

    for (y=0; y<VImageNRows(input); y++)
      for (int x=0; x<VImageNColumns(input); x++) {
        int val=VPixel(input,p,y,x,VUByte);
        real_fast_list[net]->unitList[1+(ptr++)].activation=
          val/255.0;
      }
    FastForwardPass(real_fast_list[net]);
    ptr=0;
    for (y=0; y<VImageNRows(input); y++)
      for (int x=0; x<VImageNColumns(input); x++) {
        double out=real_fast_list[net]->unitList[real_fast_list[net]->firstOutput+ptr].activation;
        double desired=VPixel(output,p,y,x,VUByte);
        double diff=desired-out;
        realerr+=diff;
        realerrsqr+=diff*diff;
        ptr++;
      }
  }
  fprintf(stderr,"average pixel error is %g (%g)\n",
          (realerr/(VImageNRows(input)*VImageNColumns(input)*
                    (VImageNBands(input)-first))),
          sqrt(realerrsqr/(VImageNRows(input)*VImageNColumns(input)*
                           (VImageNBands(input)-first))));
}

VImage TestFaceLightCorrectionExample2(VImage input, VImage output,
                                       VBand p, int net)
{
  VImage dest=VCreateImage(3,VImageNRows(input),VImageNColumns(input),
                           VFloatRepn);
  int ptr=0;
  for (int y=0; y<VImageNRows(input); y++)
    for (int x=0; x<VImageNColumns(input); x++) {
      float val=VPixel(input,p,y,x,VUByte);
      real_fast_list[net]->unitList[1+(ptr++)].activation=val/255.0;
      VPixel(dest,0,y,x,VFloat)=val;
    }

  double tot2=0, totsqr2=0;
  for (y=0; y<VImageNRows(input); y++)
    for (int x=0; x<VImageNColumns(input); x++) {
      double val=VPixel(output,p,y,x,VUByte);
      tot2+=val;
      totsqr2+=val*val;
    }
  double avg2=tot2/(VImageNRows(input)*VImageNColumns(input));
  double dev2=sqrt(totsqr2/(VImageNRows(input)*VImageNColumns(input))-
                   avg2*avg2);

  FastForwardPass(real_fast_list[net]);
  ptr=0;
  for (y=0; y<VImageNRows(input); y++)
    for (int x=0; x<VImageNColumns(input); x++) {
      float out=real_fast_list[net]->unitList[real_fast_list[net]->firstOutput+ptr].activation;
      float desired=VPixel(output,p,y,x,VUByte);
      VPixel(dest,1,y,x,VFloat)=out;
      VPixel(dest,2,y,x,VFloat)=desired;
      ptr++;
    }
  return dest;
}

void TrainFaceLightCorrection(VImage input, VImage output, 
                              int net, int holdout, 
                              int numBatches, double rate, double momentum)
{
  int batchSize=100;
  FastMakeNetTrainable(real_fast_list[net],batchSize);
  int batches=(numBatches*(VImageNBands(input)-holdout))/batchSize;
  int pat=0;
  for (int b=0; b<batches; b++) {
    for (int p=0; p<batchSize; p++) {
      double tot1=0, totsqr1=0, tot2=0, totsqr2=0;
      for (int y=0; y<VImageNRows(input); y++)
        for (int x=0; x<VImageNColumns(input); x++) {
          int val=VPixel(input,pat,y,x,VUByte);
          tot1+=val;
          totsqr1+=val*val;
          val=VPixel(output,pat,y,x,VUByte);
          tot2+=val;
          totsqr2+=val*val;
        }
      double avg1=tot1/(VImageNRows(input)*VImageNColumns(input));
      double dev1=sqrt(totsqr1/(VImageNRows(input)*VImageNColumns(input))-
                       avg1*avg1);
      double avg2=tot2/(VImageNRows(input)*VImageNColumns(input));
      double dev2=sqrt(totsqr2/(VImageNRows(input)*VImageNColumns(input))-
                       avg2*avg2);
      int ptr=0;
      for (y=0; y<VImageNRows(input); y++)
        for (int x=0; x<VImageNColumns(input); x++) {
          double val1=(VPixel(input,pat,y,x,VUByte)-avg1)/dev1;
          double val2=(VPixel(output,pat,y,x,VUByte)-avg2)/dev2-val1;
          real_fast_list[net]->unitList[1+ptr].train->activations[p]=val1;
          real_fast_list[net]->unitList[real_fast_list[net]->firstOutput+ptr].
            train->delta[p]=val2;
          ptr++;
        }
      pat++;
      if (pat>=VImageNBands(input)-holdout) pat=0;
    }
    FastTrainBatch(real_fast_list[net],rate,momentum,0);
  }
}

void TrainFaceLightCorrection2(VImage input, VImage output, 
                               int net, int holdout, 
                               int numBatches, double rate, double momentum)
{
  int batchSize=100;
  FastMakeNetTrainable(real_fast_list[net],batchSize);
  int batches=(numBatches*(VImageNBands(input)-holdout))/batchSize;
  int pat=0;
  for (int b=0; b<batches; b++) {
    for (int p=0; p<batchSize; p++) {
      int ptr=0;
      double tot2=0, totsqr2=0;
      for (int y=0; y<VImageNRows(input); y++)
        for (int x=0; x<VImageNColumns(input); x++) {
          double val=VPixel(output,pat,y,x,VUByte);
          tot2+=val;
          totsqr2+=val*val;
        }
      double avg2=tot2/(VImageNRows(input)*VImageNColumns(input));
      double dev2=sqrt(totsqr2/(VImageNRows(input)*VImageNColumns(input))-
                       avg2*avg2);
      for (y=0; y<VImageNRows(input); y++)
        for (int x=0; x<VImageNColumns(input); x++) {
          double val1=VPixel(input,pat,y,x,VUByte);
          double val2=VPixel(output,pat,y,x,VUByte);
          real_fast_list[net]->unitList[1+ptr].train->activations[p]=
            val1/255.0;
          real_fast_list[net]->unitList[real_fast_list[net]->firstOutput+ptr].
            train->delta[p]=val2;
          ptr++;
        }
      pat++;
      if (pat>=VImageNBands(input)-holdout) pat=0;
    }
    FastTrainBatch(real_fast_list[net],rate,momentum,0);
  }
}
#endif

#ifndef NoTcl
int FindNewLocationOrientationCmd(ClientData data, Tcl_Interp *interp,
                                  int argc, char *argv[])
{
  if (argc!=5)
  {
    interp->result="Usage: FindNewLocationOrientation <image> <x> <y> <a>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  double x=atof(argv[2]);
  double y=atof(argv[3]);
//  double a=atof(argv[4])*M_PI/180.0;
  double a=atof(argv[4]);
  double newX, newY, newA;
  int found=FindNewLocationOrientation(image,GetFaceMask(),
                                       x,y,a,&newX,&newY,&newA);
  if (found) {
//    sprintf(interp->result,"%g %g %g",newX,newY,newA*180.0/M_PI);
    sprintf(interp->result,"%g %g %g",newX,newY,newA);
  } else {
    interp->result="";
  }
  return TCL_OK;
}
#endif

#ifndef NoVista
VImage *AllocateImageList(int size)
{
  return new VImage[size];
}

void SetImageListItem(VImage *list, int num, VImage item)
{
  list[num]=item;
}

struct FloatInt {
  float quality;
  int index;
};

int CompareFloatIntIncreasing(const void *p1, const void *p2);

int RecognitionDistance(VImage img1, VImage img2) 
{
  int total=0;
  for (int y=0; y<VImageNRows(img1); y++)
    for (int x=0; x<VImageNColumns(img1); x++) {
      int diff=abs((int)VGetPixel(img1,0,y,x)-(int)VGetPixel(img2,0,y,x));
      if (diff>=10) total++; else {
        fprintf(stderr,"Close pixel: %d,%d (%g vs %g)\n",
                x,y,VGetPixel(img1,0,y,x),VGetPixel(img2,0,y,x));
      }
    }
  return total;
}

int *Recognition(VImage *test, VImage *train, int size)
{
  int *results=new int[size];
  FloatInt *dist=new FloatInt[size];
  for (int i=0; i<size; i++) results[i]=0;
  for (i=0; i<size; i++) {
    for (int j=0; j<size; j++) {
      dist[j].index=j;
      dist[j].quality=RecognitionDistance(test[i],train[j]);
    }
    qsort(dist,size,sizeof(FloatInt),CompareFloatIntIncreasing);
    int right=size;
    for (j=0; j<size; j++) 
      if (dist[j].index==i) {
        right=j;
//        for (int k=j; k>=0; k--)
//          if (dist[k].quality==dist[j].quality) right=k;
      }
    for (j=0; j<size; j++) if (j>=right) results[j]++;
  }
  delete[] dist;
  return results;
}

void HistogramStretch(VImage img)
{
  int min=255, max=0;
  for (int y=0; y<VImageNRows(img); y++) 
    for (int x=0; x<VImageNColumns(img); x++) {
      int val=VPixel(img,0,y,x,VUByte);
      if (val<min) min=val;
      if (val>max) max=val;
    }
  for (y=0; y<VImageNRows(img); y++) 
    for (int x=0; x<VImageNColumns(img); x++) {
      int val=VPixel(img,0,y,x,VUByte);
      val=(255*(val-min))/(max-min);
      VPixel(img,0,y,x,VUByte)=val;
    }
}
#endif

#ifndef NoTcl
void Just_Init(Tcl_Interp *interp)
{
  Tcl_CreateCommand(interp,"SearchMultiOrientation",SearchMultiOrientationCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SearchMultiAllOrientation",
                    SearchMultiAllOrientationCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SearchPreciseOrientation",
                    SearchPreciseOrientationCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"FindNewLocationOrientation",
                    FindNewLocationOrientationCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
}
#endif

