/***************************************************************************
                             classfilter.cpp

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

#include <math.h>
#include <stdlib.h>
#include "classimage.h"
#include "classfilter.h"


//image filter types
const int FILTER_GAUSSIAN   = 1;
const int FILTER_VERTICAL   = 2;
const int FILTER_HORIZONTAL = 3;


/// <summary>
/// Constructor
/// </summary>
classfilter::classfilter()
{
  filtertype=-1;
  filterradius=2;
  initialised=false;
}


/// <summary>
/// Destructor
/// </summary>
classfilter::~classfilter()
{
}


/// <summary>
/// Initialise
/// </summary>
void classfilter::init()
{
  createImage(width,height);
  initialised=true;
}


/// <summary>
/// Update the filter
/// </summary>
/// <param name="img">bitmap image</param>
/// <param name="wdth">width of the image</param>
/// <param name="hght">height of the image</param>
/// <param name="filter_type">The type of filter to be applied</param>
void classfilter::update(unsigned char ***img, int wdth, int hght, int filter_type)
{
  width = wdth;
  height = hght;
  if (filterradius>9) filterradius=9;

  if (initialised==false) init();
  applyFilter(img,filter_type,filterradius);

}


/// <summary>
/// Update the filter
/// </summary>
/// <param name="img">image object</param>
void classfilter::update(classimage *img)
{
  if (img->width>0)
  {
  width = img->width;
  height = img->height;

  if (initialised==false) init();

  applyFilter(img->image,1,filterradius);
  }
}

/// <summary>
/// Applies a filter to the given image with the given radius
/// </summary>
/// <param name="img">bitmap image</param>
/// <param name="filter_type">The type of filter to be applied</param>
/// <param name="filter_radius">The radius in pixels within which to apply the filter</param>
void classfilter::applyFilter(unsigned char ***img, int Filter_Type, int Filter_Radius)
{
  int x1,y1;
  int fx,fy;
  int ax,ay;
  int c,w;
  int tx,ty,bx,by;
  float p1, pixels;
  long pixval[3];
  unsigned char img_p;

  if (Filter_Radius < 1) Filter_Radius = 1;
  if (filtertype!=Filter_Type)
    {
    switch (Filter_Type)
      {
      case FILTER_GAUSSIAN:   { createFilter_Gaussian(Filter_Radius*2); break; }
      case FILTER_VERTICAL:   { createFilter_Vertical(Filter_Radius*2); break; }
      case FILTER_HORIZONTAL: { createFilter_Horizontal(Filter_Radius*2); break; }
      }
    }

  pixels = (float)(4 * Filter_Radius * Filter_Radius * 128);

  tx = Filter_Radius;
  ty = Filter_Radius;
  bx = width - Filter_Radius - 1;
  by = height - Filter_Radius - 1;
  y1=0;

  for (x1=0;x1<width;x1++)
  {
    for (y1=0;y1<height;y1++)
	{
      if ((y1>=ty) && (y1<by) && (x1>=tx) && (x1<bx))
	  {
        //get the current point in the raw image
	    for (c=0;c<3;c++) pixval[c] = 0;

        for (fx=0;fx<Filter_Radius*2;fx++)
		{
          ax = x1 - Filter_Radius + fx;

          for (fy=0;fy<Filter_Radius*2;fy++)
		  {
            //get the weight for this position from the filter matrix
            w = (int)(filter_matrix[fx][fy]) - 127;

		    ay = y1 - Filter_Radius + fy;

            //apply the weight to the image pixel
            for (c=0;c<3;c++)
			{
              //add a small amount otherwise filtering results in reduction of image intensity
			  img_p = img[ax][ay][c];
              p1 = (float)img_p;
              pixval[c] += (long)((p1 + 50) * w);
			}
		  }
		}

        //store the resulting value in the temporary image
        for (c=0;c<3;c++)
		{
          if (pixval[c]<0) pixval[c]=0;
          image[x1][y1][c] = (unsigned char)(pixval[c] / pixels);
		}
	  }
	  else
	  {
	    for (c=0;c<3;c++) image[x1][y1][c] = img[x1][y1][c];
	  }
	}
  }

}


/// <summary>
/// Returns a gaussian normal distribution
/// </summary>
/// <param name="Distance">distance from the centre</param>
/// <param name="radius">Radius of the filter in pixels</param>
float classfilter::function_Gaussian(float Distance, float radius)
{
  return((float)exp(-Distance / radius));
}


/// <summary>
///  Creates a gaussian filter
///  Pre-calculating the filter in this way prevents having to do a lot of floating point
///  calculations later on
/// </summary>
/// <param name="filter_width">width of the filter in pixels</param>
void classfilter::createFilter_Gaussian(int filter_width)
{
  int x,y,cx,dx,dy;
  float Dist,weight,widthSqr;

  cx = filter_width/2;
  widthSqr = (float)(filter_width * filter_width);

  for (x=0;x<10;x++)
    {
    dx = x - cx;
    for (y=0;y<10;y++)
      {
      if ((x<filter_width) && (y<filter_width))
        {
        dy = y - cx;
        Dist = (float)((dx*dx) + (dy*dy));
        weight = function_Gaussian(Dist, widthSqr) * (float)255;
        }
        else weight=0;
      filter_matrix[x][y] = (unsigned char)weight;
      }
    }
  filtertype=FILTER_GAUSSIAN;
}


/// <summary>
/// Creates a filter to detect vertical lines
/// </summary>
/// <param name="filter_width">width of the filter in pixels</param>
void classfilter::createFilter_Vertical(int filter_width)
{
  int x,y,cx,dx;
  unsigned char weight;

  cx = filter_width/2;

  for (x=0;x<10;x++)
    {
    dx = abs(x - cx);
    for (y=0;y<10;y++)
      {
      if ((x<filter_width) && (y<filter_width))
        {
        if (dx<2) weight = 255; else weight = 0;
        }
        else weight=0;
      filter_matrix[x][y] = weight;
      }
    }
  filtertype=FILTER_VERTICAL;
}


/// <summary>
/// Creates a filter to detect horizontal lines
/// </summary>
/// <param name="filter_width">width of the filter in pixels</param>
void classfilter::createFilter_Horizontal(int filter_width)
{
  int x,y,cx,dy;
  unsigned char weight;

  cx = width/2;

  for (x=0;x<10;x++)
  {
    for (y=0;y<10;y++)
    {
      if ((x<filter_width) && (y<filter_width))
      {
        dy = abs(y - cx);
        if (dy<2) weight = 255; else weight = 0;
      }
      else weight=0;
      filter_matrix[x][y] = weight;
    }
  }
  filtertype=FILTER_HORIZONTAL;
}
