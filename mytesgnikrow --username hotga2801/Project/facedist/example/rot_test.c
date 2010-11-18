#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "detect.h"

#ifndef M_PI
#define M_PI (atan(1)*4)
#endif

int main(int argc, char *argv[])
{
  int numFaces, i;
  struct FaceLocationRotation *locArray;
  int width, height;
  unsigned char *buf;
  if (argc!=2) {
    fprintf(stderr,"Usage: rot_test filename.pgm\n");
    exit(1);
  }
  SetDataPath(argv[0]);
  fprintf(stderr,"Loading image...\n");
  buf=LoadPgmToBuffer(argv[1],&width, &height);
  fprintf(stderr,"Processing image...\n");
  numFaces=Rotation_FindAllFacesSlow(width,height,buf,&locArray);
  if (numFaces==0) fprintf(stderr,"No faces found.\n"); else {
    fprintf(stderr,"%d faces found:\n",numFaces);
    for (i=0; i<numFaces; i++)
      fprintf(stderr,"  center (%d,%d), %d degrees, %d pixels\n",
              locArray[i].x,locArray[i].y,
              (int)floor(locArray[i].angle*180.0/M_PI+0.5),
              locArray[i].size);
    free((char*)locArray);
  }
  free((char*)buf);
  return 0;
}
