#ifndef EyeIncluded
#define EyeIncluded

#ifndef NoTcl
void Eye_Init(Tcl_Interp *interp);
#endif

int SearchLeftEye(ByteImage *image, ByteImage *mask, double eyewidth,
                  int *xeye, int *yeye, int leftEye);

int SearchEyes(int numScales, ByteImage *imagePyramid[], ByteImage *mask,
               int xx, int yy, int s, int basenet,
               int *eyex1, int *eyey1, int *eyex2, int *eyey2);

#endif

