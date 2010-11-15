/***************************************************************************
                          classColourClassifier.cpp

  Used to classify the type and distribution of colours.  This is used for
  classifying people's clothing

  Comments are included in machine readable XML format, for possible future 
  production of program documentation

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



#include "stdafx.h"

/// <summary>
/// Constructor
/// </summary>
classColourClassifier::classColourClassifier()
{
  clear();
}

/// <summary>
/// Destructor
/// </summary>
classColourClassifier::~classColourClassifier()
{
}


/// <summary>
/// Clear all data
/// </summary>
void classColourClassifier::clear()
{
  NoOfGroups=0;
  top_x = 99999;
  top_y = 99999;
  bottom_x = 0;
  bottom_y = 0;
  tollerance=50;
  total_pixels=0;
  sorted=false;
}



/// <summary>
/// Sort the colours based upon the number of hits
/// </summary>
void classColourClassifier::sort()
{
  int i,j,max,winner,temp[5];

  for (i=0;i<NoOfGroups-1;i++)
  {
	max=0;
	winner=-1;
    for (j=i;j<NoOfGroups;j++)
	{
      if (colour_group[i][3] > max)
	  {
	    max = colour_group[i][3];
		winner=j;
	  }
	}

	if (winner>-1)
	{
	  //swap
	  for (j=0;j<5;j++)
	  {
	    temp[j] = colour_group[i][j];
        colour_group[i][j] = colour_group[winner][j];
		colour_group[winner][j] = temp[j];
	  }
	}
  }

  sorted=true;
}


/// <summary>
/// Update
/// </summary>
/// <param name="r">red component of the detected colour</param>
/// <param name="g">green component of the detected colour</param>
/// <param name="b">blue component of the detected colour</param>
/// <param name="x">x coordinate of the detected colour blob</param>
/// <param name="y">y coordinate of the detected colour blob</param>
void classColourClassifier::update(int r, int g, int b, int x, int y)
{
  int i,dist,r2,g2,b2;
  bool found;
  long hits;

  if (x<top_x) top_x = x;
  if (y<top_y) top_y = y;
  if (x>bottom_x) bottom_x = x;
  if (y>bottom_y) bottom_y = y;

  i=0;
  found=false;
  while ((i<NoOfGroups) && (!found))
  {
	hits = colour_group[i][3];
    r2 = colour_group[i][0] / hits;
    g2 = colour_group[i][1] / hits;
    b2 = colour_group[i][2] / hits;

    dist = abs(r2 - r) +
		   abs(g2 - g) +
		   abs(b2 - b);
	if (dist<tollerance)
    {
	  colour_group[i][0] += r;
	  colour_group[i][1] += g;
	  colour_group[i][2] += b;
	  colour_group[i][3]++;
	  total_pixels++;
	  found=true;
	}
	i++;
  }

  if ((!found) && (NoOfGroups<100))
  {
    colour_group[NoOfGroups][0] = r;
    colour_group[NoOfGroups][1] = g;
    colour_group[NoOfGroups][2] = b;
	colour_group[NoOfGroups][3] = 1;
	NoOfGroups++;
  }
}


/// <summary>
/// Returns the data for a colour group, and its coverage in the range 0-100
/// </summary>
/// <param name="index">An index number for the colour group</param>
/// <param name="r">Returned red component of the colour group</param>
/// <param name="g">Returned green component of the colour group</param>
/// <param name="b">Returned blue component of the colour group</param>
/// <param name="coverage">The size of the blob compared to the image, in the range 0-100</param>
void classColourClassifier::getGroupColour(int index, int &r, int &g, int &b, int &coverage)
{
  long hits;

  if (index<NoOfGroups)
  {
    //sort the groups if they are not already sorted
    if (!sorted) sort();

	hits = colour_group[index][3];
    r = colour_group[index][0] / hits;
    g = colour_group[index][1] / hits;
    b = colour_group[index][2] / hits;
    coverage = (hits * 100) / total_pixels;
  }
}


