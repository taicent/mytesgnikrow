/***************************************************************************
                          classmotion.h

  Produces a motion image                          

    begin                : Sat Nov 23 2002
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

#ifndef CLASSMOTION_H
#define CLASSMOTION_H

#include "classimage.h"

/**
  *@author Bob Mottram
  */

/// <summary>
/// A class used for motion detection
/// </summary>
class classmotion : public classimage
{
private:

  //used to initialise
  bool initialised,firstTime;
  void init();

public:

  //array stores the previous image
  unsigned char ***image2;

  //threshold used to eliminate low level noise
  int threshold;

  //indicator of the total motion within the image in the range 0.0 - 1.0
  float motionLevel;

  bool enableSecondaryUpdate;

  void update(classimage *img);
  void update(unsigned char ***img, int wdth, int hght);
  void update_secondary(unsigned char ***img);

  classmotion();
  ~classmotion();
};

#endif
