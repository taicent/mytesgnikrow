/***************************************************************************
                          classMotionSegment.h

  motion based image segmentation

    begin                : Thu Feb 19 2004
    copyright            : (C) 2004 by Bob Mottram
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


#ifndef CLASS_MOTIONSEGMENT_H
#define CLASS_MOTIONSEGMENT_H


/**
  *@author Bob Mottram
  */

#include "stdafx.h"
#include <afxwin.h>

/////////////////////////////////////////////////////////////////////////////
// classMotionSegment
// See motionsegment.cpp for the implementation of this class
//

/// <summary>
/// This class performs motion tracking
/// </summary>
class classMotionSegment
{
private:
  void getCoefficients(classimage *img, int tx, int ty, int bx, int by, int *values);
  void update(classimage *img);
public:

  void RCmseg_update(classimage *img);
  unsigned char RCmseg_getPixel(int x, int y, int col);
  int RCmseg_getFlowHistogram(int x, int y);
  float RCmseg_getAverageFlow_x();
  float RCmseg_getAverageFlow_y();
  float RCmseg_getAspectRatio();
  void RCmseg_getBoundingBox(int &tx, int &ty, int &bx, int &by);
  int RCmseg_getFlowHistory(int t, int index);
  float RCmseg_faceProbability();
  float RCmseg_personProbability();
  float RCmseg_activityLevel();
  void RCmseg_getFeature(int index, int &fx, int &fy, int &featureType, int &featureWidth, int &featureHeight);

  classMotionSegment();
  ~classMotionSegment();
};


#endif