#ifdef __GNUC__
#include <string.h>
#endif

#ifndef WINNT
extern "C" {
#endif
#include <stdlib.h>
#include <stdio.h>
#ifndef __GNUC__
#include <string.h>
#endif
#ifndef NoVista
#include <vista/VImage.h>
#endif
#ifndef hpux
#include <math.h>
#endif
#ifndef WINNT
#include <unistd.h>
}
#endif

#if defined(WINNT) && !defined(MSVC)
#include <dir.h>
#endif

#ifdef MSVC
#include <direct.h>
#endif

#ifdef hpux
#include <math.h>
#endif

#include "system.hpp"
#include "myhebp.hpp"

FastForwardStruct *real_fast=NULL;
FastForwardStruct **real_fast_list=NULL;
#ifndef NoHEBP
network *real_net;
#endif
float real_inputs[30*30];
float real_outputs[30*30*2];
float real_targets[30*30*2];
int real_numNetworks;

//#define Test

#ifndef NoTcl
int FastRandomizeWeightsCmd(ClientData, Tcl_Interp *interp,
                            int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: FastRandomizeWeights <net>";
    return TCL_OK;
  }
  int net=atoi(argv[1]);
  FastRandomizeWeights(real_fast_list[net]);
  return TCL_OK;
}
#endif

#ifndef NoTcl
int TestNetworkSensitivityCmd(ClientData data, Tcl_Interp *interp,
                              int argc, char *argv[])
{
  if (argc!=8)
  {
    interp->result="Usage: TestNetworkSensitivity <network> <data-pos> <data-neg> <xs> <ys> <xb> <yb>";
    return TCL_ERROR;
  }
  FastForwardStruct *network=real_fast_list[atoi(argv[1])];
  char *dataFilePos=argv[2];
  char *dataFileNeg=argv[3];
  int xs=atoi(argv[4]);
  int ys=atoi(argv[5]);
  int xb=atoi(argv[6]);
  int yb=atoi(argv[7]);

  int size=xs*ys;
  int numPos=FileSize(dataFilePos);
  int numNeg=FileSize(dataFileNeg);
  numPos/=size;
  numNeg/=size;
  int numExamples=numPos+numNeg;
  signed char *dat=new signed char[numExamples*size];
  FILE *inf=fopen(dataFilePos,"r");
  fread((char*)dat,numPos*size,1,inf);
  fclose(inf);
  inf=fopen(dataFileNeg,"r");
  fread((char*)(dat+numPos*size),numNeg*size,1,inf);
  fclose(inf);
  
  Tcl_DString ds;
  Tcl_DStringInit(&ds);

  for (int j=0; j<ys/yb; j++)
  {
    Tcl_DStringAppend(&ds,"",-1);
    for (int i=0; i<xs/xb; i++)
    {
      int numWrong=0;
      for (int k=0; k<numExamples; k++)
      {
        for (int l=0; l<size; l++)
        {
          //          fprintf(stderr,"%d %d %d %d %08X %08X %d %d\n",i,j,k,l,network,dat,numExamples,size);
          network->unitList[l+1].activation=dat[k*size+l]/127.0;
        }
        for (int jj=j*yb; jj<(j+1)*yb; jj++)
          for (int ii=i*xb; ii<(i+1)*xb; ii++)
            network->unitList[ii+1+jj*xs].activation=RandomDouble()*2.0-1.0;
        int output=FastForwardPass(network)>0.0;
        if (k<numPos && !output) numWrong++;
        if (k>=numPos && output) numWrong++;
      }
      char tem[64];
      sprintf(tem,"%d ",numWrong);
      Tcl_DStringAppend(&ds,tem,-1);
      fprintf(stderr,".");
    }
    Tcl_DStringAppend(&ds,"\n",-1);
  }
  Tcl_DStringResult(interp,&ds);
  delete[] dat;
  fprintf(stderr,"\n");
  return TCL_OK;
}
#endif

