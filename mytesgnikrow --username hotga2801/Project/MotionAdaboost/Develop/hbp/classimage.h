/***************************************************************************
                          classimage.h

  Base class for all images 

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

#ifndef CLASSIMAGE_H
#define CLASSIMAGE_H

#include <afxwin.h>


/**
  *@author Bob Mottram
  */

/// <summary>
/// Generic image class for storage and manipulation of images
/// </summary>
class classimage {
private:

public:

  bool initialised;
	
  //dimensions of the image
  int width, height;

  //used as a divisor to subsample the image
  int reductionFactor_x,reductionFactor_y;

  //average intensity over the whole image
  int averageIntensity;

  unsigned char ***image;
  int ***rotationLookup;
  int imageRotation;

  long ***Integral;
  int Texture;
  void updateIntegralImage();
  long getIntegral(int tx, int ty, int bx, int by, int index);
  long detectFeature(int x, int y, int wdth, int hght, int featureType);
  void sampleFromImage(classimage *img, int tx, int ty, int bx, int by);

  void createImage(int wdth, int hght);
  void updateFromBitmap(BYTE *bmp, int RGBformat, int wdth, int hght);
  void saveToBitmap(BYTE *bmp, int RGBformat, int wdth, int hght);
  void updateFromImage(classimage *img);
  void setPoint(int x, int y, int col, BYTE value) {image[x][y][col]=value;}
  float Rnd();
  void Rotate(classimage *sourceImage, int angle);
  void Rotate90();
  void DrawBox(int cx, int cy, int boxwidth, int boxheight,int boxrotation);
  int createFeatureList(int tx, int ty, int bx, int by, int &threshold, int resolution_x, int resolution_y, long **result, int &cx, int &cy, bool clearTopFeatures);
  void floodFill(int x, int y, int r, int g, int b, int depth, int &tx, int &ty, int &bx, int &by, long &pixels, long &av_r, long &av_g, long &av_b, classimage *sourceImage);
  void replaceColour(int r, int g, int b, int new_r, int new_g, int new_b);
  long filterColour(classimage *sourceImage, int r, int g, int b, int tollerance, int &centre_x, int &centre_y);
  long filterSkinColour(classimage *sourceImage, int tollerance, int &centre_x, int &centre_y);
  void replaceColourArea(int r, int g, int b, int new_r, int new_g, int new_b, int tx, int ty, int bx, int by);
  void averageColour(int tx, int ty, int bx, int by, int &av_r, int &av_g, int &av_b);
  int relativeThreshold(int value, int tx, int ty, int bx, int by, int &averageWidth);
  void CG(int tx, int ty, int bx, int by, unsigned char targ_r, unsigned char targ_g, unsigned char targ_b, int &cx, int &cy, int maxval);
  int getAverageIntensity(int tx, int ty, int bx, int by);

  long **topFeature;  

	classimage();
	~classimage();
};

#endif
