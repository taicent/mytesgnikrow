#ifndef KDoiIncluded
#define KDoiIncluded

extern "C" {
#ifndef NoTcl
#include <tcl.h>
#endif
}

void KDoiInit(char *path);
void KDoiColorMask(unsigned char *input, int gXsize, int gYsize,
                   ByteImage *image, int maskValue);

#ifndef NoTcl
void KDoi_Init(Tcl_Interp *interp);
#endif

#endif
