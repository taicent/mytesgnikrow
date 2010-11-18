#ifndef ImFaceIncluded
#define ImFaceIncluded

extern "C" {
#ifndef NoTcl
#include <tcl.h>
#endif
}

#ifndef NoTcl
void ImFace_Init(Tcl_Interp *interp);
#endif

#endif
