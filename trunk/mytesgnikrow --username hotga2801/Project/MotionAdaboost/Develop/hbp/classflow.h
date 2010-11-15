/***************************************************************************
                          classflow.h  

    optical flow

    begin                : Sun Nov 24 2002
    copyright            : (C) 2002 by Bob Mottram
    email                : fuzzgun@btinternet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CLASSFLOW_H
#define CLASSFLOW_H

#include "classimage.h"
#include "classmotion.h"

/**
  *@author Bob Mottram
  */

const int IMAGE_FLOW_X = 0;
const int IMAGE_FLOW_Y = 1;
  
/// <summary>
/// This class performs optical flow calculations
/// </summary>
class classflow : public classimage
{
  
private:

  void init();
  bool initialised;
  int flowHistoryCtr,noflow;

public:

  classmotion *motion;

  int motion_threshold;
  int optical_flow_search_region;
  int optical_flow_patch;
  int boundingbox_tx,boundingbox_ty,boundingbox_bx,boundingbox_by;
  int averageDisparity,centre_x,centre_y;
  float aspectRatio;

  unsigned char ***flow;
  unsigned char ***segmented;

  //histogram storing flow directions
  int flowHistogram[14][14];
  long flowHistogramMax;
  int flowHistory[100][7];

  //average optical flow over the enture image
  float average_flow_x,average_flow_y;

  void update(unsigned char ***img, int wdth, int hght);
  void update(classimage *img);
  void segment(classimage *img, int distance);
  void updateDisparity(classflow *f);
  void matchFlow(classimage *img, classimage *rawimgLeft, classimage *rawimgRight);
  int getFlowHistory(int t, int index);

  classflow();
  ~classflow();
};

#endif
