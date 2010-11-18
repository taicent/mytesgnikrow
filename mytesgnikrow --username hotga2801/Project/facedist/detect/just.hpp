#ifndef JustIncluded
#define JustIncluded

#ifndef NoTcl
extern "C" {
#include <tcl.h>
}
#endif

void SearchMultiOrientation(ByteImage *image, int numNetworks, int rotationNet,
			    SearchCallback callback, ClientData data[],
                            double threshold=0);

// New arbitration procedure.  Slower and takes more memory than (say)
// FuzzyVote2Direct, but is useful for the router networks because it
// keeps track of which view each detection is associated with, and
// counts that as part of the position of the detection.  Thus, only
// detections of the same view at the same location reinforce one another.
// (However, overlap removal will remove detections which overlap with each
// other but have different views.)
void NewFuzzyVote3(List<Detection> *outputList, 
                   List<Detection> *inputList, int numInputLists,
                   int width, int height, int maskWidth, int maskHeight,
                   int spread, int threshold, int overlap);

#ifndef NoTcl
void Just_Init(Tcl_Interp *interp);
#endif

#endif
