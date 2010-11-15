/***************************************************************************
                          classimage.cpp

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "classimage.h"



//feature detection stuff
int OptimalNoOfFeatures;

/// <summary>
/// Constructor
/// </summary>
classimage::classimage()
{
  initialised=0;
  reductionFactor_x=1;
  reductionFactor_y=1;
  image=NULL;
  width=0;
  height=0;
  rotationLookup=NULL;
  OptimalNoOfFeatures=0;
  topFeature=NULL;
}


/// <summary>
/// Destructor
/// </summary>
classimage::~classimage()
{
  int x,y;

  if (image!=NULL)
  {
    for (x=0;x<width;x++)
    {
      for (y=0;y<height;y++) 
	  {
	    delete [] image[x][y];
	  }
      delete [] image[x];  
    }
  }

  if (rotationLookup!=NULL)
  {
    for (x=0;x<width;x++)
    {
      for (y=0;y<height;y++) 
	  {
	    delete [] rotationLookup[x][y];
	  }
      delete [] rotationLookup[x];  
    }
  }

}


/// <summary>
/// create a new image
/// </summary>
/// <param name="wdth">Width of the image in pixels</param>
/// <param name="hght">Height of the image in pixels</param>
void classimage::createImage(int wdth, int hght)
{
  int x,y;

  width = wdth;
  height = hght;

  image = new unsigned char**[width];
  Integral = new long**[width];
  for (x=0;x<width;x++)
    {
    image[x] = new unsigned char *[height];
	Integral[x] = new long*[height];
    for (y=0;y<height;y++)
      {
      image[x][y] = new unsigned char[3];
	  Integral[x][y] = new long[4];
      }
    }
}



/// <summary>
/// update the integral image
/// </summary>
void classimage::updateIntegralImage()
{
  int i,x,y,v1,v2,tVal;
  long p[3],p2[3],textureVal;
  
  textureVal = 0;
  for (y = 0;y<height;y++)
  {

	for (i=0;i<3;i++)
	{
      p[i] = 0;
	  p2[i] = 0;
	}

    for (x = 0;x<width;x++)
	{
      v1 = image[x][y][0];
      if (x > 0) v2 = image[x - 1][y][0]; else v2 = 0;
      tVal = abs(v1 - v2);
      if (y > 0) v2 = image[x][y - 1][0]; else v2 = 0;
      tVal += abs(v1 - v2);
      textureVal += tVal;
      
	  for (i=0;i<3;i++)
	  {
        p[i] += image[x][y][i];
        p2[i] += tVal;
	  }

      if (y > 0)
	  {
        Integral[x][y][0] = p[0] + Integral[x][y - 1][0];
        Integral[x][y][1] = p2[0] + Integral[x][y - 1][1];

        Integral[x][y][2] = p[1] + Integral[x][y - 1][2];
        Integral[x][y][3] = p[2] + Integral[x][y - 1][3];
	  }
      else
	  {
        Integral[x][y][0] = p[0];
        Integral[x][y][1] = p2[0];
        Integral[x][y][2] = p[1];
        Integral[x][y][3] = p[2];
      }
	
	}
  }
  Texture = textureVal / (width * height);

}




/// <summary>
/// Get the total pixel value for the given area
/// </summary>
/// <param name="tx">Top left x coordinate</param>
/// <param name="ty">Top left y coordinate</param>
/// <param name="bx">Bottom right x coordinate</param>
/// <param name="by">Bottom right y coordinate</param>
/// <param name="index">An index number corresponding to the type of integral</param>
long classimage::getIntegral(int tx, int ty, int bx, int by, int index)
{
  return(Integral[bx][by][index] + Integral[tx][ty][index] - (Integral[tx][by][index] + Integral[bx][ty][index]));
}



/// <summary>
/// Detect different types of feature
/// </summary>
/// <param name="x">Top left x coordinate</param>
/// <param name="y">Top left y coordinate</param>
/// <param name="wdth">Width of the feature</param>
/// <param name="hght">Height of the feature</param>
/// <param name="featureType">The type of feature to be detected</param>
long classimage::detectFeature(int x, int y, int wdth, int hght, int featureType)
{
  long area[4];
  long v=0;
  
  switch(featureType)
  {
    case 0:  //A
	{
      area[0] = getIntegral(x, y, x + wdth, y + hght + hght, 0);
      area[1] = getIntegral(x + wdth, y, x + wdth + wdth, y + hght + hght, 0);
      v = abs(area[0] - area[1]);
	  break;
	}

    case 1:  //B
	{
      area[0] = getIntegral(x, y, x + wdth, y + hght, 0);
      area[1] = getIntegral(x, y + hght, x + wdth, y + hght + hght, 0);
      v = abs(area[0] - area[1]);
	  break;
	}

    case 2:  //C
	{
      area[0] = getIntegral(x, y, x + wdth, y + hght, 0);
      area[1] = getIntegral(x + wdth, y, x + wdth + wdth, y + hght, 0);
      area[2] = getIntegral(x + wdth + wdth, y, x + wdth + wdth + wdth, y + hght, 0);
      v = abs((area[1] * 2) - (area[0] + area[2]));
	  break;
	}

    case 3:  //D
	{
      area[0] = getIntegral(x, y, x + wdth, y + hght, 0);
      area[1] = getIntegral(x + wdth, y, x + wdth + wdth, y + hght, 0);
      area[2] = getIntegral(x, y + hght, x + wdth, y + hght + hght, 0);
      area[3] = getIntegral(x + wdth, y + hght, x + wdth + wdth, y + hght + hght, 0);
      v = abs((area[1] + area[2]) - (area[0] + area[3]));
	  break;
	}

	
    case 4:  //A
	{
      area[0] = getIntegral(x, y, x + wdth, y + hght + hght, 1);
      area[1] = getIntegral(x + wdth, y, x + wdth + wdth, y + hght + hght, 1);
      v = abs(area[0] - area[1]);
	  break;
	}

    case 5:  //B
	{
      area[0] = getIntegral(x, y, x + wdth, y + hght, 1);
      area[1] = getIntegral(x, y + hght, x + wdth, y + hght + hght, 1);
      v = abs(area[0] - area[1]);
	  break;
	}

    case 6:  //C
	{
      area[0] = getIntegral(x, y, x + wdth, y + hght, 1);
      area[1] = getIntegral(x + wdth, y, x + wdth + wdth, y + hght, 1);
      area[2] = getIntegral(x + wdth + wdth, y, x + wdth + wdth + wdth, y + hght, 1);
      v = abs((area[1] * 2) - (area[0] + area[2]));
	  break;
	}

    case 7:  //D
	{
      area[0] = getIntegral(x, y, x + wdth, y + hght, 1);
      area[1] = getIntegral(x + wdth, y, x + wdth + wdth, y + hght, 1);
      area[2] = getIntegral(x, y + hght, x + wdth, y + hght + hght, 1);
      area[3] = getIntegral(x + wdth, y + hght, x + wdth + wdth, y + hght + hght, 1);
      v = abs((area[1] + area[2]) - (area[0] + area[3]));
	  break;
	}


    case 8:  
	{
      v = getIntegral(x, y, x + wdth + wdth, y + hght + hght, 0);
	  break;
	}

    case 9:  
	{
      v = getIntegral(x, y, x + wdth + wdth, y + hght + hght, 1);
	  break;
	}

  }
  
  return(v);
}




/// <summary>
/// Create feature set for the given sub region
/// </summary>
int classimage::createFeatureList(int tx, int ty, int bx, int by, int &threshold, int resolution_x, int resolution_y, long **result, int &cx, int &cy, bool clearTopFeatures)
{
  float x,y,xinc,yinc;
  int i,j,winner,hits,xx,yy;
  long v,vmax,p,pmax,pixels,tmp[6];
  bool found;
  static int initFeatureList=0;

  if (initFeatureList==0)
  {
    OptimalNoOfFeatures=50;
	topFeature = new long*[OptimalNoOfFeatures];
	for (i=0;i<OptimalNoOfFeatures;i++) topFeature[i] = new long[6];
    initFeatureList=1;
  }

  if (clearTopFeatures)
    for (i=0;i<OptimalNoOfFeatures;i++)
      for (j=0;j<6;j++) topFeature[i][j]=0;

  //xinc = (bx-tx)/(float)resolution_x;
  //yinc = (by-ty)/(float)resolution_y;
  xinc = (float)resolution_x;
  yinc = (float)resolution_y;

  xx = (int)(xinc*3);
  yy = (int)(yinc*3);
  pixels = xx*yy;

  cx = 0;
  cy = 0;
  hits=0;
  pmax=0;
  p=0;
  for (y=(float)ty;y<(float)(by-1-yy);y+=yinc)
  {
    for (x=(float)tx;x<(float)(bx-1-xx);x+=xinc)
	{
      vmax=0;
	  for (i=0;i<4;i++)
	  {
	    v = detectFeature((int)x,(int)y,(int)xinc,(int)yinc,i);
		if (v>vmax)
		{
		  vmax = v;
		  winner=i;
		}
	  }

	  if (vmax>threshold)
	  {
	    result[p][0] = winner+1;
	    result[p][1] = (vmax*1000)/pixels;
		hits++;
		cx += (int)x;
		cy += (int)y;

		j=0;
		found=false;
		while ((j<OptimalNoOfFeatures) && (!found))
		{
		  if (topFeature[j][0]<result[p][1])
		  {
		    topFeature[j][0] =result[p][1];
			topFeature[j][1] =(long)x;
			topFeature[j][2] =(long)y;
			topFeature[j][3] =(long)winner;
			topFeature[j][4] =(long)xx;
			topFeature[j][5] =(long)yy;
		    found=true;
		  }
		  j++;
		}
	  }
	  else
	  {
	    result[p][0] = 0;
		result[p][1] = 0;
	  }

	  p++;
	}
  }

  //get the centre of features
  if (hits>0)
  {
    cx/=hits;
    cy/=hits;
  }

  //if no threshold has been given then set an appropriate value
  if (threshold==0)
  {
    pmax=p;
	vmax=99999999;
    for (i=0;i<OptimalNoOfFeatures;i++)
	{
	  if ((topFeature[i][0]>0) && (topFeature[i][0]<vmax))
	    vmax = topFeature[i][0];
	}

	if (vmax!=99999999)
    {
	  threshold=vmax;

      for (p=0;p<pmax;p++)
	  {
	    if (result[p][1]<threshold)
		{
		  result[p][0]=0;
		  result[p][1]=0;
		}
	  }
	}
	else
	threshold=0;
  }


  //sort
  for (i=0;i<OptimalNoOfFeatures-1;i++)
  {
    vmax=0;
	winner=i;
    for (j=i+1;j<OptimalNoOfFeatures;j++)
      if (topFeature[j][1]>topFeature[i][1]) winner=j;

	if (winner!=i)
	{
	  for (j=0;j<6;j++)
	    tmp[j] = topFeature[i][j];

	  for (j=0;j<6;j++)
	  {
	    topFeature[i][j] = topFeature[winner][j];
		topFeature[winner][j] = tmp[j];
	  }
	}
  }



  return(hits);
}




/// <summary>
/// This produces an image based on a specified region within another image.  For example extracting the face region from a picture of a person.
/// </summary>
/// <param name="example_img">Image object</param>
/// <param name="tx">Top left x coordinate</param>
/// <param name="ty">Top left y coordinate</param>
/// <param name="bx">Bottom right x coordinate</param>
/// <param name="by">Bottom right y coordinate</param>
void classimage::sampleFromImage(classimage *example_img, int tx, int ty, int bx, int by)
{
  int x,y,xx,yy,dx,dy,c;
  
  dx = bx - tx;
  dy = by - ty;
  
  for (x = 0;x<width;x++)
  {
    xx = tx + ((x * dx) / width);
    
    for (y = 0;y<height;y++)
	{
      yy = ty + ((y * dy) / height);
    
      for (c=0;c<3;c++) image[x][y][c] = example_img->image[xx][yy][c];
    }
  }
  
  updateIntegralImage();
}



/// <summary>
/// Returns the average colour within the given region
/// </summary>
/// <param name="tx">Top left x coordinate</param>
/// <param name="ty">Top left y coordinate</param>
/// <param name="bx">Bottom right x coordinate</param>
/// <param name="by">Bottom right y coordinate</param>
/// <param name="av_r">Returned average red component</param>
/// <param name="av_g">Returned average green component</param>
/// <param name="av_b">Returned average blue component</param>
void classimage::averageColour(int tx, int ty, int bx, int by, int &av_r, int &av_g, int &av_b)
{
  int x,y;
  long r,g,b,pixels,i;
  
  pixels = (bx-tx)*(by-ty);
  r=0;
  g=0;
  b=0;
  i=0;
  for (x = tx;x<bx;x++)
  {
    for (y = ty;y<by;y++)
	{
	  i += image[x][y][0];
      r += image[x][y][0];
      g += image[x][y][1];
      b += image[x][y][2];
    }
  }
  
  averageIntensity = i/pixels;
  av_r = r/pixels;
  av_g = g/pixels;
  av_b = b/pixels;
}


/// <summary>
/// Returns the average intensity within the given region
/// </summary>
/// <param name="tx">Top left x coordinate</param>
/// <param name="ty">Top left y coordinate</param>
/// <param name="bx">Bottom right x coordinate</param>
/// <param name="by">Bottom right y coordinate</param>
/// <returns>Average intensity</returns>
int classimage::getAverageIntensity(int tx, int ty, int bx, int by)
{
  int x,y;
  long pixels,i;
  
  pixels = (bx-tx)*(by-ty);
  i=0;
  for (x = tx;x<bx;x++)
  {
    for (y = ty;y<by;y++)
	  i += image[x][y][0];
  }
  
  return((int)(i/pixels));
}



/// <summary>
/// Get image from a bitmap
/// </summary>
/// <param name="bmp">Bitmap image</param>
/// <param name="RGBformat">The format of the bitmap</param>
/// <param name="wdth">Width of the bitmap</param>
/// <param name="hght">Height of the bitmap</param>
void classimage::updateFromBitmap(BYTE *bmp, int RGBformat, int wdth, int hght)
{
  int x,y,xx,yy;
  unsigned char r,g,b;
  int p;

  //create a new image array if necessary
  if (width==0) createImage(wdth/reductionFactor_x,hght/reductionFactor_y);

  //populate the image array from the bitmap
  p=0;
  for (y=0;y<hght;y++)
  {
    for (x=0;x<wdth;x++)
    {
	  if (RGBformat==0)  //pixels in RGB order
	  {
	    r = (unsigned char)bmp[p]; p++;
	    g = (unsigned char)bmp[p]; p++;
	    b = (unsigned char)bmp[p]; p++;
	  }
	  else
	  { //pixels in BGR order
	    b = (unsigned char)bmp[p]; p++;
	    g = (unsigned char)bmp[p]; p++;
	    r = (unsigned char)bmp[p]; p++;
	  }

	  xx = x/reductionFactor_x;
	  yy = y/reductionFactor_y;

      image[xx][yy][0] = r;
      image[xx][yy][1] = g;
      image[xx][yy][2] = b;
    }
  }

  updateIntegralImage();
}


/// <summary>
/// Save the image to a bitmap
/// </summary>
/// <param name="bmp">Bitmap image</param>
/// <param name="RGBformat">The format of the bitmap</param>
/// <param name="wdth">Width of the bitmap</param>
/// <param name="hght">Height of the bitmap</param>
void classimage::saveToBitmap(BYTE *bmp, int RGBformat, int wdth, int hght)
{
  int x,y,xx,yy;
  unsigned char r,g,b;
  int p;
  int factor;


  //create a new image array if necessary
  if (width==0) createImage(wdth/reductionFactor_x,hght/reductionFactor_y);

  factor = wdth/width;

  p=0;
  for (y=0;y<hght;y++)
    {
    for (x=0;x<wdth;x++)
	  {
        xx = x * factor;
		yy = y * factor;
		if (xx>width-1) xx = width-1;
		if (yy>height-1) yy = height-1;
        
        r = image[xx][yy][0];
        g = image[xx][yy][1];
        b = image[xx][yy][2];

	  
  	    if (RGBformat==0)  //pixels in RGB order
		{
	      bmp[p] = (BYTE)r; p++;
	      bmp[p] = (BYTE)g; p++;
	      bmp[p] = (BYTE)b; p++;
		}
	    else
		{ //pixels in BGR order
	      bmp[p] = (BYTE)b; p++;
	      bmp[p] = (BYTE)g; p++;
	      bmp[p] = (BYTE)r; p++;
		}

      }
    }


  //test pattern
  /*
  int pixels = width*height;
  int p2;
  for (p=0;p<pixels*3;p++) 
  {
  bmp[p]=(BYTE)255;
  p2++;
  if (p2>100) p2=0;
  }
  */

}