#ifndef NoVista
char *TestNetworkSensitivityVista(int net, VImage posimage,
                                  char *dataFileNeg,
                                  int xb, int yb)
{
  FastForwardStruct *network=real_fast_list[net];
  int xs=VImageNColumns(posimage);
  int ys=VImageNRows(posimage);
  int size=xs*ys;
  int numPos=VImageNBands(posimage);
  int numNeg=FileSize(dataFileNeg)/size;
  int numExamples=numPos+numNeg;
  signed char *dat=new signed char[numExamples*size];
  memcpy(dat,posimage->data,numPos*size);
  FILE *inf=fopen(dataFileNeg,"r");
  fread((char*)(dat+numPos*size),numNeg*size,1,inf);
  fclose(inf);
  
  Tcl_DString ds;
  Tcl_DStringInit(&ds);

  for (int j=0; j<ys/yb; j++)
  {
    Tcl_DStringAppend(&ds,"",-1);
    for (int i=0; i<xs/xb; i++)
    {
      double error=0;
      for (int k=0; k<numExamples; k++)
      {
        for (int l=0; l<size; l++)
          network->unitList[l+1].activation=dat[k*size+l]/127.0;
        for (int jj=j*yb; jj<(j+1)*yb; jj++)
          for (int ii=i*xb; ii<(i+1)*xb; ii++)
            network->unitList[ii+1+jj*xs].activation=RandomDouble()*2.0-1.0;
        double output=FastForwardPass(network);
        if (k<numPos) error+=(output-1.0)*(output-1.0); else
          error+=(output+1.0)*(output+1.0);
      }
      char tem[64];
      sprintf(tem,"%g ",sqrt(error/(numPos+numNeg)));
//      fprintf(stderr,"%g ",sqrt(error/(numPos+numNeg)));
      Tcl_DStringAppend(&ds,tem,-1);
      fprintf(stderr,". ");
    }
    Tcl_DStringAppend(&ds,"\n",-1);
  }
  delete[] dat;
  fprintf(stderr,"\n");
  char *result=strdup(Tcl_DStringValue(&ds));
  Tcl_DStringFree(&ds);
  return result;
}
#endif

#ifndef NoHEBP
void HEBPInit(char *directory, int numNetworks, char *networkList[],
              int needbuf)
{
  real_numNetworks=numNetworks;
  net_spec *spec=new net_spec[numNetworks+1];
  for (int i=0; i<numNetworks; i++)
  {
    strcpy(spec[i].network_name,networkList[i]);
    if (needbuf)
      spec[i].num_patterns=100; else
        spec[i].num_patterns=1; 
    spec[i].num_cv_patterns=1;
    spec[i].aux_data=NULL;
  }
  spec[numNetworks].network_name[0]=0;
  real_net=HEBP_init(directory,spec,0);
  real_fast_list=new FastForwardStruct*[numNetworks];
  real_fast=FastConstructForward(real_net);
  for (i=0; i<numNetworks; i++)
  {
    real_fast_list[i]=FastConstructForward(real_net+i);
    real_net[i].ennuvits = 0;
    if (needbuf) {
      HEBP_replace_pattern_from_arrays(real_net+i,
                                       real_inputs,
                                       real_targets,
                                       HEBP_ADD_PATTERN,
                                       1, 0);
      for (int j=0; j<100; j++)
        HEBP_replace_pattern_from_arrays (real_net+i,
                                          real_inputs,
                                          real_targets,
                                          HEBP_ADD_PATTERN,
                                          0, j);
    }
  }
  fflush(stdout);
}
#endif

// USED
// Allocate an array of pointers to neural networks.  This array is global
// (yuck!).  If an array already exists, it and the networks it points to
// are deleted.
void FastAllocateNetworks(int num)
{
  if (real_fast_list!=NULL) {
    for (int i=0; i<real_numNetworks; i++) delete real_fast_list[i];
    delete[] real_fast_list;
  }
  real_fast_list=new FastForwardStruct*[num];
  for (int i=0; i<num; i++) real_fast_list[i]=NULL;
  real_numNetworks=num;
}

// USED
// Load a network, and place it in the array created by FastAllocateNetworks
void FastInitNetwork(int num, int numNetworks, char *networkList[])
{
  if (real_fast_list[num]!=NULL) delete real_fast_list[num];
  real_fast_list[num]=FastLoadMerge(numNetworks,networkList);
  FastLoadMergeWeights(real_fast_list[num],numNetworks,networkList);
}

