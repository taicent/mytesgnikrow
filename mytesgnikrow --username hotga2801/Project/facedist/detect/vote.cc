extern "C" {
#include <assert.h>
#if !defined(hpux) && !defined(WINNT)
#include <math.h>
#endif
#include <stdio.h>
}
#if defined(hpux) || defined(WINNT)
#include <math.h>
#endif

#include "search.hpp"
#include "list.cc"

#include "vote.hpp"

//#define QuickOverlapZap
//#define OrientationMerging

// A bunch of these functions are similar to those used in search.cc.
// These ones take extra parameters to store more information about
// the detections, and use more memory.

// USED
// Get the value of some location in the detection pyramid.  If that location
// has not already been allocated, create it and set it to zero.
static int FuzzyVoteAccumGet(Tcl_HashTable *accum, List<AccumElement> *bins,
			     int x, int y, int s, int d, int t)
{
  int loc[5];
#ifdef OrientationMerging
  while (d<0) d+=36; d=d%36;
#endif
  loc[0]=x; loc[1]=y; loc[2]=s; loc[3]=d; loc[4]=t;
  Tcl_HashEntry *entry=Tcl_FindHashEntry(accum,(char*)loc);
  if (entry==NULL)
  {
    int newentry;
    entry=Tcl_CreateHashEntry(accum,(char*)loc,&newentry);
    AccumElement *elem=new AccumElement(x,y,s,d,t,0);
    Tcl_SetHashValue(entry,elem);
    bins[0].addLast(elem);
  }
  return ((AccumElement*)(Tcl_GetHashValue(entry)))->value;
}

// USED
// Get the value of some location in the detection pyramid.  If that location
// does not exist, then return zero (but do not allocate that location).
static int FuzzyVoteAccumCheck(Tcl_HashTable *accum, List<AccumElement> *bins,
			       int x, int y, int s, int d, int t)
{
  int loc[5];
#ifdef OrientationMerging
  while (d<0) d+=36; d=d%36;
#endif
  loc[0]=x; loc[1]=y; loc[2]=s; loc[3]=d; loc[4]=t;
  Tcl_HashEntry *entry=Tcl_FindHashEntry(accum,(char*)loc);
  if (entry==NULL) return 0;
  return ((AccumElement*)(Tcl_GetHashValue(entry)))->value;
}

// USED
// Set a value in the detection pyramid; if that location does not exist,
// then create it.  Also, add that location to the list of locations with
// that value.  
static void FuzzyVoteAccumSet(Tcl_HashTable *accum, List<AccumElement> *bins,
			      int x, int y, int s, int d, int t, int val)
{
  int loc[5];
#ifdef OrientationMerging
  while (d<0) d+=36; d=d%36;
#endif
  loc[0]=x; loc[1]=y; loc[2]=s; loc[3]=d; loc[4]=t;
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
			       int x, int y, int s, int d, int t)
{
  int loc[5];
#ifdef OrientationMerging
  while (d<0) d+=36; d=d%36;
#endif
  loc[0]=x; loc[1]=y; loc[2]=s; loc[3]=d; loc[4]=t;
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
			      int s, int x, int y, int d, int t,
			      int minVal, int currVal,
			      double *totalS, double *totalX, double *totalY,
			      double *total)
{
  int value=FuzzyVoteAccumCheck(accum,bins,x,y,s,d,t);
  if (value>=minVal && value<=currVal)
  {
    FuzzyVoteAccumZero(accum,bins,x,y,s,d,t);
    double rvalue=value;
    (*total)+=rvalue;
    (*totalS)+=s*rvalue;
    (*totalX)+=x*pow(1.2,s)*rvalue;
    (*totalY)+=y*pow(1.2,s)*rvalue;
    FindCentroidAccum(accum,bins,numImages,s,x+1,y,d,t,minVal,value,
                      totalS,totalX,totalY,total);
    FindCentroidAccum(accum,bins,numImages,s,x-1,y,d,t,minVal,value,
                      totalS,totalX,totalY,total);
    FindCentroidAccum(accum,bins,numImages,s,x,y+1,d,t,minVal,value,
                      totalS,totalX,totalY,total);
    FindCentroidAccum(accum,bins,numImages,s,x,y-1,d,t,minVal,value,
                      totalS,totalX,totalY,total);
    FindCentroidAccum(accum,bins,numImages,
                      s-1,(int)floor(0.5+x*1.2),(int)floor(0.5+y*1.2),d,t,
                      minVal,value,
                      totalS,totalX,totalY,total);
    FindCentroidAccum(accum,bins,numImages,
                      s+1,(int)floor(0.5+x/1.2),(int)floor(0.5+y/1.2),d,t,
                      minVal,value,
                      totalS,totalX,totalY,total);
  }
}