/// <summary>
/// Random number generator
/// </summary>
/// <returns>A random number in the range 0.0 to 1.0</returns>
float classimage::Rnd()
{
  return(rand()/(float)RAND_MAX);
}


/// <summary>
/// Update from another image object
/// </summary>
/// <param name="img">Image object</param>
void classimage::updateFromImage(classimage *img)
{
  int x,y,xx,yy,c;

  if (!initialised)
    {
    createImage(img->width/reductionFactor_x,img->height/reductionFactor_y);
    initialised=true;
    }

  xx=0;
  for (x=0;x<img->width;x+=reductionFactor_x)
    {
    yy=0;
    for (y=0;y<img->height;y+=reductionFactor_y)
      {
      for (c=0;c<3;c++) image[xx][yy][c] = img->image[x][y][c];
      yy++;
      }
    xx++;
    }

  updateIntegralImage();
}




/// <summary>
/// Rotate the image by 90 degrees
/// </summary>
void classimage::Rotate90()
{
  int x,y,c;
  unsigned char temp;
  
  for (x=0;x<width;x++)
  {
    for (y=0;y<height;y++)
	{
      for (c=0;c<3;c++)
	  {
        temp = image[y][x][c];
        image[y][x][c] = image[x][y][c];
        image[x][y][c] = temp;
      }
    }
  }
}



