#include <stdio.h>
#include <stdlib.h>
#include "detect.h"

int main(int argc, char *argv[])
{
  int numFaces, i;
  struct FaceLocationOrientation *locArray;
  int width, height;
  unsigned char *buf;
  if (argc!=2) {
    fprintf(stderr,"Usage: imorient_test filename.ppm\n");
    exit(1);
  }
  SetDataPath(argv[0]);
  fprintf(stderr,"Loading image...\n");
  buf=LoadPpmToBuffer(argv[1],&width, &height);
  fprintf(stderr,"Processing image...\n");
  numFaces=Informedia_FindFacesOrientation(width,height,buf,&locArray);
  if (numFaces==0) fprintf(stderr,"No faces found.\n"); else {
    fprintf(stderr,"%d faces found:\n",numFaces);
    for (i=0; i<numFaces; i++)
      fprintf(stderr,"  (%d,%d)-(%d,%d) dir=%d\n",
              locArray[i].x1,locArray[i].y1,
              locArray[i].x2,locArray[i].y2,
              locArray[i].orientation);
    free((char*)locArray);
  }
  free((char*)buf);
  return 0;
}
