#ifdef __GNUC__
#include <string.h>
#endif

#ifndef WINNT
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#ifndef hpux
#include <math.h>
#endif
#include <time.h>
#ifndef __GNUC__
#include <string.h>
#endif
#ifndef NoTcl
#include <tk.h>
#endif
#ifndef NoVista
#include <vista/VImage.h>
#endif
#ifndef WINNT
}
#endif

#ifdef hpux
#include <math.h>
#endif

#ifndef NoTcl
#include "hacks.hpp"
#endif
#include "img.hpp"
#include "myhebp.hpp"
#include "eye.hpp"
//#include "normface.hpp"
#include "system.hpp"
#include "mat.hpp"

#ifndef NoVista
VImage InputVectorsToXY(VImage src)
{
  VImage dest=VCreateImage(VImageNFrames(src),1,2,VFloatRepn);
  VFloat *ptr=(VFloat*)VPixelPtr(dest,0,0,0);
  for (int band=0; band<VImageNBands(src); band++) {
    double sum=0.0;
    double tot=0.0;
    for (int i=0; i<VImageNColumns(src); i++) {
      sum+=i*(VPixel(src,band,0,i,VFloat)+1.0);
      tot+=VPixel(src,band,0,i,VFloat)+1.0;
    }
    *(ptr++)=sum/tot;
  }
  return dest;
}

void GenerateInputVector(VImage dest, VBand band, double x)
{
  VFloat *ptr=(VFloat*)VPixelPtr(dest,band,0,0);
  for (int i=0; i<VImageNColumns(dest); i++)
    *(ptr++)=-1.0+2.0*exp(-(i-x)*(i-x)/2.0/10.0);
}

void SynthesizeEyeNetwork(VImage dest, VBand destband,
                          VImage src, VBand srcband,
                          int network)
{
  int inputsize=VImageNComponents(src)*
    VImageNRows(src)*VImageNColumns(src);
  FastForwardUnit *unit=&(real_fast_list[network]->unitList[1]);
  VFloat *ptr=(VFloat*)VPixelPtr(src,srcband,0,0);
  for (int i=0; i<inputsize; i++) (unit++)->activation=*(ptr++);

  FastForwardPass(real_fast_list[0]);

  int outputsize=VImageNComponents(dest)*
    VImageNRows(dest)*VImageNColumns(dest);
  ptr=(VFloat*)VPixelPtr(dest,destband,0,0);
  unit=&(real_fast_list[network]->unitList[
    real_fast_list[network]->firstOutput]);
  for (i=0; i<outputsize; i++) *(ptr++)=(unit++)->activation;
}

void SynthesizeEyeClosest(VImage dest, VBand destband,
                          VImage eyes, VImage xy,
                          double x, double y)
{
  double dist=-1.0;
  int example=-1;
  for (int i=0; i<VImageNBands(xy); i++) {
    double thisdist=sqrt((x-VPixel(xy,i,0,0,VFloat))*
                         (x-VPixel(xy,i,0,0,VFloat))+
                         (y-VPixel(xy,i,0,1,VFloat))*
                         (y-VPixel(xy,i,0,1,VFloat)));
    if (example==-1 || thisdist<dist) {
      dist=thisdist;
      example=i;
    }
  }
  int outputsize=VImageNComponents(dest)*
    VImageNRows(dest)*VImageNColumns(dest);
  VFloat *ptr=(VFloat*)VPixelPtr(dest,destband,0,0);
  VUByte *iptr=(VUByte*)VPixelPtr(eyes,example*VImageNComponents(eyes),0,0);
  for (i=0; i<outputsize; i++) 
    *(ptr++)=(*(iptr++))/127.5-1.0;
}

void SynthesizeEyeClosestInterpolate(VImage dest, VBand destband,
                                     VImage eyes, VImage xy,
                                     double x, double y)
{
  double distUL=0, distUR=0, distLL=0, distLR=0;
  int exampleUL=-1, exampleUR=-1, exampleLL=-1, exampleLR=-1;
  for (int i=0; i<VImageNBands(xy); i++) {
    double thisx=VPixel(xy,i,0,0,VFloat);
    double thisy=VPixel(xy,i,0,1,VFloat);
    double thisdist=sqrt((x-thisx)*(x-thisx)+(y-thisy)*(y-thisy));
    if (thisx<=x && thisy<=y) {
      if (exampleUL==-1 || thisdist<distUL) {
        distUL=thisdist;
        exampleUL=i;
      }
    }
    if (thisx<=x && thisy>=y) {
      if (exampleLL==-1 || thisdist<distLL) {
        distLL=thisdist;
        exampleLL=i;
      }
    }
    if (thisx>=x && thisy>=y) {
      if (exampleLR==-1 || thisdist<distLR) {
        distLR=thisdist;
        exampleLR=i;
      }
    }
    if (thisx>=x && thisy<=y) {
      if (exampleUR==-1 || thisdist<distUR) {
        distUR=thisdist;
        exampleUR=i;
      }
    }
  }
  for (i=0; i<4; i++) {
    if (exampleUL!=-1 && exampleUR==-1) exampleUR=exampleUL;
    if (exampleUL!=-1 && exampleLL==-1) exampleLL=exampleUL;

    if (exampleLL!=-1 && exampleUL==-1) exampleUL=exampleLL;
    if (exampleLL!=-1 && exampleLR==-1) exampleLR=exampleLL;
    
    if (exampleLR!=-1 && exampleLL==-1) exampleLL=exampleLR;
    if (exampleLR!=-1 && exampleUR==-1) exampleUR=exampleLR;
    
    if (exampleUR!=-1 && exampleUL==-1) exampleUL=exampleUR;
    if (exampleUR!=-1 && exampleLR==-1) exampleLR=exampleUR;
  }
  double left=VPixel(xy,exampleUL,0,0,VFloat);
  double right=VPixel(xy,exampleUR,0,0,VFloat);
  double top=VPixel(xy,exampleUL,0,1,VFloat);
  double bottom=VPixel(xy,exampleLL,0,1,VFloat);
  double fracLeft=(left==right)?0.5:(right-x)/(right-left);
  double fracTop=(top==bottom)?0.5:(bottom-y)/(bottom-top);
  int outputsize=VImageNComponents(dest)*
    VImageNRows(dest)*VImageNColumns(dest);
  VFloat *ptr=(VFloat*)VPixelPtr(dest,destband,0,0);
  VUByte *ul=(VUByte*)VPixelPtr(eyes,exampleUL*VImageNComponents(eyes),0,0);
  VUByte *ur=(VUByte*)VPixelPtr(eyes,exampleUR*VImageNComponents(eyes),0,0);
  VUByte *ll=(VUByte*)VPixelPtr(eyes,exampleLL*VImageNComponents(eyes),0,0);
  VUByte *lr=(VUByte*)VPixelPtr(eyes,exampleLR*VImageNComponents(eyes),0,0);
  for (i=0; i<outputsize; i++) {
    *(ptr++)=((*(ul++))*fracLeft*fracTop+
              (*(ur++))*(1-fracLeft)*fracTop+
              (*(ll++))*fracLeft*(1-fracTop)+
              (*(lr++))*(1-fracLeft)*(1-fracTop))/127.5-1.0;
  }
}
#endif