/// <summary>
/// Draws a green box
/// </summary>
/// <param name="cx">Centre x coordinate</param>
/// <param name="cy">Centre y coordinate</param>
/// <param name="boxwidth">Width of the box</param>
/// <param name="boxheight">Height of the box</param>
/// <param name="boxrotation">Rotation of the box in degrees</param>
void classimage::DrawBox(int cx, int cy, int boxwidth, int boxheight,int boxrotation)
{
  int halfwidth = boxwidth/2;
  int halfheight = boxheight/2;
  int x,y;


  for (y=cy - halfheight;y<cy+halfheight;y++)
  {
    if ((y>-1) && (y<height-1))
	{
	  if (cx-halfwidth>-1)
	  {
		image[cx-halfwidth][y][0]=0;
		image[cx-halfwidth][y][1]=255;
		image[cx-halfwidth][y][2]=0;
	  }
	  if (cx+halfwidth<width)
	  {
		image[cx+halfwidth][y][0]=0;
		image[cx+halfwidth][y][1]=255;
		image[cx+halfwidth][y][2]=0;
	  }
	}
  }

  for (x=cx - halfwidth;x<cx+halfwidth;x++)
  {
    if ((x>-1) && (x<width-1))
    {
	  if (cy-halfheight>-1)
	  {
		image[x][cy-halfheight][0]=0;
		image[x][cy-halfheight][1]=255;
		image[x][cy-halfheight][2]=0;
	  }
	  if (cy+halfheight<height)
	  {
		image[x][cy+halfheight][0]=0;
		image[x][cy+halfheight][1]=255;
		image[x][cy+halfheight][2]=0;
	  }

    }
  }

}


