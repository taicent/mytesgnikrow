#include <stdio.h>
#include <stdlib.h>
#include "detect.h"

int main(int argc, char *argv[])
{
  int numFaces, i, iter=0;
  struct FaceLocation *locArray;
  int width, height;
  unsigned char *buf;
  if (argc!=2) {
    fprintf(stderr,"Usage: im_test filename.ppm\n");
    exit(1);
  }
  SetDataPath(argv[0]);
  fprintf(stderr,"Loading image...\n");
  buf=LoadPpmToBuffer(argv[1],&width, &height);

  for (iter=0; iter<10; iter++) {
    fprintf(stderr,"Processing image iteration %d...\n",iter);
    numFaces=Informedia_FindFaces(width,height,buf,&locArray);
    if (numFaces==0) {
      fprintf(stderr,"No faces found.\n");
      BroadenColorMask(0.001);
    } else {
      fprintf(stderr,"%d faces found:\n",numFaces);
      for (i=0; i<numFaces; i++) {
        int size=locArray[i].x2-locArray[i].x1;
        fprintf(stderr,"  (%d,%d)-(%d,%d)\n",
                locArray[i].x1,locArray[i].y1,
                locArray[i].x2,locArray[i].y2);
        UpdateColorMaskFromBuffer(width,height,buf,
                                  locArray[i].x1+size/4,
                                  locArray[i].y1+size/4,
                                  locArray[i].x2-size/4,
                                  locArray[i].y2-size/4,
                                  0.25);
      }
      free((char*)locArray);
    }
  }
  free((char*)buf);
  return 0;
}
