#ifndef FollowIncluded
#define FollowIncluded

#ifndef NoTcl
extern "C" {
#include <tcl.h>
}
#endif

#include "search.hpp"
#include "mat.hpp"

extern Mat LightingCorrectionMatrix;
extern int LightingCorrectionMatrixInitialized;
extern void InitializeLightingCorrectionMatrix();

int FindNewLocation(int numScales, ByteImage *imagePyramid[], ByteImage *mask,
                    int x, int y, int s, int dx, int dy, int ds, int step,
                    int basenet,
                    int *newx, int *newy, int *news);

#ifdef CompiledNets
int FindNewLocationCompiled(
  int numScales, ByteImage *imagePyramid[], ByteImage *mask,
  int x, int y, int s, int dx, int dy, int ds, int step,
  int basenet,
  int *newx, int *newy, int *news);

int FindNewLocationCompiled2(
  int numScales, ByteImage *imagePyramid[], ByteImage *mask,
  int x, int y, int s, int dx, int dy, int ds, int step,
  int basenet,
  int *newx, int *newy, int *news);
#endif

int FindNewLocationRecordMax(int numScales, ByteImage *imagePyramid[],
                             ByteImage *mask,
                             int x, int y, int s,
                             int dx, int dy, int ds, int step,
                             int basenet,
                             int *x1, int *y1, int *s1, double *o1,
                             int *x2, int *y2, int *s2, double *o2);
void StartFrameTimer();
void StopFrameTimer();

#ifndef NoTcl
void Follow_Init(Tcl_Interp *interp);
#endif

#endif