/// <summary>
/// Rotate the image
/// </summary>
/// <param name="sourceImage">The sourec image</param>
/// <param name="angle">The rotation angle in degrees</param>
void classimage::Rotate(classimage *sourceImage, int angle)
{
  float rot,hyp,ang;
  int cx,cy,x,y,c,xx,yy;

  if (rotationLookup==NULL)
  {
    imageRotation = angle;

	//create the lookup table
    rotationLookup = new int**[width];
    for (x=0;x<width;x++)
	{
	  rotationLookup[x] = new int*[height];
	  for (y=0;y<height;y++)
	  {
	    rotationLookup[x][y] = new int[2];
		rotationLookup[x][y][0]=999;
		rotationLookup[x][y][1]=999;
	  }
	}

    //rotate the image using floating point maths
	rot = (float)((float)angle / 180.0 * 3.1415927);
    cx = sourceImage->width/2;
    cy = sourceImage->height/2;

    for (x=0;x<sourceImage->width;x++)
	{
      for (y=0;y<sourceImage->height;y++)
	  {
	    hyp = (float)sqrt(((x-cx)*(x-cx))+((y-cy)*(y-cy)));
		if (hyp>0)
		{
		  ang = (float)acos((y-cy)/hyp);
		  if (x-cx>0) ang = (float)((2*3.1415927)-ang);
	      xx = cx + (int)(hyp * sin(rot-ang));
	      yy = cy + (int)(hyp * cos(rot-ang));
		}
		else
		{
          xx=x;
		  yy=y;
        }

     	if ((xx>=0) && (xx<width) && (yy>=0) && (yy<height))
		{
		  rotationLookup[x][y][0]=xx;
		  rotationLookup[x][y][1]=yy;
		}
		else
		{
		  rotationLookup[x][y][0] = 0;
		  rotationLookup[x][y][1] = 0;
		}
	  }
	}
  }
  else
  {
	//just use the lookup table as a quick method to rotate the image
    for (x=0;x<width;x++)
	{
	  for (y=0;y<height;y++)
	  {
	    if (rotationLookup[x][y][0]!=999)
		{
		  xx = rotationLookup[x][y][0];
		  yy = rotationLookup[x][y][1];
		  for (c=0;c<3;c++)
		    image[x][y][c] = sourceImage->image[xx][yy][c];
		}
	  }
	}

    //recalculate the integral image
    updateIntegralImage();
  }

}