// USED
// Read detections from a file, recording the coordinates at the center of 
// detection
void ReadDetectionList(char *filename, List<Detection> *detections)
{
  int done=0;
  FILE *inf=fopen(filename,"r");
  assert(inf!=NULL);
  do {
    int x, y, s, dir;
    double x1, y1, x2, y2;
    double conf;
    char buf[1024];
    if (fgets(buf,1024,inf)!=NULL) {
      int numread=sscanf(buf,"%lf %lf %lf %lf %d %d %d %lf %d\n",
                         &x1,&y1,&x2,&y2,
                         &x,&y,&s,&conf,&dir);
      if (numread<7) done=1; else {
        if (numread<8) conf=1.0;
        if (numread<9) dir=0;
        int w=(int)floor(0.5+(x2-x1)/pow(1.2,s));
        int h=(int)floor(0.5+(y2-y1)/pow(1.2,s));
	// Adjust coordinates to be center of detection
        x=x+w/2;
        y=y+h/2;
        if (conf<0.0) continue;
        detections->addLast(new Detection(x,y,s,conf,dir));
      }
    } else done=1;
  } while (!done);
  fclose(inf);
}

void ReadDetectionListRotClass(char *filename, List<Detection> *detections)
{
  int done=0;
  FILE *inf=fopen(filename,"r");
  assert(inf!=NULL);
  do {
    int x, y, s, dir, p;
    double x1, y1, x2, y2;
    double conf;
    char buf[1024];
    if (fgets(buf,1024,inf)!=NULL) {
      int numread=sscanf(buf,"%lf %lf %lf %lf %lf %d %d %d %d %d\n",
                         &x1,&y1,&x2,&y2,&conf,&s,&x,&y,&dir,&p);
      if (numread==10) {
        if (conf<0.0) continue;
        detections->addLast(new Detection(x,y,s,conf,dir,p));
      }
    } else done=1;
  } while (!done);
  fclose(inf);
}

// USED
// Read detections from a file, recording the coordinates of the 
// upper left corner of each detection
void ReadDetectionListCorner(char *filename, List<Detection> *detections)
{
  int done=0;
  FILE *inf=fopen(filename,"r");
  assert(inf!=NULL);
  do {
    int x, y, s, dir;
    double x1, y1, x2, y2;
    double conf;
    char buf[1024];
    if (fgets(buf,1024,inf)!=NULL) {
      int numread=sscanf(buf,"%lf %lf %lf %lf %d %d %d %lf %d\n",
                         &x1,&y1,&x2,&y2,
                         &x,&y,&s,&conf,&dir);
      if (numread<7) done=1; else {
        if (numread<8) conf=1.0;
        if (numread<9) dir=0;
        int w=(int)floor(0.5+(x2-x1)/pow(1.2,s));
        int h=(int)floor(0.5+(y2-y1)/pow(1.2,s));
        if (conf<0.0) continue;
        detections->addLast(new Detection(x,y,s,conf,dir));
      }
    } else done=1;
  } while (!done);
  fclose(inf);
}

void DetectionListCenterToCorner(List<Detection> *detections)
{
  for (Detection *d=detections->first; d!=NULL; d=d->next) {
    // ugly hack, need to get the actual window side from somewhere
    d->x-=10;
    d->y-=10;
  }
}

