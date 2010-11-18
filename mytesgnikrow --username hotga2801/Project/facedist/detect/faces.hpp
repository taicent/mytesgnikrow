#ifndef FacesIncluded
#define FacesIncluded

void UnpackAndFFTFast(int width, int height, signed char *bytes, 
                      FastForwardStruct *net);

void ImageToFFTFast(int width, int height, int *img, int *map, double scale,
                    FastForwardStruct *net);

#ifndef NoTcl
void Faces_Init(Tcl_Interp *interp);
#endif

// USED
// Data structure used by positive example alignment code
struct Alignment {
  // Number of faces and number of features per face
  int numFaces, numFeatures;

  // Size is 2*numFeatures, to give an average x,y location for each feature
  double *average;
  int *avgValid;
  // Size is 2*numFeatures*numFaces, locations for each feature/face
  double *data;
  // Whether the features for each face are valid
  int *valid;
  // Size is 4*numFaces, giving the transformation for each face to best
  // match the average feature locations.  The transformation is four numbers,
  // a,b,c,d, arranged in the following matrix:
  //     a -b  c
  //     b  a  d
  //     0  0  1
  double *xform;
};

struct PointList {
  int numPoints;
  double *x;
  double *y;
};

#endif