/// <summary>
/// Flood fill from the given point using the given colour
/// </summary>
void classimage::floodFill(int x, int y, int r, int g, int b, int depth, int &tx, int &ty, int &bx, int &by, long &pixels, long &av_r, long &av_g, long &av_b, classimage *sourceImage)
{
  if ((image[x][y][0]==0) && (image[x][y][1]==0) && (image[x][y][2]==0) && (sourceImage->image[x][y][0]>0) && (depth<2000))
  {
    if (x<tx) tx=x;
	if (x>bx) bx=x;
	if (y<ty) ty=y;
	if (y>by) by=y;

    av_r += sourceImage->image[x][y][0];
	av_g += sourceImage->image[x][y][1];
	av_b += sourceImage->image[x][y][2];

	pixels++;

	if ((r==0) && (g==0) && (b==0))
	{
	  r=1;
	  g=1;
	  b=1;
	}

    image[x][y][0]=r;
	image[x][y][1]=g;
	image[x][y][2]=b;

	if (x>0)
	{
      floodFill(x-1,y,r,g,b,depth+1,tx,ty,bx,by,pixels,av_r,av_g,av_b,sourceImage);

	  if (y>0)
	  {
        floodFill(x-1,y-1,r,g,b,depth+1,tx,ty,bx,by,pixels,av_r,av_g,av_b,sourceImage);

		if (x<width-1)
		{
          floodFill(x+1,y-1,r,g,b,depth+1,tx,ty,bx,by,pixels,av_r,av_g,av_b,sourceImage);
		}
	  }

	  if (x<width-1)
	  {
        floodFill(x+1,y,r,g,b,depth+1,tx,ty,bx,by,pixels,av_r,av_g,av_b,sourceImage);

		if (y<height-1)
		{
          floodFill(x+1,y+1,r,g,b,depth+1,tx,ty,bx,by,pixels,av_r,av_g,av_b,sourceImage);
		}
	  }
	}

    if (y>0)
	{
      floodFill(x,y-1,r,g,b,depth+1,tx,ty,bx,by,pixels,av_r,av_g,av_b,sourceImage);
	}

	if (y<height-1)
	{
      floodFill(x,y+1,r,g,b,depth+1,tx,ty,bx,by,pixels,av_r,av_g,av_b,sourceImage);
	}

  }
}