void DetectionListCornerToCenter(List<Detection> *detections)
{
  for (Detection *d=detections->first; d!=NULL; d=d->next) {
    // ugly hack, need to get the actual window side from somewhere
    d->x+=10;
    d->y+=10;
  }
}

void DetectionListFilterOrientation(List<Detection> *detections,
                                    int orientation)
{
  fprintf(stderr,"Before filtering orientation %d\n",detections->length);
  Detection *d=detections->first;
  while (d!=NULL) {
    Detection *next=d->next;
    if (d->orient!=orientation)
      detections->deleteNode(d);
    d=next;
  }
  fprintf(stderr,"After filtering orientation %d\n",detections->length);
}

void DetectionListFilterThreshold(List<Detection> *detections,
                                  double threshold)
{
  fprintf(stderr,"Before filtering threshold %d\n",detections->length);
  Detection *d=detections->first;
  while (d!=NULL) {
    Detection *next=d->next;
    if (d->output<threshold)
      detections->deleteNode(d);
    d=next;
  }
  fprintf(stderr,"After filtering threshold %d\n",detections->length);
}

void DetectionListFilterBorders(List<Detection> *detections,
                                int width, int height)
{
  fprintf(stderr,"Before filtering borders %d\n",detections->length);
  int levels=0;
  Detection *d;
  for (d=detections->first; d!=NULL; d=d->next) 
    if (d->s+1>levels) levels=d->s+1;
  int *widthLevel=new int[levels];
  int *heightLevel=new int[levels];
  widthLevel[0]=width;
  heightLevel[0]=height;
  for (int i=1; i<levels; i++) {
    widthLevel[i]=(int)floor(widthLevel[i-1]/1.2);
    heightLevel[i]=(int)floor(heightLevel[i-1]/1.2);
  }
  d=detections->first;
  while (d!=NULL) {
    Detection *next=d->next;
    if (d->x<10 || d->y<10 ||
        d->x>widthLevel[d->s]-10 || d->y>heightLevel[d->s]-10) {
//      fprintf(stderr,"Delete %d,%d,%d (%d,%d)-(%d,%d)\n",
//              d->x,d->y,d->s,10,10,widthLevel[d->s]-10,heightLevel[d->s]-10);
      detections->deleteNode(d);
    }
    d=next;
  }
  fprintf(stderr,"After filtering borders %d\n",detections->length);
}

// USED
// Save a detection list to a file, assumes that the detections are 
// stored as the center of the face
void WriteDetectionList(char *filename, List<Detection> *detections,
                        int width, int height)
{
  FILE *outf=fopen(filename,"w");
  assert(outf!=NULL);
  for (Detection *detect=detections->first; detect!=NULL; detect=detect->next)
    fprintf(outf,"%g %g %g %g %d %d %d %g %d\n",
            (detect->x-0.5*width)*pow(1.2,detect->s),
            (detect->y-0.5*height)*pow(1.2,detect->s),
            (detect->x+0.5*width)*pow(1.2,detect->s),
            (detect->y+0.5*height)*pow(1.2,detect->s),
            detect->x-width/2,detect->y-height/2,detect->s,
            detect->output,detect->orient);
  fclose(outf);
}

void WriteDetectionListRotClass(char *filename, List<Detection> *detections)
{
  FILE *outf=fopen(filename,"w");
  assert(outf!=NULL);
  int width=30, height=30;
  for (Detection *detect=detections->first; detect!=NULL; detect=detect->next)
    fprintf(outf,"%g %g %g %g %g %d %d %d %d %d\n",
            (detect->x-0.5*width)*pow(1.2,detect->s),
            (detect->y-0.5*height)*pow(1.2,detect->s),
            (detect->x+0.5*width)*pow(1.2,detect->s),
            (detect->y+0.5*height)*pow(1.2,detect->s),
            detect->output,
            detect->s,
            detect->x,detect->y,
            detect->orient,detect->profile);
  fclose(outf);
}

