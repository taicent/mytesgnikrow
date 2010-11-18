#include <stdio.h>
#include <stdlib.h>
#include "detect.h"

int main(int argc, char *argv[])
{
  int numFaces, i;
  struct FaceLocation *locArray;
  struct FaceLocation loc;
  int width, height;
  unsigned char *buf;
  if (argc!=2) {
    fprintf(stderr,"xavier_test filename.pgm\n");
    exit(1);
  }
  SetDataPath(argv[0]);
  fprintf(stderr,"Loading image...\n");
  buf=LoadPgmToBuffer(argv[1],&width,&height);
  fprintf(stderr,"\nFinding all faces in image...\n");
  numFaces=Xavier_FindAllFaces(width,height,buf,&locArray);
  if (numFaces==0) fprintf(stderr,"No faces found.\n"); else
  {
    fprintf(stderr,"%d faces found:\n",numFaces);
    for (i=0; i<numFaces; i++)
      fprintf(stderr,"  (%d,%d)-(%d,%d)\n",
              locArray[i].x1,locArray[i].y1,
              locArray[i].x2,locArray[i].y2);
    free((char*)locArray);
  }

  fprintf(stderr,"\nFinding first face in image in a vertical stripe...\n");
  if (Xavier_FindOneFace(320,240,buf,
                         150,240, /* Range of X coords for center of face */
                         0,240, /* Range of Y coords for center of face */
                         30,60, /* Range of face sizes */
                         Xavier_QuickVerify|Xavier_QuickVerifyNN,
                         NULL,  /* No previous image, so no motion */
                         20,    /* Threshold, if motion was used */
                         &loc   /* Where to store location of face */))
  {
    fprintf(stderr,"Found a face at (%d,%d)-(%d,%d)\n",
              loc.x1,loc.y1,loc.x2,loc.y2);
  } else
  {
    fprintf(stderr,"No face found.\n");
  }
  free((char*)buf);
  return 0;
}