/// <summary>
/// replace one colour with another
/// </summary>
/// <param name="r">The red component of the colour to be replaced</param>
/// <param name="g">The green component of the colour to be replaced</param>
/// <param name="b">The blue component of the colour to be replaced</param>
/// <param name="new_r">The new red component</param>
/// <param name="new_g">The new green component</param>
/// <param name="new_b">The new blue component</param>
void classimage::replaceColour(int r, int g, int b, int new_r, int new_g, int new_b)
{
  int x,y;

  if ((new_r==0) && (new_g==0) && (new_b==0))
  {
    new_r=1;
	new_g=1;
	new_b=1;
  }
  
  for (x=0;x<width;x++)
  {
    for (y=0;y<height;y++)
	{
      if ((image[x][y][0]==r) && (image[x][y][1]==g) && (image[x][y][2]==b))
	  {
        image[x][y][0] = new_r;
        image[x][y][1] = new_g;
        image[x][y][2] = new_b;
      }
    }
  }
}



/// <summary>
/// Replace one colour with another within the given area
/// </summary>
/// <param name="r">The red component of the colour to be replaced</param>
/// <param name="g">The green component of the colour to be replaced</param>
/// <param name="b">The blue component of the colour to be replaced</param>
/// <param name="new_r">The new red component</param>
/// <param name="new_g">The new green component</param>
/// <param name="new_b">The new blue component</param>
/// <param name="tx">Top left x coordinate</param>
/// <param name="ty">Top left y coordinate</param>
/// <param name="bx">Bottom right x coordinate</param>
/// <param name="by">Bottom right y coordinate</param>
void classimage::replaceColourArea(int r, int g, int b, int new_r, int new_g, int new_b, int tx, int ty, int bx, int by)
{
  int x,y;
  
  for (x=tx;x<bx;x++)
  {
    for (y=ty;y<by;y++)
	{
      if ((image[x][y][0]==r) && (image[x][y][1]==g) && (image[x][y][2]==b))
	  {
        image[x][y][0] = new_r;
        image[x][y][1] = new_g;
        image[x][y][2] = new_b;
      }
    }
  }
}