// USED
// Save a detection list to a file, assumes that the detections are 
// stored as the upper left cornercenter of the face
void WriteDetectionListCorner(char *filename, List<Detection> *detections,
			      int width, int height)
{
  FILE *outf=fopen(filename,"w");
  assert(outf!=NULL);
  for (Detection *detect=detections->first; detect!=NULL; detect=detect->next)
    fprintf(outf,"%g %g %g %g %d %d %d %g %d\n",
            (detect->x)*pow(1.2,detect->s),
            (detect->y)*pow(1.2,detect->s),
            (detect->x+width)*pow(1.2,detect->s),
            (detect->y+height)*pow(1.2,detect->s),
            detect->x,detect->y,detect->s,
            detect->output,detect->orient);
  fclose(outf);
}

// USED
// Create an array of detection lists
List<Detection> *AllocateDetectionListArray(int num) 
{
  return new List<Detection>[num];
}

// USED
// Free an array of detection lists
void FreeDetectionListArray(List<Detection> *detectionsArray)
{
  delete[] detectionsArray;
}

// USED
// Access an element of the detection list array
List<Detection> *GetDetectionListArrayElement(List<Detection> *detectionsArray,
                                              int num)
{
  return detectionsArray+num;
}

// USED
// Create an individual detection list
List<Detection> *AllocateDetectionList()
{
  return new List<Detection>;
}

// USED
// Free an individual detection list
void FreeDetectionList(List<Detection> *detections)
{
  delete detections;
}