#ifndef NoTcl
int FastAllocateNetworksCmd(ClientData data, Tcl_Interp *interp,
                            int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: FastAllocateNetworks <num>";
    return TCL_ERROR;
  }
  FastAllocateNetworks(atoi(argv[1]));
  return TCL_OK;
}

int FastInitNetworkCmd(ClientData data, Tcl_Interp *interp,
                       int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: FastInitNetwork <num> <network list>";
    return TCL_ERROR;
  }
  int num=atoi(argv[1]);
  int numNetworks;
  char **networkList;
  Tcl_SplitList(interp,argv[2],&numNetworks,&networkList);
  FastInitNetwork(num,numNetworks,networkList);
  free((char*)networkList);
  return TCL_OK;
}
#endif

void FastInit(char *directory, int numNetworks, char *networkList[])
{
  char *olddir=getcwd(NULL,1024);
  chdir(directory);
  real_fast_list=new FastForwardStruct*[numNetworks];
  for (int i=0; i<numNetworks; i++)
  {
    real_fast_list[i]=FastLoadMerge(1,networkList+i);
    FastLoadMergeWeights(real_fast_list[i],1,networkList+i);
    if (i==0)
    {
      real_fast=FastLoadMerge(1,networkList+i);
      FastLoadMergeWeights(real_fast,1,networkList+i);
    }
  }
  chdir(olddir);
  free(olddir);
}

#ifndef NoTcl
int FastLoadMergeCmd(ClientData data, Tcl_Interp *interp,
                     int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: FastLoadMerge {network-names}";
    return TCL_ERROR;
  }
  int numNetworks;
  char **networkList;
  if (real_fast_list==NULL) {
    real_fast_list=new FastForwardStruct*[1];
    real_fast_list[0]=NULL;
  }
  Tcl_SplitList(interp,argv[1],&numNetworks,&networkList);
  if (real_fast_list[0]!=NULL) delete real_fast_list[0];
  real_fast_list[0]=FastLoadMerge(numNetworks,networkList);
  free((char*)networkList);
  return TCL_OK;
}
#endif

#ifndef NoTcl
int FastLoadMergeWeightsCmd(ClientData data, Tcl_Interp *interp,
                            int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: FastLoadMergeWeights {network-names}";
    return TCL_ERROR;
  }
  int numNetworks;
  char **networkList;
  Tcl_SplitList(interp,argv[1],&numNetworks,&networkList);
  FastLoadMergeWeights(real_fast_list[0],numNetworks,networkList);
  free((char*)networkList);
  return TCL_OK;
}
#endif

#ifndef NoTcl
#ifndef NoHEBP
int HEBPInitCmd(ClientData data, Tcl_Interp *interp,
                int argc, char *argv[])
{
  if (argc!=4)
  {
    interp->result="Usage: HEBPInit <directory> {network-names} <needbuf>";
    return TCL_ERROR;
  }
  int numNetworks;
  char **networkList;
  char *dir=argv[1];
  Tcl_SplitList(interp,argv[2],&numNetworks,&networkList);
  int needbuf=atoi(argv[3]);
  HEBPInit(dir,numNetworks,networkList,needbuf);
  free((char*)networkList);
  return TCL_OK;
}
#endif
#endif

#ifndef NoTcl
int FastInitCmd(ClientData data, Tcl_Interp *interp,
                int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: FastInit <directory> {network-names}";
    return TCL_ERROR;
  }
  int numNetworks;
  char **networkList;
  char *dir=argv[1];
  Tcl_SplitList(interp,argv[2],&numNetworks,&networkList);
  FastInit(dir,numNetworks,networkList);
  free((char*)networkList);
  return TCL_OK;
}
#endif

#ifndef NoTcl
#ifndef NoHEBP
int ReadWeightsCmd(ClientData data, Tcl_Interp *interp,
                   int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: ReadWeights <file> <net>";
    return TCL_OK;
  }
  FILE *inf=fopen(argv[1],"r");
  int net=atoi(argv[2]);
  HEBP_read_weights(real_net+net,real_net[net].conn,inf);
  fclose(inf);
  if (real_fast!=NULL) delete real_fast;
  real_fast=FastConstructForward(real_net);
  if (real_fast_list!=NULL) delete[] real_fast_list;
  real_fast_list=new FastForwardStruct*[real_numNetworks];
  for (int i=0; i<real_numNetworks; i++)
    real_fast_list[i]=FastConstructForward(real_net+i);
  return TCL_OK;
}
#endif

