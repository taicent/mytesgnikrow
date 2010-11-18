#ifndef NNIncluded
#define NNIncluded

#include "mat.hpp"

Vec Sigmoid(const Vec &vec);

double TrainThreeLayer(Mat &con1, Mat &con2, 
                       Vec *inputs, Vec *outputs,
                       int samples, int epochs);

double TrainThreeLayerBinary(Mat &con1, Mat &con2, 
                             signed char *data, Vec *outputs,
                             int samples, int epochs);

double TrainTwoLayer(Mat &con1, 
                     Vec *inputs, Vec *outputs,
                     int samples, int epochs);

#endif
