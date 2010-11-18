// Search an image for faces

#ifndef NoHEBP
#define HEBP
#endif
#define LogFacesFound
#define MaskedHistogramEqualization
//#define OutputImage

#ifdef __GNUC__
#include <string.h>
#endif

#ifndef WINNT
extern "C" {
#endif
#include <assert.h>
#ifndef __GNUC__
#include <string.h>
#endif
#include <stdio.h>
#ifdef NoTcl
#include "tclHash.h"
#else
#include <tk.h>
#endif
#ifndef WINNT
#include <sys/time.h>
#endif
#ifndef hpux
#include <math.h>
#endif
#include <stdlib.h>
#ifndef WINNT
}
#endif

#ifdef hpux
#include <math.h>
#endif
#include "search.hpp"
#include "system.hpp"
#include "mat.hpp"
#include "nn.hpp"
#include "fast2.hpp"
#include "img.hpp"
#include "myhebp.hpp"
#include "faces.hpp"
#ifndef NoTcl
#include "cluster2.hpp"
#endif

#include "vote.hpp"

#ifdef WINNT
#undef random
#define random rand
#endif

//#define DEBUG
//#define DebugX 0
//#define DebugY 0
//#define DebugS 1
//#define DebugX 46
//#define DebugY 23
//#define DebugS 3

#define OrientationMerging

Mat NNLayer1, NNLayer2;
char *NNDirectory;

#define FaceHeight 19
#define FaceWidth 19
#define FaceSize 283

static int FaceMask[FaceWidth*FaceHeight]={
  0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
  0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
  0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
  0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
  0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
  0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
  0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0};

extern long FileSize(char *fileName);

// USED
// Check that the scale is in range; if not make it be in range, 
// and modify the x,y coords (which are associated with the scale)
// accordingly.
void LimitScale(int min, int max, int *s, int *x, int *y)
{
  int delta=0;
  if (*s<min) delta=min-(*s);
  if (*s>max) delta=max-(*s);
  if (delta!=0) {
    *x=(int)floor(0.5+*x*pow(1.2,-delta));
    *y=(int)floor(0.5+*y*pow(1.2,-delta));
    *s+=delta;
  }
}

#ifndef NoTcl
#ifndef NoHEBP
int TestHEBPCmd(ClientData data, Tcl_Interp *interp,
                int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: TestHEBP <clusters> <examples+> <examples->";
    return TCL_ERROR;
  }
  char *clusterFile=argv[1];
  char *examplePosFile=argv[2];
  char *exampleNegFile=argv[3];
  int numClusters=FileSize(clusterFile)/(283*sizeof(double));
  Vec* cluster=new Vec[numClusters];
  int positive[20];
  FILE *inf=fopen(clusterFile,"r");
  fprintf(stderr,"Positive: ");
  for (int i=0; i<numClusters; i++)
  {
    int tmp;
    fread((char*)&tmp,sizeof(int),1,inf);
    positive[i]=tmp;
    cluster[i]=LoadVec(inf);
    fprintf(stderr," %d",positive[i]?1:0);
  }
  fclose(inf);
  fprintf(stderr,"\n");

// Neural network initialization
  net_spec spec[2];
  network *net;
  strcpy(spec[0].network_name,"real");
  spec[0].num_patterns=1;
  spec[0].num_cv_patterns=0;
  spec[0].aux_data=NULL;
  spec[1].network_name[0]=0;
  net=HEBP_init("./",spec,1);
  float inputs[200];
  float outputs[1];
  float targets[1];
  HEBP_replace_pattern_from_arrays (net,
                                    inputs,
                                    outputs,
                                    HEBP_ADD_PATTERN,
                                    0, 1);
  net[0].ennuvits = 0;

  int err1=0, err2=0, err3=0;

  int num=FileSize(examplePosFile)/(21*8+4);
  fprintf(stderr,"%d positive examples\n",num);
  inf=fopen(examplePosFile,"r");
  for (int j=0; j<num; j++)
  {
    int index[20];
    double dist[20];
    Vec tem=LoadVec(inf);
    for (i=0; i<numClusters; i++)
    {
      index[i]=i;
      dist[i]=tem(i);
    }
    i=0; 
    while (i<numClusters-1)
      if (dist[i]<=dist[i+1]) i++; else
      {
        double d=dist[i]; dist[i]=dist[i+1]; dist[i+1]=d;
        int idx=index[i]; index[i]=index[i+1]; index[i+1]=idx;
        i=0;
      }
    int ptr=0;
    for (int k=0; k<10; k++) 
      for (i=0; i<numClusters; i++)
        if (i==index[k]) inputs[ptr++]=1.0; else inputs[ptr++]=-1.0;

    HEBP_current_network = 0;
    HEBP_suspend = 0;

    HEBP_replace_pattern_from_arrays (net, inputs, targets, 
                                     HEBP_REPLACE_PATTERN,0,0);
    HEBP_sweep_forward (net, 0, 1);
    HEBP_read_output_to_array(net, outputs, 0);

    if (outputs[0]<0.5) err1++;
  }
  fclose(inf);

  num=FileSize(exampleNegFile)/(21*8+4);
  fprintf(stderr,"%d negative examples\n",num);
  inf=fopen(exampleNegFile,"r");
  for (j=0; j<num; j++)
  {
    int index[20];
    double dist[20];
    Vec tem=LoadVec(inf);
    for (i=0; i<numClusters; i++)
    {
      index[i]=i;
      dist[i]=tem(i);
    }
    i=0; 
    while (i<numClusters-1)
      if (dist[i]<=dist[i+1]) i++; else
      {
        double d=dist[i]; dist[i]=dist[i+1]; dist[i+1]=d;
        int idx=index[i]; index[i]=index[i+1]; index[i+1]=idx;
        i=0;
      }
    int ptr=0;
    for (int k=0; k<10; k++) 
      for (i=0; i<numClusters; i++)
        if (i==index[k]) inputs[ptr++]=1.0; else inputs[ptr++]=-1.0;

    HEBP_current_network = 0;
    HEBP_suspend = 0;

    HEBP_replace_pattern (net, inputs, targets, 0);
    HEBP_sweep_forward (net, 0, 1);
    HEBP_read_output_to_array(net, outputs, 0);

    if (outputs[0]>-0.5)
    {
      if (j<12928) err2++; else err3++;
    }

  }
  fclose(inf);

  delete[] cluster;
  fprintf(stderr,"Errors: %d %d %d\n",err1,err2,err3);
  return TCL_OK;
}
#endif
#endif

#ifndef NoTcl
#ifndef NoHEBP
int TestHEBP2Cmd(ClientData dat, Tcl_Interp *interp,
                int argc, char *argv[])
{
  if (argc!=3 && argc!=4)
  {
    interp->result="Usage: TestHEBP2 <faces> <mask> [rank-file]";
    return TCL_ERROR;
  }
  char *faceFile=argv[1];
  FILE *ranks=NULL;
  ByteImage *mask=FindByteImageWithName(argv[2]);
  if (argc==4) ranks=fopen(argv[3],"w");

  int numFaces=FileSize(faceFile)/mask->size;
  FILE *inf=fopen(faceFile,"r");
  fprintf(stderr,"Testing %d images...\n",numFaces);
  HEBP_toggle_pattern_buffers(real_net);
  int neg=0, pos=0;
  signed char *data=new signed char[mask->size];
  for (int i=0; i<numFaces; i++)
  {
    fread((char*)data,mask->size,1,inf);
    for (int j=0; j<mask->size; j++)
      real_inputs[j]=data[j]/127.0;
    HEBP_current_network = 0;
    HEBP_suspend = 0;
    HEBP_replace_pattern_from_arrays(real_net,real_inputs,real_targets,
                                     HEBP_REPLACE_PATTERN,1,0);
    HEBP_sweep_forward (real_net, 0, 1);
    HEBP_read_output_to_array(real_net, real_outputs, 0);
    if (real_outputs[0]>0.0) pos++; else neg++;
    double err=0;
    for (j=0; j<mask->size; j++)
    {
      double dist=real_inputs[j]-real_outputs[j+mask->size];
      err+=dist*dist;
    }
    if (ranks!=NULL) fprintf(ranks,"%g\n",err);
  }
  delete[] data;
  fclose(inf);
  if (ranks!=NULL) fclose(ranks);
  HEBP_toggle_pattern_buffers(real_net);
  fprintf(stderr,"Positive: %d   Negative: %d\n",pos,neg);
  sprintf(interp->result,"%d %d",pos,neg);
  return TCL_OK;
}
#endif
#endif

#ifndef NoTcl
#ifndef NoHEBP
int TestHEBPResultsCmd(ClientData dat, Tcl_Interp *interp,
                       int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: TestHEBPResults <faces> <mask> <result-file>";
    return TCL_ERROR;
  }
  char *faceFile=argv[1];
  ByteImage *mask=FindByteImageWithName(argv[2]);
  FILE *results=fopen(argv[3],"w");

  int numFaces=FileSize(faceFile)/mask->size;
  FILE *inf=fopen(faceFile,"r");
  fprintf(stderr,"Testing %d images...\n",numFaces);
  HEBP_toggle_pattern_buffers(real_net);
  int neg=0, pos=0;
  signed char *data=new signed char[mask->size];
  for (int i=0; i<numFaces; i++)
  {
    fread((char*)data,mask->size,1,inf);
    for (int j=0; j<mask->size; j++)
      real_inputs[j]=data[j]/127.0;
    HEBP_current_network = 0;
    HEBP_suspend = 0;
    HEBP_replace_pattern_from_arrays(real_net,real_inputs,real_targets,
                                     HEBP_REPLACE_PATTERN,1,0);
    HEBP_sweep_forward (real_net, 0, 1);
    HEBP_read_output_to_array(real_net, real_outputs, 0);
    if (real_outputs[0]>0.0) pos++; else neg++;
    if (real_outputs[0]>0.0) 
      fprintf(results,"1\n"); else 
        fprintf(results,"0\n");
  }
  delete[] data;
  fclose(inf);
  HEBP_toggle_pattern_buffers(real_net);
  fclose(results);
  fprintf(stderr,"Positive: %d   Negative: %d\n",pos,neg);
  sprintf(interp->result,"%d %d",pos,neg);
  return TCL_OK;
}
#endif
#endif

#ifndef NoTcl
#ifndef NoHEBP
int TestUmeCmd(ClientData dat, Tcl_Interp *interp,
               int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: TestUme <faces> <mask>";
    return TCL_ERROR;
  }
  char *faceFile=argv[1];
  ByteImage *mask=FindByteImageWithName(argv[2]);

  int numFaces=FileSize(faceFile)/mask->size;
  FILE *inf=fopen(faceFile,"r");
  fprintf(stderr,"Testing %d images...\n",numFaces);
  HEBP_toggle_pattern_buffers(real_net);
  int neg=0, pos=0;
  signed char *data=new signed char[mask->size];
  for (int i=0; i<numFaces; i++)
  {
    fread((char*)data,mask->size,1,inf);
    for (int j=0; j<mask->size; j++)
      real_inputs[j]=data[j]/127.0;
    HEBP_current_network = 0;
    HEBP_suspend = 0;
    HEBP_replace_pattern_from_arrays(real_net,real_inputs,real_targets,
                                     HEBP_REPLACE_PATTERN,1,0);
    HEBP_sweep_forward (real_net, 0, 1);
    HEBP_read_output_to_array(real_net, real_outputs, 0);
    int thisPos=0;
    for (j=0; j<15*15; j++)
      if (real_outputs[j]>0.0) thisPos=1;
    if (thisPos) pos++; else neg++;
  }
  delete[] data;
  fclose(inf);
  HEBP_toggle_pattern_buffers(real_net);
  fprintf(stderr,"Positive: %d   Negative: %d\n",pos,neg);
  sprintf(interp->result,"%d %d",pos,neg);
  return TCL_OK;
}
#endif
#endif

#ifndef NoTcl
#ifndef NoHEBP
int RunUmeCmd(ClientData dat, Tcl_Interp *interp,
              int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: RunUme <dest> <src>";
    return TCL_ERROR;
  }
  ByteImage *dest=FindByteImageWithName(argv[1]);
  ByteImage *src=FindByteImageWithName(argv[2]);
  Byte *newdata=(Byte*)malloc(15*15);
  
  HEBP_toggle_pattern_buffers(real_net);

  for (int j=0; j<src->size; j++)
    real_inputs[j]=(((int)src->data(j))-127)/127.0;
  HEBP_replace_pattern_from_arrays(real_net,real_inputs,real_targets,
                                   HEBP_REPLACE_PATTERN,1,0);
  HEBP_sweep_forward (real_net, 0, 1);
  HEBP_read_output_to_array(real_net, real_outputs, 0);

  for (j=0; j<15*15; j++)
  {
    if (real_outputs[j]>0)
      newdata[j]=(int)(real_outputs[j]*127.0)+127; else
        newdata[j]=0;
  }
  HEBP_toggle_pattern_buffers(real_net);
  dest->NewBuffer(newdata,15,15);
  return TCL_OK;
}
#endif
#endif

#ifndef NoTcl
#ifndef NoHEBP
int TestHEBPOneOfManyCmd(ClientData dat, Tcl_Interp *interp,
                         int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: TestHEBPOneOfMany <faces> <mask> <dups>";
    return TCL_ERROR;
  }
  char *faceFile=argv[1];
  ByteImage *mask=FindByteImageWithName(argv[2]);
  int dups=atoi(argv[3]);

  int numFaces=FileSize(faceFile)/mask->size;
  FILE *inf=fopen(faceFile,"r");
  fprintf(stderr,"Testing %d images...\n",numFaces);
  HEBP_toggle_pattern_buffers(real_net);
  int neg=0, pos=0;
  int thisPos=0;
  signed char *data=new signed char[mask->size];
  for (int i=0; i<numFaces; i++)
  {
    fread((char*)data,mask->size,1,inf);
    for (int j=0; j<mask->size; j++)
      real_inputs[j]=data[j]/127.0;
    HEBP_current_network = 0;
    HEBP_suspend = 0;
    HEBP_replace_pattern_from_arrays(real_net,real_inputs,real_targets,
                                     HEBP_REPLACE_PATTERN,1,0);
    HEBP_sweep_forward (real_net, 0, 1);
    HEBP_read_output_to_array(real_net, real_outputs, 0);
    if (real_outputs[0]>0.0) thisPos=1;
    if (((i+1) % dups)==0)
    {
      if (thisPos) pos++; else neg++;
      thisPos=0;
    }
    double err=0;
    for (j=0; j<mask->size; j++)
    {
      double dist=real_inputs[j]-real_outputs[j+mask->size];
      err+=dist*dist;
    }
  }
  delete[] data;
  fclose(inf);
  HEBP_toggle_pattern_buffers(real_net);
  fprintf(stderr,"Positive: %d   Negative: %d\n",pos,neg);
  sprintf(interp->result,"%d %d",pos,neg);
  return TCL_OK;
}
#endif
#endif

#ifndef NoTcl
#ifndef NoHEBP
int TestHEBPRankedCmd(ClientData dat, Tcl_Interp *interp,
                      int argc, char *argv[])
{
  if (argc!=6)
  {
    interp->result="Usage: TestHEBPRanked <faces> <ranks> <num> <cut> <low>";
    return TCL_ERROR;
  }
  FILE *inf=fopen(argv[1],"r");
  FILE *ranks=fopen(argv[2],"r");
  int numFaces=atoi(argv[3]);
  double cut=atof(argv[4]);
  int invert=(strcmp(argv[5],"low")!=0);

  ByteImage *mask=GetFaceMask();

  fprintf(stderr,"Testing %d images...\n",numFaces);
  HEBP_toggle_pattern_buffers(real_net);
  int neg=0, pos=0, total=0;
  signed char *data=new signed char[mask->size];
  for (int i=0; i<numFaces; i++)
  {
    double rank;
    fscanf(ranks,"%lf",&rank);
    int ok=rank<=cut;
    if (invert) ok=!ok;
    if (ok)
    {
      fread((char*)data,mask->size,1,inf);
      for (int j=0; j<mask->size; j++)
        real_inputs[j]=data[j]/127.0;
      HEBP_current_network = 0;
      HEBP_suspend = 0;
      HEBP_replace_pattern_from_arrays(real_net,real_inputs,real_targets,
                                       HEBP_REPLACE_PATTERN,1,0);
      HEBP_sweep_forward (real_net, 0, 1);
      HEBP_read_output_to_array(real_net, real_outputs, 0);
      if (real_outputs[0]>0.0) pos++; else neg++;
      total++;
    }
  }
  delete[] data;
  fclose(inf);
  fclose(ranks);
  HEBP_toggle_pattern_buffers(real_net);
  fprintf(stderr,"Positive: %d   Negative: %d   Total: %d\n",pos,neg,total);
  return TCL_OK;
}
#endif
#endif

#ifndef NoTcl
#ifndef NoHEBP
int TestHEBPReconCmd(ClientData dat, Tcl_Interp *interp,
                     int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: TestHEBPRecon <faces>";
    return TCL_ERROR;
  }
  FILE *inf=fopen(argv[1],"r");

  ByteImage *mask=GetFaceMask();

  int numFaces=FileSize(argv[1])/mask->size;
  fprintf(stderr,"Testing %d images...\n",numFaces);
  HEBP_toggle_pattern_buffers(real_net);
  HEBP_toggle_pattern_buffers(real_net+1);
  int neg=0, pos=0;
  signed char *data=new signed char[mask->size];
  for (int i=0; i<numFaces; i++)
  {
    fread((char*)data,mask->size,1,inf);

    for (int j=0; j<mask->size; j++) real_inputs[j]=data[j]/127.0;
    HEBP_replace_pattern_from_arrays(real_net,real_inputs,real_targets,
                                     HEBP_REPLACE_PATTERN,1,0);
    HEBP_sweep_forward (real_net, 0, 1);
    HEBP_read_output_to_array(real_net, real_outputs, 0);
    int ans0=real_outputs[0]>0.0;
    double err0=0.0;
    for (j=0; j<mask->size; j++)
    {
      double dist=real_outputs[mask->size+j]-real_inputs[j];
      err0+=dist*dist;
    }

    for (j=0; j<mask->size; j++) real_inputs[j]=data[j]/127.0;
    HEBP_replace_pattern_from_arrays(real_net+1,real_inputs,real_targets,
                                     HEBP_REPLACE_PATTERN,1,0);
    HEBP_sweep_forward (real_net+1, 0, 1);
    HEBP_read_output_to_array(real_net+1, real_outputs, 0);
    int ans1=real_outputs[0]>0.0;
    double err1=0.0;
    for (j=0; j<mask->size; j++)
    {
      double dist=real_outputs[mask->size+j]-real_inputs[j];
      err1+=dist*dist;
    }

    int ans=ans0;
//    if (err1<err0) ans=ans1;
    if ((random() % 2) == 0) ans=ans1;
    if (ans) pos++; else neg++;
//    fprintf(stderr,"%g/%d %g/%d\n",err0,ans0,err1,ans1);
  }
  delete[] data;
  fclose(inf);
  HEBP_toggle_pattern_buffers(real_net);
  HEBP_toggle_pattern_buffers(real_net+1);
  fprintf(stderr,"Positive: %d   Negative: %d\n",pos,neg);
  return TCL_OK;
}
#endif
#endif

