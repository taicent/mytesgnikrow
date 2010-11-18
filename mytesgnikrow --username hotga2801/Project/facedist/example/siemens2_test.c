#include <stdio.h>
#include <stdlib.h>
#include "detect.h"

int main(int argc, char *argv[])
{
  int numFaces, i, x, y;
  struct FaceEyeLocation *locArray;
  unsigned char *buf;
  int width, height;
  SetDataPath(argv[0]);
  fprintf(stderr,"Loading image...\n");
  buf=LoadGifToBuffer(argv[1],&width,&height);

  fprintf(stderr,"\nFinding all faces in image...\n");
  numFaces=Siemens_FindAllFaces(width,height,buf,NULL,&locArray);
  if (numFaces==0) fprintf(stderr,"No faces found.\n"); else
  {
    fprintf(stderr,"%d faces found:\n",numFaces);
    for (i=0; i<numFaces; i++)
    {
      fprintf(stderr,"  (%d,%d)-(%d,%d)\n",
              locArray[i].x1,locArray[i].y1,
              locArray[i].x2,locArray[i].y2);
      fprintf(stderr,"    Left Eye: ");
      if (locArray[i].left==-1) fprintf(stderr,"too small\n");
      if (locArray[i].left==0) fprintf(stderr,"not found\n");
      if (locArray[i].left==1) fprintf(stderr,"(%d,%d)\n",
                                       locArray[i].leftx,locArray[i].lefty);
      fprintf(stderr,"    Right Eye: ");
      if (locArray[i].right==-1) fprintf(stderr,"too small\n");
      if (locArray[i].right==0) fprintf(stderr,"not found\n");
      if (locArray[i].right==1) fprintf(stderr,"(%d,%d)\n",
                                        locArray[i].rightx,locArray[i].righty);
    }
    free((char*)locArray);
  }
  return 0;
}