// USED 
// New arbitration procedure.  Slower and takes more memory than (say)
// FuzzyVote2Direct, but is useful for the router networks because it
// keeps track of which view each detection is associated with, and
// counts that as part of the position of the detection.  Thus, only
// detections of the same view at the same location reinforce one another.
// (However, overlap removal will remove detections which overlap with each
// other but have different views.)
void NewFuzzyVote(List<Detection> *outputList, 
                  List<Detection> *inputList, int numInputLists,
                  int width, int height, int maskWidth, int maskHeight,
                  int spread, int threshold, int overlap)
{
  // Hash table represents which locations/scales in the detection are
  // filled in, and with what values.  Any missing entries are assumed
  // to be zero.
  Tcl_HashTable accum;
  Tcl_InitHashTable(&accum,4);
  // An array of lists.  Each lists contains all the detection location/scales
  // with a particular value.  This allows the detections to be scanned in
  // order from highest to lowest with practically no overhead.
  List<AccumElement> bins[256];

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
    int numFaces=0;
    int done=0;
    // For each detection
    for (Detection *detect=inputList[num].first; detect!=NULL;
         detect=detect->next) {
      fprintf(stderr,"I");
      int xc=detect->x, yc=detect->y, s=detect->s, d=detect->orient, t=0;
#ifdef OrientationMerging
      d=(d+5)/10;
#endif
      int conf=(int)floor(0.5+detect->output);
      if (conf<1) conf=1;
      // Spread out the detection in both scale and location by 
      // "spread" levels or pixels, incrementing the value of each
      // location in the detection pyramid
      for (int si=-spread; si<=spread; si++) {
        if (si+s<0 || si+s>=numScales) continue;
        int xci=(int)floor(0.5+xc*exp(log(1.2)*-si));
        int yci=(int)floor(0.5+yc*exp(log(1.2)*-si));
        for (int xx=xci-spread; xx<=xci+spread; xx++)
          for (int yy=yci-spread; yy<=yci+spread; yy++)
              FuzzyVoteAccumSet(&accum,bins,xx,yy,s+si,d,t,
                                FuzzyVoteAccumGet(&accum,bins,xx,yy,s+si,d,t)+
                                conf);
//            for (int o=d-spread; o<=d+spread; o++)
//              FuzzyVoteAccumSet(&accum,bins,xx,yy,s+si,o,t,
//                                FuzzyVoteAccumGet(&accum,bins,xx,yy,s+si,o,t)+
//                                conf);
      }
      numFaces++;
    }
  }
  fprintf(stderr,"\n");
  outputList->deleteNodes();

  // Scan through the detection pyramid from highest to lowest value
  for (int val=255; val>=threshold; val--) {
    while (!bins[val].empty()) {
      fprintf(stderr,"Z");
      // Get the detection
      int x=bins[val].first->loc[0];
      int y=bins[val].first->loc[1];
      int s=bins[val].first->loc[2];
      int d=bins[val].first->loc[3];
      int t=0;
      //      int t=bins[val].first->loc[4];
      int cs, cx, cy;
      double total=0.0;
      double totalS=0, totalX=0, totalY=0;
//      fprintf(stderr,"%d %d %d -> ",x,y,s);
      // Find the centroid of detections around this location
      FindCentroidAccum(&accum,bins,numScales,s,x,y,d,t,threshold,val,
                        &totalS,&totalX,&totalY,&total);
      cs=(int)floor(0.5+totalS/total);
      cx=(int)floor(0.5+totalX/total*pow(1.2,-cs));
      cy=(int)floor(0.5+totalY/total*pow(1.2,-cs));
//      fprintf(stderr,"%g %g %g\n",totalX/total,totalY/total,totalS/total);      // If we are to remove overlapping detections, scan through
      // the detection pyramid, removing all possible overlaps
      if (overlap) {
#ifdef QuickOverlapZap
        for (int val=255; val>0; val--) {
          AccumElement *elem=bins[val].first;
          while (elem!=NULL) {
            AccumElement *next=elem->next;
            int xpos=(int)floor(0.5+cx*pow(1.2,cs-elem->loc[2]));
            int ypos=(int)floor(0.5+cy*pow(1.2,cs-elem->loc[2]));
            int sizex=maskWidth/2+
              (int)floor(0.5+maskWidth/2*pow(1.2,cs-elem->loc[2]));
            int sizey=maskHeight/2+
              (int)floor(0.5+maskHeight/2*pow(1.2,cs-elem->loc[2]));
            if (elem->loc[0]>=xpos-sizex && elem->loc[0]<=xpos+sizex &&
                elem->loc[1]>=ypos-sizey && elem->loc[1]<=ypos+sizey) {
              FuzzyVoteAccumZero(&accum,bins,
                                 elem->loc[0],elem->loc[1],elem->loc[2],
                                 elem->loc[3],elem->loc[4]);
            }
            elem=next;
          }
        }
#else
        for (int scale=0; scale<numScales; scale++) {
          int xpos=(int)floor(0.5+cx*pow(1.2,cs-scale));
          int ypos=(int)floor(0.5+cy*pow(1.2,cs-scale));
          int sizex=maskWidth/2+
            (int)floor(0.5+maskWidth/2*pow(1.2,cs-scale));
          int sizey=maskHeight/2+
            (int)floor(0.5+maskHeight/2*pow(1.2,cs-scale));
          for (int xx=xpos-sizex; xx<=xpos+sizex; xx++)
            for (int yy=ypos-sizey; yy<=ypos+sizey; yy++)
#ifdef OrientationMerging
              for (int dd=-18; dd<=18; dd++)
#else
              for (int dd=0; dd<5; dd++)
#endif
                if (FuzzyVoteAccumCheck(&accum,bins,xx,yy,scale,dd,y)!=0)
                  FuzzyVoteAccumZero(&accum,bins,xx,yy,scale,dd,t);
        }
#endif
      }

      // Record the detection
#ifdef OrientationMerging
      outputList->addLast(new Detection(cx,cy,cs,val,d*10));
#else
      outputList->addLast(new Detection(cx,cy,cs,val,d));
#endif
    }
  }
  fprintf(stderr,"\n");
  // Clean up
  Tcl_DeleteHashTable(&accum);
}

int CompareDetections(Detection *d1, Detection *d2)
{
  if (d1->output>d2->output) return -1;
  if (d1->output<d2->output) return 1;
  return 0;
}

