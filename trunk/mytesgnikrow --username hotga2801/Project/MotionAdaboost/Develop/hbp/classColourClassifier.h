/***************************************************************************
                          classColourClassifier.h

  used to classify the type and distribution of colours

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


#ifndef CLASS_COLOURCLASSIFIER_H
#define CLASS_COLOURCLASSIFIER_H


/**
  *@author Bob Mottram
  */

#include "stdafx.h"
#include <afxwin.h>

/// <summary>
/// This class stores a list of commonly observed colours.  It is used to classify clothing typically worn by individuals.
/// </summary>
class classColourClassifier {
private:
  //bounding box
  int top_x, top_y, bottom_x, bottom_y;
  long total_pixels;
  bool sorted;

  //colour info
  int NoOfGroups;
  long colour_group[100][5];

  //sort the groups by no of hits
  void sort();

public:

  //the tollerance within which a colour is classified as belonging to a group
  int tollerance;

  void clear();
  void update(int r, int g, int b, int x, int y);
  void getGroupColour(int index, int &r, int &g, int &b, int &coverage);

  classColourClassifier();
  ~classColourClassifier();
};

#endif
