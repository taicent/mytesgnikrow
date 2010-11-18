#include <stdio.h>
#include <stdlib.h>
#include "detect.h"

int main(int argc, char *argv[])
{
  int numFaces, i, x, y;
  struct FaceEyeLocation *locArray;
  int width, height;
  unsigned char *buf, *mask;
  if (argc!=2) {
  	 fprintf(stderr,"Usage: siemens_test filename.pgm\n");
    exit(1);
  }

  SetDataPath(argv[0]);

  fprintf(stderr,"Loading image...\n");
  buf=LoadPgmToBuffer(argv[1],&width,&height);
  mask=(unsigned char*)malloc(width*height);

  fprintf(stderr,"\nFinding all faces in image (with mask)...\n");
  for (i=0; i<width*height; i++) mask[i]=1; /* Search nowhere */
  /* uncomment mask code here to make it search part of image */
#if 0
  for (x=170; x<220; x++) for (y=180; y<230; y++)
    mask[y*width+x]=0; /* Keep first face in ../test/henry.pgm */
  for (x=220; x<270; x++) for (y=110; y<150; y++)
    mask[y*width+x]=0; /* Keep second face in ../test/henry.pgm */
#endif
  numFaces=Siemens_FindAllFaces(width,height,buf,mask,&locArray);
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
  free((char*)buf);
  free((char*)mask);
  return 0;
}
