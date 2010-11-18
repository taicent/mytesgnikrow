#include <stdio.h>
#include <stdlib.h>
#include "detect.h"

/* This example is set up to do roughly what im_test (and its associated
   face detection function do.  Specifically, it requires that the 
   candidate detector give a positive response, and also that the two
   verification networks both give a positive response at the same location.
   However, it will be 4 times slower than im_test, because the verification
   networks are applied at each pixel location, rather than every other 
   pixel in X and Y (thus they should have fewer missed faces). */

/* Just print out everything, and stop the detector when the candidate
   evaluation drops below 0. */
int CandidateCallback(double candidateRating,
                      int xc, int yc, int sc,
                      double verifyRating1,
                      int x1, int y1, int s1,
                      double verifyRating2,
                      int x2, int y2, int s2)
{
  if (candidateRating<0.0) return 0;
  fprintf(stderr,"Candidate found:\n");
  fprintf(stderr,"  candidate rating: %g location: (%d,%d)-(%d,%d)\n",
          candidateRating,xc,yc,xc+sc,yc+sc);
  fprintf(stderr,"  verifier rating 1: %g location: (%d,%d)-(%d,%d)\n",
          verifyRating1,x1,y1,x1+s1,y1+s1);
  fprintf(stderr,"  verifier rating 2: %g location: (%d,%d)-(%d,%d)\n",
          verifyRating2,x2,y2,x2+s2,y2+s2);
  return 1;
}

int main(int argc, char *argv[])
{
  int width, height;
  unsigned char *buf;
  if (argc!=2) {
    fprintf(stderr,"Usage: im2_test filename.ppm\n");
    exit(1);
  }
  SetDataPath(argv[0]);
  buf=LoadPpmToBuffer(argv[1],&width,&height);
  fprintf(stderr,"Processing image...\n");
  Informedia_FindFacesAllCandidates(width,height,buf,2,CandidateCallback);
  return 0;
}

