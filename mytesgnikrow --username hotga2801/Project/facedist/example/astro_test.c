#include <stdio.h>
#include <stdlib.h>
#include "detect.h"

int main(int argc, char *argv[])
{
  int numFaces, i, width, height;
  struct FaceLocation *locArray;
  struct FaceLocation loc;
  unsigned char *buf;

  if (argc!=2) { 
    fprintf(stderr,"Must give .gif file name on command line.\n");
    exit(1);
  }
  SetDataPath(argv[0]);

  fprintf(stderr,"Loading image...\n");
  buf=LoadGifToBuffer(argv[1],&width,&height);

  fprintf(stderr,"\nFinding all faces in image...\n");
  numFaces=Astro_FindAllFaces(width,height,buf,&locArray);
  if (numFaces==0) fprintf(stderr,"No faces found.\n"); else
  {
    fprintf(stderr,"%d faces found:\n",numFaces);
    for (i=0; i<numFaces; i++)
      fprintf(stderr,"  (%d,%d)-(%d,%d)\n",
              locArray[i].x1,locArray[i].y1,
              locArray[i].x2,locArray[i].y2);
    free((char*)locArray);
  }
  return 0;
}