#ifndef NoTcl
int Search(Tcl_Interp *interp, char *canvas, double scale,
           ByteImage *image, char *clusterFile, int numClusters,
           int *numChecked, ByteImage *mask, int correctLight,
           char *locations, char *patterns)
{
#ifdef OutputImage
  FloatImage *outputImage=FindFloatImageWithName("outputimage");
#endif
  
  *numChecked=0;
  int numFound=0;
/*
  double *cluster=new double[numClusters*FaceSize];
  int *positive=new int[numClusters];
  FILE *inf=fopen(clusterFile,"r");
  for (int i=0; i<numClusters; i++)
  {
    fread((char*)&(positive[i]),sizeof(int),1,inf);
    positive[i]=ntohl(positive[i]);
    int tmp;
    fread((char*)(&tmp),sizeof(int),1,inf);
    fread((char*)&(cluster[i*FaceSize]),sizeof(double),FaceSize,inf);
  }
  fclose(inf);
  for (i=0; i<numClusters*FaceSize; i++) cluster[i]=ntohd(cluster[i]);
*/

  int i;
  int ptr=0;
  Mat mat=Zero(3,3);
  for (int j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
//  for (int j=0; j<19; j++) for (i=0; i<19; i++) 
    if (mask->data(ptr++))
    {
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
  if (!correctLight) mat=Zero(3,3);

  char cmd[1024];

  int *tmp=new int[mask->size];

  Vec vec(3);
  for (int y=0; y<image->height-mask->height+1; y++)
  {
    if (strcmp(canvas,"")!=0)
    {
      sprintf(cmd,"%s delete -tag scan",canvas);
      Tcl_Eval(interp,cmd);
      sprintf(cmd,
              "%s create rect %g %g %g %g -tag scan -outline green -fill {};"
              "update",
              canvas,2.0,scale*y+2,
              1+scale*image->width,1+scale*(y+mask->height));
      Tcl_Eval(interp,cmd);
    }
    for (int x=0; x<image->width-mask->width+1; x++)
    {
      int ptr=0;
      double v0=0, v1=0, v2=0;
      for (int j=0; j<mask->height; j++) for (int i=0; i<mask->width; i++) 
      {
        Byte data=image->data(i+x,j+y);
        tmp[ptr]=data;
        if (mask->data(ptr++))
        {
          v0+=i*data;
          v1+=j*data;
          v2+=data;
        }
      }
      vec(0)=v0; vec(1)=v1; vec(2)=v2;
      vec=mat*vec;
      v0=vec(0); v1=vec(1); v2=vec(2);
      int hist[512];
      for (i=0; i<512; i++) hist[i]=0;
      ptr=0;
      for (j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
      {
        int val=tmp[ptr]-(int)(floor(0.5+i*v0+j*v1+v2))+256;
        if (val<0) val=0;
        if (val>=512) val=511;
#ifdef MaskedHistogramEqualization
        if (mask->data(ptr)) hist[val]++;
#else
        hist[val]++;
#endif
        tmp[ptr++]=val;
      }
      int map[512];
      int num=0;
      for (i=0; i<512; i++)
      {
        map[i]=num;
        num+=hist[i];
      }
      int tot=num;
      for (i=511; i>=0; i--)
      {
        map[i]+=num;
        num-=hist[i];
      }
      double scaleFactor=1.0/tot;
      for (i=0; i<mask->size; i++)
        real_fast->unitList[i+1].activation=map[tmp[i]]*scaleFactor-1.0;

      real_outputs[0]=FastForwardPass(real_fast);

      if (x==58 && y==25)
      {
        fprintf(stderr,"Search = %g\n",real_outputs[0]);
        for (int i=0; i<400; i++)
          fprintf(stderr," %9g",map[tmp[i]]*scaleFactor-1.0);
      }
        
//      HEBP_replace_pattern_from_arrays(real_net, real_inputs, real_targets,
//                                       HEBP_REPLACE_PATTERN,1,0);
//      HEBP_sweep_forward(real_net, 0, 1);
//      HEBP_read_output_to_array(real_net, real_outputs, 0);

#ifdef OutputImage
//      newdata[y*outputWidth+x]=(int)floor(0.5+254.0*0.5*(real_outputs[0]+1));
#endif
      (*numChecked)++;
      if (real_outputs[0]>0)
      {
#ifdef OutputImage
        int x1=(int)floor(0.5+scale*x);
        int y1=(int)floor(0.5+scale*y);
        int x2=(int)floor(0.5+scale*(x+FaceWidth));
        int y2=(int)floor(0.5+scale*(y+FaceHeight));
        if (x1>=outputImage->width) x1=outputImage->width-1;
        if (x2>=outputImage->width) x2=outputImage->width-1;
        if (y1>=outputImage->height) y1=outputImage->height-1;
        if (y2>=outputImage->height) y2=outputImage->height-1;
        for (int yy=y1; yy<=y2; yy++)
          for (int xx=x1; xx<=x2; xx++)
            outputImage->data(xx,yy)+=1.0;
#endif
        if (strcmp(canvas,"")!=0)
        {
          sprintf(cmd,
                  "%s create rect %g %g %g %g -outline red -fill {} -tag face;"
                  "update",
                  canvas,scale*x+2,scale*y+2,
                  scale*(x+mask->width)+2,scale*(y+mask->height)+2);
          Tcl_Eval(interp,cmd);
        }
        numFound++;
#ifdef LogFacesFound
        if (strcmp(patterns,"")!=0)
        {
          FILE *logf=fopen(patterns,"a");
          Vec face(mask->size);
          for (int i=0; i<mask->size; i++)
            face(i)=real_fast->unitList[i+1].activation;
          SaveVec(logf,face);
          fclose(logf);
        }
        if (strcmp(locations,"")!=0)
        {
          FILE *logf=fopen(locations,"a");
          fprintf(logf,"%g %g %g %g %d %d %d %g\n",
                  scale*x+2,scale*y+2,
                  scale*(x+mask->width)+2,scale*(y+mask->height)+2,
                  x,y,(int)(floor(0.5+log(scale)/log(1.2))),
                  real_outputs[0]);
          fclose(logf);
        }
#endif
      }
    }
  }
#ifdef OutputImage
  outputImage->Change();
#endif
  if (strcmp(canvas,"")!=0)
  {
    sprintf(cmd,"%s delete -tag scan; update",canvas);
    Tcl_Eval(interp,cmd);
  }
//  delete[] cluster;
//  delete[] positive;
  delete[] tmp;
  return numFound;
}
#endif

#ifndef NoTcl
int SearchInstrumentedCmd(ClientData data, Tcl_Interp *interp, 
                          int argc, char *argv[])
{
  if (argc!=9)
  {
    interp->result="Usage: SearchInstrumented <canvas> <scale> <image> <output> <mask> <correctlight> <locations> <patterns>";
    return TCL_ERROR;
  }
  char *canvas=argv[1];
  double scale=atof(argv[2]);
  ByteImage *image=FindByteImageWithName(argv[3]);
  ByteImage *outputImage=FindByteImageWithName(argv[4]);
  ByteImage *mask=FindByteImageWithName(argv[5]);
  int correctLight=atoi(argv[6]);
  char *locations=argv[7];
  char *patterns=argv[8];

  outputImage->NewBuffer((Byte*)malloc((image->width-mask->width+1)*
                                       (image->height-mask->height+1)),
                                       image->width-mask->width+1,
                                       image->height-mask->height+1);
  
  int i;
  int ptr=0;
  Mat mat=Zero(3,3);
  for (int j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
    if (mask->data(ptr++))
    {
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
  if (!correctLight) mat=Zero(3,3);

  char cmd[1024];

  int *tmp=new int[mask->size];

  Vec vec(3);
  for (int y=0; y<image->height-mask->height+1; y++)
  {
    if (strcmp(canvas,"")!=0)
    {
      sprintf(cmd,"%s delete -tag scan",canvas);
      Tcl_Eval(interp,cmd);
      sprintf(cmd,
              "%s create rect %g %g %g %g -tag scan -outline green -fill {};"
              "update",
              canvas,2.0,scale*y+2,
              1+scale*image->width,1+scale*(y+mask->height));
      Tcl_Eval(interp,cmd);
    }
    for (int x=0; x<image->width-mask->width+1; x++)
    {
      int ptr=0;
      double v0=0, v1=0, v2=0;
      for (int j=0; j<mask->height; j++) for (int i=0; i<mask->width; i++) 
      {
        Byte data=image->data(i+x,j+y);
        tmp[ptr]=data;
        if (mask->data(ptr++))
        {
          v0+=i*data;
          v1+=j*data;
          v2+=data;
        }
      }
      vec(0)=v0; vec(1)=v1; vec(2)=v2;
      vec=mat*vec;
      v0=vec(0); v1=vec(1); v2=vec(2);
      int hist[512];
      for (i=0; i<512; i++) hist[i]=0;
      ptr=0;
      for (j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
      {
        int val=tmp[ptr]-(int)(floor(i*v0+j*v1+v2)-256.5);
        if (val<0) val=0;
        if (val>=512) val=511;
#ifdef MaskedHistogramEqualization
        if (mask->data(ptr)) hist[val]++;
#else
        hist[val]++;
#endif
        tmp[ptr++]=val;
      }
      int map[512];
      int num=0;
      for (i=0; i<512; i++)
      {
        map[i]=num;
        num+=hist[i];
      }
      int tot=num;
      for (i=511; i>=0; i--)
      {
        map[i]+=num;
        num-=hist[i];
      }
      double scaleFactor=1.0/tot;
      for (i=0; i<mask->size; i++)
        real_fast_list[0]->unitList[i+1].activation=
          map[tmp[i]]*scaleFactor-1.0;

      double output=FastForwardPass(real_fast_list[0]);

      outputImage->data(x,y)=(int)floor(127.5+127*output);

      if (output>0)
      {
        if (strcmp(canvas,"")!=0)
        {
          sprintf(cmd,
                  "%s create rect %g %g %g %g -outline red -fill {} -tag face;"
                  "update",
                  canvas,scale*x+2,scale*y+2,
                  scale*(x+mask->width)+2,scale*(y+mask->height)+2);
          Tcl_Eval(interp,cmd);
        }

        if (strcmp(patterns,"")!=0)
        {
          FILE *logf=fopen(patterns,"a");
          Vec face(mask->size);
          for (int i=0; i<mask->size; i++)
            face(i)=real_fast->unitList[i+1].activation;
          SaveVec(logf,face);
          fclose(logf);
        }
        if (strcmp(locations,"")!=0)
        {
          FILE *logf=fopen(locations,"a");
          fprintf(logf,"%g %g %g %g %d %d %d %g\n",
                  scale*x+2,scale*y+2,
                  scale*(x+mask->width)+2,scale*(y+mask->height)+2,
                  x,y,(int)(floor(0.5+log(scale)/log(1.2))),
                  real_outputs[0]);
          fclose(logf);
        }
      }
    }
  }
  outputImage->Change();

  if (strcmp(canvas,"")!=0)
  {
    sprintf(cmd,"%s delete -tag scan; update",canvas);
    Tcl_Eval(interp,cmd);
  }
  delete[] tmp;
  return TCL_OK;
}
#endif

#ifndef NoTcl
int SearchUmeCmd(ClientData data, Tcl_Interp *interp, 
                 int argc, char *argv[])
{
  if (argc!=6)
  {
    interp->result="Usage: SearchUme <canvas> <scale> <image> <output> <mask>";
    return TCL_ERROR;
  }
  char *canvas=argv[1];
  double scale=atof(argv[2]);
  ByteImage *image=FindByteImageWithName(argv[3]);
  ByteImage *outputDiscrete=FindByteImageWithName(argv[4]);
  ByteImage *mask=FindByteImageWithName(argv[5]);

  FloatImage *output=new FloatImage(NULL);
  output->ZeroBuffer(image->width/2,image->height/2);

  int numChecked=0;
  int numFound=0;
  int i;

  char cmd[1024];

  int *tmp=new int[mask->size];

  for (int y=0; y<image->height-mask->height+1; y+=5)
  {
    if (strcmp(canvas,"")!=0)
    {
      sprintf(cmd,"%s delete -tag scan",canvas);
      Tcl_Eval(interp,cmd);
      sprintf(cmd,
              "%s create rect %g %g %g %g -tag scan -outline green -fill {};"
              "update",
              canvas,0.0,scale*y,
              scale*image->width-1,scale*(y+mask->height)-1);
      Tcl_Eval(interp,cmd);
    }
    for (int x=0; x<image->width-mask->width+1; x+=5)
    {
      int ptr=0;
      int hist[256];
      for (i=0; i<256; i++) hist[i]=0;
      ptr=0;
      for (int j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
      {
        int val=image->data(x+i,y+j);
        hist[val]++;
        tmp[ptr++]=val;
      }
      int map[256];
      int num=0;
      for (i=0; i<256; i++)
      {
        map[i]=num;
        num+=hist[i];
      }
      int tot=num;
      for (i=255; i>=0; i--)
      {
        map[i]+=num;
        num-=hist[i];
      }
      double scaleFactor=1.0/tot;
      FastForwardUnit *unit=&(real_fast->unitList[0])+1;
      for (i=0; i<mask->size; i++)
        (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;

      FastForwardPass(real_fast);

      numChecked++;
      int thisPos=0;
      unit=&(real_fast->unitList[0])+real_fast->firstOutput;
#if 1
      if (unit->activation>0) thisPos=1;
      if (thisPos)
      {
        for (j=5; j<10; j++)
          for (i=5; i<10; i++)
            output->data(x/2+i,y/2+j)+=1;
      }
#else
      for (j=0; j<15; j++)
        for (i=0; i<15; i++)
        {
          double val=(unit++)->activation;
          if (val>0.0) thisPos++;
          output->data(x/2+i,y/2+j)+=(val+1.0)*0.5;
        }
#endif
      if (thisPos) numFound++;
    }
  }
  output->Change();

  outputDiscrete->ZeroBuffer(image->width/2,image->height/2);
  for (i=0; i<output->size; i++)
    outputDiscrete->data(i)=(int)floor(0.5+output->data(i));
  outputDiscrete->Change();

  if (strcmp(canvas,"")!=0)
  {
    sprintf(cmd,"%s delete -tag scan; update",canvas);
    Tcl_Eval(interp,cmd);
  }
  delete[] tmp;
  delete output;
  sprintf(interp->result,"%d %d",numFound,numChecked);
  return TCL_OK;
}
#endif

#ifndef NoTcl
int SearchUmeFastCmd(ClientData data, Tcl_Interp *interp, 
                     int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: SearchUmeFast <image> <output> <mask>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *output=FindByteImageWithName(argv[2]);
  ByteImage *mask=FindByteImageWithName(argv[3]);

  output->ZeroBuffer((image->width-mask->width+1+4)/5+1,
                     (image->height-mask->height+1+4)/5+1);

  int numChecked=0;
  int numFound=0;
  int i;

  int *tmp=new int[mask->size];

  for (int y=0; y<image->height-mask->height+1; y+=5)
  {
    for (int x=0; x<image->width-mask->width+1; x+=5)
    {
      int ptr=0;
      int hist[256];
      for (i=0; i<256; i++) hist[i]=0;
      ptr=0;
      for (int j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
      {
        int val=image->data(x+i,y+j);
        hist[val]++;
        tmp[ptr++]=val;
      }
      int map[256];
      int num=0;
      for (i=0; i<256; i++)
      {
        map[i]=num;
        num+=hist[i];
      }
      int tot=num;
      for (i=255; i>=0; i--)
      {
        map[i]+=num;
        num-=hist[i];
      }
      double scaleFactor=1.0/tot;
      FastForwardUnit *unit=&(real_fast->unitList[0])+1;
      for (i=0; i<mask->size; i++)
        (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;

      FastForwardPass(real_fast);

      numChecked++;
      if (real_fast->unitList[real_fast->firstOutput].activation>0)
      {
        int i=x/5;
        int j=y/5;
        output->data(i,j)++;
        output->data(i,j+1)++;
        output->data(i+1,j)++;
        output->data(i+1,j+1)++;
        numFound++;
      }
    }
  }
  output->Change();

  delete[] tmp;
  sprintf(interp->result,"%d %d",numFound,numChecked);
  return TCL_OK;
}
#endif

#ifdef notdef
      if (1)
      {
        int *to=hist;
        for (int i=0; i<256; i++) *(to++)=0;
        *to=tmp;
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
        {
          int val=0;
          if (i>=0 && j>=0 & i<image->width && j<image->height)
            val=image->data(i,j);
          hist[val]++;
          *(to++)=val;
        }
      } else
      {
        int *from=tmp;
        int *to=tmp;
        int ptr=0;
        for (j=0; j<30; j++)
        {
          for (i=0; i<5; i++)
            hist[*(from++)]--;
          for (i=0; i<25; i++)
            *(to++)=*(from++);
          int yy=y+j-10;
          for (i=x+15; i<x+20; i++)
          {
            int val=0;
            if (i>=0 && yy>=0 & i<image->width && yy<image->height)
              val=image->data(i,yy);
            hist[val]++;
            *(to++)=val;
          }
        }
      }
#endif

#ifndef NoTcl
int SearchUmeFasterCmd(ClientData data, Tcl_Interp *interp, 
                       int argc, char *argv[])
{
  if (argc!=5)
  {
    interp->result="Usage: SearchUmeFaster <image> <output> <mask> <levelmask>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *output=FindByteImageWithName(argv[2]);
  ByteImage *mask=FindByteImageWithName(argv[3]);
  ByteImage *levelmask=FindByteImageWithName(argv[4]);

  output->ZeroBuffer((image->width+4)/5+2,(image->height+4)/5+2);
  
  int x, y, i, j;
  for (y=0; y<output->height; y++)
    for (x=0; x<output->width; x++)
    {
      int tot=0, num=0;
      for (j=y*5; j<y*5+5; j++)
        for (i=x*5; i<x*5+5; i++)
          if (i<levelmask->width && j<levelmask->height)
          {
            tot++;
            if (levelmask->data(i,j)) num++;
          }
      if (num+5>tot)
        output->data(x,y)=255;
    }

  int *tmp=new int[mask->size];
  int searched=0;
  int hist[256];
  for (y=-5; y<image->height; y+=10)
  {
    int jj=y/5+1;
    for (x=-5; x<image->width; x+=10)
    {
      int ii=x/5+1;
      if (output->data(ii,jj)==255 || output->data(ii+1,jj)==255 ||
          output->data(ii,jj+1)==255 || output->data(ii+1,jj+1)==255)
        continue;
      searched++;
      int i, j;

      int *to=hist;
      for (i=0; i<256; i++) *(to++)=0;
      to=tmp;
      for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
      {
        int val=0;
        if (i>=0 && j>=0 & i<image->width && j<image->height)
          val=image->data(i,j);
        hist[val]++;
        (*(to++))=val;
      }
      int map[256];
      to=map;
      int *from=hist;
      int num=0;
      for (i=0; i<256; i++)
      {
        (*(to++))=num;
        num+=*(from++);
      }
      int tot=num;
      for (i=0; i<256; i++)
      {
        (*(--to))+=num;
        num-=*(--from);
      }
      from=tmp;
      double scaleFactor=1.0/tot;
      FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
      for (i=0; i<mask->size; i++)
        (unit++)->activation=map[*(from++)]*scaleFactor-1.0;

      if (FastForwardPass(real_fast_list[0])>0)
      {
        output->data(ii,jj)++;
        output->data(ii,jj+1)++;
        output->data(ii+1,jj)++;
        output->data(ii+1,jj+1)++;
      }
    }
  }
  output->Change();

  delete[] tmp;
  sprintf(interp->result,"%d",searched);
  return TCL_OK;
}
#endif

#ifndef NoTcl
int SearchUmeEvenFasterCmd(ClientData data, Tcl_Interp *interp, 
                           int argc, char *argv[])
{
  if (argc!=5)
  {
    interp->result="Usage: SearchUmeEvenFaster <image> <output> <mask> <levelmask>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *output=FindByteImageWithName(argv[2]);
  ByteImage *mask=FindByteImageWithName(argv[3]);
  ByteImage *levelmask=FindByteImageWithName(argv[4]);

  output->ZeroBuffer((image->width+9)/10,(image->height+9)/10);
  
  int x, y, i, j;
  for (y=0; y<output->height; y++)
    for (x=0; x<output->width; x++)
    {
      int tot=0, num=0;
      for (j=y*10; j<y*10+10; j++)
        for (i=x*10; i<x*10+10; i++)
          if (i<levelmask->width && j<levelmask->height)
          {
            tot++;
            if (levelmask->data(i,j)) num++;
          }
      if (num+5>tot)
        output->data(x,y)=255;
    }

  int *tmp=new int[mask->size];
  int searched=0;
  int hist[256];
  for (y=0; y<image->height; y+=10)
  {
    int jj=y/10;
    for (x=0; x<image->width; x+=10)
    {
      int ii=x/10;
      if (output->data(ii,jj)==255)
        continue;
      searched++;
      int i, j;

      int *to=hist;
      for (i=0; i<256; i++) *(to++)=0;
      to=tmp;
      for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
      {
        int val=0;
        if (i>=0 && j>=0 & i<image->width && j<image->height)
          val=image->data(i,j);
        hist[val]++;
        (*(to++))=val;
      }
      int map[256];
      to=map;
      int *from=hist;
      int num=0;
      for (i=0; i<256; i++)
      {
        (*(to++))=num;
        num+=*(from++);
      }
      int tot=num;
      for (i=0; i<256; i++)
      {
        (*(--to))+=num;
        num-=*(--from);
      }
      from=tmp;
      double scaleFactor=1.0/tot;
      FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
      for (i=0; i<mask->size; i++)
        (unit++)->activation=map[*(from++)]*scaleFactor-1.0;

      if (FastForwardPass(real_fast_list[0])>0)
        output->data(ii,jj)=1;
    }
  }
  output->Change();

  delete[] tmp;
  sprintf(interp->result,"%d",searched);
  return TCL_OK;
}
#endif

extern "C" {
void ume_net(int *inputs, float *outputs);
}

// USED
// Search the given image using the 30x30 candidate detector.  The levelmask
// is used to indicate which portions of the image to not bother searching,
// because a face has already been found there.  When a face is found, 
// the callback function is called with informatoin about the detection.
void SearchUmeEvenFaster(ByteImage *image, ByteImage *levelmask, int level,
                         SearchCallback callback, ClientData data,
                         double threshold)
{
  int x, y, i, j;

  if (levelmask!=NULL) {
    // For each 10x10 pixel block where the center of a face could be,
    // see if more than 5 pixel locations still need to be scanned.  If
    // so, the block must be scanned.  This is indicated by a flag placed
    // at the upper-left corner of each block
    int xp=(image->width+9)/10;
    int yp=(image->height+9)/10;
    for (y=0; y<yp; y++)
      for (x=0; x<xp; x++)
      {
        int tot=0, num=0;
        for (j=y*10; j<y*10+10; j++)
          for (i=x*10; i<x*10+10; i++)
            if (i<levelmask->width && j<levelmask->height)
            {
              tot++;
              if (!levelmask->data(i,j)) num++;
            }
        if (num<=5)
          levelmask->data(x*10,y*10)=255; else
            levelmask->data(x*10,y*10)=0;
      }
  }

  int tmp[900];              // Used to store the window
  int hist[256];             // Used to store histogram
  for (y=0; y<image->height; y+=10)     // For each block
  {
    for (x=0; x<image->width; x+=10)
    {
      if (levelmask!=NULL && levelmask->data(x,y))
        continue;
      int i, j;

      int *to=hist;
      for (i=0; i<256; i++) *(to++)=0;

      // Copy the window from the image into tmp.  The first loop is used
      // when the window is entirely inside the image, the second one is
      // used otherwise.  For pixels outside the image, the pixels at the
      // edge of the image are replicated.  The histogram is updated.
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

      // Build a cummulative histogram
      int map[256];
      to=map;
      int *from=hist;
      int tot=0;
      for (i=0; i<256; i++)
      {
        int old=tot;
        tot+=*(from++);
        *(to++)=old+tot;
      }

      // Apply histogram equalization, write image into network input units
      double scaleFactor=1.0/tot;
      FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
      from=tmp;
      for (i=0; i<900; i++)
        (unit++)->activation=map[*(from++)]*scaleFactor-1.0;

      // Apply the network
      double output=FastForwardPass(real_fast_list[0]);

      // If there is a detection, call the callback function to record
      // or otherwise deal with that detection
      if (output>threshold)
        callback(data,image,x-5,y-5,20,20,level,pow(1.2,level),output,0);
    }
  }
}

#ifdef CompiledNets
void SearchUmeEvenFasterCompiled(ByteImage *image, ByteImage *levelmask,
                                 int level,
                                 SearchCallback callback, ClientData data)
{
  int x, y, i, j;
  int xp=(image->width+9)/10;
  int yp=(image->height+9)/10;
  for (y=0; y<yp; y++)
    for (x=0; x<xp; x++)
    {
      int tot=0, num=0;
      for (j=y*10; j<y*10+10; j++)
        for (i=x*10; i<x*10+10; i++)
          if (i<levelmask->width && j<levelmask->height)
          {
            tot++;
            if (!levelmask->data(i,j)) num++;
          }
      if (num<=5)
        levelmask->data(x*10,y*10)=255; else
          levelmask->data(x*10,y*10)=0;
    }

  int tmp[900];
  int hist[256];
  for (y=0; y<image->height; y+=10)
  {
    for (x=0; x<image->width; x+=10)
    {
      if (levelmask->data(x,y))
        continue;
      int i, j;

      int *to=hist;
      for (i=0; i<256; i++) *(to++)=0;
      to=tmp;
      if (x>=10 && y>=10 && x+20<=image->width && y+20<=image->height) {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
        {
          int val=image->data(i,j);
          hist[val]++;
          (*(to++))=val;
        }
      } else {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
        {
          int ii=i; if (ii<0) ii=0; if (ii>=image->width) ii=image->width-1;
          int jj=j; if (jj<0) jj=0; if (jj>=image->height) jj=image->height-1;
          int val=image->data(ii,jj);
          hist[val]++;
          (*(to++))=val;
        }
      }
      int map[256];
      to=map;
      int *from=hist;
      int tot=-450;
      for (i=0; i<256; i++)
      {
        int old=tot;
        tot+=*(from++);
        *(to++)=old+tot;
      }
      from=tmp;
      for (i=0; i<900; i++) {
        *from=map[*from];
        from++;
      }
      float output[1];
      ume_net(tmp,output);
      if (output[0]>0)
        callback(data,image,x-5,y-5,20,20,level,0,0,0);
    }
  }
}

#ifndef NoTcl
extern "C" {
void ume_net(int *inputs, float *outputs);
}
#endif
#endif

#ifndef NoTcl
int SearchUmeFasterStillCmd(ClientData data, Tcl_Interp *interp, 
                            int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: SearchUmeFasterStill <image> <levelmask> <net>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *levelmask=FindByteImageWithName(argv[2]);
  int net=atoi(argv[3]);

  Tcl_DString ds;
  Tcl_DStringInit(&ds);

  int x, y, i, j;
  int xp=(image->width+9)/10;
  int yp=(image->height+9)/10;
  for (y=0; y<yp; y++)
    for (x=0; x<xp; x++)
    {
      int tot=0, num=0;
      for (j=y*10; j<y*10+10; j++)
        for (i=x*10; i<x*10+10; i++)
          if (i<levelmask->width && j<levelmask->height)
          {
            tot++;
            if (!levelmask->data(i,j)) num++;
          }
      if (num<=5)
        levelmask->data(x*10,y*10)=255; else
          levelmask->data(x*10,y*10)=0;
    }

  int candidates=0, candidatesTested=0;

  int tmp[900];
  int hist[256];
  for (y=0; y<image->height; y+=10)
  {
    for (x=0; x<image->width; x+=10)
    {
      candidates++;
      if (levelmask->data(x,y))
        continue;
      candidatesTested++;
      int i, j;

      int *to=hist;
      for (i=0; i<256; i++) *(to++)=0;
      to=tmp;
      if (x>=10 && y>=10 && x+20<=image->width && y+20<=image->height) {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
        {
          int val=image->data(i,j);
          hist[val]++;
          (*(to++))=val;
        }
      } else {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
        {
          int ii=i; if (ii<0) ii=0; if (ii>=image->width) ii=image->width-1;
          int jj=j; if (jj<0) jj=0; if (jj>=image->height) jj=image->height-1;
          int val=image->data(ii,jj);
          hist[val]++;
          (*(to++))=val;
        }
      }
//#define OldMap
#ifdef OldMap
      int map[256];
      to=map;
      int *from=hist;
      int num=0;
      for (i=0; i<256; i++)
      {
        (*(to++))=num;
        num+=*(from++);
      }
      int tot=num;
      for (i=0; i<256; i++)
      {
        (*(--to))+=num;
        num-=*(--from);
      }
      from=tmp;
      double scaleFactor=1.0/tot;
      FastForwardUnit *unit=&(real_fast_list[net]->unitList[1]);
      for (i=0; i<900; i++)
        (unit++)->activation=map[*(from++)]*scaleFactor-1.0;
#else
      float map[256];
      float *mapto=map;
      int *from=hist;
      int tot=0;
      for (i=0; i<256; i++)
      {
        int old=tot;
        tot+=*(from++);
        (*(mapto++))=(old+tot)/900.0-1.0;
      }
      from=tmp;
      FastForwardUnit *unit=&(real_fast_list[net]->unitList[1]);
      for (i=0; i<900; i++)
        (unit++)->activation=map[*(from++)];
#endif
      if (FastForwardPass(real_fast_list[net])>0)
      {
        char tem[256];
        sprintf(tem,"{ %d %d } ",x+5,y+5);
        Tcl_DStringAppend(&ds,tem,-1);
      }
    }
  }
  fprintf(stderr,"%d of %d candidates tested.\n",
          candidatesTested,candidates);
  Tcl_DStringResult(interp,&ds);
  return TCL_OK;
}
#endif

#ifdef CompiledNets
int SearchUmeFasterStillCompiledCmd(ClientData data, Tcl_Interp *interp, 
                                    int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: SearchUmeFasterStillCompiled <image> <levelmask>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *levelmask=FindByteImageWithName(argv[2]);

  Tcl_DString ds;
  Tcl_DStringInit(&ds);

  int x, y, i, j;
  int xp=(image->width+9)/10;
  int yp=(image->height+9)/10;
  for (y=0; y<yp; y++)
    for (x=0; x<xp; x++)
    {
      int tot=0, num=0;
      for (j=y*10; j<y*10+10; j++)
        for (i=x*10; i<x*10+10; i++)
          if (i<levelmask->width && j<levelmask->height)
          {
            tot++;
            if (!levelmask->data(i,j)) num++;
          }
      if (num<=5)
        levelmask->data(x*10,y*10)=255; else
          levelmask->data(x*10,y*10)=0;
    }

  int tmp[900];
  int hist[256];
  for (y=0; y<image->height; y+=10)
  {
    for (x=0; x<image->width; x+=10)
    {
      if (levelmask->data(x,y))
        continue;
      int i, j;

      int *to=hist;
      for (i=0; i<256; i++) *(to++)=0;
      to=tmp;
      if (x>=10 && y>=10 && x+20<=image->width && y+20<=image->height) {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
        {
          int val=image->data(i,j);
          hist[val]++;
          (*(to++))=val;
        }
      } else {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
        {
          int ii=i; if (ii<0) ii=0; if (ii>=image->width) ii=image->width-1;
          int jj=j; if (jj<0) jj=0; if (jj>=image->height) jj=image->height-1;
          int val=image->data(ii,jj);
          hist[val]++;
          (*(to++))=val;
        }
      }
      int map[256];
      to=map;
      int *from=hist;
      int tot=-450;
      for (i=0; i<256; i++)
      {
        int old=tot;
        tot+=*(from++);
        *(to++)=old+tot;
      }
      from=tmp;
      for (i=0; i<900; i++) {
        *from=map[*from];
        from++;
      }
      float output[1];
      ume_net(tmp,output);
      if (output[0]>0)
      {
        char tem[256];
        sprintf(tem,"{ %d %d } ",x+5,y+5);
        Tcl_DStringAppend(&ds,tem,-1);
      }
    }
  }
  Tcl_DStringResult(interp,&ds);
  return TCL_OK;
}
#endif

#ifndef NoTcl
int SearchUmeFasterStillPreciseCmd(ClientData data, Tcl_Interp *interp, 
                                   int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: SearchUmeFasterStillPrecise <image> <levelmask> <precise-net>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *levelmask=FindByteImageWithName(argv[2]);
  int preciseNet=atoi(argv[3]);
  int candidateNet=preciseNet-4;

  Tcl_DString ds;
  Tcl_DStringInit(&ds);

  int candidates=0, candidatesTested=0;

  int x, y, i, j;
  int xp=(image->width+9)/10;
  int yp=(image->height+9)/10;
  for (y=0; y<yp; y++)
    for (x=0; x<xp; x++)
    {
      int tot=0, num=0;
      for (j=y*10; j<y*10+10; j++)
        for (i=x*10; i<x*10+10; i++)
          if (i<levelmask->width && j<levelmask->height)
          {
            tot++;
            if (!levelmask->data(i,j)) num++;
          }
      if (num<=5)
        levelmask->data(x*10,y*10)=255; else
          levelmask->data(x*10,y*10)=0;
    }

  int tmp[900];
  int hist[256];
  for (y=0; y<image->height; y+=10)
  {
    for (x=0; x<image->width; x+=10)
    {
      candidates++;
      if (levelmask->data(x,y))
        continue;
      candidatesTested++;
      int i, j;

      int *to=hist;
      for (i=0; i<256; i++) *(to++)=0;
      to=tmp;

      if (x>=10 && y>=10 && x+20<=image->width && y+20<=image->height) {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
        {
          int val=image->data(i,j);
          hist[val]++;
          (*(to++))=val;
        }
      } else {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
        {
          int ii=i; if (ii<0) ii=0; if (ii>=image->width) ii=image->width-1;
          int jj=j; if (jj<0) jj=0; if (jj>=image->height) jj=image->height-1;
          int val=image->data(ii,jj);
          hist[val]++;
          (*(to++))=val;
        }
      }
      //#define OldMap
#ifdef OldMap
      int map[256];
      to=map;
      int *from=hist;
      int num=0;
      for (i=0; i<256; i++)
      {
        (*(to++))=num;
        num+=*(from++);
      }
      int tot=num;
      for (i=0; i<256; i++)
      {
        (*(--to))+=num;
        num-=*(--from);
      }
      from=tmp;
      double scaleFactor=1.0/tot;
      FastForwardUnit *unit=&(real_fast_list[candidateNet]->unitList[1]);
      for (i=0; i<900; i++)
        (unit++)->activation=map[*(from++)]*scaleFactor-1.0;
      static int first=1;
      if (first) {
        fprintf(stderr,"tot=%d\n",tot);
        for (int i=0; i<256; i++)
          fprintf(stderr,"map[%d]=%g\n",i,map[i]*scaleFactor-1.0);
        first=0;
      }
#else
      float map[256];
      float *mapto=map;
      int *from=hist;
      int tot=0;
      for (i=0; i<256; i++)
      {
        int old=tot;
        tot+=*(from++);
        (*(mapto++))=(old+tot)/900.0-1.0;
      }
      from=tmp;
      FastForwardUnit *unit=&(real_fast_list[candidateNet]->unitList[1]);
      for (i=0; i<900; i++)
        (unit++)->activation=map[*(from++)];
#ifdef notdef
      static int first=1;
      if (first) {
        fprintf(stderr,"tot=%d\n",tot);
        for (int i=0; i<256; i++)
          fprintf(stderr,"map[%d]=%g\n",i,map[i]);
        first=0;
      }
#endif
#endif
      if (FastForwardPass(real_fast_list[candidateNet])>0)
      {
        FastForwardUnit *unit=&(real_fast_list[candidateNet]->unitList[1]);
        FastForwardUnit *unit2=&(real_fast_list[preciseNet]->unitList[1]);
        for (i=0; i<900; i++)
          (unit2++)->activation=(unit++)->activation;
        FastForwardPass(real_fast_list[preciseNet]);
        double xx=0, tot=0;
        unit=&(real_fast_list[preciseNet]->unitList
               [real_fast_list[preciseNet]->firstOutput]);
        for (j=0; j<20; j++)
        {
          double act=1.0+(unit++)->activation;
          tot+=act;
          xx+=act*(j-10);
        }
        xx/=tot;
        double yy=0;
        tot=0;
        for (j=0; j<20; j++)
        {
          double act=1.0+(unit++)->activation;
          tot+=act;
          yy+=act*(j-10);
        }
        yy/=tot;
        char tem[256];
        sprintf(tem,"{ %d %d } ",
                (int)floor(0.5+x+5-xx),
                (int)floor(0.5+y+5-yy));
        Tcl_DStringAppend(&ds,tem,-1);
      }
    }
  }
//  fprintf(stderr,"%d of %d candidates tested.\n",
//          candidatesTested,candidates);
  Tcl_DStringResult(interp,&ds);
  return TCL_OK;
}

#ifdef CompiledNets
extern "C" {
void ume_net(int *inputs, float *outputs);
void facerot_net(int *inputs, float *outputs);
}

int SearchUmeFasterStillPreciseCompiledCmd(ClientData data,
                                           Tcl_Interp *interp, 
                                           int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: SearchUmeFasterStillPreciseCompiled <image> <levelmask> <precise-net>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *levelmask=FindByteImageWithName(argv[2]);
  int preciseNet=atoi(argv[3]);

  Tcl_DString ds;
  Tcl_DStringInit(&ds);

  int x, y, i, j;
  int xp=(image->width+9)/10;
  int yp=(image->height+9)/10;
  for (y=0; y<yp; y++)
    for (x=0; x<xp; x++)
    {
      int tot=0, num=0;
      for (j=y*10; j<y*10+10; j++)
        for (i=x*10; i<x*10+10; i++)
          if (i<levelmask->width && j<levelmask->height)
          {
            tot++;
            if (!levelmask->data(i,j)) num++;
          }
      if (num<=5)
        levelmask->data(x*10,y*10)=255; else
          levelmask->data(x*10,y*10)=0;
    }

  int tmp[900];
  int hist[256];
  for (y=0; y<image->height; y+=10)
  {
    for (x=0; x<image->width; x+=10)
    {
      if (levelmask->data(x,y))
        continue;
      int i, j;

      int *to=hist;
      for (i=0; i<256; i++) *(to++)=0;
      to=tmp;
      if (x>=10 && y>=10 && x+20<=image->width && y+20<=image->height) {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
        {
          int val=image->data(i,j);
          hist[val]++;
          (*(to++))=val;
        }
      } else {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
        {
          int ii=i; if (ii<0) ii=0; if (ii>=image->width) ii=image->width-1;
          int jj=j; if (jj<0) jj=0; if (jj>=image->height) jj=image->height-1;
          int val=image->data(ii,jj);
          hist[val]++;
          (*(to++))=val;
        }
      }
      int map[256];
      to=map;
      int *from=hist;
      int tot=-450;
      for (i=0; i<256; i++)
      {
        int old=tot;
        tot+=*(from++);
        *(to++)=old+tot;
      }
      from=tmp;
      for (i=0; i<900; i++) {
        *from=map[*from];
        from++;
      }
      float output[1];
      ume_net(tmp,output);
      if (output[0]>0)
      {
        float output2[40];
        facerot_net(tmp,output2);
        double xx=0, tot=0;
        float *unit=output2;
        for (j=0; j<20; j++)
        {
          double act=1.0+*(unit++);
          tot+=act;
          xx+=act*(j-5);
        }
        xx/=tot;
        double yy=0;
        tot=0;
        for (j=0; j<20; j++)
        {
          double act=1.0+*(unit++);
          tot+=act;
          yy+=act*(j-5);
        }
        yy/=tot;
        char tem[256];
        sprintf(tem,"{ %d %d } ",(int)floor(0.5+x+xx),(int)floor(0.5+y+yy));
        Tcl_DStringAppend(&ds,tem,-1);
      }
    }
  }
  Tcl_DStringResult(interp,&ds);
  return TCL_OK;
}
#endif
#endif

void SearchUmeEvenFasterRegion(ByteImage *image, ByteImage *levelmask,
                               int minX, int maxX, int minY, int maxY,
                               int level,
                               SearchCallback callback, ClientData data)
{
  int x, y, i, j;
  int tmp[900];
  int hist[256];
  minX=(int)floor(minX*pow(1.2,-level)+0.5);
  maxX=(int)floor(maxX*pow(1.2,-level)+0.5);
  minY=(int)floor(minY*pow(1.2,-level)+0.5);
  maxY=(int)floor(maxY*pow(1.2,-level)+0.5);
  if (minX<0) minX=0;
  if (minY<0) minY=0;
  if (minX>=image->width) minX=image->width-1;
  if (minY>=image->height) minY=image->height-1;
  if (maxX<0) maxX=0;
  if (maxY<0) maxY=0;
  if (maxX>image->width) maxX=image->width;
  if (maxY>image->height) maxY=image->height;
  minX=(minX/10)*10;
  minY=(minY/10)+10;

  for (y=minY; y<maxY; y+=10)
  {
    for (x=minX; x<maxX; x+=10)
    {
      if (levelmask!=NULL) {
        int tot=0, num=0;
        for (j=y; j<y+10; j++)
          for (i=x; i<x+10; i++)
            if (i<levelmask->width && j<levelmask->height)
            {
              tot++;
              if (levelmask->data(i,j)) num++;
            }
        if (num>tot-5) continue;
      }
        
      int i, j;
      int *to=hist;
      for (i=0; i<256; i++) *(to++)=0;
      to=tmp;
      if (x>=10 && y>=10 && x+20<=image->width && y+20<=image->height) {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
        {
          int val=image->data(i,j);
          hist[val]++;
          (*(to++))=val;
        }
      } else {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
        {
          int ii=i; if (ii<0) ii=0; if (ii>=image->width) ii=image->width-1;
          int jj=j; if (jj<0) jj=0; if (jj>=image->height) jj=image->height-1;
          int val=image->data(ii,jj);
          hist[val]++;
          (*(to++))=val;
        }
      }
      int map[256];
      to=map;
      int *from=hist;
      int num=0;
      for (i=0; i<256; i++)
      {
        (*(to++))=num;
        num+=*(from++);
      }
      int tot=num;
      for (i=0; i<256; i++)
      {
        (*(--to))+=num;
        num-=*(--from);
      }
      from=tmp;
      double scaleFactor=1.0/tot;
      FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
      for (i=0; i<900; i++)
        (unit++)->activation=map[*(from++)]*scaleFactor-1.0;

      if (FastForwardPass(real_fast_list[0])>0)
        callback(data,image,x-5,y-5,20,20,level,0,0,0);
    }
  }
}

void SearchUmeEvenFasterRegionPrecise(ByteImage *image, ByteImage *levelmask,
                                      int minX, int maxX, int minY, int maxY,
                                      int level, int preciseNet,
                                      SearchCallback callback,
                                      ClientData data)
{
  int x, y, i, j;
  int tmp[900];
  int hist[256];
  minX=(int)floor(minX*pow(1.2,-level)+0.5);
  maxX=(int)floor(maxX*pow(1.2,-level)+0.5);
  minY=(int)floor(minY*pow(1.2,-level)+0.5);
  maxY=(int)floor(maxY*pow(1.2,-level)+0.5);
  if (minX<0) minX=0;
  if (minY<0) minY=0;
  if (minX>=image->width) minX=image->width-1;
  if (minY>=image->height) minY=image->height-1;
  if (maxX<0) maxX=0;
  if (maxY<0) maxY=0;
  if (maxX>image->width) maxX=image->width;
  if (maxY>image->height) maxY=image->height;
  minX=(minX/10)*10;
  minY=(minY/10)+10;

  for (y=minY; y<maxY; y+=10)
  {
    for (x=minX; x<maxX; x+=10)
    {
      if (levelmask!=NULL) {
        int tot=0, num=0;
        for (j=y; j<y+10; j++)
          for (i=x; i<x+10; i++)
            if (i<levelmask->width && j<levelmask->height)
            {
              tot++;
              if (levelmask->data(i,j)) num++;
            }
        if (num>tot-5) continue;
      }

      int i, j;
      int *to=hist;
      for (i=0; i<256; i++) *(to++)=0;
      to=tmp;
      if (x>=10 && y>=10 && x+20<=image->width && y+20<=image->height) {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
        {
          int val=image->data(i,j);
          hist[val]++;
          (*(to++))=val;
        }
      } else {
        for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
        {
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
      for (i=0; i<256; i++)
      {
        int old=tot;
        tot+=*(from++);
        (*(mapto++))=(old+tot)/900.0-1.0;
      }
      from=tmp;
      FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
      for (i=0; i<900; i++)
        (unit++)->activation=map[*(from++)];

      if (FastForwardPass(real_fast_list[0])>0)
      {
        FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
        FastForwardUnit *unit2=&(real_fast_list[preciseNet]->unitList[1]);
        for (i=0; i<900; i++)
          (unit2++)->activation=(unit++)->activation;
        FastForwardPass(real_fast_list[preciseNet]);
        double xx=0, tot=0;
        unit=&(real_fast_list[preciseNet]->unitList
               [real_fast_list[preciseNet]->firstOutput]);
        for (j=0; j<20; j++)
        {
          double act=1.0+(unit++)->activation;
          tot+=act;
          xx+=act*(j-10);
        }
        xx/=tot;
        double yy=0;
        tot=0;
        for (j=0; j<20; j++)
        {
          double act=1.0+(unit++)->activation;
          tot+=act;
          yy+=act*(j-10);
        }
        yy/=tot;
        callback(data,image,(int)floor(0.5+x-xx-5),(int)floor(0.5+y-yy-5),
                 20,20,level,0,0,0);
      }
    }
  }
}

void SearchUmeEvenFasterAll(ByteImage *image, int level,
                            SearchCallback callback, ClientData data)
{
  int x, y, i, j;
  int tmp[900];
  int hist[256];
  for (y=0; y<image->height; y+=10)
  {
    for (x=0; x<image->width; x+=10)
    {
      int i, j;
      int *to=hist;
      for (i=0; i<256; i++) *(to++)=0;
      to=tmp;
      for (j=y-10; j<y+20; j++) for (i=x-10; i<x+20; i++) 
      {
        int val=0;
        if (i>=0 && j>=0 && i<image->width && j<image->height)
          val=image->data(i,j);
        hist[val]++;
        (*(to++))=val;
      }
      int map[256];
      to=map;
      int *from=hist;
      int num=0;
      for (i=0; i<256; i++)
      {
        (*(to++))=num;
        num+=*(from++);
      }
      int tot=num;
      for (i=0; i<256; i++)
      {
        (*(--to))+=num;
        num-=*(--from);
      }
      from=tmp;
      double scaleFactor=1.0/tot;
      FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
      for (i=0; i<900; i++)
        (unit++)->activation=map[*(from++)]*scaleFactor-1.0;

      if (FastForwardPass(real_fast_list[0])>0)
        callback(data,image,x-5,y-5,20,20,level,0,0,0);
    }
  }
}

void FindCentroidUme(ByteImage *image, int x, int y, 
                     double *totalX, double *totalY, double *total)
{
  if (x>=0 && y>=0 && x<image->width && y<image->height)
  {
    double val=image->data(x,y);
    if (val>0)
    {
      (*totalX)+=x*val;
      (*totalY)+=y*val;
      (*total)+=val;
      image->data(x,y)=0;
      for (int i=-1; i<=1; i++)
        for (int j=-1; j<=1; j++)
          if (i!=0 || j!=0)
            FindCentroidUme(image,x+i,y+j,totalX,totalY,total);
    }
  }
}

#ifndef NoTcl
int PostProcessUmeCmd(ClientData data, Tcl_Interp *interp,
                      int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: PostProcessUme <image> <threshold>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  int threshold=atoi(argv[2]);
  for (int i=0; i<image->size; i++)
    if (image->data(i)>=(threshold-1)) image->data(i)-=threshold-1; else
      image->data(i)=0;
  Tcl_DString ds;
  Tcl_DStringInit(&ds);
  for (int y=0; y<image->height; y++)
    for (int x=0; x<image->width; x++)
      if (image->data(x,y)>0)
      {
        double totalX=0, totalY=0, total=0;
        FindCentroidUme(image,x,y,&totalX,&totalY,&total);
        totalX/=total;
        totalY/=total;
        char tem[256];
        sprintf(tem,"{ %g %g } ",totalX,totalY);
        Tcl_DStringAppend(&ds,tem,-1);
      }
  Tcl_DStringResult(interp,&ds);
  return TCL_OK;
}
#endif

void FindCentroidUmeFast(ByteImage *image, int x, int y, 
                         double *totalX, double *totalY, double *total,
                         int threshold)
{
  if (x>=0 && y>=0 && x<image->width && y<image->height)
  {
    double val=image->data(x,y);
    if (val>=threshold)
    {
      val=val-threshold+1;
      (*totalX)+=x*val;
      (*totalY)+=y*val;
      (*total)+=val;
      image->data(x,y)=0;
      for (int i=-1; i<=1; i++)
        for (int j=-1; j<=1; j++)
          if (i!=0 || j!=0)
            FindCentroidUmeFast(image,x+i,y+j,totalX,totalY,total,threshold);
    }
  }
}

#ifndef NoTcl
int PostProcessUmeFastCmd(ClientData data, Tcl_Interp *interp,
                          int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: PostProcessUmeFast <image> <threshold>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  int threshold=atoi(argv[2]);
  Tcl_DString ds;
  Tcl_DStringInit(&ds);
  for (int y=0; y<image->height; y++)
    for (int x=0; x<image->width; x++)
      if (image->data(x,y)>=threshold)
      {
#if 1
        double totalX=0, totalY=0, total=0;
        FindCentroidUmeFast(image,x,y,&totalX,&totalY,&total,threshold);
        totalX/=total;
        totalY/=total;
        char tem[256];
        sprintf(tem,"{ %g %g } ",totalX*5+13,totalY*5+13);
        Tcl_DStringAppend(&ds,tem,-1);
#else
        char tem[256];
        sprintf(tem,"{%d %d} ",x*5+13,y*5+13);
        Tcl_DStringAppend(&ds,tem,-1);
#endif
      }
  Tcl_DStringResult(interp,&ds);
  return TCL_OK;
}
#endif

void FindCentroidUmeFaster(ByteImage *image, int x, int y, 
                           double *totalX, double *totalY, double *total,
                           int threshold)
{
  if (x>=0 && y>=0 && x<image->width && y<image->height)
  {
    double val=image->data(x,y);
    if (val>=threshold && val!=255)
    {
      val=val-threshold+1;
      (*totalX)+=x*val;
      (*totalY)+=y*val;
      (*total)+=val;
      image->data(x,y)=0;
      for (int i=-1; i<=1; i++)
        for (int j=-1; j<=1; j++)
          if (i!=0 || j!=0)
            FindCentroidUmeFaster(image,x+i,y+j,totalX,totalY,total,threshold);
    }
  }
}

#ifndef NoTcl
int PostProcessUmeFasterCmd(ClientData data, Tcl_Interp *interp,
                            int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: PostProcessUmeFaster <image> <threshold>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  int threshold=atoi(argv[2]);
  Tcl_DString ds;
  Tcl_DStringInit(&ds);
  int ptr=0;
  for (int y=0; y<image->height; y++)
    for (int x=0; x<image->width; x++)
    {
      int val=image->data(ptr++);
      if (val>=threshold && val!=255)
      {
        double totalX=0, totalY=0, total=0;
        FindCentroidUmeFaster(image,x,y,&totalX,&totalY,&total,threshold);
        totalX/=total;
        totalY/=total;
        char tem[256];
        sprintf(tem,"{ %g %g } ",totalX*5-3,totalY*5-3);
        Tcl_DStringAppend(&ds,tem,-1);
      }
    }
  Tcl_DStringResult(interp,&ds);
  return TCL_OK;
}
#endif

#ifndef NoTcl
int PostProcessUmeEvenFasterCmd(ClientData data, Tcl_Interp *interp,
                                int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: PostProcessUmeEvenFaster <image>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  Tcl_DString ds;
  Tcl_DStringInit(&ds);
  int ptr=0;
  for (int y=0; y<image->height; y++)
    for (int x=0; x<image->width; x++)
      if (image->data(ptr++)==1)
      {
        char tem[256];
        sprintf(tem,"{ %d %d } ",x*10+5,y*10+5);
        Tcl_DStringAppend(&ds,tem,-1);
      }
  Tcl_DStringResult(interp,&ds);
  return TCL_OK;
}
#endif

#ifndef HEBP
#ifndef NoTcl
int Search(Tcl_Interp *interp, char *canvas, double scale,
           ByteImage *image, char *clusterFile, int numClusters)
{
  int numFound=0;
  double *cluster=new double[numClusters*FaceSize];
  int *positive=new int[numClusters];
  FILE *inf=fopen(clusterFile,"r");
  for (int i=0; i<numClusters; i++)
  {
    fread((char*)&(positive[i]),sizeof(int),1,inf);
    positive[i]=ntohl(positive[i]);
    int tmp;
    fread((char*)(&tmp),sizeof(int),1,inf);
    fread((char*)&(cluster[i*FaceSize]),sizeof(double),FaceSize,inf);
  }
  fclose(inf);
  for (i=0; i<numClusters*FaceSize; i++) cluster[i]=ntohd(cluster[i]);

  int ptr=0;
  Mat mat=Zero(3,3);
  for (int j=0; j<FaceHeight; j++) for (i=0; i<FaceWidth; i++) 
    if (FaceMask[ptr++])
    {
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

  char cmd[1024];

  for (int y=0; y<image->height-FaceHeight; y++)
  {
    sprintf(cmd,"%s delete -tag scan",canvas);
    Tcl_Eval(interp,cmd);
    sprintf(cmd,
            "%s create rect %g %g %g %g -tag scan -outline green -fill {};"
            "update",
            canvas,2.0,scale*y+2,
            1+scale*image->width,1+scale*(y+FaceHeight));
    Tcl_Eval(interp,cmd);
    for (int x=0; x<image->width-FaceWidth; x++)
    {
      Vec vec(3);
      int ptr=0;
      int tmp[FaceWidth*FaceHeight];
      for (int j=0; j<FaceHeight; j++) for (int i=0; i<FaceWidth; i++) 
      {
        Byte data=image->data(i+x,j+y);
        tmp[ptr]=data;
        if (FaceMask[ptr])
        {
          vec(0)+=i*data;
          vec(1)+=j*data;
          vec(2)+=data;
        }
        ptr++;
      }
      vec=mat*vec;
      int hist[512];
      for (i=0; i<512; i++) hist[i]=0;
      ptr=0;
      for (j=0; j<FaceHeight; j++) for (i=0; i<FaceWidth; i++) 
      {
        int val=tmp[ptr]-(int)(floor(0.5+i*vec(0)+j*vec(1)+vec(2)))+256;
        if (val<0) val=0;
        if (val>=512) val=511;
        hist[val]++;
        tmp[ptr++]=val;
      }
      int map[512];
      int num=0;
      for (i=0; i<512; i++)
      {
        map[i]=num;
        num+=hist[i];
      }
      int tot=num;
      for (i=511; i>=0; i--)
      {
        map[i]=(int)floor(0.5+(127*(num+map[i])/tot-127.0));
        num-=hist[i];
      }
      double vect[FaceSize];
      ptr=0;
      for (i=0; i<FaceWidth*FaceHeight; i++)
        if (FaceMask[i])
        {
          tmp[i]=map[tmp[i]];
          vect[ptr++]=tmp[i]/127.0;
        }
      int bestPos=-1;
      double bestDist=0;
      ptr=0;
      double *cptr=cluster;
      for (i=0; i<numClusters; i++)
      {
        double dist=0;
        double *vptr=vect;
        for (j=0; j<FaceSize; j++)
        {
          double tmp=(*(vptr++))-(*(cptr++));
          dist+=tmp*tmp;
        }
        if (bestPos==-1 || dist<bestDist)
        {
          bestPos=positive[i];
          bestDist=dist;
        }
      }
      if (bestPos)
      {
        sprintf(cmd,
                "%s create rect %g %g %g %g -outline red -fill {} -tag face;"
                "update",
                canvas,scale*x+2,scale*y+2,
                scale*(x+FaceWidth)+2,scale*(y+FaceHeight)+2);
        Tcl_Eval(interp,cmd);
        numFound++;
      }
    }
  }
  sprintf(cmd,"%s delete -tag scan; update",canvas);
  Tcl_Eval(interp,cmd);
  delete[] cluster;
  delete[] positive;
  return numFound;
}
#endif
#endif

#ifndef NoTcl
#ifndef HEBP
#define NormalizeClusters
#ifdef NormalizeClusters
void SearchDist(Tcl_Interp *interp, char *canvas, double scale,
                ByteImage *image, char *clusterFile, int numClusters,
                int layers)
{
  Vec *cluster=new Vec[numClusters];
  int *positive=new int[numClusters];
  FILE *inf=fopen(clusterFile,"r");
  for (int i=0; i<numClusters; i++)
  {
    fread((char*)&(positive[i]),sizeof(int),1,inf);
    positive[i]=ntohl(positive[i]);
    cluster[i]=LoadVec(inf);
  }
  fclose(inf);

  int index1[20*20], index2[20*20], j;
  double distance[20*20];
  for (i=0; i<numClusters; i++) for (j=0; j<numClusters; j++)
  {
    index1[i+20*j]=i;
    index2[i+20*j]=j;
    distance[i+20*j]=Abs(cluster[i]-cluster[j]);
  }
  i=0;
  while (i<numClusters*numClusters-1)
    if (distance[i]<=distance[i+1]) i++; else
    {
      int idx=index1[i]; index1[i]=index1[i+1]; index1[i+1]=idx;
      idx=index2[i]; index2[i]=index2[i+1]; index2[i+1]=idx;
      double d=distance[i]; distance[i]=distance[i+1]; distance[i+1]=d;
      i=0;
    }

  double var[20];

  for (i=0; i<numClusters; i++)
  {
    int num=0;
    double avg=0.0;
    for (j=0; j<numClusters*numClusters; j++)
      if (index1[j]==i && index2[j]!=i)
      {
        avg+=distance[j];
        num++;
        if (num==1) break;
      }
    avg/=num;
    double std=avg;
    var[i]=std*std;
    fprintf(stderr,"cluster %d has variance %g\n",i,var[i]);
  }

  int ptr=0;
  Mat mat=Zero(3,3);
  for (j=0; j<FaceHeight; j++) for (i=0; i<FaceWidth; i++) 
    if (FaceMask[ptr++])
    {
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

  char cmd[1024];

  Vec dists(numClusters+1);
  dists(numClusters)=0.5;
  Vec ans;

  for (int y=0; y<image->height-FaceHeight; y++)
  {
    sprintf(cmd,"%s delete -tag scan",canvas);
    Tcl_Eval(interp,cmd);
    sprintf(cmd,
            "%s create rect %g %g %g %g -tag scan -outline green -fill {};"
            "update",
            canvas,2.0,scale*y+2,
            1+scale*image->width,1+scale*(y+FaceHeight));
    Tcl_Eval(interp,cmd);
    for (int x=0; x<image->width-FaceWidth; x++)
    {
      Vec vec(3);
      int ptr=0;
      int tmp[FaceWidth*FaceHeight];
      for (int j=0; j<FaceHeight; j++) for (int i=0; i<FaceWidth; i++) 
      {
        Byte data=image->data(i+x,j+y);
        tmp[ptr]=data;
        if (FaceMask[ptr])
        {
          vec(0)+=i*data;
          vec(1)+=j*data;
          vec(2)+=data;
        }
        ptr++;
      }
      vec=mat*vec;
      int hist[512];
      for (i=0; i<512; i++) hist[i]=0;
      ptr=0;
      for (j=0; j<FaceHeight; j++) for (i=0; i<FaceWidth; i++) 
      {
        int val=tmp[ptr]-(int)(floor(0.5+i*vec(0)+j*vec(1)+vec(2)))+256;
        if (val<0) val=0;
        if (val>=512) val=511;
        hist[val]++;
        tmp[ptr++]=val;
      }
      int map[512];
      int num=0;
      for (i=0; i<512; i++)
      {
        map[i]=num;
        num+=hist[i];
      }
      int tot=num;
      for (i=511; i>=0; i--)
      {
        map[i]=(int)floor(0.5+(127*(num+map[i])/tot-127.0));
        num-=hist[i];
      }
      Vec vect(FaceSize);
      ptr=0;
      for (i=0; i<FaceWidth*FaceHeight; i++)
        if (FaceMask[i])
        {
          tmp[i]=map[tmp[i]];
          vect(ptr++)=tmp[i]/127.0;
        }

      for (i=0; i<numClusters; i++)
        dists(i)=Abs(vect-cluster[i]);
      double total=0;
      for (i=0; i<numClusters; i++)
      {
        dists(i)=dists(i)*dists(i);
        total+=dists(i);
      }
      dists/=total;
      dists(numClusters)=0.5;

      if (layers==3)
        ans=Sigmoid(NNLayer2*Sigmoid(NNLayer1*dists)); else
        {
//          for (i=0; i<numClusters; i++) dists(i)=log(dists(i));
          ans=NNLayer1*dists;
        }
      if (ans(0)>0.5)
      {
        sprintf(cmd,
                "%s create rect %g %g %g %g -outline red -fill {} -tag face;"
                "update",
                canvas,scale*x+2,scale*y+2,
                scale*(x+FaceWidth)+2,scale*(y+FaceHeight)+2);
        Tcl_Eval(interp,cmd);
      }
    }
  }
  sprintf(cmd,"%s delete -tag scan; update",canvas);
  Tcl_Eval(interp,cmd);
  delete[] cluster;
  delete[] positive;
}
#else
void SearchDist(Tcl_Interp *interp, char *canvas, double scale,
                ByteImage *image, char *clusterFile, int numClusters,
                int layers)
{
  double *cluster=new double[numClusters*FaceSize];
  int *positive=new int[numClusters];
  FILE *inf=fopen(clusterFile,"r");
  for (int i=0; i<numClusters; i++)
  {
    fread((char*)&(positive[i]),sizeof(int),1,inf);
    positive[i]=ntohl(positive[i]);
    int tmp;
    fread((char*)(&tmp),sizeof(int),1,inf);
    fread((char*)&(cluster[i*FaceSize]),sizeof(double),FaceSize,inf);
  }
  fclose(inf);
  for (i=0; i<numClusters*FaceSize; i++) cluster[i]=ntohd(cluster[i]);

  int ptr=0;
  Mat mat=Zero(3,3);
  for (int j=0; j<FaceHeight; j++) for (i=0; i<FaceWidth; i++) 
    if (FaceMask[ptr++])
    {
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

  char cmd[1024];

  Vec dists(numClusters+1);
  dists(numClusters)=0.5;
  Vec ans;

  for (int y=0; y<image->height-FaceHeight; y++)
  {
    sprintf(cmd,"%s delete -tag scan",canvas);
    Tcl_Eval(interp,cmd);
    sprintf(cmd,
            "%s create rect %g %g %g %g -tag scan -outline green -fill {};"
            "update",
            canvas,2.0,scale*y+2,
            1+scale*image->width,1+scale*(y+FaceHeight));
    Tcl_Eval(interp,cmd);
    for (int x=0; x<image->width-FaceWidth; x++)
    {
      Vec vec(3);
      int ptr=0;
      int tmp[FaceWidth*FaceHeight];
      for (int j=0; j<FaceHeight; j++) for (int i=0; i<FaceWidth; i++) 
      {
        Byte data=image->data(i+x,j+y);
        tmp[ptr]=data;
        if (FaceMask[ptr])
        {
          vec(0)+=i*data;
          vec(1)+=j*data;
          vec(2)+=data;
        }
        ptr++;
      }
      vec=mat*vec;
      int hist[512];
      for (i=0; i<512; i++) hist[i]=0;
      ptr=0;
      for (j=0; j<FaceHeight; j++) for (i=0; i<FaceWidth; i++) 
      {
        int val=tmp[ptr]-(int)(floor(0.5+i*vec(0)+j*vec(1)+vec(2)))+256;
        if (val<0) val=0;
        if (val>=512) val=511;
        hist[val]++;
        tmp[ptr++]=val;
      }
      int map[512];
      int num=0;
      for (i=0; i<512; i++)
      {
        map[i]=num;
        num+=hist[i];
      }
      int tot=num;
      for (i=511; i>=0; i--)
      {
        map[i]=(int)floor(0.5+(127*(num+map[i])/tot-127.0));
        num-=hist[i];
      }
      double vect[FaceSize];
      ptr=0;
      for (i=0; i<FaceWidth*FaceHeight; i++)
        if (FaceMask[i])
        {
          tmp[i]=map[tmp[i]];
          vect[ptr++]=tmp[i]/127.0;
        }

      ptr=0;
      for (i=0; i<numClusters; i++)
      {
        double d=0;
        for (j=0; j<FaceSize; j++)
        {
          double tmp=vect[j]-cluster[ptr++];
          d+=tmp*tmp;
        }
        dists(i)=sqrt(d);
      }

      if (layers==3)
        ans=Sigmoid(NNLayer2*Sigmoid(NNLayer1*dists)); else
        {
          for (i=0; i<numClusters; i++) dists(i)=log(dists(i));
          ans=NNLayer1*dists;
        }
      if (ans(0)>0.5)
      {
        sprintf(cmd,
                "%s create rect %g %g %g %g -outline red -fill {} -tag face;"
                "update",
                canvas,scale*x+2,scale*y+2,
                scale*(x+FaceWidth)+2,scale*(y+FaceHeight)+2);
        Tcl_Eval(interp,cmd);
      }
    }
  }
  sprintf(cmd,"%s delete -tag scan; update",canvas);
  Tcl_Eval(interp,cmd);
  delete[] cluster;
  delete[] positive;
}
#endif
#endif
#endif

#ifndef NoTcl
#ifndef NoHEBP
void SearchDist(Tcl_Interp *interp, char *canvas, double scale,
                ByteImage *image, char *clusterFile, int numClusters,
                int layers)
{
  double *cluster=new double[numClusters*FaceSize];
  int *positive=new int[numClusters];
  FILE *inf=fopen(clusterFile,"r");
  for (int i=0; i<numClusters; i++)
  {
    fread((char*)&(positive[i]),sizeof(int),1,inf);
    positive[i]=ntohl(positive[i]);
    int tmp;
    fread((char*)(&tmp),sizeof(int),1,inf);
    fread((char*)&(cluster[i*FaceSize]),sizeof(double),FaceSize,inf);
  }
  fclose(inf);
  for (i=0; i<numClusters*FaceSize; i++) cluster[i]=ntohd(cluster[i]);

// Neural network initialization
  net_spec spec[2];
  network *net;
  strcpy(spec[0].network_name,"real");
  spec[0].num_patterns=1;
  spec[0].num_cv_patterns=0;
  spec[0].aux_data=NULL;
  spec[1].network_name[0]=0;
  net=HEBP_init("./",spec,1);
  float inputs[200];
  float outputs[1];
  float targets[1];
  HEBP_replace_pattern_from_arrays (net,
                                    inputs,
                                    outputs,
                                    HEBP_ADD_PATTERN,
                                    0, 1);
  net[0].ennuvits = 0;

  int ptr=0;
  Mat mat=Zero(3,3);
  for (int j=0; j<FaceHeight; j++) for (i=0; i<FaceWidth; i++) 
    if (FaceMask[ptr++])
    {
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

  char cmd[1024];

  double dists[20];

  for (int y=0; y<image->height-FaceHeight; y++)
  {
    sprintf(cmd,"%s delete -tag scan",canvas);
    Tcl_Eval(interp,cmd);
    sprintf(cmd,
            "%s create rect %g %g %g %g -tag scan -outline green -fill {};"
            "update",
            canvas,2.0,scale*y+2,
            1+scale*image->width,1+scale*(y+FaceHeight));
    Tcl_Eval(interp,cmd);
    for (int x=0; x<image->width-FaceWidth; x++)
    {
      Vec vec(3);
      int ptr=0;
      int tmp[FaceWidth*FaceHeight];
      for (int j=0; j<FaceHeight; j++) for (int i=0; i<FaceWidth; i++) 
      {
        Byte data=image->data(i+x,j+y);
        tmp[ptr]=data;
        if (FaceMask[ptr])
        {
          vec(0)+=i*data;
          vec(1)+=j*data;
          vec(2)+=data;
        }
        ptr++;
      }
      vec=mat*vec;
      int hist[512];
      for (i=0; i<512; i++) hist[i]=0;
      ptr=0;
      for (j=0; j<FaceHeight; j++) for (i=0; i<FaceWidth; i++) 
      {
        int val=tmp[ptr]-(int)(floor(0.5+i*vec(0)+j*vec(1)+vec(2)))+256;
        if (val<0) val=0;
        if (val>=512) val=511;
        hist[val]++;
        tmp[ptr++]=val;
      }
      int map[512];
      int num=0;
      for (i=0; i<512; i++)
      {
        map[i]=num;
        num+=hist[i];
      }
      int tot=num;
      for (i=511; i>=0; i--)
      {
        map[i]=(int)floor(0.5+(127*(num+map[i])/tot-127.0));
        num-=hist[i];
      }
      double vect[FaceSize];
      ptr=0;
      for (i=0; i<FaceWidth*FaceHeight; i++)
        if (FaceMask[i])
        {
          tmp[i]=map[tmp[i]];
          vect[ptr++]=tmp[i]/127.0;
        }

      ptr=0;
      for (i=0; i<numClusters; i++)
      {
        double d=0;
        for (j=0; j<FaceSize; j++)
        {
          double tmp=vect[j]-cluster[ptr++];
          d+=tmp*tmp;
        }
        dists[i]=sqrt(d);
      }

// Run the network
    {
      int index[20];
      for (int i=0; i<20; i++) index[i]=i;
      i=0;
      while (i<numClusters-1)
        if (dists[i]<=dists[i+1]) i++; else
        {
          double d=dists[i]; dists[i]=dists[i+1]; dists[i+1]=d;
          int idx=index[i]; index[i]=index[i+1]; index[i+1]=idx;
          i=0;
        }
      int ptr=0;
      for (int k=0; k<10; k++) 
        for (i=0; i<numClusters; i++)
          if (i==index[k]) inputs[ptr++]=1; else inputs[ptr++]=-1;
//      for (k=0; k<200; k++)
//        fprintf(stderr,"%3g",inputs[k]);
//      fprintf(stderr,"\n");
    }

      HEBP_current_network = 0;
      HEBP_suspend = 0;

      HEBP_replace_pattern (net, inputs, targets, 0);
      HEBP_sweep_forward (net, 0, 1);
      HEBP_read_output_to_array(net, outputs, 0);

//      if (layers==3)
//        ans=Sigmoid(NNLayer2*Sigmoid(NNLayer1*dists)); else
//        {
//          for (i=0; i<numClusters; i++) dists(i)=log(dists(i));
//          ans=NNLayer1*dists;
//        }
//      fprintf(stderr,"output=%g\n",outputs[0]);
      if (outputs[0]>0)
      {
        fprintf(stderr,"output=%g\n",outputs[0]);
        sprintf(cmd,
                "%s create rect %g %g %g %g -outline red -fill {} -tag face;"
                "update",
                canvas,scale*x+2,scale*y+2,
                scale*(x+FaceWidth)+2,scale*(y+FaceHeight)+2);
        Tcl_Eval(interp,cmd);
      }
    }
  }
  sprintf(cmd,"%s delete -tag scan; update",canvas);
  Tcl_Eval(interp,cmd);
  delete[] cluster;
  delete[] positive;
}
#endif
#endif

static int CompareIntPair(const void *p1, const void *p2)
{
  if (((int*)p1)[1] < ((int*)p2)[1]) return -1;
  if (((int*)p1)[1] > ((int*)p2)[1]) return 1;
  return 0;
}

#ifndef HEBP
int SearchFalsePositives(ByteImage *image,
                         char *clusterFile, int numClusters,
                         char *exampleFile, int maxExamples)
{
  double *cluster=new double[numClusters*FaceSize];
  int *positive=new int[numClusters];
  FILE *inf=fopen(clusterFile,"r");
  for (int i=0; i<numClusters; i++)
  {
    fread((char*)&(positive[i]),sizeof(int),1,inf);
    positive[i]=ntohl(positive[i]);
    int tmp;
    fread((char*)(&tmp),sizeof(int),1,inf);
    fread((char*)&(cluster[i*FaceSize]),sizeof(double),FaceSize,inf);
  }
  fclose(inf);
  for (i=0; i<numClusters*FaceSize; i++) cluster[i]=ntohd(cluster[i]);

  int ptr=0;
  Mat mat=Zero(3,3);
  for (int j=0; j<FaceHeight; j++) for (i=0; i<FaceWidth; i++) 
    if (FaceMask[ptr++])
    {
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

  int numExamples=0;

  int *rowIndex=new int[(image->height-FaceHeight)*2];
  for (i=0; i<image->height-FaceHeight; i++)
  {
    rowIndex[i*2]=i;
    rowIndex[i*2+1]=random();
  }
  qsort((char*)rowIndex,image->height-FaceHeight,sizeof(int)*2,CompareIntPair);

  int *colIndex=new int[(image->width-FaceWidth)*2];
  for (i=0; i<image->width-FaceWidth; i++)
  {
    colIndex[i*2]=i;
    colIndex[i*2+1]=random();
  }
  qsort((char*)colIndex,image->width-FaceWidth,sizeof(int)*2,CompareIntPair);

  for (int xInd=0; xInd<image->width-FaceWidth; xInd++)
  {
    int x=colIndex[xInd<<1];
    for (int yInd=0; yInd<image->height-FaceHeight; yInd++)
    {
      int y=rowIndex[yInd<<1];
      Vec vec(3);
      int ptr=0;
      int tmp[FaceWidth*FaceHeight];
      for (int j=0; j<FaceHeight; j++) for (int i=0; i<FaceWidth; i++) 
      {
        Byte data=image->data(i+x,j+y);
        tmp[ptr]=data;
        if (FaceMask[ptr])
        {
          vec(0)+=i*data;
          vec(1)+=j*data;
          vec(2)+=data;
        }
        ptr++;
      }
      vec=mat*vec;
      int hist[512];
      for (i=0; i<512; i++) hist[i]=0;
      ptr=0;
      for (j=0; j<FaceHeight; j++) for (i=0; i<FaceWidth; i++) 
      {
        int val=tmp[ptr]-(int)(floor(0.5+i*vec(0)+j*vec(1)+vec(2)))+256;
        if (val<0) val=0;
        if (val>=512) val=511;
        hist[val]++;
        tmp[ptr++]=val;
      }
      int map[512];
      int num=0;
      for (i=0; i<512; i++)
      {
        map[i]=num;
        num+=hist[i];
      }
      int tot=num;
      for (i=511; i>=0; i--)
      {
        map[i]=(int)floor(0.5+(127*(num+map[i])/tot-127.0));
        num-=hist[i];
      }
      double vect[FaceSize];
      ptr=0;
      for (i=0; i<FaceWidth*FaceHeight; i++)
        if (FaceMask[i])
        {
          tmp[i]=map[tmp[i]];
          vect[ptr++]=tmp[i]/127.0;
        }
      int bestPos=-1;
      double bestDist=0;
      ptr=0;
      for (i=0; i<numClusters; i++)
      {
        double dist=0;
        for (j=0; j<FaceSize; j++)
        {
          double tmp=vect[j]-cluster[ptr++];
          dist+=tmp*tmp;
        }
        if (bestPos==-1 || dist<bestDist)
        {
          bestPos=positive[i];
          bestDist=dist;
        }
      }
      if (bestPos)
      {
        FILE *outf=fopen(exampleFile,"a");
        signed char data[FaceSize];
        ptr=0;
        for (i=0; i<FaceWidth*FaceHeight; i++)
          if (FaceMask[i]) data[ptr++]=tmp[i];
        fwrite((char*)data,sizeof(signed char),FaceSize,outf);
        fclose(outf);
        numExamples++;
        if (numExamples==maxExamples)
        {
          fprintf(stderr,"Found %d false positives.\n",numExamples);
          return numExamples;
        }
      }
    }
    fprintf(stderr,".");
    fflush(stderr);
  }
  fprintf(stderr,"Found %d false positives.\n",numExamples);
  delete[] cluster;
  delete[] positive;
  delete[] rowIndex;
  delete[] colIndex;
  return numExamples;
}
#endif

#ifndef NoHEBP
int SearchFalsePositives(ByteImage *image,
                         char *clusterFile, int numClusters,
                         char *exampleFile, int maxExamples,
                         ByteImage *mask, int correctLight)
{
/*
  double *cluster=new double[numClusters*FaceSize];
  int *positive=new int[numClusters];
  FILE *inf=fopen(clusterFile,"r");
  for (int i=0; i<numClusters; i++)
  {
    fread((char*)&(positive[i]),sizeof(int),1,inf);
    positive[i]=ntohl(positive[i]);
    int tmp;
    fread((char*)(&tmp),sizeof(int),1,inf);
    fread((char*)&(cluster[i*FaceSize]),sizeof(double),FaceSize,inf);
  }
  fclose(inf);
  for (i=0; i<numClusters*FaceSize; i++) cluster[i]=ntohd(cluster[i]);
*/
  int ptr=0;
  Mat mat=Zero(3,3);
  int i;
  for (int j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
    if (mask->data(ptr++))
    {
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
  if (!correctLight) mat=Zero(3,3);

  int numExamples=0;
  int numTried=0;

  int *rowIndex=new int[(image->height-mask->height+1)*2];
  for (i=0; i<image->height-mask->height+1; i++)
  {
    rowIndex[i*2]=i;
    rowIndex[i*2+1]=random();
  }
  qsort((char*)rowIndex,image->height-mask->height+1,sizeof(int)*2,
        CompareIntPair);

  int *colIndex=new int[(image->width-mask->width+1)*2];
  for (i=0; i<image->width-mask->width+1; i++)
  {
    colIndex[i*2]=i;
    colIndex[i*2+1]=random();
  }
  qsort((char*)colIndex,image->width-mask->width+1,sizeof(int)*2,
        CompareIntPair);

  int *tmp=new int[mask->size];

  for (int xInd=0; xInd<image->width-mask->width+1; xInd++)
  {
    int x=colIndex[xInd<<1];
    for (int yInd=0; yInd<image->height-mask->width+1; yInd++)
    {
      int y=rowIndex[yInd<<1];
      Vec vec(3);
      int ptr=0;
      for (int j=0; j<mask->height; j++) for (int i=0; i<mask->width; i++) 
      {
        Byte data=image->data(i+x,j+y);
        tmp[ptr]=data;
        if (mask->data(ptr++))
        {
          vec(0)+=i*data;
          vec(1)+=j*data;
          vec(2)+=data;
        }
      }
      vec=mat*vec;
      int hist[512];
      for (i=0; i<512; i++) hist[i]=0;
      ptr=0;
      for (j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
      {
        int val=tmp[ptr]-(int)(floor(0.5+i*vec(0)+j*vec(1)+vec(2)))+256;
        if (val<0) val=0;
        if (val>=512) val=511;
#ifdef MaskedHistogramEqualization
        if (mask->data(ptr)) hist[val]++;
#else
        hist[val]++;
#endif
        tmp[ptr++]=val;
      }
      int map[512];
      int num=0;
      for (i=0; i<512; i++)
      {
        map[i]=num;
        num+=hist[i];
      }
      int tot=num;
      for (i=511; i>=0; i--)
      {
        map[i]=(int)floor(0.5+(127*(num+map[i])/tot-127.0));
        num-=hist[i];
      }
      ptr=0;
      for (i=0; i<mask->size; i++)
      {
        tmp[i]=map[tmp[i]];
        real_inputs[ptr++]=tmp[i]/127.0;
      }

      HEBP_current_network = 0;
      HEBP_suspend = 0;

      HEBP_replace_pattern_from_arrays(real_net, real_inputs, real_targets,
                                       HEBP_REPLACE_PATTERN,1,0);
      HEBP_sweep_forward(real_net, 0, 1);
      HEBP_read_output_to_array(real_net, real_outputs, 0);

      numTried++;
      if (real_outputs[0]>0)
      {
        FILE *outf=fopen(exampleFile,"a");
        signed char *data=new signed char[mask->size];
        for (i=0; i<mask->size; i++) data[i]=tmp[i];
        fwrite((char*)data,sizeof(signed char),mask->size,outf);
        fclose(outf);
        delete[] data;
        numExamples++;
        if (numExamples==maxExamples)
        {
          fprintf(stderr,"Found %d false positives in %d examples.\n",
                  numExamples,numTried);
          delete[] rowIndex;
          delete[] colIndex;
          delete[] tmp;
          return numExamples;
        }
      }
    }
    fprintf(stderr,".");
    fflush(stderr);
  }
  fprintf(stderr,"Found %d false positives in %d examples.\n",
          numExamples,numTried);
/*  delete[] cluster;
  delete[] positive; */
  delete[] rowIndex;
  delete[] colIndex;
  delete[] tmp;
  return numExamples;
}
#endif

void SearchFalsePositivesDist(ByteImage *image,
                              char *clusterFile, int numClusters,
                              char *exampleFile, int maxExamples)
{
  double *cluster=new double[numClusters*FaceSize];
  int *positive=new int[numClusters];
  FILE *inf=fopen(clusterFile,"r");
  for (int i=0; i<numClusters; i++)
  {
    fread((char*)&(positive[i]),sizeof(int),1,inf);
    positive[i]=ntohl(positive[i]);
    int tmp;
    fread((char*)(&tmp),sizeof(int),1,inf);
    fread((char*)&(cluster[i*FaceSize]),sizeof(double),FaceSize,inf);
  }
  fclose(inf);
  for (i=0; i<numClusters*FaceSize; i++) cluster[i]=ntohd(cluster[i]);

  int ptr=0;
  Mat mat=Zero(3,3);
  for (int j=0; j<FaceHeight; j++) for (i=0; i<FaceWidth; i++) 
    if (FaceMask[ptr++])
    {
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

  int numExamples=0;

  int *rowIndex=new int[(image->height-FaceHeight)*2];
  for (i=0; i<image->height-FaceHeight; i++)
  {
    rowIndex[i*2]=i;
    rowIndex[i*2+1]=random();
  }
  qsort((char*)rowIndex,image->height-FaceHeight,sizeof(int)*2,CompareIntPair);

  int *colIndex=new int[(image->width-FaceWidth)*2];
  for (i=0; i<image->width-FaceWidth; i++)
  {
    colIndex[i*2]=i;
    colIndex[i*2+1]=random();
  }
  qsort((char*)colIndex,image->width-FaceWidth,sizeof(int)*2,CompareIntPair);

  Vec dists(numClusters+1);
  dists(numClusters)=0.5;
  Vec ans;

  for (int xInd=0; xInd<image->width-FaceWidth; xInd++)
  {
    int x=colIndex[xInd<<1];
    for (int yInd=0; yInd<image->height-FaceHeight; yInd++)
    {
      int y=rowIndex[yInd<<1];
      Vec vec(3);
      int ptr=0;
      int tmp[FaceWidth*FaceHeight];
      for (int j=0; j<FaceHeight; j++) for (int i=0; i<FaceWidth; i++) 
      {
        Byte data=image->data(i+x,j+y);
        tmp[ptr]=data;
        if (FaceMask[ptr])
        {
          vec(0)+=i*data;
          vec(1)+=j*data;
          vec(2)+=data;
        }
        ptr++;
      }
      vec=mat*vec;
      int hist[512];
      for (i=0; i<512; i++) hist[i]=0;
      ptr=0;
      for (j=0; j<FaceHeight; j++) for (i=0; i<FaceWidth; i++) 
      {
        int val=tmp[ptr]-(int)(floor(0.5+i*vec(0)+j*vec(1)+vec(2)))+256;
        if (val<0) val=0;
        if (val>=512) val=511;
        hist[val]++;
        tmp[ptr++]=val;
      }
      int map[512];
      int num=0;
      for (i=0; i<512; i++)
      {
        map[i]=num;
        num+=hist[i];
      }
      int tot=num;
      for (i=511; i>=0; i--)
      {
        map[i]=(int)floor(0.5+(127*(num+map[i])/tot-127.0));
        num-=hist[i];
      }
      double vect[FaceSize];
      ptr=0;
      for (i=0; i<FaceWidth*FaceHeight; i++)
        if (FaceMask[i])
        {
          tmp[i]=map[tmp[i]];
          vect[ptr++]=tmp[i]/127.0;
        }

      ptr=0;
      for (i=0; i<numClusters; i++)
      {
        double d=0;
        for (j=0; j<FaceSize; j++)
        {
          double tmp=vect[j]-cluster[ptr++];
          d+=tmp*tmp;
        }
        dists(i)=sqrt(d);
      }

      ans=Sigmoid(NNLayer2*Sigmoid(NNLayer1*dists));
      if (ans(0)>0.5)
      {
        FILE *outf=fopen(exampleFile,"a");
        SaveVec(outf,dists);
        fclose(outf);
        numExamples++;
        if (numExamples==maxExamples)
        {
          fprintf(stderr,"Found %d false positives.\n",numExamples);
          return;
        }
      }
    }
    fprintf(stderr,".");
    fflush(stderr);
  }
  fprintf(stderr,"Found %d false positives.\n",numExamples);
  delete[] cluster;
  delete[] positive;
  delete[] rowIndex;
  delete[] colIndex;
}

#ifndef NoTcl
int NormFaceCmd(ClientData data, Tcl_Interp *interp,
                 int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: NormFace <image>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *mask=GetFaceMask();
  int i, j;
  int ptr=0;
  Mat mat=Zero(3,3);
  for (j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
    if (mask->data(ptr++))
    {
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

  Vec vec(3);
  ptr=0;
  int *tmp=new int[mask->size];
  for (j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
  {
    Byte data=image->data(i,j);
    tmp[ptr]=data;
    if (mask->data(ptr++))
    {
      vec(0)+=i*data;
      vec(1)+=j*data;
      vec(2)+=data;
    }
  }
  vec=mat*vec;
  int hist[512];
  for (i=0; i<512; i++) hist[i]=0;
  ptr=0;
  for (j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
  {
    int val=tmp[ptr]-(int)(floor(0.5+i*vec(0)+j*vec(1)+vec(2)))+256;
    if (val<0) val=0;
    if (val>=512) val=511;
#ifdef MaskedHistogramEqualization
    if (mask->data(ptr)) hist[val]++;
#else
    hist[val]++;
#endif
    tmp[ptr++]=val;
  }
  int map[512];
  int num=0;
  for (i=0; i<512; i++)
  {
    map[i]=num;
    num+=hist[i];
  }
  int tot=num;
  for (i=511; i>=0; i--)
  {
    map[i]=(int)floor(0.5+(127*(num+map[i])/tot-127.0));
    num-=hist[i];
  }
  for (i=0; i<mask->size; i++)
    image->data(i)=map[tmp[i]]+127;
  image->Change();
  delete[] tmp;

  return TCL_OK;
}
#endif

#ifndef NoTcl
int NormFaceSmartCmd(ClientData data, Tcl_Interp *interp,
                     int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: NormFaceSmart <image>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  ByteImage *mask=GetFaceMask();
  int i, j;
  int ptr=0;
  Mat mat=Zero(3,3);
  for (j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
    if (mask->data(ptr++))
    {
      double x=(i-0.5*(mask->width-1))*2.0/(mask->width-1);
      if (x>1) x=1;
      if (x<-1) x=-1;
      double y=sqrt(1.0-x*x);
      mat(0,0)+=x*x;
      mat(0,1)+=x*y;
      mat(0,2)+=x;
      mat(1,0)+=x*y;
      mat(1,1)+=y*y;
      mat(1,2)+=y;
      mat(2,0)+=x;
      mat(2,1)+=y;
      mat(2,2)+=1;
    }
  mat=LUInverse(mat);

  Vec vec(3);
  ptr=0;
  int *tmp=new int[mask->size];
  for (j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
  {
    Byte data=image->data(i,j);
    tmp[ptr]=data;
    if (mask->data(ptr++))
    {
      double x=(i-0.5*(mask->width-1))*2.0/(mask->width-1);
      if (x>1) x=1;
      if (x<-1) x=-1;
      double y=sqrt(1.0-x*x);
      vec(0)+=x*data;
      vec(1)+=y*data;
      vec(2)+=data;
    }
  }
  vec=mat*vec;
  int hist[512];
  for (i=0; i<512; i++) hist[i]=0;
  ptr=0;
  for (j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
  {
    double x=(i-0.5*(mask->width-1))*2.0/(mask->width-1);
    if (x>1) x=1;
    if (x<-1) x=-1;
    double y=sqrt(1.0-x*x);
    int val=tmp[ptr]-(int)(floor(0.5+x*vec(0)+y*vec(1)+vec(2)))+256;
//    int val=(int)(floor(0.5+x*vec(0)+y*vec(1)+vec(2)))+256;
    if (val<0) val=0;
    if (val>=512) val=511;
#ifdef MaskedHistogramEqualization
    if (mask->data(ptr)) hist[val]++;
#else
    hist[val]++;
#endif
    tmp[ptr++]=val;
  }
  int map[512];
  int num=0;
  for (i=0; i<512; i++)
  {
    map[i]=num;
    num+=hist[i];
  }
  int tot=num;
  for (i=511; i>=0; i--)
  {
    map[i]=(int)floor(0.5+(127*(num+map[i])/tot-127.0));
    num-=hist[i];
  }
  for (i=0; i<mask->size; i++)
    image->data(i)=map[tmp[i]]+127;
  image->Change();
  delete[] tmp;

  return TCL_OK;
}
#endif

#ifndef NoTcl
int NormFaceHistogramCmd(ClientData data, Tcl_Interp *interp,
                         int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: NormFaceHistogram <image>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  int i;
  int hist[256];
  for (i=0; i<256; i++) hist[i]=0;
  for (i=0; i<image->size; i++)
    hist[image->data(i)]++;
  int map[256];
  int num=0;
  for (i=0; i<256; i++)
  {
    map[i]=num;
    num+=hist[i];
  }
  int tot=num;
  for (i=255; i>=0; i--)
  {
    map[i]=(int)floor(0.5+(127.0*(num+map[i])/tot));
    num-=hist[i];
  }
  for (i=0; i<image->size; i++)
    image->data(i)=map[image->data(i)];
  image->Change();
  return TCL_OK;
}
#endif

#ifndef NoTcl
int EdgeDetectFaceCmd(ClientData data, Tcl_Interp *interp,
                      int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: EdgeDetectFace <image>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  Byte *newdata=(Byte*)malloc(image->size);
  FloatImage *dx=new FloatImage(NULL);
  FloatImage *dy=new FloatImage(NULL);
  FloatImage *mag=new FloatImage(NULL);
  dx->NewBuffer((float*)malloc(image->size*sizeof(float)),
                image->width,image->height);
  dy->NewBuffer((float*)malloc(image->size*sizeof(float)),
                image->width,image->height);
  mag->NewBuffer((float*)malloc(image->size*sizeof(float)),
                 image->width,image->height);
  int i, j;
  for (i=0; i<image->width; i++)
    for (j=0; j<image->height; j++)
    {
      float d;
      if (i==0) d=image->data(1,j)-image->data(0,j); else
        if (i==image->width-1) d=image->data(i,j)-image->data(i-1,j); else
          d=(image->data(i+1,j)-image->data(i-1,j))*0.5;
      dx->data(i,j)=d;
      if (j==0) d=image->data(i,1)-image->data(i,0); else
        if (j==image->height-1) d=image->data(i,j)-image->data(i,j-1); else
          d=(image->data(i,j+1)-image->data(i,j-1))/2;
      dy->data(i,j)=d;
      mag->data(i,j)=sqrt(dx->data(i,j)*dx->data(i,j)+
                          dy->data(i,j)*dy->data(i,j));
      dx->data(i,j)/=mag->data(i,j);
      dy->data(i,j)/=mag->data(i,j);
    }
  for (i=0; i<image->width; i++)
    for (j=0; j<image->height; j++)
      if (mag->data(i,j)>mag->interpolate(i+dx->data(i,j),j+dy->data(i,j)) &&
          mag->data(i,j)>mag->interpolate(i-dx->data(i,j),j-dy->data(i,j)))
      {
        int m=(int)floor(0.5+mag->data(i,j));
        if (m<0) m=0; 
        if (m>255) m=255;
        newdata[i+j*image->width]=m;
      } else
        newdata[i+j*image->width]=0;
  image->NewBuffer(newdata,image->width,image->height);
  delete dx;
  delete dy;
  delete mag;
  return TCL_OK;
}
#endif

#ifndef NoTcl
int EuclideanDistanceCmd(ClientData data, Tcl_Interp *interp,
                         int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: EuclideanDistance <img1> <img2>";
    return TCL_ERROR;
  }
  ByteImage *img1=FindByteImageWithName(argv[1]);
  ByteImage *img2=FindByteImageWithName(argv[2]);
  double tot;
  for (int i=0; i<img1->size; i++)
    tot+=(img1->data(i)-img2->data(i))*(img1->data(i)-img2->data(i));
  sprintf(interp->result,"%g",sqrt(tot));
  return TCL_OK;
}
#endif

#ifdef notdef

int EdgeDetectFaceCmd(ClientData data, Tcl_Interp *interp,
                      int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: EdgeDetectFace <image>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  Byte *newdata=(Byte*)malloc(image->size);
  for (int i=0; i<image->width; i++)
    for (int j=0; j<image->height; j++)
    {
      int dx=0;
      if (i==0) dx=image->data(1,j)-image->data(0,j); else
        if (i==image->width-1) dx=image->data(i,j)-image->data(i-1,j); else
          dx=(image->data(i+1,j)-image->data(i-1,j))/2;
      int dy=0;
      if (j==0) dy=image->data(i,1)-image->data(i,0); else
        if (j==image->height-1) dy=image->data(i,j)-image->data(i,j-1); else
          dy=(image->data(i,j+1)-image->data(i,j-1))/2;
      int val=(int)floor(0.5+sqrt(dx*dx+dy*dy));
      if (val>255) val=255;
      newdata[i+j*image->width]=val;
    }
  image->NewBuffer(newdata,image->width,image->height);
  return TCL_OK;
}

#endif

#ifndef NoTcl
int NormHistFaceCmd(ClientData data, Tcl_Interp *interp,
                    int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: NormHistFace <image>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  int i, j;
  int ptr=0;

  ptr=0;
  int tmp[FaceWidth*FaceHeight];

  int hist[256];
  for (i=0; i<256; i++) hist[i]=0;
  ptr=0;
  for (j=0; j<FaceHeight; j++) for (i=0; i<FaceWidth; i++) 
  {
    int val=image->data(i,j);

#ifdef MaskedHistogramEqualization
    if (FaceMask[ptr]) hist[val]++;
#else
    hist[val]++;
#endif
    tmp[ptr++]=val;
  }
  int map[256];
  int num=0;
  for (i=0; i<256; i++)
  {
    map[i]=num;
    num+=hist[i];
  }
  int tot=num;
  for (i=255; i>=0; i--)
  {
    map[i]=(int)floor(0.5+(127*(num+map[i])/tot-127.0));
    num-=hist[i];
  }
  for (i=0; i<FaceWidth*FaceHeight; i++)
    if (FaceMask[i])
      image->data(i)=map[tmp[i]]+127; else
        image->data(i)=0;

  return TCL_OK;
}
#endif

#ifndef NoTcl
int SearchScaleCmd(ClientData data, Tcl_Interp *interp,
                   int argc, char *argv[])
{
  if (argc!=5)
  {
    interp->result="Usage: SearchScale <width> <height> <locationfile> <outputfile>";
    return TCL_OK;
  }
  int width=atoi(argv[1]);
  int height=atoi(argv[2]);
  char *locFile=argv[3];
  char *outFile=argv[4];
  int temWidth=width, temHeight=height;
  int numScales=0;
  while (temWidth>=20 && temHeight>=20) {
    char name[256];
    sprintf(name,"scale%d",numScales);
    FloatImage *image=new FloatImage(name);
    image->ZeroBuffer(temWidth,temHeight);
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }
  FloatImage **imageScale=new FloatImage*[numScales];
  for (int i=0; i<numScales; i++) {
    char name[256];
    sprintf(name,"scale%d",i);
    imageScale[i]=FindFloatImageWithName(name);
  }
  int numFaces=0;
  float maxValue=0;
  int done=0;
  FILE *inf=fopen(locFile,"r");
  do {
    int x, y, s;
    int num=fscanf(inf,"%*g %*g %*g %*g %d %d %d %*g\n",&x,&y,&s);
    if (num<=0) done=1; else {
      int xc=x+10;
      int yc=y+10;
      for (int si=-2; si<=2; si++)
      {
        if (si+s<0 || si+s>=numScales) continue;
        int xci=(int)floor(0.5+xc*exp(log(1.2)*-si));
        int yci=(int)floor(0.5+yc*exp(log(1.2)*-si));
        FloatImage *image=imageScale[si+s];
        for (int xx=xci-3; xx<=xci+3; xx++)
          for (int yy=yci-3; yy<=yci+3; yy++)
            if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
            {
              image->data(xx,yy)+=1.0;
              if (image->data(xx,yy)>maxValue) maxValue=image->data(xx,yy);
            }
      }
      numFaces++;
    }
  } while (!done);
  fclose(inf);
  FILE *outf=fopen(outFile,"w");
  for (int val=(int)floor(0.5+maxValue); val>=1; val--)
  {
    fprintf(stderr,"Scanning for value %d...\n",val);
    for (int s=0; s<numScales; s++)
    {
      FloatImage *image=imageScale[s];
      for (int x=0; x<image->width; x++)
        for (int y=0; y<image->height; y++)
          if ((int)(floor(0.5+image->data(x,y)))==val)
          {
            // Now zap all the nearby faces
            for (int scale=0; scale<numScales; scale++)
            {
              int xpos=(int)floor(0.5+x*(exp(log(1.2)*(s-scale))));
              int ypos=(int)floor(0.5+y*(exp(log(1.2)*(s-scale))));
              int size=(int)floor(0.5+10*(exp(log(1.2)*(s-scale))));
              FloatImage *img=imageScale[scale];
              for (int xx=xpos-size; xx<=xpos+size; xx++)
                for (int yy=ypos-size; yy<=ypos+size; yy++)
                  if (xx>=0 && xx<img->width && yy>=0 && yy<img->height)
                    img->data(xx,yy)=0;
            }
            int xpos=(int)floor(0.5+x*(exp(log(1.2)*s)));
            int ypos=(int)floor(0.5+y*(exp(log(1.2)*s)));
            int size=(int)floor(0.5+10*(exp(log(1.2)*s)));
            fprintf(stderr,"%d %d %d %d\n",xpos-size+2,ypos-size+2,
                    xpos+size+2,ypos+size+2);
            fprintf(outf,"%d %d %d %d\n",xpos-size+2,ypos-size+2,
                    xpos+size+2,ypos+size+2);
          }
    }
  }
  fclose(outf);
  for (i=0; i<numScales; i++)
    delete imageScale[i];
  delete[] imageScale;
  return TCL_OK;
}
#endif

#ifndef NoTcl
// USED
// Check the results of a face detector, given an image, locationfile,
// correct location file, and a canvas to draw the detections on
int CheckResultsCmd(ClientData data, Tcl_Interp *interp,
                    int argc, char *argv[])
{
  if (argc!=7) {
    interp->result="Usage: CheckResults <canvas> <image> <file> <locationfile> <answers> <mask>";
    return TCL_OK;
  }
  char *canvas=argv[1];
  ByteImage *image=FindByteImageWithName(argv[2]);
  char *imageFile=argv[3];
  char *locationFile=argv[4];
  char *answerFile=argv[5];
  ByteImage *mask=FindByteImageWithName(argv[6]);
  int temWidth=image->width, temHeight=image->height;
  int numScales=0;

  // Create a detection pyramid to store the correct locations and
  // the detector output
  while (temWidth>=mask->width && temHeight>=mask->height) {
    char name[256];
    sprintf(name,"scale%d",numScales);
    ByteImage *image=new ByteImage(name);
    image->ZeroBuffer(temWidth,temHeight);
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }
  ByteImage **imageScale=new ByteImage*[numScales];
  for (int i=0; i<numScales; i++) {
    char name[256];
    sprintf(name,"scale%d",i);
    imageScale[i]=FindByteImageWithName(name);
  }
  int numFaces=0;
  int done=0;

  // Read in the correct face locations
  FILE *inf=fopen(answerFile,"r");
  do {
    char fileName[1024];
    double xe1, ye1, xe2, ye2, xn, yn, xl, yl, xc, yc, xr, yr;
    int num=fscanf(inf,"%s {leye %lf %lf} {reye %lf %lf} {nose %lf %lf} "
                   "{lmouth %lf %lf} {cmouth %lf %lf} {rmouth %lf %lf}",
                   &(fileName[0]),
                   &xe1, &ye1, &xe2, &ye2, &xn, &yn,
                   &xl, &yl, &xc, &yc, &xr, &yr);
    if (num<=0) done=1; else
      if (strcmp(fileName,imageFile)==0) {
	// Figure out at what position and scale the face would 
	// be detected
        double xe=(xe1+xe2)/2.0;
        double ye=(ye1+ye2)/2.0;
        double scale=12.0/(sqrt((xe-xc)*(xe-xc)+(ye-yc)*(ye-yc)));
        double xcenter=(xe+xc)/2.0;
        double ycenter=(ye+yc)/2.0-scale/12.0;

	// Draw a blue dot at the center of the desired detection
        char cmd[1024];
        sprintf(cmd,"%s create oval %g %g %g %g -fill blue -outline blue"
                " -tag real; update",
                canvas,
                xcenter-1,ycenter-1,xcenter+1,ycenter+1);
        Tcl_Eval(interp,cmd);
	
	// Record the location in the detection pyramid
        int s=(int)floor(0.5-log(scale)/log(1.2));
        int xx=(int)floor(0.5+xcenter*exp(log(1.2)*-s));
        int yy=(int)floor(0.5+ycenter*exp(log(1.2)*-s));
	LimitScale(0,numScales-1,&s,&xx,&yy);
        ByteImage *image=imageScale[s];
        if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
          image->data(xx,yy)=1;
        numFaces++;
      }
  } while (!done);
  fclose(inf);

  // Read the location file
  inf=fopen(locationFile,"r");
  done=0;
  int falsePos=0, falseNeg=0;
  do {
    int x, y, s;
    double confidence;
    char buf[1024];
    if (fgets(buf,1024,inf)==NULL) { done=1; break; }
    int num=sscanf(buf,"%*g %*g %*g %*g %d %d %d %lf",&x,&y,&s,&confidence);
    if (num<=0) { done=1; break; }
    if (confidence>0.9996) {
//    if (confidence>0.0) {
      int xc=x+(mask->width/2);
      int yc=y+(mask->height/2);
      int found=0;
      // Look in an area around the detection to see if there is actually
      // a face.  If so, change its label in the detection pyramid to 2,
      // to indicate that it was found.
      for (int si=-3; si<=3; si++) {
        if (si+s<0 || si+s>=numScales) continue;
        int xci=(int)floor(0.5+xc*exp(log(1.2)*-si));
        int yci=(int)floor(0.5+yc*exp(log(1.2)*-si));
        ByteImage *image=imageScale[si+s];
	int size=(int)floor(0.5+6*pow(1.2,-si));
        for (int xx=xci-size; xx<=xci+size; xx++)
          for (int yy=yci-size; yy<=yci+size; yy++)
            if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
              if (image->data(xx,yy)) {
                found=1;
                image->data(xx,yy)=2;
              }
      }
      // Draw a green square if the detection was a face, red otherwise
      char cmd[1024];
      if (found) {
        sprintf(cmd,
                "%s create rect %g %g %g %g -tag scan -outline green -fill {}"
                " -tag right; update",
                canvas,x*exp(log(1.2)*s),y*exp(log(1.2)*s),
                (x+mask->width)*exp(log(1.2)*s),
                (y+mask->height)*exp(log(1.2)*s));
      } else {
        falsePos++;
        sprintf(cmd,
                "%s create rect %g %g %g %g -tag scan -outline red -fill {}"
                " -tag wrong; update",
                canvas,x*exp(log(1.2)*s),y*exp(log(1.2)*s),
                (x+mask->width)*exp(log(1.2)*s),
                (y+mask->height)*exp(log(1.2)*s));
      }
      Tcl_Eval(interp,cmd);
    }
  } while (!done);
  fclose(inf);

  // Scan through the detection pyramid.  Any faces that were not detected
  // are counted.
  int examples=0;
  for (int s=0; s<numScales; s++) {
    ByteImage *image=imageScale[s];
    examples+=image->width*image->height;
    for (int x=0; x<image->width; x++)
      for (int y=0; y<image->height; y++)
        if (image->data(x,y)==1)
          falseNeg++;
  }
        
  // Clean up
  for (i=0; i<numScales; i++)
    delete imageScale[i];
  delete[] imageScale;

  // Return the number of missed faces, number of faces in image,
  // number of false alarms, and number of windows that were scanned.
  sprintf(interp->result,"%d %d %d %d",falseNeg,numFaces,falsePos,examples);
  return TCL_OK;
}
#endif

#ifndef NoTcl
extern Tcl_Interp *theInterp;
#endif

struct Alignment;

void CorrectFaceDetection(Alignment *alignment, double *features,
                          int *x, int *y, int *s, int *angle);

void CorrectFaceDetectionValid(Alignment *alignment, double *features,
                               int *valid,
                               int *x, int *y, int *s, int *angle);

// USED
// Create a hash table which maps file names to lists of desired detection
// locations, used for automatically checking detector outputs
Tcl_HashTable *ReadCorrectFaceDetectionsAlignment(Alignment *alignment, 
                                                  char *answerFile)
{
  Tcl_HashTable *table=new Tcl_HashTable;
  Tcl_InitHashTable(table,TCL_STRING_KEYS);

  // Read in the answer file
  FILE *inf=fopen(answerFile,"r");
  int done=0;
  do {
    char line[1024];
    if (fgets(line,1024,inf)==NULL) done=1; else {
      double features[12];
      int valid[6];
      for (int i=0; i<6; i++) valid[i]=0;
      char *filename=line;
      char *info=strchr(line,'{');
      *(strchr(filename,' '))=0;
//      fprintf(stderr,"filename:%s\n",filename);
      while (info!=NULL) {
//        fprintf(stderr,"info:%s\n",info);
        info++;
        int num=-1;
        if (strncmp(info,"leye ",strlen("leye "))==0) num=0;
        if (strncmp(info,"reye ",strlen("reye "))==0) num=1;
        if (strncmp(info,"nose ",strlen("nose "))==0) num=2;
        if (strncmp(info,"lmouth ",strlen("lmouth "))==0) num=3;
        if (strncmp(info,"cmouth ",strlen("cmouth "))==0) num=4;
        if (strncmp(info,"rmouth ",strlen("rmouth "))==0) num=5;
        if (num!=-1) {
          double x, y;
//          fprintf(stderr,"coord:%s\n",strchr(info,' ')+1); 
          sscanf(strchr(info,' ')+1,"%lf %lf}",&x,&y);
          valid[num]=1;
          features[num*2]=x;
          features[num*2+1]=y;
        }
        info=strchr(info,'{');
      }
      // Look up filename in hash table
      Tcl_HashEntry *entry=Tcl_FindHashEntry(table,filename);
      if (entry==NULL) {
	// If it is not there, add it
        int newEntry;
        entry=Tcl_CreateHashEntry(table,filename,&newEntry);
        Tcl_SetHashValue(entry,new List<Detection>);
      }
      // Figure out where the face should be detected, and add it to the list
      List<Detection> *detections=(List<Detection>*)(Tcl_GetHashValue(entry));
      int x, y, s, angle;
      CorrectFaceDetectionValid(alignment,features,valid,&x,&y,&s,&angle);
      detections->addLast(new Detection(x,y,s,1,angle));
    } 
  } while (!done);
  fclose(inf);
  return table;
}

List<Detection> *CorrectDetectionList(Tcl_HashTable *table,
                                      char *imagename) 
{
  Tcl_HashEntry *entry=Tcl_FindHashEntry(table,imagename);
  if (entry==NULL) {
    // If it is not there, add it
    int newEntry;
    entry=Tcl_CreateHashEntry(table,imagename,&newEntry);
    Tcl_SetHashValue(entry,new List<Detection>);
  }
  List<Detection> *detections=(List<Detection>*)(Tcl_GetHashValue(entry));
  return detections;
}

Tcl_HashTable *ReadCorrectFaceDetectionsFixed(char *answerFile)
{
  Tcl_HashTable *table=new Tcl_HashTable;
  Tcl_InitHashTable(table,TCL_STRING_KEYS);
  FILE *inf=fopen(answerFile,"r");
  int done=0;
  do {
    char fileName[1024];
    double xe1, ye1, xe2, ye2, xn, yn, xl, yl, xc, yc, xr, yr;
    int num=fscanf(inf,"%s {leye %lf %lf} {reye %lf %lf} {nose %lf %lf} "
                   "{lmouth %lf %lf} {cmouth %lf %lf} {rmouth %lf %lf}",
                   fileName,
                   &xe1, &ye1, &xe2, &ye2, &xn, &yn,
                   &xl, &yl, &xc, &yc, &xr, &yr);
    if (num<=0) done=1; else {
      Tcl_HashEntry *entry=Tcl_FindHashEntry(table,fileName);
      if (entry==NULL) {
        int newEntry;
        entry=Tcl_CreateHashEntry(table,fileName,&newEntry);
        Tcl_SetHashValue(entry,new List<Detection>);
      }
      List<Detection> *detections=(List<Detection>*)(Tcl_GetHashValue(entry));
      double xe=(xe1+xe2)/2.0;
      double ye=(ye1+ye2)/2.0;
      double scale=12.0/(sqrt((xe-xc)*(xe-xc)+(ye-yc)*(ye-yc)));
      double xcenter=(xe+xc)/2.0;
      double ycenter=(ye+yc)/2.0-scale/12.0;
      int s=(int)floor(0.5-log(scale)/log(1.2));
      if (scale<0) scale=0;
      int xx=(int)floor(0.5+xcenter*exp(log(1.2)*-s));
      int yy=(int)floor(0.5+ycenter*exp(log(1.2)*-s));
      detections->addLast(new Detection(xx,yy,s,1,0));
//      if (strcmp(fileName,"/IUS/har1/har/usr0/har/faces/test/original1.gif")==0) {
//        fprintf(stderr,"%g %g %g %g %g %g %g %g %g %g %g %g\n",
//                xe1,ye1,xe2,ye2,xn,yn,xl,yl,xc,yc,xr,yr);
//        fprintf(stderr,"%g %g %g %g %g\n",xe,ye,scale,xcenter,ycenter);
//        fprintf(stderr,"%d %d %d\n\n",s,xx,yy);
//      }
    }
  } while (!done);
  fclose(inf);
  return table;
}

void FreeCorrectFaceDetections(Tcl_HashTable *table)
{
  Tcl_HashSearch search;
  for (Tcl_HashEntry *entry=Tcl_FirstHashEntry(table,&search);
       entry!=NULL;
       entry=Tcl_NextHashEntry(&search)) {
    List<Detection> *detections=(List<Detection>*)Tcl_GetHashValue(entry);
    delete detections;
  }
  Tcl_DeleteHashTable(table);
}

CheckResultsStruct *CheckResultsDirect(char *canvas,
                                       ByteImage *image, char *imageFile, 
                                       List<Detection> *detections, 
                                       Tcl_HashTable *answers, ByteImage *mask)
{
  int temWidth=image->width, temHeight=image->height;
  int numScales=0;
  while (temWidth>=mask->width && temHeight>=mask->height)
  {
    char name[256];
    sprintf(name,"scale%d",numScales);
    ByteImage *image=new ByteImage(name);
    image->ZeroBuffer(temWidth,temHeight);
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }
  ByteImage **imageScale=new ByteImage*[numScales];
  for (int i=0; i<numScales; i++)
  {
    char name[256];
    sprintf(name,"scale%d",i);
    imageScale[i]=FindByteImageWithName(name);
  }
  int numFaces=0;
  Tcl_HashEntry *entry=Tcl_FindHashEntry(answers,imageFile);
  if (entry!=NULL) {
    List<Detection>* detections=(List<Detection>*)Tcl_GetHashValue(entry);
    for (Detection *d=detections->first; d!=NULL; d=d->next) {
#ifdef OrientationMerging
      assert(d->orient>=0 && d->orient<360);
      if (d->orient>30 && d->orient<330) continue;
#endif
      int s=d->s;
      int xx=d->x;
      int yy=d->y;
      LimitScale(0,numScales-1,&s,&xx,&yy);
      double xcenter=xx*pow(1.2,s);
      double ycenter=yy*pow(1.2,s);
#ifndef NoTcl
      char cmd[512];
      sprintf(cmd,"%s create oval %g %g %g %g -fill blue -outline blue"
              " -tag real",
              canvas,xcenter-1,ycenter-1,xcenter+1,ycenter+1);
      Tcl_Eval(theInterp,cmd);
#endif
      ByteImage *image=imageScale[s];
      if (xx<0) xx=0;
      if (yy<0) yy=0;
      if (xx>=image->width) xx=image->width-1;
      if (yy>=image->height) yy=image->height-1;
      image->data(xx,yy)=1;
      numFaces++;
    }
  }

  int falsePos=0, falseNeg=0;
  for (Detection *d=detections->first; d!=NULL; d=d->next) {
    int x=d->x, y=d->y, s=d->s;
    double confidence=d->output;
    if (confidence>0.0) {
      int xc=x+(mask->width/2);
      int yc=y+(mask->height/2);
      int found=0;
      for (int si=-4; si<=4; si++)
      {
        if (si+s<0 || si+s>=numScales) continue;
        int xci=(int)floor(0.5+xc*exp(log(1.2)*-si));
        int yci=(int)floor(0.5+yc*exp(log(1.2)*-si));
        ByteImage *image=imageScale[si+s];
//	int size=(int)floor(0.5+3*pow(1.2,si));
	int size=4;
        for (int xx=xci-size; xx<=xci+size; xx++)
          for (int yy=yci-size; yy<=yci+size; yy++)
            if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
              if (image->data(xx,yy)) {
                found=1;
                image->data(xx,yy)=2;
              }
      }
#ifndef NoTcl
    {
      char cmd[1024];
      double angle=M_PI/180.0*(d->orient-90);
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
                "%s create line %g %g %g %g %g %g %g %g %g %g -fill green -tag right",
                canvas,x1,y1,x2,y2,x3,y3,x4,y4,x1,y1);
        Tcl_Eval(theInterp,cmd);
        sprintf(cmd,"%s create oval %g %g %g %g -outline green -fill {} -tag right",
                canvas,
                xc+0.5*(rightX+upX)-0.15*size,yc+0.5*(rightY+upY)-0.15*size,
                xc+0.5*(rightX+upX)+0.15*size,yc+0.5*(rightY+upY)+0.15*size);
        Tcl_Eval(theInterp,cmd);
        sprintf(cmd,"%s create oval %g %g %g %g -outline green -fill {} -tag right",
                canvas,
                xc+0.5*(-rightX+upX)-0.15*size,yc+0.5*(-rightY+upY)-0.15*size,
                xc+0.5*(-rightX+upX)+0.15*size,yc+0.5*(-rightY+upY)+0.15*size);
        Tcl_Eval(theInterp,cmd);
      } else {
        falsePos++;
        sprintf(cmd,
                "%s create line %g %g %g %g %g %g %g %g %g %g -fill red -tag wrong",
                canvas,x1,y1,x2,y2,x3,y3,x4,y4,x1,y1);
        Tcl_Eval(theInterp,cmd);
        sprintf(cmd,"%s create oval %g %g %g %g -outline red -fill {} -tag wrong",
                canvas,
                xc+0.5*(rightX+upX)-0.15*size,yc+0.5*(rightY+upY)-0.15*size,
                xc+0.5*(rightX+upX)+0.15*size,yc+0.5*(rightY+upY)+0.15*size);
        Tcl_Eval(theInterp,cmd);
        sprintf(cmd,"%s create oval %g %g %g %g -outline red -fill {} -tag wrong",
                canvas,
                xc+0.5*(-rightX+upX)-0.15*size,yc+0.5*(-rightY+upY)-0.15*size,
                xc+0.5*(-rightX+upX)+0.15*size,yc+0.5*(-rightY+upY)+0.15*size);
        Tcl_Eval(theInterp,cmd);
      }
    }
#endif
    }
  }

  int examples=0;
  for (int s=0; s<numScales; s++)
  {
    ByteImage *image=imageScale[s];
    examples+=(image->width-mask->width+1)*(image->height-mask->height+1);
    for (int x=0; x<image->width; x++)
      for (int y=0; y<image->height; y++)
        if (image->data(x,y)==1)
          falseNeg++;
  }
              
  for (i=0; i<numScales; i++)
    delete imageScale[i];
  delete[] imageScale;

  CheckResultsStruct *results=new CheckResultsStruct;
  results->falseNeg=falseNeg;
  results->numFaces=numFaces;
  results->falsePos=falsePos;
  results->examples=examples;
  
  return results;
}

void CountVotes2Direct(int width, int height, 
                       List<Detection> *detections, char *outFile,
                       Tcl_HashTable *answers, char *imageFile)
{
  int temWidth=width, temHeight=height;
  int numScales=0;
  while (temWidth>=20 && temHeight>=20)
  {
    char name[256];
    sprintf(name,"scale%d",numScales);
    ByteImage *image=new ByteImage(name);
    image->ZeroBuffer(temWidth,temHeight);
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }
  ByteImage **imageScale=new ByteImage*[numScales];
  for (int i=0; i<numScales; i++)
  {
    char name[256];
    sprintf(name,"scale%d",i);
    imageScale[i]=FindByteImageWithName(name);
  }
  int numFaces=0;
  int done=0;

  Tcl_HashEntry *entry=Tcl_FindHashEntry(answers,imageFile);
  if (entry!=NULL) {
    List<Detection>* detections=(List<Detection>*)Tcl_GetHashValue(entry);
    for (Detection *d=detections->first; d!=NULL; d=d->next) {
      int s=d->s;
      int xx=d->x;
      int yy=d->y;
      LimitScale(0,numScales-1,&s,&xx,&yy);
      ByteImage *image=imageScale[s];
      if (xx<0) xx=0;
      if (yy<0) yy=0;
      if (xx>=image->width) xx=image->width-1;
      if (yy>=image->height) yy=image->height-1;
      image->data(xx,yy)=0x80;
      numFaces++;
    }
  }
  fprintf(stderr,"Num correct faces=%d\n",numFaces);
  for (int num=0; num<3; num++)
  {
    int numFaces=0;
    for (Detection *d=detections[num].first; d!=NULL; d=d->next) {
      int x=d->x+10;
      int y=d->y+10;
      int s=d->s;
      LimitScale(0,numScales-1,&s,&x,&y);
      ByteImage *image=imageScale[s];
      if (x<0) x=0;
      if (y<0) y=0;
      if (x>=image->width) x=image->width-1;
      if (y>=image->height) y=image->height-1;
      image->data(x,y)|=(1<<num);
      numFaces++;
    }
    fprintf(stderr,"numFaces=%d\n",numFaces);
  }

  FILE *outf=fopen(outFile,"a");
  for (int s=0; s<numScales; s++)
  {
    ByteImage *image=imageScale[s];
    for (int x=0; x<image->width; x++)
      for (int y=0; y<image->height; y++)
        if (image->data(x,y) & 0x7F)
        {
          int correct=0;
          for (int si=-4; si<=4; si++)
          {
            if (si+s<0 || si+s>=numScales) continue;
            int xci=(int)floor(0.5+x*exp(log(1.2)*-si));
            int yci=(int)floor(0.5+y*exp(log(1.2)*-si));
            ByteImage *image=imageScale[si+s];
//            int sspread=(int)floor(0.5+6*pow(1.2,-si));
            int sspread=4;
            for (int xx=xci-sspread; xx<=xci+sspread; xx++)
              for (int yy=yci-sspread; yy<=yci+sspread; yy++)
                if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
                  if (image->data(xx,yy) & 0x80)
                    correct=1;
          }
          fprintf(outf,"%d",correct);
          for (int net=0; net<3; net++)
          {
            for (int si=-1; si<=1; si++)
            {
              int xci=(int)floor(0.5+x*exp(log(1.2)*-si));
              int yci=(int)floor(0.5+y*exp(log(1.2)*-si));
              for (int xx=xci-1; xx<=xci+1; xx++)
                for (int yy=yci-1; yy<=yci+1; yy++)
                {
                  int val=0;
                  if (si+s>=0 && si+s<numScales)
                  {
                    ByteImage *image=imageScale[si+s];
                    if (xx>=0 && yy>=0 && xx<image->width && yy<image->height)
                      val=(image->data(xx,yy) & (1<<net)) ? 1 : 0;
                  }
                  fprintf(outf," %d",val);
                }
            }
          }
          fprintf(outf,"\n");
        }
  }
  fclose(outf);
  for (i=0; i<numScales; i++)
    delete imageScale[i];
  delete[] imageScale;
}

#ifndef NoTcl
void PhotoBookGenerate(ByteImage *images[], ByteImage *inputs[], int numImages,
                       int s, int x, int y, int val, FILE *outf, 
                       ByteImage *mask, Tcl_Interp *interp,
                       FILE *text, char *shortName)
{
  if (s>=0 && s<numImages &&
      x>=0 && x<images[s]->width &&
      y>=0 && y<images[s]->height &&
      images[s]->data(x,y)==val)
  {
    ByteImage *face=FindByteImageWithName("face");
    face->ZeroBuffer(mask->width,mask->height);
    for (int i=x-mask->width/2; i<x+mask->width/2; i++)
      for (int j=y-mask->height/2; j<y+mask->height/2; j++)
        if (i>=0 && j>=0 && i<images[s]->width && j<images[s]->height)
          face->data(i-(x-mask->width/2),j-(y-mask->height/2))=
            inputs[s]->data(i,j);
    Tcl_Eval(interp,"NormFace face; update");

    signed char *dat=(signed char *)malloc(face->size);
    int ptr=0;
    for (int j=0; j<mask->height; j++) for (int i=0; i<mask->width; i++)
      dat[ptr++]=(signed char)(floor(face->data(i,j)-127));
    fwrite((char*)dat,sizeof(signed char),ptr,outf);
    fflush(outf);
    free((char*)dat);
    fprintf(text,"%s %d %d %d\n",shortName,x-mask->width/2,y-mask->height/2,s);
    fflush(text);
    
    images[s]->data(x,y)=0;
    PhotoBookGenerate(images,inputs,numImages,s,x+1,y,val,outf,mask,interp,
                      text,shortName);
    PhotoBookGenerate(images,inputs,numImages,s,x-1,y,val,outf,mask,interp,
                      text,shortName);
    PhotoBookGenerate(images,inputs,numImages,s,x,y+1,val,outf,mask,interp,
                      text,shortName);
    PhotoBookGenerate(images,inputs,numImages,s,x,y-1,val,outf,mask,interp,
                      text,shortName);
    PhotoBookGenerate(images,inputs,numImages,
                      s-1,(int)floor(0.5+x*1.2),(int)floor(0.5+y*1.2),val,
                      outf,mask,interp,
                      text,shortName);
    PhotoBookGenerate(images,inputs,numImages,
                      s+1,(int)floor(0.5+x/1.2),(int)floor(0.5+y/1.2),val,
                      outf,mask,interp,
                      text,shortName);
  }
}
#endif

#ifndef NoTcl
int ProducePhotoBookCmd(ClientData data, Tcl_Interp *interp,
                        int argc, char *argv[])
{
  if (argc!=12)
  {
    interp->result="Usage: ProducePhotoBook <file> <locationfilelist> <answerfile> <mask> <detects> <falseneg> <falsepos> <detects.txt> <falseneg.txt> <falsepos.txt> <shortname>";
    return TCL_OK;
  }
  char *imageFile=argv[1];
  char *locationFileList=argv[2];
  char *answerFile=argv[3];
  ByteImage *mask=FindByteImageWithName(argv[4]);
  FILE *detectFile=fopen(argv[5],"a");
  FILE *falseNegFile=fopen(argv[6],"a");
  FILE *falsePosFile=fopen(argv[7],"a");
  FILE *detectTextFile=fopen(argv[8],"a");
  FILE *falseNegTextFile=fopen(argv[9],"a");
  FILE *falsePosTextFile=fopen(argv[10],"a");
  char *shortName=argv[11];
  ByteImage *image=new ByteImage(NULL);
  char cmd[1024];
  sprintf(cmd,"giftopnm %s 2> /dev/null",imageFile);
  FILE *inf=popen(cmd,"r");
  LoadByteImagePgm(image,inf);
  pclose(inf);
  int temWidth=image->width, temHeight=image->height;
  int numScales=0;
  while (temWidth>=mask->width && temHeight>=mask->height)
  {
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }
  ByteImage **imageScale=new ByteImage*[numScales];
  ByteImage **inputScale=new ByteImage*[numScales];
  temWidth=image->width;
  temHeight=image->height;
  for (int i=0; i<numScales; i++)
  {
    imageScale[i]=new ByteImage(NULL);
    imageScale[i]->ZeroBuffer(temWidth,temHeight);
    if (i==0) inputScale[i]=image; else
    {
      inputScale[i]=new ByteImage(NULL);
//      CopyByteImage(inputScale[i],inputScale[i-1]);
//      SmoothByteImage(inputScale[i]);
//      ZoomByteImage(inputScale[i],1.2,inputScale[i]);
      ReduceSize(inputScale[i],inputScale[i-1]);
    }
//    if (inputScale[i]->width!=temWidth ||
//        inputScale[i]->height!=temHeight)
//      exit(1);
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
  }
  int numFaces=0;
  int done=0;

  inf=fopen(answerFile,"r");
  while (1)
  {
    char fileName[1024];
    double xe1, ye1, xe2, ye2, xn, yn, xl, yl, xc, yc, xr, yr;
    int num=fscanf(inf,"%s {leye %lf %lf} {reye %lf %lf} {nose %lf %lf} "
                   "{lmouth %lf %lf} {cmouth %lf %lf} {rmouth %lf %lf}",
                   &(fileName[0]),
                   &xe1, &ye1, &xe2, &ye2, &xn, &yn,
                   &xl, &yl, &xc, &yc, &xr, &yr);
    if (num<=0) break;
    if (strcmp(fileName,imageFile)==0) 
    {
      double xe=(xe1+xe2)/2.0;
      double ye=(ye1+ye2)/2.0;
      double scale=12.0/(sqrt((xe-xc)*(xe-xc)+(ye-yc)*(ye-yc)));
      double xcenter=(xe+xc)/2.0;
      double ycenter=(ye+yc)/2.0-scale/12.0;
      int s=(int)floor(0.5-log(scale)/log(1.2));
      if (s<0) s=0;
      if (s>=numScales) s=numScales-1;
      int xx=(int)floor(0.5+xcenter*exp(log(1.2)*-s));
      int yy=(int)floor(0.5+ycenter*exp(log(1.2)*-s));
      ByteImage *image=imageScale[s];
      if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
        image->data(xx,yy)=1;
      numFaces++;
    }
  }
  fclose(inf);

  char **locFileList;
  int numLocFiles;
  Tcl_SplitList(interp,locationFileList,&numLocFiles,&locFileList);
  for (i=0; i<numLocFiles; i++)
  {
    inf=fopen(locFileList[i],"r");
    do {
      int x, y, s;
      double confidence;
      int num=fscanf(inf,"%*g %*g %*g %*g %d %d %d %lf\n",
                     &x,&y,&s,&confidence);
      if (num<=0) break;
      if (confidence>0.0)
      {
        int xc=x+(mask->width/2);
        int yc=y+(mask->height/2);
        int found=0;
        for (int si=-2; si<=2; si++)
        {
          if (si+s<0 || si+s>=numScales) continue;
          int xci=(int)floor(0.5+xc*exp(log(1.2)*-si));
          int yci=(int)floor(0.5+yc*exp(log(1.2)*-si));
          ByteImage *image=imageScale[si+s];
          for (int xx=xci-4; xx<=xci+4; xx++)
            for (int yy=yci-4; yy<=yci+4; yy++)
              if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
                if (image->data(xx,yy) & 1)
                {
                  found=1;
                  image->data(xx,yy)|=16;
                }
        }
        if (s<0 || s>=numScales || xc<0 || yc<0 ||
            xc>=imageScale[s]->width || yc>=imageScale[s]->height) {
//          exit(1);
        } else {
        if (found)
          imageScale[s]->data(xc,yc)|=2; else
            imageScale[s]->data(xc,yc)|=4;
        }
      }
    } while (!done);
    fclose(inf);
  }
  free((char*)locFileList);

  for (int s=0; s<numScales; s++)
  {
    ByteImage *image=imageScale[s];
    for (int x=0; x<image->width; x++)
      for (int y=0; y<image->height; y++)
      {
        if (image->data(x,y) & 2) image->data(x,y)=2;
        if (image->data(x,y) & 16) image->data(x,y) = image->data(x,y) &
                                     (8+4+2);
        if (image->data(x,y)==1)
          for (int ss=s-1; ss<=s+1; ss++)
            if (ss>=0 && ss<numScales)
            {
              ByteImage *img=imageScale[ss];
              int xc=(int)floor(0.5+x*exp(log(1.2)*-(ss-s)));
              int yc=(int)floor(0.5+y*exp(log(1.2)*-(ss-s)));
              for (int xx=xc-1; xx<=xc+1; xx++)
                for (int yy=yc-1; yy<=yc+1; yy++)
                  if (xx>=0 && yy>=0 && xx<img->width && yy<img->height)
                    img->data(xx,yy)=8;
            }
      }
  }

  for (s=0; s<numScales; s++)
  {
    ByteImage *image=imageScale[s];
    for (int x=0; x<image->width; x++)
      for (int y=0; y<image->height; y++)
        switch (image->data(x,y))
        {
          case 0:
            break;
          case 2:
            PhotoBookGenerate(imageScale,inputScale,numScales,
                              s,x,y,2,detectFile,mask,interp,
                              detectTextFile,shortName);
            break;
          case 4:
            PhotoBookGenerate(imageScale,inputScale,numScales,
                              s,x,y,4,falsePosFile,mask,interp,
                              falsePosTextFile,shortName);
            break;
          case 8:
            PhotoBookGenerate(imageScale,inputScale,numScales,
                              s,x,y,8,falseNegFile,mask,interp,
                              falseNegTextFile,shortName);
            break;
          default:
            fprintf(stderr,"This isn't supposed to happen: %d\n",
                    image->data(x,y));
            exit(1);
            break;
        }
  }

  for (i=0; i<numScales; i++)
  {
    delete imageScale[i];
    delete inputScale[i];
  }
  delete[] imageScale;
  delete[] inputScale;

  fclose(detectFile);
  fclose(falseNegFile);
  fclose(falsePosFile);
  fclose(detectTextFile);
  fclose(falseNegTextFile);
  fclose(falsePosTextFile);

  return TCL_OK;
}
#endif

#ifndef NoTcl
int CheckSearchInstrumentedUmeCmd(ClientData data, Tcl_Interp *interp,
                                  int argc, char *argv[])
{
  if (argc!=6)
  {
    interp->result="Usage: CheckSeachInstrumentedUme <numpasses> <imagefile> <answers> <mask> <output>";
    return TCL_OK;
  }
  int numScales=atoi(argv[1]);
  char *imageFile=argv[2];
  char *answerFile=argv[3];
  ByteImage *mask=FindByteImageWithName(argv[4]);
  FILE *outf=fopen(argv[5],"w");

  ByteImage **outputScale=new ByteImage*[numScales];
  ByteImage **inputScale=new ByteImage*[numScales];
  for (int i=0; i<numScales; i++)
  {
    char name[256];
    sprintf(name,"output%d",i);
    outputScale[i]=FindByteImageWithName(name);
    sprintf(name,"input%d",i);
    inputScale[i]=new ByteImage(name);
    inputScale[i]->ZeroBuffer(outputScale[i]->width,
                              outputScale[i]->height);
  }
  int numFaces=0;
  FILE *inf=fopen(answerFile,"r");
  while (1)
  {
    char fileName[1024];
    double xe1, ye1, xe2, ye2, xn, yn, xl, yl, xc, yc, xr, yr;
    int num=fscanf(inf,"%s {leye %lf %lf} {reye %lf %lf} {nose %lf %lf} "
                   "{lmouth %lf %lf} {cmouth %lf %lf} {rmouth %lf %lf}",
                   &(fileName[0]),
                   &xe1, &ye1, &xe2, &ye2, &xn, &yn,
                   &xl, &yl, &xc, &yc, &xr, &yr);
    if (num<=0) break;
    if (strcmp(fileName,imageFile)==0) 
    {
      double xe=(xe1+xe2)/2.0;
      double ye=(ye1+ye2)/2.0;
      double scale=12.0/(sqrt((xe-xc)*(xe-xc)+(ye-yc)*(ye-yc)));
      double xcenter=(xe+xc)/2.0;
      double ycenter=(ye+yc)/2.0-scale/12.0;
      char cmd[1024];
      int s=(int)floor(0.5-log(scale)/log(1.2));
      if (s<0) s=0;
      if (s>=numScales) s=numScales-1;
      int xx=((int)floor(0.5+xcenter*exp(log(1.2)*-s)))/2;
      int yy=((int)floor(0.5+ycenter*exp(log(1.2)*-s)))/2;
      ByteImage *image=inputScale[s];
      if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
        image->data(xx,yy)=1;
      numFaces++;
    }
  }
  fclose(inf);

  int falsePos=0, facesFound=0;
  for (int val=255; val>=0; val--)
  {
    int numVal=0;
    for (int s=0; s<numScales; s++)
    {
      ByteImage *output=outputScale[s];
      for (int y=0; y<output->height; y++)
        for (int x=0; x<output->width; x++)
          if (output->data(x,y)==val)
          {
            int found=0;
            for (int si=-2; si<=2; si++)
            {
              if (si+s<0 || si+s>=numScales) continue;
              int xci=(int)floor(0.5+x*exp(log(1.2)*-si));
              int yci=(int)floor(0.5+y*exp(log(1.2)*-si));
              ByteImage *image=inputScale[si+s];
              for (int xx=xci-2; xx<=xci+2; xx++)
                for (int yy=yci-2; yy<=yci+2; yy++)
                  if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
                    if (image->data(xx,yy))
                    {
                      found=1;
                      if (image->data(xx,yy)==1) facesFound++;
                      image->data(xx,yy)=2;
                    }
            }
            if (!found) falsePos++;
            numVal++;
          }
    }
    fprintf(outf,"%d %d %d %d\n",
            val,numVal,falsePos,numFaces-facesFound);
  }
  for (i=0; i<numScales; i++)
  {
    delete inputScale[i];
    delete outputScale[i];
  }
  delete[] inputScale;
  delete[] outputScale;
  fclose(outf);
  return TCL_OK;
}
#endif

#ifndef NoTcl
int CheckSearchInstrumentedCmd(ClientData data, Tcl_Interp *interp,
                               int argc, char *argv[])
{
  if (argc!=6)
  {
    interp->result="Usage: CheckSeachInstrumented <numpasses> <imagefile> <answers> <mask> <output>";
    return TCL_OK;
  }
  int numScales=atoi(argv[1]);
  char *imageFile=argv[2];
  char *answerFile=argv[3];
  ByteImage *mask=FindByteImageWithName(argv[4]);
  FILE *outf=fopen(argv[5],"w");

  ByteImage **outputScale=new ByteImage*[numScales];
  ByteImage **inputScale=new ByteImage*[numScales];
  for (int i=0; i<numScales; i++)
  {
    char name[256];
    sprintf(name,"output%d",i);
    outputScale[i]=FindByteImageWithName(name);
    sprintf(name,"input%d",i);
    inputScale[i]=new ByteImage(name);
    inputScale[i]->ZeroBuffer(outputScale[i]->width,
                              outputScale[i]->height);
  }
  int numFaces=0;
  FILE *inf=fopen(answerFile,"r");
  while (1)
  {
    char fileName[1024];
    double xe1, ye1, xe2, ye2, xn, yn, xl, yl, xc, yc, xr, yr;
    int num=fscanf(inf,"%s {leye %lf %lf} {reye %lf %lf} {nose %lf %lf} "
                   "{lmouth %lf %lf} {cmouth %lf %lf} {rmouth %lf %lf}",
                   &(fileName[0]),
                   &xe1, &ye1, &xe2, &ye2, &xn, &yn,
                   &xl, &yl, &xc, &yc, &xr, &yr);
    if (num<=0) break;
    if (strcmp(fileName,imageFile)==0) 
    {
      double xe=(xe1+xe2)/2.0;
      double ye=(ye1+ye2)/2.0;
      double scale=12.0/(sqrt((xe-xc)*(xe-xc)+(ye-yc)*(ye-yc)));
      double xcenter=(xe+xc)/2.0;
      double ycenter=(ye+yc)/2.0-scale/12.0;
      char cmd[1024];
      int s=(int)floor(0.5-log(scale)/log(1.2));
      if (s<0) s=0;
      if (s>=numScales) s=numScales-1;
      int xx=(int)floor(0.5+xcenter*exp(log(1.2)*-s));
      int yy=(int)floor(0.5+ycenter*exp(log(1.2)*-s));
      ByteImage *image=inputScale[s];
      if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
        image->data(xx,yy)=1;
      numFaces++;
    }
  }
  fclose(inf);

  int falsePos=0, facesFound=0;
  for (int val=200; val>=0; val--)
  {
    int numVal=0;
    for (int s=0; s<numScales; s++)
    {
      ByteImage *output=outputScale[s];
      for (int y=0; y<output->height; y++)
        for (int x=0; x<output->width; x++)
          if (output->data(x,y)==val)
          {
            int xc=x+(mask->width/2);
            int yc=y+(mask->height/2);
            int found=0;
            for (int si=-3; si<=3; si++)
            {
              if (si+s<0 || si+s>=numScales) continue;
              int xci=(int)floor(0.5+xc*exp(log(1.2)*-si));
              int yci=(int)floor(0.5+yc*exp(log(1.2)*-si));
              ByteImage *image=inputScale[si+s];
              int size=(int)floor(0.5+6*pow(1.2,-si));
              for (int xx=xci-size; xx<=xci+size; xx++)
                for (int yy=yci-size; yy<=yci+size; yy++)
                  if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
                    if (image->data(xx,yy))
                    {
                      found=1;
                      if (image->data(xx,yy)==1) facesFound++;
                      image->data(xx,yy)=2;
                    }
            }
            if (!found) falsePos++;
            numVal++;
          }
    }
    fprintf(outf,"%.2f %d %d %d\n",
            (val-100)/100.0,numVal,falsePos,numFaces-facesFound);
  }
  for (i=0; i<numScales; i++)
  {
    delete inputScale[i];
    delete outputScale[i];
  }
  delete[] inputScale;
  delete[] outputScale;
  fclose(outf);
  return TCL_OK;
}
#endif

void CheckSearchInstrumentedDirect(int numScales, char *imageFile,
                                   Tcl_HashTable *answers, ByteImage *mask,
                                   char *tradeName)
{
  FILE *outf=fopen(tradeName,"w");

  ByteImage **outputScale=new ByteImage*[numScales];
  ByteImage **inputScale=new ByteImage*[numScales];
  for (int i=0; i<numScales; i++)
  {
    char name[256];
    sprintf(name,"output%d",i);
    outputScale[i]=FindByteImageWithName(name);
    sprintf(name,"input%d",i);
    inputScale[i]=new ByteImage(name);
    inputScale[i]->ZeroBuffer(outputScale[i]->width,
                              outputScale[i]->height);
  }
  int numFaces=0;

  Tcl_HashEntry *entry=Tcl_FindHashEntry(answers,imageFile);
  if (entry!=NULL) {
    List<Detection>* detections=(List<Detection>*)Tcl_GetHashValue(entry);
    for (Detection *d=detections->first; d!=NULL; d=d->next) {
      int s=d->s;
      int xx=d->x;
      int yy=d->y;
      LimitScale(0,numScales-1,&s,&xx,&yy);
      ByteImage *image=inputScale[s];
      if (xx<0) xx=0;
      if (yy<0) yy=0;
      if (xx>=image->width) xx=image->width-1;
      if (yy>=image->height) yy=image->height-1;
      image->data(xx,yy)=1;
      numFaces++;
    }
  }
  fprintf(stderr,"Num faces (correct) = %d\n",numFaces);

  int falsePos=0, facesFound=0;
  for (int val=200; val>=0; val--)
  {
    int numVal=0;
    for (int s=0; s<numScales; s++)
    {
      ByteImage *output=outputScale[s];
      for (int y=0; y<output->height; y++)
        for (int x=0; x<output->width; x++)
          if (output->data(x,y)==val)
          {
            int xc=x;
            int yc=y;
            int found=0;
            for (int si=-4; si<=4; si++)
            {
              if (si+s<0 || si+s>=numScales) continue;
              int xci=(int)floor(0.5+xc*exp(log(1.2)*-si));
              int yci=(int)floor(0.5+yc*exp(log(1.2)*-si));
              ByteImage *image=inputScale[si+s];
//              int size=(int)floor(0.5+6*pow(1.2,-si));
              int size=4;
              for (int xx=xci-size; xx<=xci+size; xx++)
                for (int yy=yci-size; yy<=yci+size; yy++)
                  if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
                    if (image->data(xx,yy))
                    {
                      found=1;
                      if (image->data(xx,yy)==1) facesFound++;
                      image->data(xx,yy)=2;
                    }
            }
            if (!found) falsePos++;
            numVal++;
          }
    }
    fprintf(outf,"%.2f %d %d %d\n",
            (val-100)/100.0,numVal,falsePos,numFaces-facesFound);
  }
  for (i=0; i<numScales; i++)
  {
    delete inputScale[i];
    delete outputScale[i];
  }
  delete[] inputScale;
  delete[] outputScale;
  fclose(outf);
}

void FindCentroid(ByteImage *images[], int numImages,
                  int s, int x, int y, int val,
                  double *totalS, double *totalX, double *totalY,
                  double *total)
{
  if (s>=0 && s<numImages &&
      x>=0 && x<images[s]->width &&
      y>=0 && y<images[s]->height &&
      images[s]->data(x,y)>=val)
  {
    (*total)+=images[s]->data(x,y);
    (*totalS)+=s*images[s]->data(x,y);
    (*totalX)+=x*exp(log(1.2)*s)*images[s]->data(x,y);
    (*totalY)+=y*exp(log(1.2)*s)*images[s]->data(x,y);
    images[s]->data(x,y)=0;
    FindCentroid(images,numImages,s,x+1,y,val,
                 totalS,totalX,totalY,total);
    FindCentroid(images,numImages,s,x-1,y,val,
                 totalS,totalX,totalY,total);
    FindCentroid(images,numImages,s,x,y+1,val,
                 totalS,totalX,totalY,total);
    FindCentroid(images,numImages,s,x,y-1,val,
                 totalS,totalX,totalY,total);
    FindCentroid(images,numImages,
                 s-1,(int)floor(0.5+x*1.2),(int)floor(0.5+y*1.2),val,
                 totalS,totalX,totalY,total);
    FindCentroid(images,numImages,
                 s+1,(int)floor(0.5+x/1.2),(int)floor(0.5+y/1.2),val,
                 totalS,totalX,totalY,total);
  }
}

#ifndef NoTcl
int FuzzyVoteCmd(ClientData data, Tcl_Interp *interp,
                 int argc, char *argv[])
{
  if (argc!=7)
  {
    interp->result="Usage: FuzzyVote <width> <height> [location-files] <outputfile> <or|and|vote> <spread>";
    return TCL_OK;
  }
  int width=atoi(argv[1]);
  int height=atoi(argv[2]);
  char *locFiles=argv[3];
  char *outFile=argv[4];
  char *mode=argv[5];
  int spread=atoi(argv[6]);
  int temWidth=width, temHeight=height;
  int numScales=0;
  while (temWidth>=20 && temHeight>=20)
  {
    char name[256];
    sprintf(name,"scale%d",numScales);
    ByteImage *image=new ByteImage(name);
    image->ZeroBuffer(temWidth,temHeight);
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }
  ByteImage **imageScale=new ByteImage*[numScales];
  for (int i=0; i<numScales; i++)
  {
    char name[256];
    sprintf(name,"scale%d",i);
    imageScale[i]=FindByteImageWithName(name);
  }
  int numFaces=0;
  int done=0;
  char **locFileList;
  int numLocFiles;
  Tcl_SplitList(interp,locFiles,&numLocFiles,&locFileList);
  for (int num=0; num<numLocFiles; num++)
  {
    int numFaces=0;
    int done=0;
    FILE *inf=fopen(locFileList[num],"r");
    do {
      int x, y, s;
      int numread=fscanf(inf,"%*g %*g %*g %*g %d %d %d %*g\n",&x,&y,&s);
      if (numread<0) done=1; else
      {
        int xc=x+10;
        int yc=y+10;
        for (int si=-spread; si<=spread; si++)
        {
          if (si+s<0 || si+s>=numScales) continue;
          int xci=(int)floor(0.5+xc*exp(log(1.2)*-si));
          int yci=(int)floor(0.5+yc*exp(log(1.2)*-si));
          ByteImage *image=imageScale[si+s];
          for (int xx=xci-spread; xx<=xci+spread; xx++)
            for (int yy=yci-spread; yy<=yci+spread; yy++)
              if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
                image->data(xx,yy)|=(1<<num);
        }
        numFaces++;
      }
    } while (!done);
    fclose(inf);
    fprintf(stderr,"numFaces=%d\n",numFaces);
  }
  free((char*)locFileList);
  int search=0;
  if (strcmp(mode,"and")==0) search=(1 << numLocFiles)-1;
  if (strcmp(mode,"or")==0)
  {
    search=1;
    for (int s=0; s<numScales; s++)
    {
      ByteImage *image=imageScale[s];
      for (int i=0; i<image->size; i++)
        if (image->data(i)!=0) image->data(i)=1; else image->data(i)=0;
    }
  }
  int max=search;
  if (strcmp(mode,"vote")==0)
  {
    search=(numLocFiles+1)/2;
    int popcount[256];
    for (int i=0; i<256; i++)
    {
      popcount[i]=0;
      for (int bit=0; bit<8; bit++) if (i & (1<<bit)) popcount[i]++;
//      if (popcount[i]>search) popcount[i]=search;
    }
    for (int s=0; s<numScales; s++)
    {
      ByteImage *image=imageScale[s];
      for (int i=0; i<image->size; i++)
      {
        image->data(i)=popcount[image->data(i)];
	if (image->data(i)>max) max=image->data(i);
      }
    }
  }    
  FILE *outf=fopen(outFile,"w");
  for (int val=max; val>=search; val--)
  {
    fprintf(stderr,"Scanning for value %d...\n",val);
    for (int s=0; s<numScales; s++)
    {
      ByteImage *image=imageScale[s];
      for (int x=0; x<image->width; x++)
        for (int y=0; y<image->height; y++)
          if (image->data(x,y)==val)
          {
//            double total=0;
//            double totalS=0, totalX=0, totalY=0;
//            FindCentroid(imageScale,numScales,
//                         s,x,y,val,&totalS,&totalX,&totalY,&total);
            int total=1;
            double totalS=s, totalX=x*pow(1.2,s), totalY=y*pow(1.2,s);
            int cs=(int)floor(0.5+totalS/total);
            int cx=(int)floor(0.5+totalX/total*exp(log(1.2)*-cs));
            int cy=(int)floor(0.5+totalY/total*exp(log(1.2)*-cs));

            // Now zap all the nearby faces
#ifdef notdef
            for (int scale=0; scale<numScales; scale++)
            {
              int xpos=(int)floor(0.5+cx*(exp(log(1.2)*(cs-scale))));
              int ypos=(int)floor(0.5+cy*(exp(log(1.2)*(cs-scale))));
              int size=10+(int)floor(0.5+10*(exp(log(1.2)*(cs-scale))));
              ByteImage *img=imageScale[scale];
              for (int xx=xpos-size; xx<=xpos+size; xx++)
                for (int yy=ypos-size; yy<=ypos+size; yy++)
                  if (xx>=0 && xx<img->width && yy>=0 && yy<img->height)
                    img->data(xx,yy)=0;
            }
#endif
            int xpos=(int)floor(0.5+cx*(exp(log(1.2)*cs)));
            int ypos=(int)floor(0.5+cy*(exp(log(1.2)*cs)));
            int size=(int)floor(0.5+10*(exp(log(1.2)*cs)));
            fprintf(stderr,"%d %d %d %d %d %d %d 1.0\n",xpos-size,ypos-size,
                    xpos+size,ypos+size,
                    cx-10,cy-10,cs);
            fprintf(outf,"%d %d %d %d %d %d %d 1.0\n",xpos-size,ypos-size,
                    xpos+size,ypos+size,
                    cx-10,cy-10,cs);
          }
    }
  }
  fclose(outf);
  for (i=0; i<numScales; i++)
    delete imageScale[i];
  delete[] imageScale;
  return TCL_OK;
}
#endif

#ifndef NoTcl
int CountVotesCmd(ClientData data, Tcl_Interp *interp,
                  int argc, char *argv[])
{
  if (argc!=7)
  {
    interp->result="Usage: CountVotes <width> <height> [location-files] <outputfile> <answerfile> <imageFile>";
    return TCL_OK;
  }
  int width=atoi(argv[1]);
  int height=atoi(argv[2]);
  char *locFiles=argv[3];
  char *outFile=argv[4];
  char *answerFile=argv[5];
  char *imageFile=argv[6];
  int temWidth=width, temHeight=height;
  int numScales=0;
  while (temWidth>=20 && temHeight>=20)
  {
    char name[256];
    sprintf(name,"scale%d",numScales);
    ByteImage *image=new ByteImage(name);
    image->ZeroBuffer(temWidth,temHeight);
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }
  ByteImage **imageScale=new ByteImage*[numScales];
  for (int i=0; i<numScales; i++)
  {
    char name[256];
    sprintf(name,"scale%d",i);
    imageScale[i]=FindByteImageWithName(name);
  }
  int numFaces=0;
  int done=0;

  FILE *inf=fopen(answerFile,"r");
  do {
    char fileName[1024];
    double xe1, ye1, xe2, ye2, xn, yn, xl, yl, xc, yc, xr, yr;
    int num=fscanf(inf,"%s {leye %lf %lf} {reye %lf %lf} {nose %lf %lf} "
                   "{lmouth %lf %lf} {cmouth %lf %lf} {rmouth %lf %lf}",
                   &(fileName[0]),
                   &xe1, &ye1, &xe2, &ye2, &xn, &yn,
                   &xl, &yl, &xc, &yc, &xr, &yr);
    if (num<=0) done=1; else
      if (strcmp(fileName,imageFile)==0) 
      {
        double xe=(xe1+xe2)/2.0;
        double ye=(ye1+ye2)/2.0;
        double scale=12.0/(sqrt((xe-xc)*(xe-xc)+(ye-yc)*(ye-yc)));
        double xcenter=(xe+xc)/2.0;
        double ycenter=(ye+yc)/2.0-scale/12.0;
        int s=(int)floor(0.5-log(scale)/log(1.2));
        if (s<0) s=0;
        if (s>=numScales) s=numScales-1;
        int xx=(int)floor(0.5+xcenter*exp(log(1.2)*-s));
        int yy=(int)floor(0.5+ycenter*exp(log(1.2)*-s));
        ByteImage *image=imageScale[s];
        if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
          image->data(xx,yy)=0x80;
        numFaces++;
      }
  } while (!done);
  fclose(inf);

  char **locFileList;
  int numLocFiles;
  Tcl_SplitList(interp,locFiles,&numLocFiles,&locFileList);
  for (int num=0; num<numLocFiles; num++)
  {
    int numFaces=0;
    int done=0;
    FILE *inf=fopen(locFileList[num],"r");
    do {
      int x, y, s;
      char buf[1024];
      if (fgets(buf,1024,inf)==NULL) { done=1; break; }
      int numread=sscanf(buf,"%*g %*g %*g %*g %d %d %d %*g\n",&x,&y,&s);
      ByteImage *image=imageScale[s];
      image->data(x+10,y+10)|=(1<<num);
      numFaces++;
    } while (!done);
    fclose(inf);
    fprintf(stderr,"numFaces=%d\n",numFaces);
  }
  free((char*)locFileList);

  FILE *outf=fopen(outFile,"a");
  for (int s=0; s<numScales; s++)
  {
    ByteImage *image=imageScale[s];
    for (int x=0; x<image->width; x++)
      for (int y=0; y<image->height; y++)
        if (image->data(x,y) & 0x7F)
        {
          int correct=0;
          for (int si=-3; si<=3; si++)
          {
            if (si+s<0 || si+s>=numScales) continue;
            int xci=(int)floor(0.5+x*exp(log(1.2)*-si));
            int yci=(int)floor(0.5+y*exp(log(1.2)*-si));
            ByteImage *image=imageScale[si+s];
            int sspread=(int)floor(0.5+6*pow(1.2,-si));
            for (int xx=xci-sspread; xx<=xci+sspread; xx++)
              for (int yy=yci-sspread; yy<=yci+sspread; yy++)
                if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
                  if (image->data(xx,yy) & 0x80)
                    correct=1;
          }
          fprintf(outf,"%d",correct);
          for (int net=0; net<numLocFiles; net++)
          {
            for (int si=-1; si<=1; si++)
            {
              int xci=(int)floor(0.5+x*exp(log(1.2)*-si));
              int yci=(int)floor(0.5+y*exp(log(1.2)*-si));
              for (int xx=xci-1; xx<=xci+1; xx++)
                for (int yy=yci-1; yy<=yci+1; yy++)
                {
                  int val=0;
                  if (si+s>=0 && si+s<numScales)
                  {
                    ByteImage *image=imageScale[si+s];
                    if (xx>=0 && yy>=0 && xx<image->width && yy<image->height)
                      val=(image->data(xx,yy) & (1<<net)) ? 1 : 0;
                  }
                  fprintf(outf," %d",val);
                }
            }
          }
          fprintf(outf,"\n");
        }
  }
  fclose(outf);
  for (i=0; i<numScales; i++)
    delete imageScale[i];
  delete[] imageScale;
  return TCL_OK;
}
#endif

#ifndef NoTcl
int CountVotes2Cmd(ClientData data, Tcl_Interp *interp,
                   int argc, char *argv[])
{
  if (argc!=7)
  {
    interp->result="Usage: CountVotes2 <width> <height> [location-files] <outputfile> <answerfile> <imageFile>";
    return TCL_OK;
  }
  int width=atoi(argv[1]);
  int height=atoi(argv[2]);
  char *locFiles=argv[3];
  char *outFile=argv[4];
  char *answerFile=argv[5];
  char *imageFile=argv[6];
  int temWidth=width, temHeight=height;
  int numScales=0;
  while (temWidth>=20 && temHeight>=20)
  {
    char name[256];
    sprintf(name,"scale%d",numScales);
    ByteImage *image=new ByteImage(name);
    image->ZeroBuffer(temWidth,temHeight);
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }
  ByteImage **imageScale=new ByteImage*[numScales];
  for (int i=0; i<numScales; i++)
  {
    char name[256];
    sprintf(name,"scale%d",i);
    imageScale[i]=FindByteImageWithName(name);
  }
  int numFaces=0;
  int done=0;

  FILE *inf=fopen(answerFile,"r");
  do {
    int x, y, s;
    char fileName[1024];
    double xe1, ye1, xe2, ye2, xn, yn, xl, yl, xc, yc, xr, yr;
    int num=fscanf(inf,"%s {leye %lf %lf} {reye %lf %lf} {nose %lf %lf} "
                   "{lmouth %lf %lf} {cmouth %lf %lf} {rmouth %lf %lf}",
                   &(fileName[0]),
                   &xe1, &ye1, &xe2, &ye2, &xn, &yn,
                   &xl, &yl, &xc, &yc, &xr, &yr);
    if (num<=0) done=1; else
      if (strcmp(fileName,imageFile)==0) 
      {
        double xe=(xe1+xe2)/2.0;
        double ye=(ye1+ye2)/2.0;
        double scale=12.0/(sqrt((xe-xc)*(xe-xc)+(ye-yc)*(ye-yc)));
        double xcenter=(xe+xc)/2.0;
        double ycenter=(ye+yc)/2.0-scale/12.0;
        int s=(int)floor(0.5-log(scale)/log(1.2));
        if (s<0) s=0;
        if (s>=numScales) s=numScales-1;
        int xx=(int)floor(0.5+xcenter*exp(log(1.2)*-s));
        int yy=(int)floor(0.5+ycenter*exp(log(1.2)*-s));
        ByteImage *image=imageScale[s];
        if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
          image->data(xx,yy)=0x80;
        numFaces++;
      }
  } while (!done);
  fclose(inf);

  char **locFileList;
  int numLocFiles;
  Tcl_SplitList(interp,locFiles,&numLocFiles,&locFileList);
  for (int num=0; num<numLocFiles; num++)
  {
    int numFaces=0;
    int done=0;
    FILE *inf=fopen(locFileList[num],"r");
    do {
      int x, y, s;
      int numread=fscanf(inf,"%*g %*g %*g %*g %d %d %d %*g\n",&x,&y,&s);
      if (numread<0) done=1; else
      {
        ByteImage *image=imageScale[s];
        image->data(x+10,y+10)|=(1<<num);
        numFaces++;
      }
    } while (!done);
    fclose(inf);
    fprintf(stderr,"numFaces=%d\n",numFaces);
  }
  free((char*)locFileList);

  FILE *outf=fopen(outFile,"a");
  for (int s=0; s<numScales; s++)
  {
    ByteImage *image=imageScale[s];
    for (int x=0; x<image->width; x++)
      for (int y=0; y<image->height; y++)
        if (image->data(x,y) & 0x7F)
        {
          int correct=0;
          for (int si=-3; si<=3; si++)
          {
            if (si+s<0 || si+s>=numScales) continue;
            int xci=(int)floor(0.5+x*exp(log(1.2)*-si));
            int yci=(int)floor(0.5+y*exp(log(1.2)*-si));
            ByteImage *image=imageScale[si+s];
            int sspread=(int)floor(0.5+6*pow(1.2,-si));
            for (int xx=xci-sspread; xx<=xci+sspread; xx++)
              for (int yy=yci-sspread; yy<=yci+sspread; yy++)
                if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
                  if (image->data(xx,yy) & 0x80)
                    correct=1;
          }
          fprintf(outf,"%d",correct);
          for (int net=0; net<numLocFiles; net++)
          {
            int num[3]={0,0,0};
            int tot[3]={0,0,0};
            for (int si=-2; si<=2; si++)
            {
              int xci=(int)floor(0.5+x*exp(log(1.2)*-si));
              int yci=(int)floor(0.5+y*exp(log(1.2)*-si));
              for (int xx=xci-2; xx<=xci+2; xx++)
                for (int yy=yci-2; yy<=yci+2; yy++)
                {
                  int val=0;
                  if (si+s>=0 && si+s<numScales)
                  {
                    ByteImage *image=imageScale[si+s];
                    if (xx>=0 && yy>=0 && xx<image->width && yy<image->height)
                      val=(image->data(xx,yy) & (1<<net)) ? 1 : 0;
                  }
                  int idx=0;
                  if (xx==xci && yy==yci) idx=0; else
                    if (abs(xx-xci)<=1 && abs(yy-yci)<=1) idx=1; else
                      idx=2;
                  num[idx]+=val;
                  tot[idx]++;
                }
            }
            fprintf(outf," %g %g %g",(double)num[0]/(double)tot[0],
                    (double)num[1]/(double)tot[1],
                    (double)num[2]/(double)tot[2]);
          }
          fprintf(outf,"\n");
        }
  }
  fclose(outf);
  for (i=0; i<numScales; i++)
    delete imageScale[i];
  delete[] imageScale;
  return TCL_OK;
}
#endif

#ifndef NoTcl
int NetArbitrateCmd(ClientData data, Tcl_Interp *interp,
                    int argc, char *argv[])
{
  if (argc!=5)
  {
    interp->result="Usage: NetArbitrate <width> <height> [location-files] <outputfile>";
    return TCL_OK;
  }
  int width=atoi(argv[1]);
  int height=atoi(argv[2]);
  char *locFiles=argv[3];
  char *outFile=argv[4];

  int temWidth=width, temHeight=height;
  int numScales=0;
  while (temWidth>=20 && temHeight>=20)
  {
    char name[256];
    sprintf(name,"scale%d",numScales);
    ByteImage *image=new ByteImage(name);
    image->ZeroBuffer(temWidth,temHeight);
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }
  ByteImage **imageScale=new ByteImage*[numScales];
  for (int i=0; i<numScales; i++)
  {
    char name[256];
    sprintf(name,"scale%d",i);
    imageScale[i]=FindByteImageWithName(name);
  }
  int numFaces=0;
  int done=0;

  char **locFileList;
  int numLocFiles;
  Tcl_SplitList(interp,locFiles,&numLocFiles,&locFileList);
  for (int num=0; num<numLocFiles; num++)
  {
    int numFaces=0;
    int done=0;
    FILE *inf=fopen(locFileList[num],"r");
    do {
      int x, y, s;

      char buf[1024];
      if (fgets(buf,1024,inf)==NULL) { done=1; break; }
      int numread=sscanf(buf,"%*g %*g %*g %*g %d %d %d %*lf",&x,&y,&s);
      if (numread<0) done=1; else
      {
        ByteImage *image=imageScale[s];
        image->data(x+10,y+10)|=(1<<num);
        numFaces++;
      }
    } while (!done);
    fclose(inf);
//    fprintf(stderr,"numFaces=%d\n",numFaces);
  }
  free((char*)locFileList);
//FILE *ans=fopen("/usr0/har/faces/our.votes","r");
  FILE *outf=fopen(outFile,"w");
  for (int s=0; s<numScales; s++)
  {
    ByteImage *image=imageScale[s];
    for (int x=0; x<image->width; x++)
      for (int y=0; y<image->height; y++)
        if (image->data(x,y))
        {
//          int ptr=1;
          int ptr2=1;
          for (int net=0; net<numLocFiles; net++)
          {
            for (int si=-1; si<=1; si++)
            {
              int tot=0;
              int xci=(int)floor(0.5+x*exp(log(1.2)*-si));
              int yci=(int)floor(0.5+y*exp(log(1.2)*-si));
              for (int xx=xci-1; xx<=xci+1; xx++)
                for (int yy=yci-1; yy<=yci+1; yy++)
                {
                  int val=0;
                  if (si+s>=0 && si+s<numScales)
                  {
                    ByteImage *image=imageScale[si+s];
                    if (xx>=0 && yy>=0 && xx<image->width && yy<image->height)
                      val=(image->data(xx,yy) & (1<<net)) ? 1 : 0;
                  }
//                  real_fast_list[0]->unitList[ptr++].activation=val;
                  tot+=val;
                }
              real_fast_list[0]->unitList[ptr2++].activation=tot/9.0;
            }
          }
          double out=FastForwardPass(real_fast_list[0]);
//char ansStr[1024];
//fgets(ansStr,1024,ans);
int ansVal=0;
//sscanf(ansStr,"%d",&ansVal);
          if (out>0.0)
          {
#ifdef notdef
            fprintf(stderr,"%s %g %d %d %d %d %d %d %d %g\n",
		    ansVal?"POS":"NEG",out,
                    (int)floor(0.5+(x-10)*pow(1.2,s)),
                    (int)floor(0.5+(y-10)*pow(1.2,s)),
                    (int)floor(0.5+(x+10)*pow(1.2,s)),
                    (int)floor(0.5+(y+10)*pow(1.2,s)),
                    x-10,y-10,s,out);
#endif
            fprintf(outf,"%d %d %d %d %d %d %d %g\n",
//		    ansVal?"POS":"NEG",out,
                    (int)floor(0.5+(x-10)*pow(1.2,s)),
                    (int)floor(0.5+(y-10)*pow(1.2,s)),
                    (int)floor(0.5+(x+10)*pow(1.2,s)),
                    (int)floor(0.5+(y+10)*pow(1.2,s)),
                    x-10,y-10,s,out);
          }
        }
  }
  fclose(outf);
  for (i=0; i<numScales; i++)
    delete imageScale[i];
  delete[] imageScale;
  return TCL_OK;
}
#endif

// USED
// Use NN arbitration to merge the outputs from three networks.
// For each network, there are three inputs to the arbitrator, which
// are the fraction of inputs in a 3x3 square in three levels of 
// the detection pyramid around the current point of interest.  The
// output from the arbitrator net is the detection output.  The arbitation
// network is applied only when one of the individual networks responds
// at the point of interest.
void NetArbitrateDirect(int width, int height, int numLocFiles,
                        List<Detection> *detections, 
                        List<Detection> *outputDetections,
                        int arbnet)
{
  // Create the detection pyramid
  int temWidth=width, temHeight=height;
  int numScales=0;
  while (temWidth>=20 && temHeight>=20) {
    char name[256];
    sprintf(name,"scale%d",numScales);
    ByteImage *image=new ByteImage(name);
    image->ZeroBuffer(temWidth,temHeight);
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }
  ByteImage **imageScale=new ByteImage*[numScales];
  for (int i=0; i<numScales; i++) {
    char name[256];
    sprintf(name,"scale%d",i);
    imageScale[i]=FindByteImageWithName(name);
  }
  int numFaces=0;
  int done=0;

  // For each of the input detection lists (networks)
  for (int num=0; num<numLocFiles; num++) {
    int numFaces=0;
    int done=0;

    // Write the detections into the detection pyramid
    for (Detection *d=detections[num].first; d!=NULL; d=d->next) {
      int x=d->x, y=d->y, s=d->s;
      ByteImage *image=imageScale[s];
      image->data(x+10,y+10)|=(1<<num);
      numFaces++;
    }
//    fprintf(stderr,"numFaces=%d\n",numFaces);
  }
//FILE *ans=fopen("/usr0/har/faces/our.votes","r");

  // Scan through the detection pyramid
  for (int s=0; s<numScales; s++) {
    ByteImage *image=imageScale[s];
    for (int x=0; x<image->width; x++)
      for (int y=0; y<image->height; y++)
	// If any of the networks responded at this point
        if (image->data(x,y)) {
//          int ptr=1;
          int ptr2=1;
	  // For each of the individual networks
          for (int net=0; net<numLocFiles; net++) {
	    // Go over the 3 scales
            for (int si=-1; si<=1; si++) {
              int tot=0;
              int xci=(int)floor(0.5+x*exp(log(1.2)*-si));
              int yci=(int)floor(0.5+y*exp(log(1.2)*-si));
	      // Total up the number of responses in a 3x3 region in this
	      // scale
              for (int xx=xci-1; xx<=xci+1; xx++)
                for (int yy=yci-1; yy<=yci+1; yy++) {
                  int val=0;
                  if (si+s>=0 && si+s<numScales) {
                    ByteImage *image=imageScale[si+s];
                    if (xx>=0 && yy>=0 && xx<image->width && yy<image->height)
                      val=(image->data(xx,yy) & (1<<net)) ? 1 : 0;
                  }
//                  real_fast_list[0]->unitList[ptr++].activation=val;
                  tot+=val;
                }
	      // Write fraction of detections into network
              real_fast_list[arbnet]->unitList[ptr2++].activation=tot/9.0;
            }
          }
	  // Apply arbitrator
          double out=FastForwardPass(real_fast_list[arbnet]);
//char ansStr[1024];
//fgets(ansStr,1024,ans);
//int ansVal=0;
//sscanf(ansStr,"%d",&ansVal);
	  if (out>0.0) {
#ifdef notdef
            fprintf(stderr,"%s %g %d %d %d %d %d %d %d %g\n",
		    ansVal?"POS":"NEG",out,
                    (int)floor(0.5+(x-10)*pow(1.2,s)),
                    (int)floor(0.5+(y-10)*pow(1.2,s)),
                    (int)floor(0.5+(x+10)*pow(1.2,s)),
                    (int)floor(0.5+(y+10)*pow(1.2,s)),
                    x-10,y-10,s,out);
#endif
	    // Record positive outputs
            outputDetections->addLast(new Detection(x-10,y-10,s,out,0));
          }
        }
  }
  // Remove detection pyramid
  for (i=0; i<numScales; i++)
    delete imageScale[i];
  delete[] imageScale;
}

#ifndef NoTcl
int NetArbitrate2Cmd(ClientData data, Tcl_Interp *interp,
                    int argc, char *argv[])
{
  if (argc!=5)
  {
    interp->result="Usage: NetArbitrate <width> <height> [location-files] <outputfile>";
    return TCL_OK;
  }
  int width=atoi(argv[1]);
  int height=atoi(argv[2]);
  char *locFiles=argv[3];
  char *outFile=argv[4];

  int temWidth=width, temHeight=height;
  int numScales=0;
  while (temWidth>=20 && temHeight>=20)
  {
    char name[256];
    sprintf(name,"scale%d",numScales);
    ByteImage *image=new ByteImage(name);
    image->ZeroBuffer(temWidth,temHeight);
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }
  ByteImage **imageScale=new ByteImage*[numScales];
  for (int i=0; i<numScales; i++)
  {
    char name[256];
    sprintf(name,"scale%d",i);
    imageScale[i]=FindByteImageWithName(name);
  }
  char **locFileList;
  int numLocFiles;
  Tcl_SplitList(interp,locFiles,&numLocFiles,&locFileList);
  for (int num=0; num<numLocFiles; num++)
  {
    int numFaces=0;
    int done=0;
    FILE *inf=fopen(locFileList[num],"r");
    do {
      int x, y, s;
      int numread=fscanf(inf,"%*g %*g %*g %*g %d %d %d %*g\n",&x,&y,&s);
      if (numread<0) done=1; else
      {
        ByteImage *image=imageScale[s];
        image->data(x+10,y+10)|=(1<<num);
        numFaces++;
      }
    } while (!done);
    fclose(inf);
    fprintf(stderr,"numFaces=%d\n",numFaces);
  }
  free((char*)locFileList);
FILE *ans=fopen("/usr0/har/faces/our.votes","r");
  FILE *outf=fopen(outFile,"w");
  for (int s=0; s<numScales; s++)
  {
    ByteImage *image=imageScale[s];
    for (int x=0; x<image->width; x++)
      for (int y=0; y<image->height; y++)
        if (image->data(x,y))
        {
          int ptr=1;
          for (int net=0; net<numLocFiles; net++)
          {
            int num[3]={0,0,0};
            int tot[3]={0,0,0};
            for (int si=-2; si<=2; si++)
            {
              int xci=(int)floor(0.5+x*exp(log(1.2)*-si));
              int yci=(int)floor(0.5+y*exp(log(1.2)*-si));
              for (int xx=xci-2; xx<=xci+2; xx++)
                for (int yy=yci-2; yy<=yci+2; yy++)
                {
                  int val=0;
                  if (si+s>=0 && si+s<numScales)
                  {
                    ByteImage *image=imageScale[si+s];
                    if (xx>=0 && yy>=0 && xx<image->width && yy<image->height)
                      val=(image->data(xx,yy) & (1<<net)) ? 1 : 0;
                  }
                  int idx=0;
                  if (xx==xci && yy==yci) idx=0; else
                    if (abs(xx-xci)<=1 && abs(yy-yci)<=1) idx=1; else
                      idx=2;
                  num[idx]+=val;
                  tot[idx]++;
                }
            }
            for (int i=0; i<3; i++)
              real_fast_list[0]->unitList[ptr++].activation=
                (double)num[i]/(double)tot[i];
          }
          double out=FastForwardPass(real_fast_list[0]);
char ansStr[1024];
fgets(ansStr,1024,ans);
int ansVal;
sscanf(ansStr,"%d",&ansVal);
//          if (out>0.0)
          {
            fprintf(stderr,"%s %g %d %d %d %d %d %d %d %g\n",
		    ansVal?"POS":"NEG",out,
                    (int)floor(0.5+(x-10)*pow(1.2,s)),
                    (int)floor(0.5+(y-10)*pow(1.2,s)),
                    (int)floor(0.5+(x+10)*pow(1.2,s)),
                    (int)floor(0.5+(y+10)*pow(1.2,s)),
                    x-10,y-10,s,out);
            fprintf(outf,"%d %d %d %d %d %d %d %g\n",
//		    ansVal?"POS":"NEG",out,
                    (int)floor(0.5+(x-10)*pow(1.2,s)),
                    (int)floor(0.5+(y-10)*pow(1.2,s)),
                    (int)floor(0.5+(x+10)*pow(1.2,s)),
                    (int)floor(0.5+(y+10)*pow(1.2,s)),
                    x-10,y-10,s,out);
          }
        }
  }
  fclose(outf);
  for (i=0; i<numScales; i++)
    delete imageScale[i];
  delete[] imageScale;
  return TCL_OK;
}
#endif

#ifdef notdef
int FuzzyVote2Cmd(ClientData data, Tcl_Interp *interp,
                  int argc, char *argv[])
{
  if (argc!=10)
  {
    interp->result="Usage: FuzzyVote2 <width> <height> [location-files] <outputfile> <spread> <min> <collapse> <overlap> <filterodd>";
    return TCL_OK;
  }
  int width=atoi(argv[1]);
  int height=atoi(argv[2]);
  char *locFiles=argv[3];
  char *outFile=argv[4];
  int spread=atoi(argv[5]);
  int search=atoi(argv[6]);
  int collapse=atoi(argv[7]);
  int overlap=atoi(argv[8]);
  int filterodd=atoi(argv[9]);
  int temWidth=width, temHeight=height;
  int numScales=0;
  ByteImage *mask=GetFaceMask();
  while (temWidth>=mask->width && temHeight>=mask->height)
  {
    char name[256];
    sprintf(name,"scale%d",numScales);
    ByteImage *image=new ByteImage(name);
    image->ZeroBuffer(temWidth,temHeight);
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }
  ByteImage **imageScale=new ByteImage*[numScales];
  for (int i=0; i<numScales; i++)
  {
    char name[256];
    sprintf(name,"scale%d",i);
    imageScale[i]=FindByteImageWithName(name);
  }
  char **locFileList;
  int numLocFiles;
  Tcl_SplitList(interp,locFiles,&numLocFiles,&locFileList);
  for (int num=0; num<numLocFiles; num++)
  {
    int numFaces=0;
    int done=0;
    FILE *inf=fopen(locFileList[num],"r");
    do {
      int x, y, s;
      double conf;
      char buf[1024];
      if (fgets(buf,1024,inf)!=NULL) {
        int numread=sscanf(buf,"%*f %*f %*f %*f %d %d %d %lf\n",
                           &x,&y,&s,&conf);
        if (numread<0) done=1; else {
          if (conf<0.0) continue;
          if (filterodd && ((x&1) || (y&1))) continue;
          int xc=x+mask->width/2;
          int yc=y+mask->height/2;
          for (int si=-spread; si<=spread; si++) {
            if (si+s<0 || si+s>=numScales) continue;
            int xci=(int)floor(0.5+xc*exp(log(1.2)*-si));
            int yci=(int)floor(0.5+yc*exp(log(1.2)*-si));
            ByteImage *image=imageScale[si+s];
            int sspread=(int)floor(0.5+spread*pow(1.2,-si));
            for (int xx=xci-sspread; xx<=xci+sspread; xx++)
              for (int yy=yci-sspread; yy<=yci+sspread; yy++)
                if (xx>=0 && xx<image->width && yy>=0 && yy<image->height) {
                  if (image->data(xx,yy)==255) {
		    fprintf(stderr,"Overflow!\n");
		    exit(1);
		  }
                  image->data(xx,yy)++;
                }
          }
        } else done=1;
        numFaces++;
      }
    } while (!done);
    fclose(inf);
//    fprintf(stderr,"numFaces=%d\n",numFaces);
  }
  free((char*)locFileList);
  int max=0;
  for (int s=0; s<numScales; s++)
  {
    ByteImage *image=imageScale[s];
    for (int i=0; i<image->size; i++)
      if (image->data(i)>max) max=image->data(i);
  }
  FILE *outf=fopen(outFile,"w");

  for (int val=max; val>=search; val--)
  {
    fprintf(stderr,"Scanning for %d\n",val);
    for (s=0; s<numScales; s++)
    {
      ByteImage *image=imageScale[s];
      for (int x=0; x<image->width; x++)
        for (int y=0; y<image->height; y++)
          if (image->data(x,y)==val)
          {
            int cs, cx, cy;
            if (collapse)
            {
              double total=0.0;
              double totalS=0, totalX=0, totalY=0;
              FindCentroid(imageScale,numScales,s,x,y,search,
                           &totalS,&totalX,&totalY,&total);
              cs=(int)floor(0.5+totalS/total);
              cx=(int)floor(0.5+totalX/total*exp(log(1.2)*-cs));
              cy=(int)floor(0.5+totalY/total*exp(log(1.2)*-cs));
            } else
            {
              cs=s;
              cx=x;
              cy=y;
            }

            // Now zap all the nearby faces
            if (overlap)
            {
              for (int scale=0; scale<numScales; scale++)
              {
                int xpos=(int)floor(0.5+cx*pow(1.2,cs-scale));
                int ypos=(int)floor(0.5+cy*pow(1.2,cs-scale));
                int size=10+(int)floor(0.5+10*pow(1.2,cs-scale));
                ByteImage *img=imageScale[scale];
                for (int xx=xpos-size; xx<=xpos+size; xx++)
                  for (int yy=ypos-size; yy<=ypos+size; yy++)
                    if (xx>=0 && xx<img->width && yy>=0 && yy<img->height)
                      img->data(xx,yy)=0;
              }
            }

            int xpos=(int)floor(0.5+cx*(exp(log(1.2)*cs)));
            int ypos=(int)floor(0.5+cy*(exp(log(1.2)*cs)));
            int sizex=(int)floor(0.5+0.5*mask->width*(exp(log(1.2)*cs)));
            int sizey=(int)floor(0.5+0.5*mask->height*(exp(log(1.2)*cs)));
            fprintf(stderr,"%d %d %d %d %d %d %d 1.0\n",xpos-sizex,ypos-sizey,
                    xpos+sizex,ypos+sizey,
                    cx-mask->width/2,cy-mask->height/2,cs);
            fprintf(outf,"%d %d %d %d %d %d %d 1.0\n",xpos-sizex,ypos-sizey,
                    xpos+sizex,ypos+sizey,
                    cx-mask->width/2,cy-mask->height/2,cs);
          }
    }
  }
  fclose(outf);
  for (i=0; i<numScales; i++)
    delete imageScale[i];
  delete[] imageScale;
  return TCL_OK;
}
#endif

#ifdef __GNUC__
template class List<AccumElement>;
template class ListNode<AccumElement>;
template class List<Detection>;
template class ListNode<Detection>;
#endif

// USED
// Get the value of some location in the detection pyramid.  If that location
// has not already been allocated, create it and set it to zero.
static int FuzzyVoteAccumGet(Tcl_HashTable *accum, List<AccumElement> *bins,
                             int x, int y, int s)
{
  int loc[3];
  loc[0]=x; loc[1]=y; loc[2]=s;
  Tcl_HashEntry *entry=Tcl_FindHashEntry(accum,(char*)loc);
  if (entry==NULL)
  {
    int newentry;
    entry=Tcl_CreateHashEntry(accum,(char*)loc,&newentry);
    AccumElement *elem=new AccumElement(x,y,s,0,0,0);
    Tcl_SetHashValue(entry,elem);
    bins[0].addLast(elem);
  }
  return ((AccumElement*)(Tcl_GetHashValue(entry)))->value;
}

// USED
// Get the value of some location in the detection pyramid.  If that location
// does not exist, they return zero (but do not allocate that location).
static int FuzzyVoteAccumCheck(Tcl_HashTable *accum, List<AccumElement> *bins,
                               int x, int y, int s)
{
  int loc[3];
  loc[0]=x; loc[1]=y; loc[2]=s;
  Tcl_HashEntry *entry=Tcl_FindHashEntry(accum,(char*)loc);
  if (entry==NULL) return 0;
  return ((AccumElement*)(Tcl_GetHashValue(entry)))->value;
}

// USED
// Set a value in the detection pyramid; if that location does not exist,
// then create it.  Also, add that location to the list of locations with
// that value.  
static void FuzzyVoteAccumSet(Tcl_HashTable *accum, List<AccumElement> *bins,
                              int x, int y, int s, int val)
{
  int loc[3];
  loc[0]=x; loc[1]=y; loc[2]=s;
  Tcl_HashEntry *entry=Tcl_FindHashEntry(accum,(char*)loc);
  AccumElement *elem=(AccumElement*)(Tcl_GetHashValue(entry));
  bins[elem->value].unchain(elem);
  if (val>255) val=255;
  bins[val].addLast(elem);
  elem->value=val;
}

// USED
// Set a value in the detection pyramid to zero.  If the location does not
// exist, then do not create it (because the default value is zero).
static void FuzzyVoteAccumZero(Tcl_HashTable *accum, List<AccumElement> *bins,
                               int x, int y, int s)
{
  int loc[3];
  loc[0]=x; loc[1]=y; loc[2]=s;
  Tcl_HashEntry *entry=Tcl_FindHashEntry(accum,(char*)loc);
  if (entry==NULL) return;
  AccumElement *elem=(AccumElement*)(Tcl_GetHashValue(entry));
  bins[elem->value].unchain(elem);
  bins[0].addLast(elem);
  elem->value=0;
}

// USED 
// Given some point in the detection pyramid, locate all 6-connection 
// locations with a value greater than or equal to the specified amount,
// and find their centroid.  The locations in the centroid are set to zero.
// Centroid in scale is computed by averaging the pyramid levels at which
// the faces are detected.
static void FindCentroidAccum(Tcl_HashTable *accum, List<AccumElement> *bins,
                              int numImages,
                              int s, int x, int y, int val,
                              double *totalS, double *totalX, double *totalY,
                              double *total)
{
  int value=FuzzyVoteAccumCheck(accum,bins,x,y,s);
  if (value>=val) {
    FuzzyVoteAccumZero(accum,bins,x,y,s);
    (*total)+=value;
    (*totalS)+=s*value;
    (*totalX)+=x*pow(1.2,s)*value;
    (*totalY)+=y*pow(1.2,s)*value;
    FindCentroidAccum(accum,bins,numImages,s,x+1,y,val,
                      totalS,totalX,totalY,total);
    FindCentroidAccum(accum,bins,numImages,s,x-1,y,val,
                      totalS,totalX,totalY,total);
    FindCentroidAccum(accum,bins,numImages,s,x,y+1,val,
                      totalS,totalX,totalY,total);
    FindCentroidAccum(accum,bins,numImages,s,x,y-1,val,
                      totalS,totalX,totalY,total);
    FindCentroidAccum(accum,bins,numImages,
                      s-1,(int)floor(0.5+x*1.2),(int)floor(0.5+y*1.2),val,
                      totalS,totalX,totalY,total);
    FindCentroidAccum(accum,bins,numImages,
                      s+1,(int)floor(0.5+x/1.2),(int)floor(0.5+y/1.2),val,
                      totalS,totalX,totalY,total);
  }
}

// Given some point in the detection pyramid, locate all 6-connection 
// locations with a value greater than or equal to the specified amount,
// and find their centroid.  The locations in the centroid are set to zero.
// Centroid in scale is computed by averaging the sizes of the detected
// faces.
static void FindCentroidAccum3(Tcl_HashTable *accum, List<AccumElement> *bins,
                               int numImages,
                               int s, int x, int y, int val,
                               double *totalS, double *totalX, double *totalY,
                               double *total)
{
  int value=FuzzyVoteAccumCheck(accum,bins,x,y,s);
  if (value>=val)
  {
    FuzzyVoteAccumZero(accum,bins,x,y,s);
    (*total)+=value;
    (*totalS)+=pow(1.2,s)*value;
    (*totalX)+=x*pow(1.2,s)*value;
    (*totalY)+=y*pow(1.2,s)*value;
    FindCentroidAccum3(accum,bins,numImages,s,x+1,y,val,
                       totalS,totalX,totalY,total);
    FindCentroidAccum3(accum,bins,numImages,s,x-1,y,val,
                       totalS,totalX,totalY,total);
    FindCentroidAccum3(accum,bins,numImages,s,x,y+1,val,
                       totalS,totalX,totalY,total);
    FindCentroidAccum3(accum,bins,numImages,s,x,y-1,val,
                       totalS,totalX,totalY,total);
    FindCentroidAccum3(accum,bins,numImages,
                       s-1,(int)floor(0.5+x*1.2),(int)floor(0.5+y*1.2),val,
                       totalS,totalX,totalY,total);
    FindCentroidAccum3(accum,bins,numImages,
                       s+1,(int)floor(0.5+x/1.2),(int)floor(0.5+y/1.2),val,
                       totalS,totalX,totalY,total);
  }
}

// USED
// Implementation of the arbitration mechanisms described in the paper.
// Inputs are lists of detections, the width and height of the image that
// is being processed, a callback function used to report the arbitration
// results, and a bunch of parameters for the arbitration itself.
void FuzzyVote2(int width, int height, int numLocFiles,
                List<Detection> detections[],
                SearchCallback callback, ClientData data,
                int spread, int search, int collapse, int overlap,
                int filterodd, ByteImage *mask)
{
  // Hash table represents which locations/scales in the detection are
  // filled in, and with what values.  Any missing entries are assumed
  // to be zero.
  Tcl_HashTable accum;
  Tcl_InitHashTable(&accum,3);
  // An array of lists.  Each lists contains all the detection location/scales
  // with a particular value.  This allows the detections to be scanned in
  // order from highest to lowest with practically no overhead.
  List<AccumElement> bins[256];

  // Figure out the number of scales in the pyramid
  int temWidth=width, temHeight=height;
  int numScales=0;
  while (temWidth>=mask->width && temHeight>=mask->height) {
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }

  // For each detection list given as input (all detections are treated
  // equally)
  for (int num=0; num<numLocFiles; num++) {
    int numFaces=0;
    int done=0;
    // For each detection
    for (Detection *detect=detections[num].first; detect!=NULL;
	 detect=detect->next) {
      int x=detect->x, y=detect->y, s=detect->s;
      if (filterodd && ((x&1) || (y&1))) continue;
      int xc=x+mask->width/2;
      int yc=y+mask->height/2;
      // Spread out the detection in both scale and location by 
      // "spread" levels or pixels, incrementing the value of each
      // location in the detection pyramid
      for (int si=-spread; si<=spread; si++) {
        if (si+s<0 || si+s>=numScales) continue;
        int xci=(int)floor(0.5+xc*exp(log(1.2)*-si));
        int yci=(int)floor(0.5+yc*exp(log(1.2)*-si));
//        int sspread=(int)floor(0.5+spread*pow(1.2,-si));
        int sspread=spread;
        for (int xx=xci-sspread; xx<=xci+sspread; xx++)
          for (int yy=yci-sspread; yy<=yci+sspread; yy++)
            FuzzyVoteAccumSet(&accum,bins,xx,yy,s+si,
                              FuzzyVoteAccumGet(&accum,bins,xx,yy,s+si)+1);
      }
      numFaces++;
    }
  }

  // Scan through the detection pyramid from highest to lowest value
  for (int val=255; val>=search; val--) {
    while (!bins[val].empty()) {
      // Get the detection
      int x=bins[val].first->loc[0];
      int y=bins[val].first->loc[1];
      int s=bins[val].first->loc[2];
      int cs, cx, cy;

      // If we are to collapse nearby overlapping detections, find the
      // centroid of the detections around this location.  Otherwise, just
      // record this location.
      if (collapse) {
        double total=0.0;
        double totalS=0, totalX=0, totalY=0;
        FindCentroidAccum(&accum,bins,numScales,s,x,y,search,
                          &totalS,&totalX,&totalY,&total);
        cs=(int)floor(0.5+totalS/total);
        cx=(int)floor(0.5+totalX/total*exp(log(1.2)*-cs));
        cy=(int)floor(0.5+totalY/total*exp(log(1.2)*-cs));
      } else {
        cs=s;
        cx=x;
        cy=y;
        FuzzyVoteAccumZero(&accum,bins,x,y,s);
      }

      // If we are to remove overlapping detections, scan through
      // the detection pyramid, removing all possible overlaps
      if (overlap) {
        for (int scale=0; scale<numScales; scale++) {
          int xpos=(int)floor(0.5+cx*pow(1.2,cs-scale));
          int ypos=(int)floor(0.5+cy*pow(1.2,cs-scale));
          int sizex=mask->width/2+
            (int)floor(0.5+mask->width/2*pow(1.2,cs-scale));
          int sizey=mask->height/2+
            (int)floor(0.5+mask->height/2*pow(1.2,cs-scale));
          for (int xx=xpos-sizex; xx<=xpos+sizex; xx++)
            for (int yy=ypos-sizey; yy<=ypos+sizey; yy++)
              FuzzyVoteAccumZero(&accum,bins,xx,yy,scale);
        }
      }

      // Record (or otherwise deal with) this detection
      callback(data,NULL,cx-mask->width/2,cy-mask->height/2,
               mask->width,mask->height,
               cs,pow(1.2,cs),1.0,0);
    }
  }
  // Clean up
  Tcl_DeleteHashTable(&accum);
}

// USED
// Implementation of the arbitration mechanisms described in the paper.
// Inputs are lists of detections, the width and height of the image that
// is being processed, a callback function used to report the arbitration
// results, and a bunch of parameters for the arbitration itself.
void FuzzyVote2Direct(int width, int height, int numLocFiles,
                      List<Detection> *detections,
                      List<Detection> *outputDetections, 
                      int spread, int search, int collapse, int overlap,
                      int filterodd, ByteImage *mask)
{
  // Hash table represents which locations/scales in the detection are
  // filled in, and with what values.  Any missing entries are assumed
  // to be zero.
  Tcl_HashTable accum;
  Tcl_InitHashTable(&accum,3);
  // An array of lists.  Each lists contains all the detection location/scales
  // with a particular value.  This allows the detections to be scanned in
  // order from highest to lowest with practically no overhead.
  List<AccumElement> bins[256];

  // Figure out the number of scales in the pyramid
  int temWidth=width, temHeight=height;
  int numScales=0;
  while (temWidth>=mask->width && temHeight>=mask->height) {
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }

  // For each detection list given as input (all detections are treated
  // equally)
  for (int num=0; num<numLocFiles; num++) {
    int numFaces=0;
    int done=0;
    for (Detection *detect=detections[num].first; detect!=NULL;
	 detect=detect->next) {
      // For each detection
      int x=detect->x, y=detect->y, s=detect->s;
      if (filterodd && ((x&1) || (y&1))) continue;
      if (detect->output<=0.0) continue;
      int xc=x+mask->width/2;
      int yc=y+mask->height/2;
      // Spread out the detection in both scale and location by 
      // "spread" levels or pixels, incrementing the value of each
      // location in the detection pyramid
      for (int si=-spread; si<=spread; si++) {
        if (si+s<0 || si+s>=numScales) continue;
        int xci=(int)floor(0.5+xc*exp(log(1.2)*-si));
        int yci=(int)floor(0.5+yc*exp(log(1.2)*-si));
//        int sspread=(int)floor(0.5+spread*pow(1.2,-si));
        int sspread=spread;
        for (int xx=xci-sspread; xx<=xci+sspread; xx++)
          for (int yy=yci-sspread; yy<=yci+sspread; yy++)
            FuzzyVoteAccumSet(&accum,bins,xx,yy,s+si,
                              FuzzyVoteAccumGet(&accum,bins,xx,yy,s+si)+1);
      }
      numFaces++;
    }
  }

  // Scan through the detection pyramid from highest to lowest value
  for (int val=255; val>=search; val--) {
    while (!bins[val].empty()) {
      // Get the detection
      int x=bins[val].first->loc[0];
      int y=bins[val].first->loc[1];
      int s=bins[val].first->loc[2];
      int cs, cx, cy;

      // If we are to collapse nearby overlapping detections, find the
      // centroid of the detections around this location.  Otherwise, just
      // record this location.
      if (collapse) {
        double total=0.0;
        double totalS=0, totalX=0, totalY=0;
        FindCentroidAccum(&accum,bins,numScales,s,x,y,search,
                          &totalS,&totalX,&totalY,&total);
        cs=(int)floor(0.5+totalS/total);
        cx=(int)floor(0.5+totalX/total*exp(log(1.2)*-cs));
        cy=(int)floor(0.5+totalY/total*exp(log(1.2)*-cs));
      } else {
        cs=s;
        cx=x;
        cy=y;
        FuzzyVoteAccumZero(&accum,bins,x,y,s);
      }

      // If we are to remove overlapping detections, scan through
      // the detection pyramid, removing all possible overlaps
      if (overlap) {
        for (int scale=0; scale<numScales; scale++)
        {
          int xpos=(int)floor(0.5+cx*pow(1.2,cs-scale));
          int ypos=(int)floor(0.5+cy*pow(1.2,cs-scale));
          int sizex=mask->width/2+
            (int)floor(0.5+mask->width/2*pow(1.2,cs-scale));
          int sizey=mask->height/2+
            (int)floor(0.5+mask->height/2*pow(1.2,cs-scale));
          for (int xx=xpos-sizex; xx<=xpos+sizex; xx++)
            for (int yy=ypos-sizey; yy<=ypos+sizey; yy++)
              FuzzyVoteAccumZero(&accum,bins,xx,yy,scale);
        }
      }
      // Record this detection
      outputDetections->addLast(new Detection(cx-mask->width/2,
                                              cy-mask->height/2,
                                              cs,1.0,0));
    }
  }
  Tcl_DeleteHashTable(&accum);
}

void FuzzyVote3(int width, int height, int numLocFiles,
                List<Detection> detections[],
                SearchCallback callback, ClientData data,
                int spread, int search, int collapse, int overlap,
                int filterodd, ByteImage *mask)
{
  Tcl_HashTable accum;
  Tcl_InitHashTable(&accum,3);
  List<AccumElement> bins[256];
  int temWidth=width, temHeight=height;
  int numScales=0;
  while (temWidth>=mask->width && temHeight>=mask->height)
  {
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }
  for (int num=0; num<numLocFiles; num++)
  {
    int numFaces=0;
    int done=0;
    for (Detection *detect=detections[num].first; detect!=NULL;
                                                  detect=detect->next)
    {
      int xc=detect->x, yc=detect->y, s=detect->s;
      if (filterodd && ((xc&1) || (yc&1))) continue;
      for (int si=-spread; si<=spread; si++)
      {
        if (si+s<0 || si+s>=numScales) continue;
        int xci=(int)floor(0.5+xc*exp(log(1.2)*-si));
        int yci=(int)floor(0.5+yc*exp(log(1.2)*-si));
        int sspread=(int)floor(0.5+spread*pow(1.2,-si));
        for (int xx=xci-sspread; xx<=xci+sspread; xx++)
          for (int yy=yci-sspread; yy<=yci+sspread; yy++)
            FuzzyVoteAccumSet(&accum,bins,xx,yy,s+si,
                              FuzzyVoteAccumGet(&accum,bins,xx,yy,s+si)+1);
      }
      numFaces++;
    }
  }
  for (int val=255; val>=search; val--)
  {
    while (!bins[val].empty()) {
      int x=bins[val].first->loc[0];
      int y=bins[val].first->loc[1];
      int s=bins[val].first->loc[2];
      int cs, cx, cy;
      if (collapse)
      {
        double total=0.0;
        double totalS=0, totalX=0, totalY=0;
        FindCentroidAccum3(&accum,bins,numScales,s,x,y,search,
                           &totalS,&totalX,&totalY,&total);
        cs=(int)floor(0.5+log(totalS/total)/log(1.2));
        cx=(int)floor(0.5+totalX/total*exp(log(1.2)*-cs));
        cy=(int)floor(0.5+totalY/total*exp(log(1.2)*-cs));
      } else
      {
        cs=s;
        cx=x;
        cy=y;
        FuzzyVoteAccumZero(&accum,bins,x,y,s);
      }

      // Now zap all the nearby faces
      if (overlap)
      {
        for (int scale=0; scale<numScales; scale++)
        {
          int xpos=(int)floor(0.5+cx*pow(1.2,cs-scale));
          int ypos=(int)floor(0.5+cy*pow(1.2,cs-scale));
          int sizex=mask->width/2+
            (int)floor(0.5+mask->width/2*pow(1.2,cs-scale));
          int sizey=mask->height/2+
            (int)floor(0.5+mask->height/2*pow(1.2,cs-scale));
          for (int xx=xpos-sizex; xx<=xpos+sizex; xx++)
            for (int yy=ypos-sizey; yy<=ypos+sizey; yy++)
              FuzzyVoteAccumZero(&accum,bins,xx,yy,scale);
        }
      }
      callback(data,NULL,cx-mask->width/2,cy-mask->height/2,
               mask->width,mask->height,
               cs,pow(1.2,cs),1.0,0);
    }
  }
  Tcl_DeleteHashTable(&accum);
}

void FuzzyVote2Callback(ClientData data, ByteImage *image,
                        int x, int y, int width, int height,
                        int level,
                        double scale, double output, int orient)
{
  FILE *outf=(FILE*)data;
  fprintf(outf,"%g %g %g %g %d %d %d %g %d\n",
          scale*x,scale*y,
          scale*(x+width),scale*(y+height),
          x,y,level,output,orient);
}

#ifndef NoTcl
int FuzzyVote2Cmd(ClientData data, Tcl_Interp *interp,
                  int argc, char *argv[])
{
  if (argc!=11)
  {
    interp->result="Usage: FuzzyVote2 <width> <height> [location-files] <outputfile> <spread> <min> <collapse> <overlap> <filterodd> <mask>";
    return TCL_ERROR;
  }
  int width=atoi(argv[1]);
  int height=atoi(argv[2]);
  char *locFiles=argv[3];
  char *outFile=argv[4];
  int spread=atoi(argv[5]);
  int search=atoi(argv[6]);
  int collapse=atoi(argv[7]);
  int overlap=atoi(argv[8]);
  int filterodd=atoi(argv[9]);
  ByteImage *mask=FindByteImageWithName(argv[10]);
  char **locFileList;
  int numLocFiles;
  Tcl_SplitList(interp,locFiles,&numLocFiles,&locFileList);
  List<Detection> *detections=new List<Detection>[numLocFiles];
  for (int num=0; num<numLocFiles; num++)
  {
    int done=0;
    FILE *inf=fopen(locFileList[num],"r");
    do {
      int x, y, s;
      double conf;
      char buf[1024];
      if (fgets(buf,1024,inf)!=NULL) {
        int numread=sscanf(buf,"%*f %*f %*f %*f %d %d %d %lf\n",
                           &x,&y,&s,&conf);
        if (numread<0) done=1; else {
          if (conf<0.0) continue;
          detections[num].addLast(new Detection(x,y,s,conf,0));
        }
      } else done=1;
    } while (!done);
    fclose(inf);
  }
  free((char*)locFileList);
  FILE *outf=fopen(outFile,"w");
  FuzzyVote2(width,height,numLocFiles,detections,
             FuzzyVote2Callback,(ClientData)outf,
             spread,search,collapse,overlap,filterodd,mask);
  fclose(outf);
  delete[] detections;
  return TCL_OK;
}
#endif

#ifndef NoTcl
int FuzzyVote3Cmd(ClientData data, Tcl_Interp *interp,
                  int argc, char *argv[])
{
  if (argc!=12)
  {
    interp->result="Usage: FuzzyVote3 <width> <height> [location-files] <outputfile> <spread> <min> <collapse> <overlap> <filterodd> <mask> <offset>";
    return TCL_ERROR;
  }
  int width=atoi(argv[1]);
  int height=atoi(argv[2]);
  char *locFiles=argv[3];
  char *outFile=argv[4];
  int spread=atoi(argv[5]);
  int search=atoi(argv[6]);
  int collapse=atoi(argv[7]);
  int overlap=atoi(argv[8]);
  int filterodd=atoi(argv[9]);
  ByteImage *mask=FindByteImageWithName(argv[10]);
  //  int offset=atoi(argv[10]);
  char **locFileList;
  int numLocFiles;
  Tcl_SplitList(interp,locFiles,&numLocFiles,&locFileList);
  List<Detection> *detections=new List<Detection>[numLocFiles];
  for (int num=0; num<numLocFiles; num++)
  {
    int done=0;
    FILE *inf=fopen(locFileList[num],"r");
    assert(inf!=NULL);
    do {
      int x, y, s;
      double x1, y1, x2, y2;
      double conf;
      char buf[1024];
      if (fgets(buf,1024,inf)!=NULL) {
        int numread=sscanf(buf,"%lf %lf %lf %lf %d %d %d %lf\n",
                           &x1,&y1,&x2,&y2,
                           &x,&y,&s,&conf);
        int w=(int)floor(0.5+(x2-x1)/pow(1.2,s));
        int h=(int)floor(0.5+(y2-y1)/pow(1.2,s));
        x=x+w/2;
        y=y+h/2;
        if (conf<0.0) continue;
        detections[num].addLast(new Detection(x,y,s,conf,0));
      } else done=1;
    } while (!done);
    fclose(inf);
  }
  free((char*)locFileList);
  FILE *outf=fopen(outFile,"w");
  FuzzyVote3(width,height,numLocFiles,detections,
             FuzzyVote2Callback,(ClientData)outf,
             spread,search,collapse,overlap,filterodd,mask);
  fclose(outf);
  delete[] detections;
  return TCL_OK;
}
#endif

#ifdef notdef
void FuzzyVote2(int width, int height, int numLocFiles,
                List<Detection> detections[],
                SearchCallback callback, ClientData data,
                int spread, int search, int collapse, int overlap,
                int filterodd)
{
  int temWidth=width, temHeight=height;
  int numScales=0;
  ByteImage *mask=GetFaceMask();
  while (temWidth>=mask->width && temHeight>=mask->height)
  {
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    numScales++;
  }
  temWidth=width;
  temHeight=height;
  ByteImage **imageScale=new ByteImage*[numScales];
  for (int i=0; i<numScales; i++)
  {
    ByteImage *image=new ByteImage(NULL);
    image->ZeroBuffer(temWidth,temHeight);
    temWidth=(int)(floor(temWidth/1.2));
    temHeight=(int)(floor(temHeight/1.2));
    imageScale[i]=image;
  }
  for (int num=0; num<numLocFiles; num++)
  {
    int numFaces=0;
    int done=0;
    for (Detection *detect=detections[num].first; detect!=NULL;
                                                  detect=detect->next)
    {
      int x=detect->x, y=detect->y, s=detect->s;
      if (filterodd && ((x&1) || (y&1))) continue;
      int xc=x+mask->width/2;
      int yc=y+mask->height/2;
      for (int si=-spread; si<=spread; si++)
      {
        if (si+s<0 || si+s>=numScales) continue;
        int xci=(int)floor(0.5+xc*exp(log(1.2)*-si));
        int yci=(int)floor(0.5+yc*exp(log(1.2)*-si));
        ByteImage *image=imageScale[si+s];
        int sspread=(int)floor(0.5+spread*pow(1.2,-si));
        for (int xx=xci-sspread; xx<=xci+sspread; xx++)
          for (int yy=yci-sspread; yy<=yci+sspread; yy++)
            if (xx>=0 && xx<image->width && yy>=0 && yy<image->height)
            {
              if (image->data(xx,yy)==255)
              {
                fprintf(stderr,"Overflow!\n");
                exit(1);
              }
              image->data(xx,yy)++;
            }
      }
      numFaces++;
    }
    fprintf(stderr,"numFaces=%d\n",numFaces);
  }
  int max=0;
  for (int s=0; s<numScales; s++)
  {
    ByteImage *image=imageScale[s];
    for (int i=0; i<image->size; i++)
      if (image->data(i)>max) max=image->data(i);
  }
  for (int val=max; val>=search; val--)
  {
    fprintf(stderr,"Scanning for %d\n",val);
    for (s=0; s<numScales; s++)
    {
      ByteImage *image=imageScale[s];
      for (int x=0; x<image->width; x++)
        for (int y=0; y<image->height; y++)
          if (image->data(x,y)==val)
          {
            int cs, cx, cy;
            if (collapse)
            {
              double total=0.0;
              double totalS=0, totalX=0, totalY=0;
              FindCentroid(imageScale,numScales,s,x,y,search,
                           &totalS,&totalX,&totalY,&total);
              cs=(int)floor(0.5+totalS/total);
              cx=(int)floor(0.5+totalX/total*exp(log(1.2)*-cs));
              cy=(int)floor(0.5+totalY/total*exp(log(1.2)*-cs));
            } else
            {
              cs=s;
              cx=x;
              cy=y;
            }

            // Now zap all the nearby faces
            if (overlap)
            {
              for (int scale=0; scale<numScales; scale++)
              {
                int xpos=(int)floor(0.5+cx*pow(1.2,cs-scale));
                int ypos=(int)floor(0.5+cy*pow(1.2,cs-scale));
                int size=10+(int)floor(0.5+10*pow(1.2,cs-scale));
                ByteImage *img=imageScale[scale];
                for (int xx=xpos-size; xx<=xpos+size; xx++)
                  for (int yy=ypos-size; yy<=ypos+size; yy++)
                    if (xx>=0 && xx<img->width && yy>=0 && yy<img->height)
                      img->data(xx,yy)=0;
              }
            }
            callback(data,imageScale[cs],cx-mask->width/2,cy-mask->height/2,
                     mask->width,mask->height,
                     cs,pow(1.2,cs),1.0,0);
          }
    }
  }
  for (i=0; i<numScales; i++)
    delete imageScale[i];
  delete[] imageScale;
}
#endif

#ifdef notdef
// This version is faster, but applies the lighting correction calculation
// to the entire window rather than just to the masked portion.  Applying
// it to just the masked portion is slightly more complex, and the 
// performance improvement is < 10%.
int SearchMultiCmd(ClientData data, Tcl_Interp *interp, 
                   int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: SearchMulti <image> {filelist}";
    return TCL_ERROR;
  }
  char **fileList;
  int numFiles;
  ByteImage *image=FindByteImageWithName(argv[1]);
  Tcl_SplitList(interp,argv[2],&numFiles,&fileList);
  int i;
  FILE **openFileList=new FILE*[numFiles];
  for (i=0; i<numFiles; i++) openFileList[i]=fopen(fileList[i],"w");
  free((char*)fileList);
  int ptr=0;
  Mat mat=Zero(3,3);
  ByteImage *mask=GetFaceMask();
  for (int j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
    if (mask->data(ptr++))
    {
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

  char cmd[1024];
  int *tmp=new int[mask->size];

  Vec vec(3);
  double v0=0, v1=0, v2=0;
  double scale=1.0;
  while (image->height>=mask->height && image->width>=mask->width)
  {
    for (int y=0; y<image->height-mask->height+1; y++)
    {
      v0=0; v1=0; v2=0;
      int ptr=0;
      for (int j=0; j<mask->height; j++) for (int i=0; i<mask->width; i++) 
      {
        Byte data=image->data(i,j+y);
        tmp[ptr]=data;
        if (mask->data(ptr++))
        {
          v0+=i*data;
          v1+=j*data;
          v2+=data;
        }
      }
      for (int x=0; x<image->width-mask->width+1; x++)
      {
        int ptr=0;
        if (x>0)
        {
          for (int i=0; i<mask->size-1; i++)
            tmp[i]=tmp[i+1];
          ptr=mask->width-1;
          for (int j=0; j<mask->height; j++)
          {
            Byte data=image->data(x-1,j+y);
            v1-=j*data;
            v2-=data;
            data=image->data(x+mask->width-1,j+y);
            v0+=mask->width*data;
            v1+=j*data;
            v2+=data;
            tmp[ptr]=data;
            ptr+=mask->width;
          }
          v0-=v2;
        }
        vec(0)=v0; vec(1)=v1; vec(2)=v2;
        vec=mat*vec;
        v0=vec(0); v1=vec(1); v2=vec(2);
        int hist[512];
        for (i=0; i<512; i++) hist[i]=0;
        ptr=0;
        for (j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
        {
          int val=tmp[ptr]-(int)(floor(0.5+i*v0+j*v1+v2))+256;
          if (val<0) val=0;
          if (val>=512) val=511;
          if (mask->data(ptr)) hist[val]++;
          tmp[ptr++]=val;
        }
        int map[512];
        int num=0;
        for (i=0; i<512; i++)
        {
          map[i]=num;
          num+=hist[i];
        }
        int tot=num;
        for (i=511; i>=0; i--)
        {
          map[i]+=num;
          num-=hist[i];
        }
        double scaleFactor=1.0/tot;
        for (i=0; i<mask->size; i++)
          real_fast_list[0]->unitList[i+1].activation=
            map[tmp[i]]*scaleFactor-1.0;
        for (int j=0; j<numFiles; j++)
          for (i=1; i<=mask->size; i++)
            real_fast_list[j]->unitList[i].activation=
              real_fast_list[0]->unitList[i].activation;

        for (i=0; i<numFiles; i++)
        {
          real_outputs[0]=FastForwardPass(real_fast_list[i]);
          if (real_outputs[0]>0)
          {
            fprintf(openFileList[i],"%g %g %g %g %d %d %d %g\n",
                    scale*x+2,scale*y+2,
                    scale*(x+mask->width)+2,scale*(y+mask->height)+2,
                    x,y,(int)(floor(0.5+log(scale)/log(1.2))),
                    real_outputs[0]);
          }
        }
      }
    }
    SmoothByteImage(image);
    ZoomByteImage(image,1.2,image);
    scale*=1.2;
  }
  delete[] tmp;
  for (i=0; i<numFiles; i++)
    fclose(openFileList[i]);
  delete[] openFileList;
  return TCL_OK;
}
#endif

void SearchMultiCallbackSave(ClientData data, ByteImage *image,
                             int x, int y, int width, int height, int level,
                             double scale, double output, int orient)
{
  FILE *outf=(FILE*)data;
  fprintf(outf,"%g %g %g %g %d %d %d %g %d\n",
          scale*x,scale*y,
          scale*(x+width),scale*(y+height),
          x,y,level,output,orient);
}

void SearchMulti(ByteImage *image, int numNetworks,
                 SearchCallback callback, ClientData data[])
{
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
        for (int j=-halfH; j<halfH; j++) {
          for (int i=-halfW; i<halfW; i++) {
            int ii=i+x;
            int jj=j+y;
            if (ii<0) ii=0;
            if (ii>=image->width) ii=image->width-1;
            if (jj<0) jj=0;
            if (jj>=image->height) jj=image->height-1;
            int data=image->data(ii,jj);

#ifdef DEBUG
            if (x==DebugX && y==DebugY && level==DebugS) {
              fprintf(stderr,"i,j=%d,%d data=%d\n",i+x,j+y,data);
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
        vec(0)=v0; vec(1)=v1; vec(2)=v2;
        vec=mat*vec;
        v0=vec(0); v1=vec(1); v2=vec(2);
        int hist[512];
        for (int i=0; i<512; i++) hist[i]=0;
        ptr=0;
        for (j=-halfH; j<halfH; j++) for (i=-halfW; i<halfW; i++) {
          int val=tmp[ptr]-(int)(i*v0+j*v1+v2-256.5);
          if (val<0) val=0;
          if (val>=512) val=511;
          if (mask->data(ptr)) hist[val]++;
          tmp[ptr++]=val;
        }
        int map[512];
        int num=0;
        for (i=0; i<512; i++) {
          map[i]=num;
          num+=hist[i];
        }
        int tot=num;
        for (i=511; i>=0; i--) {
          map[i]+=num;
          num-=hist[i];
        }
        double scaleFactor=1.0/tot;
        FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;

        for (i=0; i<real_numNetworks; i++) {
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
//          if (output>0.9996) {
            callback(data[i],image,x-halfW,y-halfH,mask->width,mask->height,
                     level,scale,output,0);
            //            fprintf(stderr,"x=%d,y=%d:\n",x,y);
            //            FastDebugNetwork(real_fast_list[i]);
            //            exit(0);
          }
        }
      }
    }
    ReduceSize(image,image);
//    SmoothByteImage(image);
//    ZoomByteImage(image,1.2,image);
    scale*=1.2;
    level++;
  }
  delete[] tmp;
}

void SearchMultiAverage(ByteImage *image, int numNetworks,
                        SearchCallback callback, ClientData data)
{
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
        for (int j=-halfH; j<halfH; j++) {
          for (int i=-halfW; i<halfW; i++) {
            int ii=i+x;
            int jj=j+y;
            if (ii<0) ii=0;
            if (ii>=image->width) ii=image->width-1;
            if (jj<0) jj=0;
            if (jj>=image->height) jj=image->height-1;
            int data=image->data(ii,jj);

#ifdef DEBUG
            if (x==DebugX && y==DebugY && level==DebugS) {
              fprintf(stderr,"i,j=%d,%d data=%d\n",i+x,j+y,data);
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
        vec(0)=v0; vec(1)=v1; vec(2)=v2;
        vec=mat*vec;
        v0=vec(0); v1=vec(1); v2=vec(2);
        int hist[512];
        for (int i=0; i<512; i++) hist[i]=0;
        ptr=0;
        for (j=-halfH; j<halfH; j++) for (i=-halfW; i<halfW; i++) {
          int val=tmp[ptr]-(int)(i*v0+j*v1+v2-256.5);
          if (val<0) val=0;
          if (val>=512) val=511;
          if (mask->data(ptr)) hist[val]++;
          tmp[ptr++]=val;
        }
        int map[512];
        int num=0;
        for (i=0; i<512; i++) {
          map[i]=num;
          num+=hist[i];
        }
        int tot=num;
        for (i=511; i>=0; i--) {
          map[i]+=num;
          num-=hist[i];
        }
        double scaleFactor=1.0/tot;
        FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;

        double averageOutput=0;

        for (i=0; i<real_numNetworks; i++) {
          if (i>0) {
            FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
            FastForwardUnit *unit1=&(real_fast_list[i]->unitList[1]);
            for (int j=0; j<mask->size; j++)
              (unit1++)->activation=(unit++)->activation;
          }
          double output=FastForwardPass(real_fast_list[i]);
          averageOutput+=output;
#ifdef DEBUG
          if (x==DebugX && y==DebugY && level==DebugS) {
            fprintf(stderr,"output=%g\n",output);
            exit(1);
          }
#endif
        }
        averageOutput/=real_numNetworks;
        if (averageOutput>0.9996) {
          callback(data,image,x-halfW,y-halfH,mask->width,mask->height,
                   level,scale,averageOutput,0);
        }
      }
    }
    ReduceSize(image,image);
//    SmoothByteImage(image);
//    ZoomByteImage(image,1.2,image);
    scale*=1.2;
    level++;
  }
  delete[] tmp;
}

// Version that doesn't search off the edges of the image 
void SearchMulti2(ByteImage *image, int numNetworks,
                  SearchCallback callback, ClientData data[])
{
  int ptr=0;
  Mat mat=Zero(3,3);
  ByteImage *mask=FindByteImageWithName("facemask");
  int halfW=mask->width/2;
  int halfH=mask->height/2;

  for (int j=0; j<mask->width; j++) for (int i=0; i<mask->height; i++) 
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
    for (int y=0; y<image->height-mask->height+1; y++) {
      for (int x=0; x<image->width-mask->width+1; x++) {
        int ptr=0;
        v0=0; v1=0; v2=0;
        for (int j=0; j<mask->height; j++) {
          for (int i=0; i<mask->width; i++) {
            int ii=i+x;
            int jj=j+y;
//            if (ii<0) ii=0;
//            if (ii>=image->width) ii=image->width-1;
//            if (jj<0) jj=0;
//            if (jj>=image->height) jj=image->height-1;
            int data=image->data(ii,jj);

#ifdef DEBUG
            if (x==DebugX && y==DebugY && level==DebugS) {
              fprintf(stderr,"i,j=%d,%d data=%d\n",i+x,j+y,data);
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
        vec(0)=v0; vec(1)=v1; vec(2)=v2;
        vec=mat*vec;
        v0=vec(0); v1=vec(1); v2=vec(2);
        int hist[512];
        for (int i=0; i<512; i++) hist[i]=0;
        ptr=0;
        for (j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) {
          int val=tmp[ptr]-(int)(i*v0+j*v1+v2-256.5);
          if (val<0) val=0;
          if (val>=512) val=511;
          if (mask->data(ptr)) hist[val]++;
          tmp[ptr++]=val;
        }
        int map[512];
        int num=0;
        for (i=0; i<512; i++) {
          map[i]=num;
          num+=hist[i];
        }
        int tot=num;
        for (i=511; i>=0; i--) {
          map[i]+=num;
          num-=hist[i];
        }
        double scaleFactor=1.0/tot;
        FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;

        for (i=0; i<real_numNetworks; i++) {
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
            callback(data[i],image,x,y,mask->width,mask->height,
                     level,scale,output,0);
            //            fprintf(stderr,"x=%d,y=%d:\n",x,y);
            //            FastDebugNetwork(real_fast_list[i]);
            //            exit(0);
          }
        }
      }
    }
    ReduceSize(image,image);
//    SmoothByteImage(image);
//    ZoomByteImage(image,1.2,image);
    scale*=1.2;
    level++;
  }
  delete[] tmp;
}

void SearchAstro(ByteImage *image, SearchCallback callback, ClientData data)
{
  int ptr=0;
  Mat mat=Zero(3,3);
  ByteImage *mask=FindByteImageWithName("facemask");
  for (int j=0; j<mask->height; j++) for (int i=0; i<mask->width; i++) 
    if (mask->data(ptr++))
    {
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
  while (image->height>=mask->height && image->width>=mask->width)
  {
    for (int y=0; y<image->height-mask->height+1; y++)
    {
      for (int x=0; x<image->width-mask->width+1; x++)
      {
        int ptr=0;
        v0=0; v1=0; v2=0;
        for (int j=0; j<mask->height; j++) {
          for (int i=0; i<mask->width; i++) {
          Byte data=image->data(i+x,j+y);
          tmp[ptr]=data;
          if (mask->data(ptr++))
          {
            v0+=i*data;
            v1+=j*data;
            v2+=data;
          }
        } }
        vec(0)=v0; vec(1)=v1; vec(2)=v2;
        vec=mat*vec;
        v0=vec(0); v1=vec(1); v2=vec(2);
        int hist[512];
        for (int i=0; i<512; i++) hist[i]=0;
        ptr=0;
        for (j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
        {
          int val=tmp[ptr]-(int)(i*v0+j*v1+v2-256.5);
          if (val<0) val=0;
          if (val>=512) val=511;
          if (mask->data(ptr)) hist[val]++;
          tmp[ptr++]=val;
        }
        int map[512];
        int num=0;
        for (i=0; i<512; i++)
        {
          map[i]=num;
          num+=hist[i];
        }
        int tot=num;
        for (i=511; i>=0; i--)
        {
          map[i]+=num;
          num-=hist[i];
        }
        double scaleFactor=1.0/tot;
        for (i=0; i<mask->size; i++)
          tmp[i]=(int)floor(0.5+127.0*(map[tmp[i]]*scaleFactor-1.0));
        double output=0 /* Call your program here using tmp[] */;
        if (output>0)
          callback(data,image,x,y,mask->width,mask->height,
                   level,scale,output,0);
      }
    }
    SmoothByteImage(image);
    ZoomByteImage(image,1.2,image);
    scale*=1.2;
    level++;
  }
  delete[] tmp;
}

// USED
// Apply multiple NNs to an image, using both lighting correction and
// histogram equalization on each window.
void SearchMultiMerge(ByteImage *image, int numNetworks,
                      SearchCallback callback, ClientData data[])
{
  int ptr=0;
  ByteImage *mask=FindByteImageWithName("facemask");

  int halfW=mask->width/2;
  int halfH=mask->height/2;

  // Set up the matrix for lighting correction
  Mat mat=Zero(3,3);
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

  // Loop over levels in image pyramid
  while (image->height>=mask->height && image->width>=mask->width) {
    for (int y=0; y<image->height; y++) {
      for (int x=0; x<image->width; x++) {
        int ptr=0;
        v0=0; v1=0; v2=0;

        // Extract the window and begin the affine lighting fit
        for (int j=-halfH; j<halfH; j++) {
          for (int i=-halfW; i<halfW; i++) {
          int ii=i+x;
          int jj=j+y;
          if (ii<0) ii=0;
          if (ii>=image->width) ii=image->width-1;
          if (jj<0) jj=0;
          if (jj>=image->height) jj=image->height-1;
          int data=image->data(ii,jj);
          tmp[ptr]=data;
          if (mask->data(ptr++)) {
            v0+=i*data;
            v1+=j*data;
            v2+=data;
          }
        } }
        // Compute affine lighting
        vec(0)=v0; vec(1)=v1; vec(2)=v2;
        vec=mat*vec;
        v0=vec(0); v1=vec(1); v2=vec(2);
        int hist[512];
        for (int i=0; i<512; i++) hist[i]=0;
        ptr=0;

        // Apply lighting correction, and compute histogram
        for (j=-halfH; j<halfH; j++) for (i=-halfW; i<halfW; i++) {
          int val=tmp[ptr]-(int)(i*v0+j*v1+v2-256.5);
          if (val<0) val=0;
          if (val>=512) val=511;
          if (mask->data(ptr)) hist[val]++;
          tmp[ptr++]=val;
        }

        // Compute cumulative histogram
        int map[512];
        int num=0;
        for (i=0; i<512; i++) {
          map[i]=num;
          num+=hist[i];
        }
        int tot=num;
        for (i=511; i>=0; i--) {
          map[i]+=num;
          num-=hist[i];
        }

        // Copy window into network, applying histogram equalization
        double scaleFactor=1.0/tot;
        FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;
        FastForwardPass(real_fast_list[0]);
        // Check the output from each of the networks
        for (i=0; i<numNetworks; i++) {
          double output=real_fast_list[0]->
            unitList[real_fast_list[0]->firstOutput+i].activation;
          if (output>0)
            callback(data[i],image,x,y,mask->width,mask->height,
                     level,scale,output,0);
        }
      }
    }
//    SmoothByteImage(image);
//    ZoomByteImage(image,1.2,image);
    ReduceSize(image,image);
    scale*=1.2;
    level++;
  }
  // Clean up
  delete[] tmp;
}

void SearchMultiMergeFFT(ByteImage *image, int numNetworks,
                         SearchCallback callback, ClientData data[])
{
  int ptr=0;
  Mat mat=Zero(3,3);
  ByteImage *mask=FindByteImageWithName("facemask");
  for (int j=0; j<mask->height; j++) for (int i=0; i<mask->width; i++) 
    if (mask->data(ptr++))
    {
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
  while (image->height>=mask->height && image->width>=mask->width)
  {
    for (int y=0; y<image->height-mask->height+1; y++)
    {
      for (int x=0; x<image->width-mask->width+1; x++)
      {
        int ptr=0;
        v0=0; v1=0; v2=0;
        for (int j=0; j<mask->height; j++) {
          for (int i=0; i<mask->width; i++) {
          Byte data=image->data(i+x,j+y);
          tmp[ptr]=data;
          if (mask->data(ptr++))
          {
            v0+=i*data;
            v1+=j*data;
            v2+=data;
          }
        } }
        vec(0)=v0; vec(1)=v1; vec(2)=v2;
        vec=mat*vec;
        v0=vec(0); v1=vec(1); v2=vec(2);
        int hist[512];
        for (int i=0; i<512; i++) hist[i]=0;
        ptr=0;
        for (j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
        {
          int val=tmp[ptr]-(int)(i*v0+j*v1+v2-256.5);
          if (val<0) val=0;
          if (val>=512) val=511;
          if (mask->data(ptr)) hist[val]++;
          tmp[ptr++]=val;
        }
        int map[512];
        int num=0;
        for (i=0; i<512; i++)
        {
          map[i]=num;
          num+=hist[i];
        }
        int tot=num;
        for (i=511; i>=0; i--)
        {
          map[i]+=num;
          num-=hist[i];
        }
        double scaleFactor=1.0/tot;
        ImageToFFTFast(mask->width,mask->height,tmp,map,scaleFactor,
                       real_fast_list[0]);
        FastForwardPass(real_fast_list[0]);
        for (i=0; i<numNetworks; i++)
        {
          double output=real_fast_list[0]->
            unitList[real_fast_list[0]->firstOutput+i].activation;
          if (output>0)
            callback(data[i],image,x,y,mask->width,mask->height,
                     level,scale,output,0);
        }
      }
    }
    SmoothByteImage(image);
    ZoomByteImage(image,1.2,image);
    scale*=1.2;
    level++;
  }
  delete[] tmp;
}

#ifndef NoTcl
int SearchMultiCmd(ClientData data, Tcl_Interp *interp, 
                   int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: SearchMulti <image> {filelist}";
    return TCL_ERROR;
  }
  char **fileList;
  int numFiles;
  ByteImage *image=FindByteImageWithName(argv[1]);
  Tcl_SplitList(interp,argv[2],&numFiles,&fileList);
  int i;
  FILE **openFileList=new FILE*[numFiles];
  for (i=0; i<numFiles; i++) openFileList[i]=fopen(fileList[i],"w");
  free((char*)fileList);
  SearchMulti(image,numFiles,SearchMultiCallbackSave,
              (ClientData*)openFileList);
  for (i=0; i<numFiles; i++)
    fclose(openFileList[i]);
  delete[] openFileList;
  return TCL_OK;
}

int SearchMultiAverageCmd(ClientData data, Tcl_Interp *interp, 
                          int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: SearchMultiAverage <image> <outputfile>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  FILE *outf=fopen(argv[2],"w");
  SearchMultiAverage(image,real_numNetworks,SearchMultiCallbackSave,
                     (ClientData)outf);
  fclose(outf);
  return TCL_OK;
}

int SearchMultiMergeCmd(ClientData data, Tcl_Interp *interp, 
                        int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: SearchMultiMerge <image> {filelist}";
    return TCL_ERROR;
  }
  char **fileList;
  int numFiles;
  ByteImage *image=FindByteImageWithName(argv[1]);
  Tcl_SplitList(interp,argv[2],&numFiles,&fileList);
  int i;
  FILE **openFileList=new FILE*[numFiles];
  for (i=0; i<numFiles; i++) openFileList[i]=fopen(fileList[i],"w");
  free((char*)fileList);
  SearchMultiMerge(image,numFiles,SearchMultiCallbackSave,
                   (ClientData*)openFileList);
  for (i=0; i<numFiles; i++)
    fclose(openFileList[i]);
  delete[] openFileList;
  return TCL_OK;
}

int SearchMultiMergeFFTCmd(ClientData data, Tcl_Interp *interp, 
                           int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: SearchMulti <image> {filelist}";
    return TCL_ERROR;
  }
  char **fileList;
  int numFiles;
  ByteImage *image=FindByteImageWithName(argv[1]);
  Tcl_SplitList(interp,argv[2],&numFiles,&fileList);
  int i;
  FILE **openFileList=new FILE*[numFiles];
  for (i=0; i<numFiles; i++) openFileList[i]=fopen(fileList[i],"w");
  free((char*)fileList);
  SearchMultiMergeFFT(image,numFiles,SearchMultiCallbackSave,
                      (ClientData*)openFileList);
  for (i=0; i<numFiles; i++)
    fclose(openFileList[i]);
  delete[] openFileList;
  return TCL_OK;
}
#endif

#ifndef NoTcl
int SearchMultiInstrumentedCmd(ClientData data, Tcl_Interp *interp, 
                               int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: SearchMultiInstrumented <image> {filelist}";
    return TCL_ERROR;
  }
  char **fileList;
  int numFiles;
  ByteImage *image=FindByteImageWithName(argv[1]);
  Tcl_SplitList(interp,argv[2],&numFiles,&fileList);
  int i;
  FILE **openFileList=new FILE*[numFiles];
  for (i=0; i<numFiles; i++) openFileList[i]=fopen(fileList[i],"w");
  free((char*)fileList);
  int ptr=0;
  Mat mat=Zero(3,3);
  ByteImage *mask=GetFaceMask();
  int halfW=mask->width/2, halfH=mask->height/2;
  for (int j=-halfH; j<halfH; j++) for (i=-halfW; i<halfW; i++) 
    if (mask->data(ptr++))
    {
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
  int scaleNum=0;
  while (image->height>=mask->height && image->width>=mask->width)
  {
    char name[256];
    sprintf(name,"output%d",scaleNum);
    ByteImage *outputImage=new ByteImage(name);
    outputImage->NewBuffer((Byte*)malloc(image->size),
                                         image->width,
                                         image->height);
    for (int y=0; y<image->height; y++)
    {
      for (int x=0; x<image->width; x++)
      {
        int ptr=0;
        v0=0; v1=0; v2=0;
        for (int j=-halfH; j<halfH; j++) for (int i=-halfW; i<halfW; i++) 
        {
          int ii=i+x;
          int jj=j+y;
          if (ii<0) ii=0; if (ii>=image->width) ii=image->width-1;
          if (jj<0) jj=0; if (jj>=image->height) jj=image->height-1;
          Byte data=image->data(ii,jj);
          tmp[ptr]=data;
          if (mask->data(ptr++))
          {
            v0+=i*data;
            v1+=j*data;
            v2+=data;
          }
        }
        vec(0)=v0; vec(1)=v1; vec(2)=v2;
        vec=mat*vec;
        v0=vec(0); v1=vec(1); v2=vec(2);
        int hist[512];
        for (i=0; i<512; i++) hist[i]=0;
        ptr=0;
        for (j=-halfH; j<halfH; j++) for (i=-halfW; i<halfW; i++) 
        {
          int val=tmp[ptr]-(int)(i*v0+j*v1+v2-256.5);
          if (val<0) val=0;
          if (val>=512) val=511;
          if (mask->data(ptr)) hist[val]++;
          tmp[ptr++]=val;
        }
        int map[512];
        int num=0;
        for (i=0; i<512; i++)
        {
          map[i]=num;
          num+=hist[i];
        }
        int tot=num;
        for (i=511; i>=0; i--)
        {
          map[i]+=num;
          num-=hist[i];
        }
        double scaleFactor=1.0/tot;
        FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;

        for (i=0; i<numFiles; i++)
        {
          if (i>0)
          {
            FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
            FastForwardUnit *unit1=&(real_fast_list[i]->unitList[1]);
            for (int j=0; j<mask->size; j++)
              (unit1++)->activation=(unit++)->activation;
          }
          double output=FastForwardPass(real_fast_list[i]);
          int val=(int)floor((output+1)*100);
          if (val<0) val=0;
          if (val>=199) val=199;
          outputImage->data(x,y)=val;
          if (output>0) {
            fprintf(openFileList[i],"%g %g %g %g %d %d %d %g\n",
                    scale*(x-halfW),scale*(y-halfH),
                    scale*(x+halfW),scale*(y+halfH),
                    x,y,(int)(floor(0.5+log(scale)/log(1.2))),
                    output);
          }
        }
      }
    }
    ReduceSize(image,image);
//    SmoothByteImage(image);
//    ZoomByteImage(image,1.2,image);
    scale*=1.2;
    scaleNum++;
  }
  delete[] tmp;
  for (i=0; i<numFiles; i++)
    fclose(openFileList[i]);
  delete[] openFileList;
  sprintf(interp->result,"%d",scaleNum);
  return TCL_OK;
}
#endif

#ifndef NoTcl
int SearchMultiMirrorCmd(ClientData data, Tcl_Interp *interp, 
                         int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: SearchMultiMirror <image> {filelist} <mask>";
    return TCL_ERROR;
  }
  char **fileList;
  int numFiles;
  ByteImage *image=FindByteImageWithName(argv[1]);
  Tcl_SplitList(interp,argv[2],&numFiles,&fileList);
  ByteImage *mask=FindByteImageWithName(argv[3]);
  int i;
  FILE **openFileList=new FILE*[numFiles];
  for (i=0; i<numFiles; i++) openFileList[i]=fopen(fileList[i],"w");
  free((char*)fileList);
  int ptr=0;
  Mat mat=Zero(3,3);
  for (int j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
    if (mask->data(ptr++))
    {
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
  mat=Zero(3,3);

  int *tmp=new int[mask->size];

  Vec vec(3);
  double v0=0, v1=0, v2=0;
  double scale=1.0;
  while (image->height>=mask->height && image->width>=mask->width)
  {
    for (int y=0; y<image->height-mask->height+1; y++)
    {
      for (int x=0; x<image->width-mask->width+1; x++)
      {
        int ptr=0;
        v0=0; v1=0; v2=0;
        for (int j=0; j<mask->height; j++) for (int i=0; i<mask->width; i++) 
        {
          Byte data=image->data(mask->width-1-i+x,j+y);
          tmp[ptr]=data;
          if (mask->data(ptr++))
          {
            v0+=i*data;
            v1+=j*data;
            v2+=data;
          }
        }
        vec(0)=v0; vec(1)=v1; vec(2)=v2;
        vec=mat*vec;
        v0=vec(0); v1=vec(1); v2=vec(2);
        int hist[512];
        for (i=0; i<512; i++) hist[i]=0;
        ptr=0;
        for (j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
        {
          int val=tmp[ptr]-(int)(i*v0+j*v1+v2-256.5);
          if (val<0) val=0;
          if (val>=512) val=511;
          if (mask->data(ptr)) hist[val]++;
          tmp[ptr++]=val;
        }
        int map[512];
        int num=0;
        for (i=0; i<512; i++)
        {
          map[i]=num;
          num+=hist[i];
        }
        int tot=num;
        for (i=511; i>=0; i--)
        {
          map[i]+=num;
          num-=hist[i];
        }
        double scaleFactor=1.0/tot;
        FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;

        for (i=0; i<numFiles; i++)
        {
          if (i>0)
          {
            FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
            FastForwardUnit *unit1=&(real_fast_list[i]->unitList[1]);
            for (int j=0; j<mask->size; j++)
              (unit1++)->activation=(unit++)->activation;
          }
          double output=FastForwardPass(real_fast_list[i]);
          if (output>0)
          {
            fprintf(openFileList[i],"%g %g %g %g %d %d %d %g\n",
                    scale*x,scale*y,
                    scale*(x+mask->width),scale*(y+mask->height),
                    x,y,(int)(floor(0.5+log(scale)/log(1.2))),
                    output);
          }
        }
      }
    }
    SmoothByteImage(image);
    ZoomByteImage(image,1.2,image);
    scale*=1.2;
  }
  delete[] tmp;
  for (i=0; i<numFiles; i++)
    fclose(openFileList[i]);
  delete[] openFileList;
  return TCL_OK;
}
#endif

// USED
// Apply combination of router and individual view networks to an image.
// The router indicates which of the view networks to apply.
void SearchRouter(ByteImage *image, ByteImage *mask,
                  SearchCallback callback, ClientData data[])
{
  int i;
  int ptr=0;
  int *tmp=new int[mask->size];

  int halfW=mask->width/2;
  int halfH=mask->height/2;
  int level=0;
  double scale=1.0;
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
	// Check the router outputs, invoking the view network for each
	// output that is turned on.
        for (i=0; i<5; i++) {
          int outlist=0;      // Which detection list to append to
          if (real_fast_list[0]->unitList[real_fast_list[0]->firstOutput+i]
              .activation>0.0) {
	    // If router was positive, figure out which network to use
            int net=i-1, flip=0;
            if (i==0) { net=3; flip=1; }
            if (i==1) { net=2; flip=1; }
            double output=0.0;
            while (net<real_numNetworks) {
	      // Copy image into network, mirroring if necessary
              FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
              FastForwardUnit *unit1=&(real_fast_list[net]->unitList[1]);
              if (!flip) {
                for (int j=0; j<mask->size; j++)
                  (unit1++)->activation=(unit++)->activation;
              } else {
                for (int y=0; y<mask->height; y++) {
                  for (int x=0; x<mask->width; x++)
                    (unit1++)->activation=(unit+mask->width-1-x)->activation;
                  unit+=mask->width;
                }
              }
              output=FastForwardPass(real_fast_list[net]);
	      // If network detect face, record it
              if (output>0) {
                callback(data[outlist],image,x-halfW,y-halfH,
                         mask->width,mask->height,
                         level,scale,output,i);
              }
              net+=3;     // Go to next network for this view
              outlist++;  // Go to next output list for this view
            }
          }
        }
      }
    }
    ZoomByteImage(image,1.2,image);
    scale*=1.2;
    level++;
  }
  // Clean up
  delete[] tmp;
}

#ifndef NoTcl
#ifdef notdef
int SearchMultiHistOnlyCmd(ClientData data, Tcl_Interp *interp, 
                           int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: SearchMultiHistOnly <image> {filelist} <mask>";
    return TCL_ERROR;
  }
  char **fileList;
  int numFiles;
  ByteImage *image=FindByteImageWithName(argv[1]);
  Tcl_SplitList(interp,argv[2],&numFiles,&fileList);
  ByteImage *mask=FindByteImageWithName(argv[3]);
  int i;
  FILE **openFileList=new FILE*[numFiles];
  for (i=0; i<numFiles; i++) openFileList[i]=fopen(fileList[i],"w");
  free((char*)fileList);
  int ptr=0;

  int *tmp=new int[mask->size];

  double scale=1.0;
  while (image->height>=mask->height && image->width>=mask->width)
  {
    for (int y=0; y<image->height-mask->height+1; y++)
    {
      for (int x=0; x<image->width-mask->width+1; x++)
      {
        int ptr=0;
        int hist[256];
        for (i=0; i<256; i++) hist[i]=0;
        for (int j=0; j<mask->height; j++) for (int i=0; i<mask->width; i++) 
        {
          Byte data=image->data(i+x,j+y);
          tmp[ptr++]=data;
          hist[data]++;
        }
        int map[256];
        int num=0;
        for (i=0; i<256; i++)
        {
          map[i]=num;
          num+=hist[i];
        }
        int tot=num;
        for (i=255; i>=0; i--)
        {
          map[i]+=num;
          num-=hist[i];
        }
        double scaleFactor=1.0/tot;
        FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;

        for (i=0; i<numFiles; i++)
        {
          if (i>0)
          {
            FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
            FastForwardUnit *unit1=&(real_fast_list[i]->unitList[1]);
            for (int j=0; j<mask->size; j++)
              (unit1++)->activation=(unit++)->activation;
          }
          double output=FastForwardPass(real_fast_list[i]);
          if (output>0)
          {
            fprintf(openFileList[i],"%g %g %g %g %d %d %d %g\n",
                    scale*x,scale*y,
                    scale*(x+mask->width),scale*(y+mask->height),
                    x,y,(int)(floor(0.5+log(scale)/log(1.2))),
                    output);
          }
        }
      }
    }
    //    SmoothByteImage(image);
    ZoomByteImage(image,1.2,image);
    scale*=1.2;
  }
  delete[] tmp;
  for (i=0; i<numFiles; i++)
    fclose(openFileList[i]);
  delete[] openFileList;
  return TCL_OK;
}
#endif

// USED
// Apply multiple networks to an image, using only histogram 
// equalization for preprocessing.
int SearchMultiHistOnlyCmd(ClientData data, Tcl_Interp *interp, 
                           int argc, char *argv[])
{
  if (argc!=4) {
    interp->result="Usage: SearchMultiHistOnly <image> {filelist} <mask>";
    return TCL_ERROR;
  }
  char **fileList;
  int numFiles;
  ByteImage *image=FindByteImageWithName(argv[1]);
  Tcl_SplitList(interp,argv[2],&numFiles,&fileList);
  ByteImage *mask=FindByteImageWithName(argv[3]);
  int i;

  // Create the output files
  FILE **openFileList=new FILE*[numFiles];
  for (i=0; i<numFiles; i++) openFileList[i]=fopen(fileList[i],"w");
  free((char*)fileList);
  int ptr=0;

  int *tmp=new int[mask->size];

  int halfW=mask->width/2;
  int restW=mask->width-halfW;
  int halfH=mask->height/2;
  int restH=mask->height-halfH;

  int level=0;
  double scale=1.0;
  // Loop over levels in input pyramid
  while (image->height>=mask->height && image->width>=mask->width) {
    for (int y=0; y<image->height; y++) {
      for (int x=0; x<image->width; x++) {
        int ptr=0;
        int hist[256];
        for (i=0; i<256; i++) hist[i]=0;
	// Extract window and compute histogram
        for (int j=-halfH; j<restH; j++) for (int i=-halfW; i<restW; i++) {
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
	// Compute cumulative histogram 
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
	// Put window into network, applying histogram equalization
        double scaleFactor=1.0/tot;
        FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;

        for (i=0; i<numFiles; i++) {
	  // Apply other networks also
          if (i>0) {
            FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
            FastForwardUnit *unit1=&(real_fast_list[i]->unitList[1]);
            for (int j=0; j<mask->size; j++)
              (unit1++)->activation=(unit++)->activation;
          }
	  // Record any positive outputs
          double output=FastForwardPass(real_fast_list[i]);
#ifdef DEBUG
          if (x==DebugX && y==DebugY && level==DebugS) {
            fprintf(stderr,"output=%g\n",output);
            exit(1);
          }
#endif
          if (output>0) {
            fprintf(openFileList[i],"%g %g %g %g %d %d %d %g\n",
                    scale*(x-halfW),scale*(y-halfH),
                    scale*(x+halfW),scale*(y+halfH),
                    x-halfW,y-halfH,(int)(floor(0.5+log(scale)/log(1.2))),
                    output);
          }
        }
      }
    }
    //    SmoothByteImage(image);
    ZoomByteImage(image,1.2,image);
    scale*=1.2;
    level++;
  }
  // Clean up and close output files
  delete[] tmp;
  for (i=0; i<numFiles; i++)
    fclose(openFileList[i]);
  delete[] openFileList;
  return TCL_OK;
}

// USED 
// Apply the router + view networks to an image, recording locations at
// which all the networks for a particular view agree there is a face.
int SearchRouterCmd(ClientData data, Tcl_Interp *interp, 
                    int argc, char *argv[])
{
  if (argc!=4) {
    interp->result="Usage: SearchRouter <image> <outputfile> <mask>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  FILE *outf=fopen(argv[2],"w");
  ByteImage *mask=FindByteImageWithName(argv[3]);
  assert(image!=NULL);
  assert(outf!=NULL);
  assert(mask!=NULL);

  int i;
  int ptr=0;
  int *tmp=new int[mask->size];

  int halfW=mask->width/2;
  int halfH=mask->height/2;

  double scale=1.0;
  // Go over all levels in input pyramid
  while (image->height>=mask->height && image->width>=mask->width) {
    for (int y=0; y<image->height; y++) {
      for (int x=0; x<image->width; x++) {
        int ptr=0;
        int hist[256];
        for (i=0; i<256; i++) hist[i]=0;
	// Extract window and compute histogram
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
	// Compute cumulative histogram
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
	// Put window into router network, applying histogram equalization
        double scaleFactor=1.0/tot;
        FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
//        fprintf(stderr,"mask=%dx%d=%d\n",mask->width,mask->height,mask->size);
//        fprintf(stderr,"input=%d\n",real_fast_list[0]->numInputs);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;
        FastForwardPass(real_fast_list[0]);
	// Loop through the outputs from the router
        for (i=0; i<5; i++) 
          if (real_fast_list[0]->unitList[real_fast_list[0]->firstOutput+i]
              .activation>0.0) {
	    // Figure out which view network to use
            int net=i-1, flip=0;
            if (i==0) { net=3; flip=1; }
            if (i==1) { net=2; flip=1; }
            int okay=1; 
            double output=0.0;
            while (net<real_numNetworks && okay) {
	      // Apply the view network to the image, flipping the window
	      // if necessary.
              FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
              FastForwardUnit *unit1=&(real_fast_list[net]->unitList[1]);
              if (!flip) {
                for (int j=0; j<mask->size; j++)
                  (unit1++)->activation=(unit++)->activation;
              } else {
                for (int y=0; y<mask->height; y++) {
                  for (int x=0; x<mask->width; x++)
                    (unit1++)->activation=(unit+mask->width-1-x)->activation;
                  unit+=mask->width;
                }
              }
              output=FastForwardPass(real_fast_list[net]);
	      // Accumulate outputs from all view networks
              if (output<=0) okay=0;
              net+=3;
            }
            if (okay) {
	      // Record output
              fprintf(outf,"%g %g %g %g %d %d %d %g %d\n",
                      scale*(x-halfW),scale*(y-halfH),
                      scale*(x+halfW),scale*(y+halfH),
                      x-halfW,y-halfH,
                      (int)(floor(0.5+log(scale)/log(1.2))),
                      output,i);
            }
          }
      }
    }
    // Go to next level
    ZoomByteImage(image,1.2,image);
    scale*=1.2;
  }
  delete[] tmp;
  fclose(outf);
  return TCL_OK;
}

// USED
// Apply multiple networks to an image, using only histogram 
// equalization for preprocessing.
int SearchMultiMirrorHistOnlyCmd(ClientData data, Tcl_Interp *interp, 
                                 int argc, char *argv[])
{
  if (argc!=4) {
    interp->result="Usage: SearchMultiMirrorHistOnly <image> {filelist} <mask>";
    return TCL_ERROR;
  }
  char **fileList;
  int numFiles;
  ByteImage *image=FindByteImageWithName(argv[1]);
  Tcl_SplitList(interp,argv[2],&numFiles,&fileList);
  ByteImage *mask=FindByteImageWithName(argv[3]);
  int i;

  // Create the output files
  FILE **openFileList=new FILE*[numFiles];
  for (i=0; i<numFiles; i++) openFileList[i]=fopen(fileList[i],"w");
  free((char*)fileList);
  int ptr=0;

  int *tmp=new int[mask->size];

  int halfW=mask->width/2;
  int restW=mask->width-halfW;
  int halfH=mask->height/2;
  int restH=mask->height-halfH;

  double scale=1.0;
  // Loop over levels in input pyramid
  while (image->height>=mask->height && image->width>=mask->width) {
    for (int y=0; y<image->height; y++) {
      for (int x=0; x<image->width; x++) {
        int ptr=0;
        int hist[256];
        for (i=0; i<256; i++) hist[i]=0;
	// Extract window and compute histogram
        for (int j=-halfH; j<restH; j++) for (int i=-halfW; i<restW; i++) {
          int ii=x-i-1;
          int jj=j+y;
          if (ii<0) ii=0;
          if (ii>=image->width) ii=image->width-1;
          if (jj<0) jj=0;
          if (jj>=image->height) jj=image->height-1;
          int data=((int)image->data(ii,jj));
          tmp[ptr++]=data;
          hist[data]++;
        }
	// Compute cumulative histogram 
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
	// Put window into network, applying histogram equalization
        double scaleFactor=1.0/tot;
        FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;

        for (i=0; i<numFiles; i++) {
	  // Apply other networks also
          if (i>0) {
            FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
            FastForwardUnit *unit1=&(real_fast_list[i]->unitList[1]);
            for (int j=0; j<mask->size; j++)
              (unit1++)->activation=(unit++)->activation;
          }
	  // Record any positive outputs
          double output=FastForwardPass(real_fast_list[i]);
          if (output>0) {
            fprintf(openFileList[i],"%g %g %g %g %d %d %d %g\n",
                    scale*(x-halfW),scale*(y-halfH),
                    scale*(x+halfW),scale*(y+halfH),
                    x-halfW,y-halfH,(int)(floor(0.5+log(scale)/log(1.2))),
                    output);
          }
        }
      }
    }
    //    SmoothByteImage(image);
    ZoomByteImage(image,1.2,image);
    scale*=1.2;
  }
  // Clean up and close output files
  delete[] tmp;
  for (i=0; i<numFiles; i++)
    fclose(openFileList[i]);
  delete[] openFileList;
  return TCL_OK;
}
#endif

#ifndef NoTcl
int SearchClustersHistOnlyCmd(ClientData data, Tcl_Interp *interp, 
                              int argc, char *argv[])
{
  if (argc!=5)
  {
    interp->result="Usage: SearchClustersMultiHistOnly <image> <output> <mask> <cluster file>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  FILE *outf=fopen(argv[2],"w");
  ByteImage *mask=FindByteImageWithName(argv[3]);
  FILE *inf=fopen(argv[4],"r");
  Clusters2 clusters(10,0,0,0,mask->size);
  clusters.MakeClusters();
  clusters.Read(inf);
  fclose(inf);

  int *tmp=new int[mask->size];
  double scale=1.0;
  Vec check(mask->size);
  while (image->height>=mask->height && image->width>=mask->width)
  {
    for (int y=0; y<image->height-mask->height+1; y++)
    {
      for (int x=0; x<image->width-mask->width+1; x++)
      {
        int ptr=0;
        int hist[256];
        for (int i=0; i<256; i++) hist[i]=0;
        for (int j=0; j<mask->height; j++) for (int i=0; i<mask->width; i++) 
        {
          Byte data=image->data(i+x,j+y);
          tmp[ptr++]=data;
          hist[data]++;
        }
        int map[256];
        int num=0;
        for (i=0; i<256; i++)
        {
          map[i]=num;
          num+=hist[i];
        }
        int tot=num;
        for (i=255; i>=0; i--)
        {
          map[i]+=num;
          num-=hist[i];
        }
        double scaleFactor=1.0/tot;
        for (i=0; i<mask->size; i++)
          check(i)=map[tmp[i]]*scaleFactor-1.0;
        int bestPos=-1;
        double bestDist=0;
        for (Cluster2 *c=clusters.clusters.first; c!=NULL; c=c->next)
        {
          double dist=Abs(c->average-check);
          if (bestPos==-1 || dist<bestDist)
          {
            bestPos=c->positive;
            bestDist=dist;
          }
        }
        if (bestPos)
        {
          fprintf(outf,"%g %g %g %g %d %d %d %g\n",
                  scale*x,scale*y,
                  scale*(x+mask->width),scale*(y+mask->height),
                  x,y,(int)(floor(0.5+log(scale)/log(1.2))),
                  1.0);
        }
      }
    }
    SmoothByteImage(image);
    ZoomByteImage(image,1.2,image);
    scale*=1.2;
  }
  fclose(outf);
  delete[] tmp;
  return TCL_OK;
}
#endif

#ifndef NoTcl
int SearchMultiEdgeCmd(ClientData data, Tcl_Interp *interp, 
                       int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: SearchMultiEdge <image> {filelist}";
    return TCL_ERROR;
  }
  char **fileList;
  int numFiles;
  ByteImage *image=FindByteImageWithName(argv[1]);
  Tcl_SplitList(interp,argv[2],&numFiles,&fileList);
  int i;
  FILE **openFileList=new FILE*[numFiles];
  for (i=0; i<numFiles; i++) openFileList[i]=fopen(fileList[i],"w");
  free((char*)fileList);
  ByteImage *mask=GetFaceMask();
  int *tmp=new int[mask->size];

  double scale=1.0;
  while (image->height>=mask->height && image->width>=mask->width)
  {
    for (int y=0; y<image->height-mask->height+1; y++)
    {
      for (int x=0; x<image->width-mask->width+1; x++)
      {
        int ptr=0;

        for (int j=y; j<y+mask->height; j++)
          for (int i=x; i<x+mask->width; i++)
          {
            int dx=0;
            if (i==0) dx=image->data(1,j)-image->data(0,j); else
              if (i==image->width-1) dx=image->data(i,j)-image->data(i-1,j); else
                dx=(image->data(i+1,j)-image->data(i-1,j))/2;
            int dy=0;
            if (j==0) dy=image->data(i,1)-image->data(i,0); else
              if (j==image->height-1) dy=image->data(i,j)-image->data(i,j-1); else
                dy=(image->data(i,j+1)-image->data(i,j-1))/2;
            int val=(int)floor(0.5+sqrt(dx*dx+dy*dy));
            if (val>254) val=254;
            tmp[ptr++]=val;
          }

        FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=(tmp[i]-127)/127.0;

        for (i=0; i<numFiles; i++)
        {
          if (i>0)
          {
            FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
            FastForwardUnit *unit1=&(real_fast_list[i]->unitList[1]);
            for (int j=0; j<mask->size; j++)
              (unit1++)->activation=(unit++)->activation;
          }
          double output=FastForwardPass(real_fast_list[i]);
          if (output>0)
          {
            fprintf(openFileList[i],"%g %g %g %g %d %d %d %g\n",
                    scale*x+2,scale*y+2,
                    scale*(x+mask->width)+2,scale*(y+mask->height)+2,
                    x,y,(int)(floor(0.5+log(scale)/log(1.2))),
                    output);
          }
        }
      }
    }
    SmoothByteImage(image);
    ZoomByteImage(image,1.2,image);
    scale*=1.2;
  }
  delete[] tmp;
  for (i=0; i<numFiles; i++)
    fclose(openFileList[i]);
  delete[] openFileList;
  return TCL_OK;
}
#endif

#ifndef NoTcl
int SearchMultiAndCmd(ClientData data, Tcl_Interp *interp, 
                      int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: SearchMultiAnd <image> <file>";
    return TCL_ERROR;
  }
  ByteImage *image=FindByteImageWithName(argv[1]);
  FILE *outf=fopen(argv[2],"w");
  int i;
  int ptr=0;
  Mat mat=Zero(3,3);
  ByteImage *mask=GetFaceMask();
  for (int j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
    if (mask->data(ptr++))
    {
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
  while (image->height>=mask->height && image->width>=mask->width)
  {
    for (int y=0; y<image->height-mask->height+1; y++)
    {
      for (int x=0; x<image->width-mask->width+1; x++)
      {
        int ptr=0;
        v0=0; v1=0; v2=0;
        for (int j=0; j<mask->height; j++) for (int i=0; i<mask->width; i++) 
        {
          Byte data=image->data(i+x,j+y);
          tmp[ptr]=data;
          if (mask->data(ptr++))
          {
            v0+=i*data;
            v1+=j*data;
            v2+=data;
          }
        }
        vec(0)=v0; vec(1)=v1; vec(2)=v2;
        vec=mat*vec;
        v0=vec(0); v1=vec(1); v2=vec(2);
        int hist[512];
        for (i=0; i<512; i++) hist[i]=0;
        ptr=0;
        for (j=0; j<mask->height; j++) for (i=0; i<mask->width; i++) 
        {
          int val=tmp[ptr]-(int)(i*v0+j*v1+v2)+256;
          if (val<0) val=0;
          if (val>=512) val=511;
          if (mask->data(ptr)) hist[val]++;
          tmp[ptr++]=val;
        }
        int map[512];
        int num=0;
        for (i=0; i<512; i++)
        {
          map[i]=num;
          num+=hist[i];
        }
        int tot=num;
        for (i=511; i>=0; i--)
        {
          map[i]+=num;
          num-=hist[i];
        }
        double scaleFactor=1.0/tot;
        FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
        for (i=0; i<mask->size; i++)
          (unit++)->activation=map[tmp[i]]*scaleFactor-1.0;

        double minOutput=1.0;
        for (i=0; i<real_numNetworks; i++)
        {
          if (i>0)
          {
            FastForwardUnit *unit=&(real_fast_list[0]->unitList[1]);
            FastForwardUnit *unit1=&(real_fast_list[i]->unitList[1]);
            for (int j=0; j<mask->size; j++)
              (unit1++)->activation=(unit++)->activation;
          }
          double output=FastForwardPass(real_fast_list[i]);
          if (output<minOutput) minOutput=output;
        }
        if (minOutput>0)
        {
          fprintf(outf,"%g %g %g %g %d %d %d %g\n",
                  scale*x+2,scale*y+2,
                  scale*(x+mask->width)+2,scale*(y+mask->height)+2,
                  x,y,(int)(floor(0.5+log(scale)/log(1.2))),
                  minOutput);
        }
      }
    }
    SmoothByteImage(image);
    ZoomByteImage(image,1.2,image);
    scale*=1.2;
  }
  delete[] tmp;
  fclose(outf);
  return TCL_OK;
}
#endif

#ifndef NoTcl
int DuplicateFaceCmd(ClientData data, Tcl_Interp *interp,
                     int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: DuplicateFace <face1> <face2>";
    return TCL_ERROR;
  }
  double x1, y1, x2, y2;
  int s1, s2;
  sscanf(argv[1],"%lf %lf %d",&x1,&y1,&s1);
  sscanf(argv[2],"%lf %lf %d",&x2,&y2,&s2);
  int dup=0;
  if (abs(s1-s2)<=2)
  {
    x1/=pow(1.2,s2-s1);
    y1/=pow(1.2,s2-s1);
    if (sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))<=8)
      dup=1;
  }
  sprintf(interp->result,"%d",dup);
  return TCL_OK;
}
#endif

#ifndef NoTcl
void Search_Init(Tcl_Interp *interp)
{
#ifndef NoHEBP
  Tcl_CreateCommand(interp,"TestHEBPResults",TestHEBPResultsCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#endif

  Tcl_CreateCommand(interp,"SearchScale",SearchScaleCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"CheckResults",CheckResultsCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"FuzzyVote",FuzzyVoteCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"FuzzyVote2",FuzzyVote2Cmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"FuzzyVote3",FuzzyVote3Cmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"NormFace",NormFaceCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"NormFaceSmart",NormFaceSmartCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"NormFaceHistogram",NormFaceHistogramCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"EdgeDetectFace",EdgeDetectFaceCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"NormHistFace",NormHistFaceCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#ifndef NoHEBP
  Tcl_CreateCommand(interp,"TestHEBPRanked",TestHEBPRankedCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"TestHEBPRecon",TestHEBPReconCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"TestHEBPOneOfMany",TestHEBPOneOfManyCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"TestUme",TestUmeCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"RunUme",RunUmeCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#endif
  Tcl_CreateCommand(interp,"SearchUme",SearchUmeCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"PostProcessUme",PostProcessUmeCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);

  Tcl_CreateCommand(interp,"SearchUmeFast",SearchUmeFastCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SearchUmeFaster",SearchUmeFasterCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SearchUmeEvenFaster",SearchUmeEvenFasterCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SearchUmeFasterStill",SearchUmeFasterStillCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#ifdef CompiledNets
  Tcl_CreateCommand(interp,"SearchUmeFasterStillCompiled",
                    SearchUmeFasterStillCompiledCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#endif
  Tcl_CreateCommand(interp,"SearchUmeFasterStillPrecise",
                    SearchUmeFasterStillPreciseCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#ifdef CompiledNets
  Tcl_CreateCommand(interp,"SearchUmeFasterStillPreciseCompiled",
                    SearchUmeFasterStillPreciseCompiledCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#endif
  Tcl_CreateCommand(interp,"PostProcessUmeFast",PostProcessUmeFastCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"PostProcessUmeFaster",PostProcessUmeFasterCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"PostProcessUmeEvenFaster",
                    PostProcessUmeEvenFasterCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);

  Tcl_CreateCommand(interp,"SearchInstrumented",SearchInstrumentedCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"CheckSearchInstrumented",
                    CheckSearchInstrumentedCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"CheckSearchInstrumentedUme",
                    CheckSearchInstrumentedUmeCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);

  Tcl_CreateCommand(interp,"ProducePhotoBook",
                    ProducePhotoBookCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);

  Tcl_CreateCommand(interp,"CountVotes",
                    CountVotesCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"CountVotes2",
                    CountVotes2Cmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"NetArbitrate",
                    NetArbitrateCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"NetArbitrate2",
                    NetArbitrate2Cmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);

  Tcl_CreateCommand(interp,"SearchMulti",SearchMultiCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SearchMultiAverage",SearchMultiAverageCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SearchMultiMerge",SearchMultiMergeCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SearchMultiMergeFFT",SearchMultiMergeFFTCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SearchMultiInstrumented",
                    SearchMultiInstrumentedCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SearchMultiMirror",SearchMultiMirrorCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SearchMultiHistOnly",SearchMultiHistOnlyCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SearchRouter",SearchRouterCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SearchMultiMirrorHistOnly",SearchMultiMirrorHistOnlyCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SearchClustersHistOnly",SearchClustersHistOnlyCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SearchMultiEdge",SearchMultiEdgeCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"SearchMultiAnd",SearchMultiAndCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);

  Tcl_CreateCommand(interp,"EuclideanDistance",EuclideanDistanceCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"DuplicateFace",DuplicateFaceCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
}
#endif