/// <summary>
/// Filter using the given colour
/// </summary>
long classimage::filterColour(classimage *sourceImage, int r, int g, int b, int tollerance, int &centre_x, int &centre_y)
{
  int x,y,dr,dg,db,count;
  long pixels,cx,cy;
  
  cx=0;
  cy=0;
  pixels=0;
  count=1;
  for (x=0;x<width;x++)
  {
    for (y=0;y<height;y++)
	{
	  dr = abs(sourceImage->image[x][y][0] - r);
	  dg = abs(sourceImage->image[x][y][1] - g);
	  db = abs(sourceImage->image[x][y][2] - b);

      if ((dr<tollerance) && (dg<tollerance) && (db<tollerance))
	  {
        image[x][y][0] = 255;
        image[x][y][1] = 255;
        image[x][y][2] = 255;
		pixels++;
		cx += x;
		cy += y;
		count++;
      }
	  else
	  {
        image[x][y][0] = 0;
        image[x][y][1] = 0;
        image[x][y][2] = 0;
	  }
    }
  }

  centre_x = cx / count;
  centre_y = cy / count;
  updateIntegralImage();

  return(pixels);
}



//------------------------------------------------------------------------------------------------------------------------
//filter using skin colour
//------------------------------------------------------------------------------------------------------------------------
long classimage::filterSkinColour(classimage *sourceImage, int tollerance, int &centre_x, int &centre_y)
{
  int x,y,dr,dg,count;
  long pixels,cx,cy,r,g,b;
  
  cx=0;
  cy=0;
  pixels=0;
  count=1;
  for (x=0;x<width;x++)
  {
    for (y=0;y<height;y++)
	{
	  r = sourceImage->image[x][y][0];
	  g = sourceImage->image[x][y][1];
	  b = sourceImage->image[x][y][2];

	  dr = 2*r;
	  dg = g+b;

      if ((dr-dg>tollerance) && (dr-dg<100))
	  {
        image[x][y][0] = 255;
        image[x][y][1] = 255;
        image[x][y][2] = 255;
		pixels++;
		cx += x;
		cy += y;
		count++;
      }
	  else
	  {
        image[x][y][0] = 0;
        image[x][y][1] = 0;
        image[x][y][2] = 0;
	  }
    }
  }

  centre_x = cx / count;
  centre_y = cy / count;
  updateIntegralImage();

  return(pixels);
}



