#include <stdio.h>
#include <stdlib.h>
#include "detect.h"

int main(int argc, char *argv[])
{
  int numFaces, i;
  struct FaceLocation *locArray;
  struct FaceLocationOrientation *locOrientArray;
  int width, height;
  unsigned char *buf;
  if (argc!=2) {
    fprintf(stderr,"Usage: imgray_test filename.pgm\n");
    exit(1);
  }
  SetDataPath(argv[0]);
  fprintf(stderr,"Loading image...\n");
  buf=LoadPgmToBuffer(argv[1],&width, &height);
  fprintf(stderr,"Processing image...\n");
  numFaces=Informedia_FindFacesGray(width,height,buf,&locArray);
  if (numFaces==0) fprintf(stderr,"No faces found.\n"); else {
    fprintf(stderr,"%d faces found:\n",numFaces);
    for (i=0; i<numFaces; i++)
      fprintf(stderr,"  (%d,%d)-(%d,%d)\n",
              locArray[i].x1,locArray[i].y1,
              locArray[i].x2,locArray[i].y2);
    free((char*)locArray);
  }
  fprintf(stderr,"\nProcessing image (with orientations)...\n");
  numFaces=Informedia_FindFacesOrientationGray(width,height,buf,
                                               &locOrientArray);
  if (numFaces==0) fprintf(stderr,"No faces found.\n"); else {
    fprintf(stderr,"%d faces found:\n",numFaces);
    for (i=0; i<numFaces; i++)
      fprintf(stderr,"  (%d,%d)-(%d,%d) dir=%d\n",
              locOrientArray[i].x1,locOrientArray[i].y1,
              locOrientArray[i].x2,locOrientArray[i].y2,
              locOrientArray[i].orientation);
    free((char*)locArray);
  }
  free((char*)buf);
  return 0;
}