struct FloatInt {
  float quality;
  int index;
};

int CompareFloatIntIncreasing(const void *p1, const void *p2);

#ifndef NoVista
void SynthesizeEyeClosestInterpolate2(VImage dest, VBand destband,
                                      VImage eyes, VImage xy,
                                      double x, double y)
{
  int num=VImageNBands(xy);
  FloatInt *dists=new FloatInt[num];
  for (int i=0; i<VImageNBands(xy); i++) {
    double thisx=VPixel(xy,i,0,0,VFloat);
    double thisy=VPixel(xy,i,0,1,VFloat);
    double thisdist=sqrt((x-thisx)*(x-thisx)+(y-thisy)*(y-thisy));
    dists[i].quality=thisdist;
    dists[i].index=i;
  }
  qsort(dists,num,sizeof(FloatInt),CompareFloatIntIncreasing);
  Vec a(2), b(2), c(2), pt(2);
  pt(0)=x; pt(1)=y; 
  a(0)=VPixel(xy,dists[0].index,0,0,VFloat);
  a(1)=VPixel(xy,dists[0].index,0,1,VFloat);
  b(0)=VPixel(xy,dists[1].index,0,0,VFloat);
  b(1)=VPixel(xy,dists[1].index,0,1,VFloat);
  c(0)=VPixel(xy,dists[2].index,0,0,VFloat);
  c(1)=VPixel(xy,dists[2].index,0,1,VFloat);
  double bcomp=Dot(pt-a,Normalize(b-a))/Abs(b-a);
  double ccomp=Dot(pt-a,Normalize(c-a))/Abs(c-a);
  bcomp=0;
  ccomp=0;
  double acomp=1.0-bcomp-ccomp;
  int outputsize=VImageNComponents(dest)*
    VImageNRows(dest)*VImageNColumns(dest);
  VFloat *ptr=(VFloat*)VPixelPtr(dest,destband,0,0);
  VUByte *aptr=(VUByte*)VPixelPtr(eyes,dists[0].index*VImageNComponents(eyes),
                                  0,0);
  VUByte *bptr=(VUByte*)VPixelPtr(eyes,dists[1].index*VImageNComponents(eyes),
                                  0,0);
  VUByte *cptr=(VUByte*)VPixelPtr(eyes,dists[2].index*VImageNComponents(eyes),
                                  0,0);
  for (i=0; i<outputsize; i++) {
    *(ptr++)=((*(aptr++))*acomp+
              (*(bptr++))*bcomp+
              (*(cptr++))*ccomp);
  }
}

void PermuteExamples(VImage inputs, VImage outputs)
{
  int incomp=VImageNComponents(inputs);
  int outcomp=VImageNComponents(outputs);
  VImage tmpin=VCreateImage(1,
                            VImageNRows(inputs),
                            VImageNColumns(inputs),
                            VPixelRepn(inputs));
  VImage tmpout=VCreateImage(1,
                             VImageNRows(outputs),
                             VImageNColumns(outputs),
                             VPixelRepn(outputs));
  srandom(time(NULL));
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
  }
  VDestroyImage(tmpin);
  VDestroyImage(tmpout);
}

// USED
// Permute the given training examples
void PermuteExamplesSingle(VImage inputs)
{
  int incomp=VImageNComponents(inputs);
  VImage tmpin=VCreateImage(1,
                            VImageNRows(inputs),
                            VImageNColumns(inputs),
                            VPixelRepn(inputs));
  srandom(time(NULL));
  for (int i=0; i<VImageNFrames(inputs); i++) {
    // For each example, compute a new location for it in the list,
    // and swap the two examples
    int j=random() % VImageNFrames(inputs);
    for (int c=0; c<incomp; c++) {
      VCopyBand(inputs,i*incomp+c,tmpin,0);
      VCopyBand(inputs,j*incomp+c,inputs,i*incomp+c);
      VCopyBand(tmpin,0,inputs,j*incomp+c);
    }
  }
  VDestroyImage(tmpin);
}

