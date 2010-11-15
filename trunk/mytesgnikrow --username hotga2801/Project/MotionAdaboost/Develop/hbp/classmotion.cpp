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

#include <stdlib.h>
#include "classimage.h"
#include "classmotion.h"

//------------------------------------------------------------------------------------------------------------------------
//  constructor
//------------------------------------------------------------------------------------------------------------------------
classmotion::classmotion()
{
  initialised=false;
  threshold=0;
  enableSecondaryUpdate=false;
  firstTime=true;
}


//------------------------------------------------------------------------------------------------------------------------
//  destructor
//------------------------------------------------------------------------------------------------------------------------
classmotion::~classmotion()
{
}


//------------------------------------------------------------------------------------------------------------------------
//  initialise
//------------------------------------------------------------------------------------------------------------------------
void classmotion::init()
{
  int x,y;

  image2 = new unsigned char**[width];
  for (x=0;x<width;x++)
  {
    image2[x] = new unsigned char*[height];
    for (y=0;y<height;y++)
	{
      image2[x][y] = new unsigned char[3];
	}
  }

  createImage(width,height);

  initialised=true;
}




//------------------------------------------------------------------------------------------------------------------------
//  update the motion image
//------------------------------------------------------------------------------------------------------------------------
void classmotion::update(unsigned char ***img, int wdth, int hght)
{
  int x,y,c,p1,p2,dp,min_thresh;
  long av,totMotion;

  width = wdth;
  height = hght;

  if (initialised==false) init();

  av=0;
  totMotion=0;
  for (x=0;x<width;x++)
  {
    for (y=0;y<height;y++)
    {
      dp=0;
      for (c=0;c<3;c++)
      {
        p1 = img[x][y][c];
        p2 = image2[x][y][c];
        dp += abs(p1-p2);
        if (!enableSecondaryUpdate) image2[x][y][c] = img[x][y][c];
      }

      totMotion += dp;

      if (dp>threshold)
      {
        if (dp>255) dp=255;
        for (c=0;c<3;c++) image[x][y][c] = dp;
      }
      else
        for (c=0;c<3;c++) image[x][y][c] = 0;

      av += image[x][y][0];
    }
  }

  totMotion /= width * height;

  min_thresh = (int)(totMotion * 3); //5
  for (x=0;x<width;x++)
  {
    for (y=0;y<height;y++)
    {
      if (image[x][y][0]<min_thresh)
	    for (c=0;c<3;c++) image[x][y][c] = 0;
	}
  }

  totMotion /= 3;
  motionLevel = (float)(totMotion / 255);
  firstTime=false;
  averageIntensity = (int)(av / (width * height));
}




//------------------------------------------------------------------------------------------------------------------------
//  update the motion image
//------------------------------------------------------------------------------------------------------------------------
void classmotion::update(classimage *img)
{
  update(img->image,img->width,img->height);
}

//------------------------------------------------------------------------------------------------------------------------
//  secondary update phase
//------------------------------------------------------------------------------------------------------------------------
void classmotion::update_secondary(unsigned char ***img)
{
  int x,y,c;

  if ((initialised) && (enableSecondaryUpdate))
  {
    for (x=0;x<width;x++)
    {
      for (y=0;y<height;y++)
      {
        for (c=0;c<3;c++)
        {
          image2[x][y][c] = img[x][y][c];
        }
      }
    }
  }
}

