#include <stdio.h>
#include <stdlib.h>
#include "detect.h"

int main(int argc, char *argv[])
{
  int numFaces, i;
  struct FaceLocation *locArray;
  int width, height, mode;
  unsigned char *buf;
  if (argc!=2) {
    fprintf(stderr,"Usage: slow_test filename.pgm\n");
    exit(1);
  }
  SetDataPath(argv[0]);
  fprintf(stderr,"Loading image...\n");
  buf=LoadPgmToBuffer(argv[1],&width, &height);
  for (mode=1; mode<=16; mode++) {
    fprintf(stderr,"Processing image in arbitration mode %d:\n",mode);
    numFaces=Slow_FindAllFaces(width,height,buf,mode,&locArray);
    if (numFaces==0) fprintf(stderr,"No faces found.\n"); else {
      fprintf(stderr,"%d faces found:\n",numFaces);
      for (i=0; i<numFaces; i++)
	fprintf(stderr,"  (%d,%d)-(%d,%d)\n",
		locArray[i].x1,locArray[i].y1,
		locArray[i].x2,locArray[i].y2);
      free((char*)locArray);
    }
  }
  free((char*)buf);
  return 0;
}