void TrainNetwork(VImage inputs, VImage outputs, int network,
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
      VFloat *ptr=(VFloat*)VPixelPtr(inputs,incomp*pattern,0,0);
      for (int j=0; j<inputsize; j++)
        (unit++)->train->activations[i]=*(ptr++);
      unit=real_fast_list[network]->unitList+
        real_fast_list[network]->firstOutput;
      VUByte *iptr=(VUByte*)VPixelPtr(outputs,outcomp*pattern,0,0);
      for (j=0; j<outputsize; j++)
        (unit++)->train->delta[i]=(*(iptr++))/127.5-1.0;
      pattern=(pattern+1)%(numPatterns-holdout);
    }
    FastTrainBatch(real_fast_list[network],rate,momentum,decay);
  }
}

double TestNetwork(VImage inputs, VImage outputs, int network)
{
  int incomp=VImageNComponents(inputs);
  int outcomp=VImageNComponents(outputs);
  int inputsize=incomp*VImageNRows(inputs)*VImageNColumns(inputs);
  int outputsize=outcomp*VImageNRows(outputs)*VImageNColumns(outputs);
  int numPatterns=VImageNFrames(inputs);
  double error=0.0;
  for (int pattern=0; pattern<numPatterns; pattern++) {
    FastForwardUnit *unit=real_fast_list[0]->unitList+
      real_fast_list[0]->firstInput;
    VFloat *ptr=(VFloat*)VPixelPtr(inputs,incomp*pattern,0,0);
    for (int j=0; j<inputsize; j++)
      (unit++)->activation=*(ptr++);
    FastForwardPass(real_fast_list[network]);
    unit=real_fast_list[0]->unitList+
      real_fast_list[0]->firstOutput;
    VUByte *iptr=(VUByte*)VPixelPtr(outputs,outcomp*pattern,0,0);
    for (j=0; j<outputsize; j++) {
      double diff=(unit++)->activation-(*(iptr++)/127.5-1.0);
      error+=diff*diff;
    }
  }
  return sqrt(error/(numPatterns*outputsize));
}

VImage AverageImage(VImage src)
{
  VImage dest=VCreateImage(1,VImageNRows(src),VImageNColumns(src),VFloatRepn);
  for (int y=0; y<VImageNRows(src); y++) 
    for (int x=0; x<VImageNColumns(src); x++) {
      double tot=0;
      for (int b=0; b<VImageNBands(src); b++) 
        tot+=VPixel(src,b,y,x,VFloat);
      VPixel(dest,0,y,x,VFloat)=tot/VImageNBands(src);
    }
  return dest;
}
#endif

/*
VImage ExtractEye(VImage src, int width, int height,
                  double x1, double y1,
                  double x2, double y2,
                  double x3, double y3,
                  double x1a, double y1a,
                  double x2a, double y2a,
                  double x3a, double y3a)
{
  VImage dest=VCreateImage(1,height,width,VFloatRepn);
  VFillImage(dest,VAllBands,0);
  if (x1==x2 && y1==y2) return dest;
  if (x1a==x2a && y1a==y2a) return dest;
  double aa=atan2(y2a-y1a,x2a-x1a);
  double a=atan2(y2-y1,x2-x1);
  double da=sqrt((y2a-y1a)*(y2a-y1a)+(x2a-x1a)*(x2a-x1a));
  double d=sqrt((y2-y1)*(y2-y1)+(x2-x1)*(x2-x1));
  for (int y=0; y<height; y++)
    for (int x=0; x<width; x++) {
      double xt1=(x-(width-1)*0.5+x3a-x1a)*d/da;
      double yt1=(y-(height-1)*0.5+y3a-y1a)*d/da;
      double xt2=xt1*cos(a-aa)-yt1*sin(a-aa)+x1;
      double yt2=xt1*sin(a-aa)+yt1*cos(a-aa)+y1;
      VPixel(dest,0,y,x,VFloat)=127.5*InterpolatePixel(src,0,xt2,yt2)-1.0;
    }
  return dest;
}
*/

#ifndef NoTcl
int ComputeEyeCmd(ClientData data, Tcl_Interp *interp,
                  int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: ComputeEye <image> <x> <y>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  double mean1=atof(argv[2]);
  double mean2=atof(argv[3]);
  FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
  for (int i=0; i<50; i++)
    (unit++)->activation=-1.0+2.0*exp(-(i-mean1)*(i-mean1)/2.0/10.0);
  for (i=0; i<50; i++)
    (unit++)->activation=-1.0+2.0*exp(-(i-mean2)*(i-mean2)/2.0/10.0);
  FastForwardPass(real_fast_list[0]);
  Byte *buffer=image->AllocBuffer(40,15);
  Byte *ptr=buffer;
  unit=&(real_fast_list[0]->unitList[real_fast_list[0]->firstOutput]);
  for (i=0; i<40*15; i++)
  {
    int val=(int)(floor(0.5+0.5*((unit++)->activation+1.0)*255.0));
    if (val<0) val=0;
    if (val>255) val=255;
    *(ptr++)=val;
  }
  image->NewBuffer(buffer,40,15);
  return TCL_OK;
}
#endif

