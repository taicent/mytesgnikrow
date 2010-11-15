/***************************************************************************
                             classfilter.h 

    Used to implement gaussian and other types of low level filter

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


#ifndef CLASSFILTER_H
#define CLASSFILTER_H

#include "classimage.h"


/**
  *@author Bob Mottram
  */

/// <summary>
/// This class implements various types of image filter, such as gaussian.
/// </summary>
class classfilter : public classimage
{

private:

  int initialised;
  void init();

  //array used to apply weighted filters to the raw image
  unsigned char filter_matrix[10][10];

  void applyFilter(unsigned char ***img, int Filter_Type, int Filter_Radius);
  void createFilter_Horizontal(int width);
  void createFilter_Vertical(int width);
  void createFilter_Gaussian(int width);
  float function_Gaussian(float Distance, float radius);

public:

  int filtertype;
  int filterradius;

  void update(unsigned char ***img, int wdth, int hght, int filter_type);
  void update(classimage *img);

	classfilter();
	~classfilter();
};

#endif