#ifndef Test
int FastReadWeightsCmd(ClientData data, Tcl_Interp *interp,
                       int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: FastReadWeights <file> <net>";
    return TCL_OK;
  }
  FILE *inf=fopen(argv[1],"r");
  int net=atoi(argv[2]);
  FastReadWeights(real_fast_list[net],inf);
  fclose(inf);
  return TCL_OK;
}
#endif
#endif

#ifndef NoTcl
#ifndef NoHEBP
int WriteWeightsCmd(ClientData data, Tcl_Interp *interp,
                    int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: WriteWeights <file> <net>";
    return TCL_OK;
  }
  FILE *outf=fopen(argv[1],"w");
  int net=atoi(argv[2]);
  HEBP_write_weights(real_net+net,real_net[net].conn,outf);
  fclose(outf);
  return TCL_OK;
}
#endif

#ifndef Test
int FastWriteWeightsCmd(ClientData data, Tcl_Interp *interp,
                    int argc, char *argv[])
{
  if (argc!=3)
  {
    interp->result="Usage: FastWriteWeights <file> <net>";
    return TCL_OK;
  }
  FILE *outf=fopen(argv[1],"w");
  int net=atoi(argv[2]);
  FastWriteWeights(real_fast_list[net],outf);
  fclose(outf);
  return TCL_OK;
}
#endif
#endif

#ifndef NoTcl
int FastForwardPassCmd(ClientData data, Tcl_Interp *interp,
                       int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: FastForwardPass <net>";
    return TCL_ERROR;
  }
  int net=atoi(argv[1]);
  sprintf(interp->result,"%g",FastForwardPass(real_fast_list[net]));
  return TCL_OK;
}

#ifndef Test
int FastWriteActivationsCmd(ClientData data, Tcl_Interp *interp,
                            int argc, char *argv[])
{
  if (argc!=3) {
    interp->result="Usage: FastWriteActivations <net> <file>";
    return TCL_ERROR;
  }
  int net=atoi(argv[1]);
  char *filename=argv[2];
  FastWriteActivations(real_fast_list[net],filename);  
  return TCL_OK;
}

int FastReadActivationsCmd(ClientData data, Tcl_Interp *interp,
                            int argc, char *argv[])
{
  if (argc!=3) {
    interp->result="Usage: FastReadActivations <net> <file>";
    return TCL_ERROR;
  }
  int net=atoi(argv[1]);
  char *filename=argv[2];
  FastReadActivations(real_fast_list[net],filename);  
  return TCL_OK;
}
#endif
#endif

void MyHebp_InitNoTcl()
{
}

#ifndef NoTcl
int MyHebp_Init(Tcl_Interp *interp)
{
#ifndef NoHEBP
  Tcl_CreateCommand(interp,"HEBPInit",HEBPInitCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL); 
#endif
  Tcl_CreateCommand(interp,"FastInit",FastInitCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL); 
  Tcl_CreateCommand(interp,"FastLoadMerge",FastLoadMergeCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL); 
  Tcl_CreateCommand(interp,"FastLoadMergeWeights",FastLoadMergeWeightsCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL); 
#ifndef NoHEBP
  Tcl_CreateCommand(interp,"ReadWeights",ReadWeightsCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"WriteWeights",WriteWeightsCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#endif
  Tcl_CreateCommand(interp,"TestNetworkSensitivity",
                    TestNetworkSensitivityCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"FastAllocateNetworks",
                    FastAllocateNetworksCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"FastInitNetwork",
                    FastInitNetworkCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#ifndef Test
  Tcl_CreateCommand(interp,"FastReadWeights",FastReadWeightsCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"FastWriteWeights",FastWriteWeightsCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"FastWriteActivations",FastWriteActivationsCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"FastReadActivations",FastReadActivationsCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
#endif
  Tcl_CreateCommand(interp,"FastForwardPass",FastForwardPassCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  Tcl_CreateCommand(interp,"FastRandomizeWeights",FastRandomizeWeightsCmd,
                    NULL,(Tcl_CmdDeleteProc*)NULL);
  return TCL_OK;
}
#endif