#ifndef NoTcl
int ZoomEyeCmd(ClientData data, Tcl_Interp *interp,
               int argc, char *argv[])
{
  if (argc!=9)
  {
    interp->result="Usage: ZoomEye <dest> <src> <x1,y1> <x2,y2> <xm,ym>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  double xe1=atof(argv[3]);
  double ye1=atof(argv[4]);
  double xe2=atof(argv[5]);
  double ye2=atof(argv[6]);
  double xm=atof(argv[7]);
  double ym=atof(argv[8]);

  double xe=(xe1+xe2)/2.0;
  double ye=(ye1+ye2)/2.0;
  double scale=16.5/(sqrt((xe-xm)*(xe-xm)+(ye-ym)*(ye-ym)));
//  double scale=16.0/(sqrt((xe1-xe2)*(xe1-xe2)+(ye1-ye2)*(ye1-ye2)));

  if (scale>=1)
  {
    interp->result="";
    return TCL_OK;
  }

  int inc=(int)floor(1.0/scale);
  Byte *newdata=(Byte*)malloc(sizeof(Byte)*(src->width/inc)*
                              (src->height/inc));
  for (int x=0; x<src->width/inc; x++)
    for (int y=0; y<src->height/inc; y++)
    {
      double tot=0;
      for (int i=x*inc; i<(x+1)*inc; i++)
        for (int j=y*inc; j<(y+1)*inc; j++)
          tot+=src->data(i,j);
      newdata[x+y*(src->width/inc)]=(int)floor(0.5+tot/(inc*inc));
    }
  image->NewBuffer(newdata,src->width/inc,src->height/inc);
  sprintf(interp->result,"%g %g %g %g %g %g",xe1/inc,ye1/inc,xe2/inc,ye2/inc,
          xm/inc,ym/inc);
  return TCL_OK;
}
#endif

#ifndef NoTcl
int RandEyeCmd(ClientData data, Tcl_Interp *interp,
               int argc, char *argv[])
{
  if (argc!=10)
  {
    interp->result="Usage: RandEye <dest> <src> <xe1,ye1> <xe2,ye2> <xm,ym> <left|right>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  double xe1=atof(argv[3]);
  double ye1=atof(argv[4]);
  double xe2=atof(argv[5]);
  double ye2=atof(argv[6]);
  double xm=atof(argv[7]);
  double ym=atof(argv[8]);

  char *which=argv[9];

  double dx=RandomDouble()-0.5;
  double dy=RandomDouble()-0.5;
  
  double xe=(xe1+xe2)/2.0;
  double ye=(ye1+ye2)/2.0;
  double scale=(16.0+(RandomDouble()-0.5))/
    (sqrt((xe-xm)*(xe-xm)+(ye-ym)*(ye-ym)));
//  double scale=(16.0+(RandomDouble()-0.5))/
//    (sqrt((xe1-xe2)*(xe1-xe2)+(ye1-ye2)*(ye1-ye2)));

  double angle=atan2(ye2-ye1,xe2-xe1)+(RandomDouble()*10.0-5.0)*M_PI/180.0;
  int flip=(strcmp(which,"right")==0);

  SetEyeMask();
  Byte *newdata=(Byte*)malloc(EyeMask->size*sizeof(Byte));
  
  for (int i=0; i<EyeMask->width; i++)
    for (int j=0; j<EyeMask->height; j++)
      if (EyeMask->data(i,j))
      {
        double x=(i-EyeMask->width*0.5+dx)/scale;
        double y=(j-EyeMask->height*0.5+dy)/scale;
        double rx, ry;
        if (!flip)
        {
          rx=xe1+(cos(angle)*x-sin(angle)*y);
          ry=ye1+(sin(angle)*x+cos(angle)*y);
        } else
        {
          rx=xe2-(cos(-angle)*x-sin(-angle)*y);
          ry=ye2+(sin(-angle)*x+cos(-angle)*y);
        }
        newdata[i+j*EyeMask->width]=(int)floor(0.5+src->interpolate(rx,ry));
      } else
        newdata[i+j*EyeMask->width]=0;

  image->NewBuffer(newdata,EyeMask->width,EyeMask->height);
  return TCL_OK;
}
#endif

#ifndef NoTcl
// USED
// 
int RandEye2Cmd(ClientData data, Tcl_Interp *interp,
                int argc, char *argv[])
{
  if (argc!=14) {
    interp->result="Usage: RandEye2 <dest> <src> <xe1,ye1> <xe2,ye2> <xm,ym> <left|right> <xs> <ys> <size> <rand>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  double xe1=atof(argv[3]);
  double ye1=atof(argv[4]);
  double xe2=atof(argv[5]);
  double ye2=atof(argv[6]);
  double xm=atof(argv[7]);
  double ym=atof(argv[8]);
  char *which=argv[9];
  int xs=atoi(argv[10]);
  int ys=atoi(argv[11]);
  int size=atoi(argv[12]);
  int rand=atoi(argv[13]);

  double dx=rand*(RandomDouble()-0.5);
  double dy=rand*(RandomDouble()-0.5);
  
  double xe=(xe1+xe2)/2.0;
  double ye=(ye1+ye2)/2.0;
  double scale=size/sqrt((xe-xm)*(xe-xm)+(ye-ym)*(ye-ym));

  interp->result="1";
  if (scale>1) interp->result="0";

  if (rand) scale*=(RandomDouble()*0.2+0.9);

  int reduce=(int)floor(1.0/scale);
  if (reduce>=1) {
    SubSampleWithAveraging(image,src,reduce);
    src=image;
    scale*=reduce;
    xe1/=reduce;
    ye1/=reduce;
    xe2/=reduce;
    ye2/=reduce;
  }

  double angle=atan2(ye2-ye1,xe2-xe1)+
    rand*(RandomDouble()*10.0-5.0)*M_PI/180.0;
  int flip=(strcmp(which,"right")==0);

  Byte *newdata=(Byte*)malloc(xs*ys);
  
  for (int i=0; i<xs; i++)
    for (int j=0; j<ys; j++)
    {
      double x=(i-xs*0.5+dx)/scale;
      double y=(j-ys*0.5+dy)/scale;
      double rx, ry;
      if (!flip)
      {
        rx=xe1+(cos(angle)*x-sin(angle)*y);
        ry=ye1+(sin(angle)*x+cos(angle)*y);
      } else
      {
        rx=xe2-(cos(-angle)*x-sin(-angle)*y);
        ry=ye2+(sin(-angle)*x+cos(-angle)*y);
      }
      newdata[i+j*xs]=(int)floor(0.5+src->interpolate(rx,ry));
    }

  image->NewBuffer(newdata,xs,ys);
  return TCL_OK;
}
#endif

#ifndef NoTcl
int NormEyeCmd(ClientData data, Tcl_Interp *interp,
               int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: NormEye <eye>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);

  SetEyeMask();
  int hist[256], map[256];

  for (int k=0; k<256; k++) hist[k]=0;
  
  for (int i=0; i<EyeMask->width; i++)
    for (int j=0; j<EyeMask->height; j++)
      if (EyeMask->data(i,j))
        hist[image->data(i,j)]++;

  int tot=0;
  for (k=0; k<256; k++)
  {
    map[k]=tot;
    tot+=hist[k];
  }
  int num=tot;
  for (k=255; k>=0; k--)
  {
    map[k]=(int)floor(0.5+254.0/(2.0*num)*(map[k]+tot));
    tot-=hist[k];
  }

  for (i=0; i<EyeMask->width; i++)
    for (int j=0; j<EyeMask->height; j++)
      if (EyeMask->data(i,j))
        image->data(i,j)=map[image->data(i,j)];
  
  image->Change();
  return TCL_OK;
}
#endif

// USED
// Search for the eyes in the face.  Inputs are the image pyramid, 
// the face location and scale.  Returns with the two eye positions.
// If the return value is 0, then the face is considered to be too 
// small for the eye detector to be effective.  If the function returns
// 1, then the eye detector has been run.  However, in this case the
// x and y positions being set to -1 indicate that no eye was detected.
int SearchEyes(int numScales, ByteImage *imagePyramid[], ByteImage *mask,
               int xx, int yy, int s, int basenet,
               int *eyex1, int *eyey1, int *eyex2, int *eyey2)
{
  // Figure out the scale at which to run the eye detector 
  // (the "7" was determined empirically)
  int scale=s-7;
  if (scale<0 && scale>=-3) scale=0;
  if (scale>=0)
  {
    ByteImage *image=imagePyramid[scale];
    double x1=0, y1=0, n1=0, x2=0, y2=0, n2=0;

    int *tmp=new int[mask->size];      // Window for eye detector

    // Possible upper-left X positions for the left eye
    int startx1=(int)floor(0.5+xx*pow(1.2,s-scale))-mask->width/2;
    int endx1=(int)floor(0.5+(xx+10)*pow(1.2,s-scale))-mask->width/2;
    if (startx1<0) startx1=0;
    if (endx1>=image->width-mask->width+1) endx1=image->width-mask->width+1;

    // Postible upper-left X positions for the right eye
    int startx2=(int)floor(0.5+(xx+10)*pow(1.2,s-scale))-mask->width/2;
    int endx2=(int)floor(0.5+(xx+20)*pow(1.2,s-scale))-mask->width/2;
    if (startx2<0) startx2=0;
    if (endx2>=image->width-mask->width+1) endx2=image->width-mask->width+1;

    // Possible upper-left Y positions for the eyes
    int starty=(int)floor(0.5+yy*pow(1.2,s-scale))-mask->height/2;
    int endy=(int)floor(0.5+(yy+10)*pow(1.2,s-scale))-mask->height;
    if (starty<0) starty=0;
    if (endy>=image->height-mask->height+1) endy=image->height-mask->height+1;

    for (int y=starty; y<endy; y++) {

      // Look for right eye on this scan line
      for (int x=startx2; x<endx2; x++) {
        int ptr=0;
        int hist[256];
        for (int i=0; i<256; i++) hist[i]=0;

	// Copy the window into tmp (using mirror image), and compute
	// the histogram over the entire window
        for (int j=0; j<mask->height; j++) for (i=mask->width-1; i>=0; i--) {
          int data=image->data(i+x,j+y);
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

	// Apply the histogram equalization, and write window to network inputs
        double scaleFactor=1.0/tot;
        FastForwardUnit *unit=&(real_fast_list[basenet]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;

	// If the network responds positively, add the detection to centroid
        double output=FastForwardPass(real_fast_list[basenet]);
        if (output>0) {
          n2+=output;
          x2+=output*(x+mask->width/2)*pow(1.2,scale);
          y2+=output*(y+mask->height/2)*pow(1.2,scale);
          /*
	    char name[1024];
	    sprintf(name,".t.c create line %g %g %g %g -fill yellow -tag face",
	    (x+mask->width/2)*pow(1.2,scale),
	    (y+mask->height/2)*pow(1.2,scale),
	    (x+mask->width/2)*pow(1.2,scale)+1,
	    (y+mask->height/2)*pow(1.2,scale));
	    Tcl_Eval(interp,name); 
	    */
        }
      }

      // Look for left eye in this scan line
      for (x=startx1; x<endx1; x++) {
        int ptr=0;
        int hist[256];
        for (int i=0; i<256; i++) hist[i]=0;

	// Copy the window into tmp (using mirror image), and compute
	// the histogram over the entire window
        for (int j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) {
          int data=image->data(i+x,j+y);
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

	// Apply the histogram equalization, and write window to network inputs
        double scaleFactor=1.0/tot;
        FastForwardUnit *unit=&(real_fast_list[basenet]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;

	// If the network responds positively, add the detection to centroid
        double output=FastForwardPass(real_fast_list[basenet]);
        if (output>0) {
          n1+=output;
          x1+=output*(x+mask->width/2)*pow(1.2,scale);
          y1+=output*(y+mask->height/2)*pow(1.2,scale);
          /*
	    char name[1024];
	    sprintf(name,".t.c create line %g %g %g %g -fill yellow -tag face",
	    (x+mask->width/2)*pow(1.2,scale),
	    (y+mask->height/2)*pow(1.2,scale),
	    (x+mask->width/2)*pow(1.2,scale)+1,
	    (y+mask->height/2)*pow(1.2,scale));
	    Tcl_Eval(interp,name);
	    */
        }
      }
    }
    // If the left eye was detected at least once, return centroid
    if (n1>0) {
      *eyex1=(int)floor(0.5+x1/n1); *eyey1=(int)floor(0.5+y1/n1);
    } else {
      *eyex1=-1; *eyey1=-1;
    }

    // If the right eye was detected at least once, return centroid
    if (n2>0) {
      *eyex2=(int)floor(0.5+x2/n2); *eyey2=(int)floor(0.5+y2/n2);
    } else {
      *eyex2=-1; *eyey2=-1;
    }
    delete[] tmp;
    return 1;
  } 

  // Return fact that face was too small
  *eyex1=-1; *eyey1=-1;
  *eyex2=-1; *eyey2=-1;
  return 0;
}

#ifdef CompiledNets
extern "C" {
void eye_net(int *inputs, float *outputs);
}

int SearchEyesCompiled(int numScales, ByteImage *imagePyramid[],
                       ByteImage *mask,
                       int xx, int yy, int s, int basenet,
                       int *eyex1, int *eyey1, int *eyex2, int *eyey2)
{
  int scale=s-7;
  if (scale<0 && scale>=-3) scale=0;
  if (scale>=0)
  {
    double x1=0, y1=0, n1=0, x2=0, y2=0, n2=0;

    int *tmp=new int[mask->size];
    int startx1=(int)floor(0.5+xx*pow(1.2,s-scale))-mask->width/2;
    int endx1=(int)floor(0.5+(xx+10)*pow(1.2,s-scale))-mask->width/2;
    int startx2=(int)floor(0.5+(xx+10)*pow(1.2,s-scale))-mask->width/2;
    int endx2=(int)floor(0.5+(xx+20)*pow(1.2,s-scale))-mask->width/2;
    int starty=(int)floor(0.5+yy*pow(1.2,s-scale))-mask->height/2;
    int endy=(int)floor(0.5+(yy+10)*pow(1.2,s-scale))-mask->height;
    ByteImage *image=imagePyramid[scale];
    if (startx1<0) startx1=0;
    if (endx1>=image->width-mask->width+1) endx1=image->width-mask->width+1;
    if (startx2<0) startx2=0;
    if (endx2>=image->width-mask->width+1) endx2=image->width-mask->width+1;
    if (starty<0) starty=0;
    if (endy>=image->height-mask->height+1) endy=image->height-mask->height+1;

    for (int y=starty; y<endy; y++)
    {
      {for (int x=startx2; x<endx2; x++)
      {
        int ptr=0;
        int hist[256];
        for (int i=0; i<256; i++) hist[i]=0;
        for (int j=0; j<mask->height; j++) for (i=mask->width-1; i>=0; i--)
        {
          int data=image->data(i+x,j+y);
          tmp[ptr++]=data;
          hist[data]++;
        }
        int map[256];
        int tot=0;
        int *to=map;
        int *from=hist;
        for (i=0; i<256; i++) {
          int old=tot;
          tot+=*(from++);
          *(to++)=old+tot;
        }
        from=tmp;
        for (i=0; i<mask->size; i++) {
          *from=map[*from];
          from++;
        }
        float output[1];
        eye_net(tmp,output);
        if (output[0]>0)
        {
          n2+=output[0];
          x2+=output[0]*(x+mask->width/2)*pow(1.2,scale);
          y2+=output[0]*(y+mask->height/2)*pow(1.2,scale);
          /*          char name[1024];
          sprintf(name,".t.c create line %g %g %g %g -fill yellow -tag face",
                  (x+mask->width/2)*pow(1.2,scale),
                  (y+mask->height/2)*pow(1.2,scale),
                  (x+mask->width/2)*pow(1.2,scale)+1,
                  (y+mask->height/2)*pow(1.2,scale));
          Tcl_Eval(interp,name); */
        }
      }}

      {for (int x=startx1; x<endx1; x++)
      {
        int ptr=0;
        int hist[256];
        for (int i=0; i<256; i++) hist[i]=0;
        for (int j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
        {
          int data=image->data(i+x,j+y);
          tmp[ptr++]=data;
          hist[data]++;
        }
        int map[256];
        int tot=-375;
        int *to=map;
        int *from=hist;
        for (i=0; i<256; i++) {
          int old=tot;
          tot+=*(from++);
          *(to++)=old+tot;
        }
        from=tmp;
        for (i=0; i<mask->size; i++) {
          *from=map[*from];
          from++;
        }
        float output[1];
        eye_net(tmp,output);
        if (output[0]>0)
        {
          n1+=output[0];
          x1+=output[0]*(x+mask->width/2)*pow(1.2,scale);
          y1+=output[0]*(y+mask->height/2)*pow(1.2,scale);
          /*          char name[1024];
          sprintf(name,".t.c create line %g %g %g %g -fill yellow -tag face",
                  (x+mask->width/2)*pow(1.2,scale),
                  (y+mask->height/2)*pow(1.2,scale),
                  (x+mask->width/2)*pow(1.2,scale)+1,
                  (y+mask->height/2)*pow(1.2,scale));
          Tcl_Eval(interp,name); */
        }
      }
      }
    }
    if (n1>0) {
      *eyex1=(int)floor(0.5+x1/n1); *eyey1=(int)floor(0.5+y1/n1);
    } else {
      *eyex1=-1; *eyey1=-1;
    }
    if (n2>0) {
      *eyex2=(int)floor(0.5+x2/n2); *eyey2=(int)floor(0.5+y2/n2);
    } else {
      *eyex2=-1; *eyey2=-1;
    }
    delete[] tmp;
    return 1;
  } 
  *eyex1=-1; *eyey1=-1;
  *eyex2=-1; *eyey2=-1;
  return 0;
}
#endif

int SearchLeftEye(ByteImage *image, ByteImage *mask, double eyewidth,
                  int *xeye, int *yeye, int leftEye)
{
  ByteImage *myimage=new ByteImage(NULL);
  CopyByteImage(myimage,image);
  if (!leftEye) MirrorByteImage(myimage);
  int centerScale=(int)(floor(0.5+log(eyewidth/22.0)/log(1.2)));
  int lowScale=centerScale-2;
  int highScale=centerScale+2;
  if (lowScale<0) lowScale=0;
  if (highScale<0) highScale=0;
//  lowScale=0;
//  highScale=100;
  double total=0;
  double totalX=0.0;
  double totalY=0.0;
  int *tmp=new int[mask->size];
  for (int scale=0; scale<=highScale &&
                      myimage->width>=mask->width &&
                      myimage->height>=mask->height;
                    scale++) {
    if (scale>=lowScale) {
      for (int y=0; y<myimage->height-mask->height+1; y++)
        for (int x=0; x<myimage->width-mask->width+1; x++) {
          int ptr=0;
          int hist[256];
          for (int i=0; i<256; i++) hist[i]=0;
          for (int j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) {
            int data=myimage->data(i+x,j+y);
            tmp[ptr++]=data;
            hist[data]++;
          }
          int map[256];
          int tot=0;
          int *to=map;
          int *from=hist;
          for (i=0; i<256; i++) {
            int old=tot;
            tot+=*(from++);
            *(to++)=old+tot;
          }
          double scaleFactor=1.0/tot;
          FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
          for (i=0; i<mask->size; i++)
            (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;
        
          double output=FastForwardPass(real_fast_list[0]);
          if (output>0) {
/*            fprintf(stderr,"(%g,%g/%d,%d,%d)=%g\n",
                    (x+mask->width/2)*pow(1.2,scale),
                    (y+mask->height/2)*pow(1.2,scale),
                    x+mask->width/2,y+mask->height/2,scale,
                    output); */
            total+=output;
            if (leftEye) {
              totalX+=output*(x+mask->width/2)*pow(1.2,scale);
            } else {
              totalX+=output*(myimage->width-1-(x+mask->width/2))*
                pow(1.2,scale);
            }
            totalY+=output*(y+mask->height/2)*pow(1.2,scale);
          }
        }
    }
    ReduceSize(myimage,myimage);
  }
  if (total>0) {
    *xeye=(int)floor(0.5+totalX/total); *yeye=(int)floor(0.5+totalY/total);
    delete[] tmp;
    delete myimage;
    return 1;
  }
  *xeye=-1; *yeye=-1;
  delete[] tmp;
  delete myimage;
  return 0;
}

#ifndef NoTcl
int FindEyeLocationsCmd(ClientData data, Tcl_Interp *interp,
                        int argc, char *argv[])
{
  if (argc!=8)
  {
    interp->result="Usage: FindEyeLocations <prefix> <numscales> <mask> <x> <y> <s> <basenet>";
    return TCL_ERROR;
  }
  char *prefix=argv[1];
  int numScales=atoi(argv[2]);
  ByteImage *mask=FindByteImageWithName(argv[3]);
  int x=atoi(argv[4]);
  int y=atoi(argv[5]);
  int s=atoi(argv[6]);
  int basenet=atoi(argv[7]);
  ByteImage **imagePyramid=new ByteImage*[numScales];
  for (int i=0; i<numScales; i++)
  {
    char name[256];
    sprintf(name,"%s-%d",prefix,i);
    imagePyramid[i]=FindByteImageWithName(name);
  }
  int x1, y1, x2, y2;
  int searched=
    SearchEyes(numScales,imagePyramid,mask,x,y,s,basenet,&x1,&y1,&x2,&y2);
  delete[] imagePyramid;
  if (searched)
    sprintf(interp->result,"{ %d %d } { %d %d }",x1,y1,x2,y2); else
      strcpy(interp->result,"");
  return TCL_OK;
}

#ifdef CompiledNets
int FindEyeLocationsCompiledCmd(ClientData data, Tcl_Interp *interp,
                                int argc, char *argv[])
{
  if (argc!=8)
  {
    interp->result="Usage: FindEyeLocationsCompiled <prefix> <numscales> <mask> <x> <y> <s> <basenet>";
    return TCL_ERROR;
  }
  char *prefix=argv[1];
  int numScales=atoi(argv[2]);
  ByteImage *mask=FindByteImageWithName(argv[3]);
  int x=atoi(argv[4]);
  int y=atoi(argv[5]);
  int s=atoi(argv[6]);
  int basenet=atoi(argv[7]);
  ByteImage **imagePyramid=new ByteImage*[numScales];
  for (int i=0; i<numScales; i++)
  {
    char name[256];
    sprintf(name,"%s-%d",prefix,i);
    imagePyramid[i]=FindByteImageWithName(name);
  }
  int x1, y1, x2, y2;
  int searched=
    SearchEyesCompiled(numScales,imagePyramid,mask,
                       x,y,s,basenet,&x1,&y1,&x2,&y2);
  delete[] imagePyramid;
  if (searched)
    sprintf(interp->result,"{ %d %d } { %d %d }",x1,y1,x2,y2); else
      strcpy(interp->result,"");
  return TCL_OK;
}
#endif
#endif

#ifndef NoTcl
int ExtractFaceUsingEyesCmd(ClientData data, Tcl_Interp *interp,
                            int argc, char *argv[])
{
  if (argc!=8)
  {
    interp->result="Usage: ExtractFaceUsingEyes <dest> <src> <x1> <y1> <x2> <y2> <scale>";
    return TCL_ERROR;
  }
  ByteImage *dest=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  double x1=atof(argv[3]);
  double y1=atof(argv[4]);
  double x2=atof(argv[5]);
  double y2=atof(argv[6]);
  int sc=atoi(argv[7]);
  double angle=atan2(y2-y1,x2-x1);
  double c=cos(angle);
  double s=sin(angle);
  x1/=pow(1.2,sc);
  x2/=pow(1.2,sc);
  y1/=pow(1.2,sc);
  y2/=pow(1.2,sc);
  double scale=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))/12.0;
  Byte *newdata=(Byte*)malloc(900);
  int ptr=0;
  for (int y=0; y<30; y++) for (int x=0; x<30; x++)
  {
    double rx=(c*(x-9)-s*(y-10))*scale+x1;
    double ry=(s*(x-9)+c*(y-10))*scale+y1;
    int val=(int)src->interpolateExtend(rx,ry);
    if (val<0) val=0;
    if (val>255) val=255;
    newdata[ptr++]=val;
  }
  dest->NewBuffer(newdata,30,30);
  return TCL_OK;
}
#endif

#ifndef NoTcl
int ExtractFaceUsingEyes2Cmd(ClientData data, Tcl_Interp *interp,
                             int argc, char *argv[])
{
  if (argc!=8)
  {
    interp->result="Usage: ExtractFaceUsingEyes2 <dest> <src> <x1> <y1> <x2> <y2> <scale>";
    return TCL_ERROR;
  }
  ByteImage *dest=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  double x1=atof(argv[3]);
  double y1=atof(argv[4]);
  double x2=atof(argv[5]);
  double y2=atof(argv[6]);
  int sc=atoi(argv[7]);
  double angle=atan2(y2-y1,x2-x1);
  double c=cos(angle);
  double s=sin(angle);
  x1/=pow(1.2,sc);
  x2/=pow(1.2,sc);
  y1/=pow(1.2,sc);
  y2/=pow(1.2,sc);
  double scale=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))/14.0;
  Byte *newdata=(Byte*)malloc(900);
  int ptr=0;
  for (int y=0; y<36; y++) for (int x=0; x<25; x++)
  {
    double rx=(c*(x-5)-s*(y-13))*scale+x1;
    double ry=(s*(x-5)+c*(y-13))*scale+y1;
    int val=(int)src->interpolateExtend(rx,ry);
    if (val<0) val=0;
    if (val>255) val=255;
    newdata[ptr++]=val;
  }
  dest->NewBuffer(newdata,25,36);
  return TCL_OK;
}
#endif

#ifndef NoTcl
int WriteHeaderCmd(ClientData data, Tcl_Interp *interp,
                   int argc, char *argv[])
{
  if (argc!=5) {
    interp->result="Usage: WriteHeader <file> <inputs> <outputs> <patterns>";
    return TCL_ERROR;
  }
  FILE *outf=GetOpenFile(interp,argv[1],TCL_WRITABLE);
  fprintf(outf,"\002");
  int num=(atoi(argv[2]));
  fwrite((char*)&num,sizeof(float),1,outf);
  num=(atoi(argv[3]));
  fwrite((char*)&num,sizeof(float),1,outf);
  num=(atoi(argv[4]));
  fwrite((char*)&num,sizeof(float),1,outf);
  FreeOpenFile(interp,argv[1],outf);
  return TCL_OK;
}

int WritePatternCmd(ClientData data, Tcl_Interp *interp,
                    int argc, char *argv[])
{
  if (argc!=5) {
    interp->result="Usage: WritePattern <file> <img> <x> <y>";
    return TCL_ERROR;
  }
  float x=atof(argv[3]);
  float y=atof(argv[4]);
  float in[101];
  in[0]=htonf(1);
  for (int i=0; i<50; i++)
    in[i+1]=(-1.0+2.0*exp(-(i-x)*(i-x)/2.0/10.0));
  for (i=0; i<50; i++)
    in[i+51]=(-1.0+2.0*exp(-(i-y)*(i-y)/2.0/10.0));
  FILE *outf=GetOpenFile(interp,argv[1],TCL_WRITABLE);
  fwrite((char*)in,sizeof(float),101,outf);
  ByteImage *image=FindByteImageWithName(argv[2]);
  float *out=(float*)malloc(image->size*sizeof(float));
  for (i=0; i<image->size; i++)
    out[i]=(image->data(i)/127.5-1.0);
  fwrite((char*)out,sizeof(float),image->size,outf);
  free((char*)out);
  FreeOpenFile(interp,argv[1],outf);
  return TCL_OK;
}
#endif

#ifndef NoTcl               
void Eye_Init(Tcl_Interp *interp)
{
  Tcl_CreateCommand(interp,"ExtractFaceUsingEyes",ExtractFaceUsingEyesCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"ExtractFaceUsingEyes2",ExtractFaceUsingEyes2Cmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"ComputeEye",ComputeEyeCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"ZoomEye",ZoomEyeCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"RandEye",RandEyeCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"RandEye2",RandEye2Cmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"NormEye",NormEyeCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"FindEyeLocations",FindEyeLocationsCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#ifdef CompiledNets
  Tcl_CreateCommand(interp,"FindEyeLocationsCompiled",
                    FindEyeLocationsCompiledCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#endif
  Tcl_CreateCommand(interp,"WriteHeader",WriteHeaderCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"WritePattern",WritePatternCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
}
#endif