//------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------
int classimage::relativeThreshold(int value, int tx, int ty, int bx, int by, int &averageWidth)
{
  int av,pixels,NoOfRows;
  int x,y,min,hits,rowhits,totrowhits,minrowhits;
  
  pixels = width * height;
  hits = 0;
  av = 0;
  for (x=0;x<width;x++)
    for (y=0;y<height;y++) av = av + image[x][y][0];
  
  av = av / pixels;
  min = (int)((av * (100 - value)) / 100);
  
  NoOfRows=1;
  minrowhits = (bx-tx)/5;
  totrowhits=0;
  for (y=ty;y<by;y++)
  {
	rowhits=0;
    for (x=tx;x<bx;x++)
	{
      if (image[x][y][0] < min)
		rowhits++;
	}

	hits+=rowhits;
	if (rowhits>minrowhits)
	{
      totrowhits+=rowhits;
	  NoOfRows++;
	}
  }
  totrowhits /= NoOfRows;
  averageWidth = (((by-ty) - NoOfRows) + totrowhits)/2;
  
  return((hits * 100) / pixels);
}

//------------------------------------------------------------------------------------------------------------------------
//returns the 'centre of gravity' for the given region relative to the given colour
//------------------------------------------------------------------------------------------------------------------------
void classimage::CG(int tx, int ty, int bx, int by, unsigned char targ_r, unsigned char targ_g, unsigned char targ_b, int &cx, int &cy, int maxval)
{
  int x,y,p[3],c;
  long tot,tot_x,tot_y,dp;
  
  tot = 1;
  tot_x = 0;
  tot_y = 0;
  for (x=tx;x<bx;x++)
  {
    for (y=ty;y<by;y++)
	{
      for (c=0;c<3;c++) p[c] = image[x][y][c];
      dp = abs(p[0] - (int)targ_r);
      dp += abs(p[1] - (int)targ_g);
      dp += abs(p[2] - (int)targ_b);
      dp = maxval - dp;
	  if (dp<0) dp=0;
    
      tot += dp;
      tot_x += x * dp;
      tot_y += y * dp;
    }
  }
  
  cx = tot_x / tot;
  cy = tot_y / tot;
}


