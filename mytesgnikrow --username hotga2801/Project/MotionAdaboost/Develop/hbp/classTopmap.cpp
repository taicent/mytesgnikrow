/*
Rodney robot - Topological map
Copyright (C) 2002  Bob Mottram

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


Comments are included in machine readable XML format, for possible future 
production of program documentation
*/

//------------------------------------------------------------------------------------------------------------------------
//  Video for windows stuff
//------------------------------------------------------------------------------------------------------------------------

//#include <vcl.h>                                                       //CB
//#pragma hdrstop                                                        //CB

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "classTopmap.h"

//------------------------------------------------------------------------------------------------------------------------
//#pragma package(smart_init)                                            //CB


/// <summary>
/// Initialise the map with the desired dimensions.  It is assumed that the input to the classifier will be a two 
/// dimensional image, but if you wish to have only one dimensional input just set InputsHeight to 1.
/// </summary>
/// <param name="InputsWidth">Width of the input image to be classified</param>
/// <param name="InputsHeight">Height of the input image to be classified</param>
/// <param name="mapWidth">Width of the topological map</param>
/// <param name="mapHeight">Height of the topological map</param>
Ttopmap::Ttopmap(int inputsWidth, int inputsHeight, int mapWidth, int mapHeight)
{
  int x,y,xx,yy,i;

  max_hits=0;
  inputs_width = inputsWidth;
  inputs_height = inputsHeight;
  inputs = new unsigned char *[inputs_width];
  for (x=0;x<inputs_width;x++) inputs[x] = new unsigned char [inputs_height];

  map_width = mapWidth;
  map_height = mapHeight;

  unit = new float ***[map_width];
  image = new unsigned char ***[map_width];
  outputs = new unsigned char *[map_width];
  hits = new int *[map_width];
  classification = new unsigned char *[map_width];
  classificationMulti = new int **[map_width];
  availability = new bool *[map_width];

  for (x=0;x<map_width;x++)
  {
    unit[x] = new float **[map_height];
    image[x] = new unsigned char **[map_height];
    outputs[x] = new unsigned char [map_height];
    hits[x] = new int [map_height];
    classification[x] = new unsigned char [map_height];
	availability[x] = new bool [map_height];
    classificationMulti[x] = new int *[map_height];

    for (y=0;y<map_height;y++)
    {
	  hits[x][y]=0;
      unit[x][y] = new float *[inputs_width];
	  image[x][y] = new unsigned char *[inputs_width];
	  classificationMulti[x][y] = new int [10];

	  classification[x][y] = 0;
	  availability[x][y]=true;
      for (i=0;i<10;i++) classificationMulti[x][y][i] = 0;

      for (xx=0;xx<inputs_width;xx++)
      {
        unit[x][y][xx] = new float [inputs_height];
		image[x][y][xx] = new unsigned char [inputs_height];
        for (yy=0;yy<inputs_height;yy++)
		{
		  unit[x][y][xx][yy] = 0;
		  image[x][y][xx][yy] = 0;
		}
      }
    }
  }


  max_hits=0;
  learningRate = (float)0.5;
  RadiusExcite = 1;
  randomness = (float)0.01;
  Threshold = (float)0.0;
  initWeights(0,50);
}


/// <summary>
/// Destructor
/// </summary>
Ttopmap::~Ttopmap()
{
}


/// <summary>
/// Random number generator
/// </summary>
/// <returns>A random number in the range 0.0 - 1.0</returns>
float Ttopmap::Rnd()
{
  return(rand()/(float)RAND_MAX);
}


/// <summary>
/// Sets a colour to be assigned to the given class index.  At present this function is not used within the facememory program
/// </summary>
/// <param name="classificationID">Index number or personID for the classification</param>
/// <param name="Red">Red component of the desired colour</param>
/// <param name="Green">Green component of the desired colour</param>
/// <param name="Blue">Blue component of the desired colour</param>
void Ttopmap::setClassificationColour(int classificationID, int Red, int Green, int Blue)
{
  ClassificationColour[classificationID][0] = Red;
  ClassificationColour[classificationID][1] = Green;
  ClassificationColour[classificationID][2] = Blue;
}


/// <summary>
/// Returns a colour for the given class index.  At present this function is not used within the facememory program
/// </summary>
/// <param name="classificationID">Index number or personID for the classification</param>
/// <returns>A long integer value corresponding to the colour</returns>
/// <param name="Red">The returned red component of the desired colour</param>
/// <param name="Green">The returned green component of the desired colour</param>
/// <param name="Blue">The returned blue component of the desired colour</param>
void Ttopmap::getClassificationColour(int classificationID, int &Red, int &Green, int &Blue)
{
  Red = ClassificationColour[classificationID][0];
  Green = ClassificationColour[classificationID][1];
  Blue = ClassificationColour[classificationID][2];
}


/// <summary>
/// Sets the value of an input in the range 0.0 - 1.0
/// </summary>
/// <param name="x">X coordinate of the input unit</param>
/// <param name="y">Y coordinate of the input unit</param>
/// <param name="Value">Value to be assigned to the input</param>
void Ttopmap::setInput(int x, int y, unsigned char value)
{
  inputs[x][y] = value;
}


/// <summary>
/// Returns the value of an output
/// </summary>
/// <param name="x">X coordinate on the topological map</param>
/// <param name="y">Y coordinate on the topological map</param>
/// <returns>Value of the output at the given topological map coordinates</returns>
float Ttopmap::getOutput(int x, int y)
{
  return(outputs[x][y]);
}


/// <summary>
/// Returns a value indicating how similar the topological map unit is to the current inputs. 
/// Smaller values indicate greater similarity
/// </summary>
/// <param name="x">X coordinate on the topological map</param>
/// <param name="y">Y coordinate on the topological map</param>
/// <returns>Similarity value</returns>
float Ttopmap::similarity(int x, int y)
{
  int xx,yy;
  float value;
  float dv;

  value = 0;
  for (xx=0;xx<inputs_width;xx++)
  {
    for (yy=0;yy<inputs_width;yy++)
    {
      dv = (float)fabs(unit[x][y][xx][yy] - inputs[xx][yy]);
      value += dv;
    }
  }
  value/=255;
  return(((1 - randomness) * (value / (inputs_width*inputs_height))) + (randomness * Rnd()));
}


/// <summary>
/// Re-scale the hit scores if they get too big.  This function is presently not used
/// </summary>
void Ttopmap::rescaleHits()
{
  int x,y;
  long h;

  for (x=0;x<map_width;x++)
  {
    for (y=0;y<map_height;y++)
	{
	  h = hits[x][y];
	  h = (h * 1000) / 32000;
	  hits[x][y] = (int)h;
	}
  }
}




/// <summary>
/// Main update routine for the topological map.  This should be called after 
/// all current inputs have been assigned using the setInput method
/// </summary>
void Ttopmap::update()
{
  int x,y,least_hits;
  long n=0;
  float value;
  float min;

  WinnerX = -1;
  WinnerY = -1;
  min = 9999;
  average_similarity=0;
  for (x = 0;x<map_width;x++)
  {
    for (y = 0;y<map_height;y++)
    {
	  if (availability[x][y])
	  {
        value = similarity(x, y);
	    average_similarity += value;

        if (value < Threshold)
		{
          if (value < min)
		  {
            min = value;
            WinnerX = x;
            WinnerY = y;
		  }
          outputs[x][y] = (unsigned char)(((1 - value) * (1 - value))*255);
		}
        else
        outputs[x][y] = 0;
	  }
      else
      outputs[x][y] = 0;
    }
  }

  //calculate the average similarity across the map
  average_similarity /= map_width * map_height;

  //update hit score
  if (WinnerX>-1)
  {
    hits[WinnerX][WinnerY]++;
	if (hits[WinnerX][WinnerY] > max_hits)
	{
	  max_hits = hits[WinnerX][WinnerY];
	  if (max_hits > 32000) rescaleHits();
	}
  }
  else
  {
	
    //find the least used unit
	least_hits = max_hits+1;
    for (x = 0;x<map_width;x++)
	{
      for (y = 0;y<map_height;y++)
	  {
		if ((hits[x][y]<least_hits) && (classification[x][y]==0))
		{
		  least_hits = hits[x][y];
		  WinnerX = x;
		  WinnerY = y;
		}
	  }
	}

    hits[WinnerX][WinnerY]++;
	if (hits[WinnerX][WinnerY] > max_hits)
	{
	  max_hits = hits[WinnerX][WinnerY];
	  if (max_hits > 32000) rescaleHits();
	}
	
  }
}


/// <summary>
/// Returns the squared two dimensional distance between the given topological map coordinate and the winning unit
/// </summary>
/// <param name="x">X coordinate on the topological map</param>
/// <param name="y">Y coordinate on the topological map</param>
/// <returns>Distance</returns>
float Ttopmap::Dist(int x, int y)
{
  return((float)(((x - WinnerX) * (x - WinnerX)) + ((y - WinnerY) * (y - WinnerY))));
}


/// <summary>
/// Takes a value and adds a little random noise to it
/// </summary>
/// <param name="Value">The value to be made noisy</param>
/// <returns>A noisy value</returns>
float Ttopmap::randVal(float value)
{
  return(((1 - randomness) * value) + (randomness * Rnd()));
}


/// <summary>
/// Adjusts weights of the unit in accordance with its value
/// </summary>
/// <param name="x">X coordinate on the topological map</param>
/// <param name="y">Y coordinate on the topological map</param>
void Ttopmap::adjustWeights(int x, int y, float value)
{
  int xx,yy;
  float dw;
  float w;

  if (classification[x][y]==0) //if the unit is unclassified then adjust its weights
  {
    for (xx = 0;xx<inputs_width;xx++)
	{
      for (yy = 0;yy<inputs_height;yy++)
	  {
        w = unit[x][y][xx][yy];
        dw = inputs[xx][yy] - w;
        w += (learningRate * dw * (float)exp(-value * value));
        unit[x][y][xx][yy] = w;
	  }
    }
  }
}


/// <summary>
/// Perform learning (weight adjustments) for all units on the topological map
/// </summary>
void Ttopmap::learn()
{
  int x;
  int y,i;
  float d;
  float value;
  float max;


  //store the current input image on the winning unit
  if (WinnerX>-1)
  {

    max = (float)(RadiusExcite * RadiusExcite);
    for (x = (WinnerX - RadiusExcite);x<=(WinnerX + RadiusExcite);x++)
	{
      for (y = (WinnerY - RadiusExcite);y<=(WinnerY + RadiusExcite);y++)
	  {
        if ((x >= 0) && (x < map_width) && (y >= 0) && (y < map_height))
		{
          d = Dist(x, y) / (2 * max);
          if (d < 1)
		  {
            value = randVal(d);
            adjustWeights(x, y, value);

            if (!((x == WinnerX) && (y == WinnerY)))
			{
              for (i=0;i<10;i++)
                classificationMulti[x][y][i] = classificationMulti[x][y][i] + (int)((classificationMulti[WinnerX][WinnerY][i] - classificationMulti[x][y][i]) * (1 - d) * learningRate);
			}

		  }
		}
	  }
	}

    for (x=0;x<inputs_width;x++)
      for (y=0;y<inputs_height;y++)
	    image[WinnerX][WinnerY][x][y] = inputs[x][y];
  }

  //update the threshold based upon the average similarity
  Threshold = (Threshold + (average_similarity*10)) / 2;
}


/// <summary>
/// Create some initial random weights in the given range
/// </summary>
/// <param name="minVal">Minimum weight value</param>
/// <param name="maxVal">Maximum weight value</param>
void Ttopmap::initWeights(float minVal, float maxVal)
{
  int x,y,xx,yy;

  for (x=0;x<map_width;x++)
    {
    for (y=0;y<map_height;y++)
      {
      for (xx=0;xx<inputs_width;xx++)
        {
        for (yy=0;yy<inputs_height;yy++)
          unit[x][y][xx][yy] = minVal + (Rnd() * (maxVal - minVal));
        }
      }
    }
}





/// <summary>
/// Greates a random input image, used for testing purposes
/// </summary>
void Ttopmap::randomInputs()
{
  int x,y;

  for (x = 0;x<inputs_width;x++)
    {
    for (y = 0;y<inputs_height;y++)
      {
      inputs[x][y] = (unsigned char)(Rnd()*255);
      }
    }
}




/// <summary>
/// Sets the classification (personID) for the current winning unit on the topological map
/// </summary>
/// <param name="classificationID">Classification index or personID</param>
void Ttopmap::setClassification(int classificationID)
{
  if (WinnerX > -1) classification[WinnerX][WinnerY] = classificationID;
}



/// <summary>
/// Returns the classification index (or personID) of the given topological map unit
/// </summary>
/// <param name="x">X coordinate on the topological map</param>
/// <param name="y">Y coordinate on the topological map</param>
/// <returns>Classification index or personID</returns>
int Ttopmap::getClassification(int x, int y)
{
  return(classification[x][y]);
}



/// <summary>
/// Returns the geographically nearest classification (or personID) for the given topological map coordinates
/// </summary>
/// <param name="x">X coordinate on the topological map</param>
/// <param name="y">Y coordinate on the topological map</param>
/// <returns>Classification index or personID</returns>
int Ttopmap::getNearestClassification(int x, int y)
{
  int xx,yy,classID=0,dx,dy;
  float dist,minDist;

  if (x>-1)
  {
    classID = classification[x][y];

    //if this unit is unclassified then return the nearest classification on the map
    if (classID==0)
	{
	  minDist=9999;
      for (xx=0;xx<map_width;xx++)
	  {
        for (yy=0;yy<map_height;yy++)
		{
	      if (classification[xx][yy]>0)
		  {
            dx = xx - x;
		    dy = yy - y;
		    dist = (float)sqrt((dx*dx)+(dy*dy));
		    if (dist<minDist)
			{
		      minDist = dist;
              classID = classification[xx][yy];
			}
		  }
		}
	  }
	}
  }

  return(classID);
}





/// <summary>
/// Classify the given image using the given classification index (or personID)
/// </summary>
/// <param name="classificationID">The classification index or personID to be assigned to the winning unit</param>
void Ttopmap::classify(int classificationID)
{
  update();
  setClassification(classificationID);
  learn();
}


/// <summary>
/// Sets the multi-dimensional classification value of the winning unit
/// </summary>
/// <param name="classificationIndex">Classification index or personID</param>
/// <param name="Value">Value to be assigned</param>
void Ttopmap::setClassificationMulti(int classificationIndex, int value)
{
  if (WinnerX > -1)
    classificationMulti[WinnerX][WinnerY][classificationIndex] = value;
}


/// <summary>
/// Returns the multi-dimensional classification of the given topological map unit
/// </summary>
/// <param name="x">X coordinate on the topological map</param>
/// <param name="y">Y coordinate on the topological map</param>
/// <param name="classificationIndex">Dimension index for the classification</param>
/// <returns>Classification index or personID</returns>
int Ttopmap::getClassificationMulti(int x, int y, int classificationIndex)
{
  return(classificationMulti[x][y][classificationIndex]);
}

/// <summary>
/// Save the topological map data
/// </summary>
/// <param name="filename">Filename for the data</param>
void Ttopmap::save(char *filename)
{
  int x,y,xx,yy,i,j;
  FILE *fp;
 
  fp = fopen(filename, "w");

  if (fp != NULL)
  {
	//save dimensions of the image
    fprintf(fp,"%d\n",map_width);
    fprintf(fp,"%d\n",map_height);
    fprintf(fp,"%d\n",inputs_width);
    fprintf(fp,"%d\n",inputs_height);

     for (x=0;x<map_width;x++)
	 {
       for (y=0;y<map_height;y++)
	   {
         for (xx=0;xx<inputs_width;xx++)
		 {
           for (yy=0;yy<inputs_height;yy++)
		   {
             fprintf(fp,"%f\n",unit[x][y][xx][yy]);
			 fprintf(fp,"%d\n",image[x][y][xx][yy]);
		   }
		 }

		 fprintf(fp,"%d\n",hits[x][y]);

		 fprintf(fp,"%d\n",classification[x][y]);
         for (i=0;i<10;i++)
           fprintf(fp,"%d\n",classificationMulti[x][y][i]);
	   }
	 }

     for (i=0;i<20;i++)
	 {
       for (j=0;j<20;j++)
		 fprintf(fp,"%d\n",classificationName[i][j]);
	 }

     for (i=0;i<20;i++)
	 {
       for (j=0;j<150;j++)
		 fprintf(fp,"%d\n",portrait[i][j]);
	 }

	fclose(fp);
  }

}


/// <summary>
/// Load the topological map data
/// </summary>
/// <param name="filename">Filename for the data</param>
void Ttopmap::load(char *filename)
{
  int x,y,xx,yy,i,j,v;
  float n;
  char c;
  FILE *fp;
 
  fp = fopen(filename, "r");

  if (fp != NULL)
  {
	//save dimensions of the image
    fscanf(fp,"%d\n",&map_width);
    fscanf(fp,"%d\n",&map_height);
    fscanf(fp,"%d\n",&inputs_width);
    fscanf(fp,"%d\n",&inputs_height);

     max_hits=0;
     for (x=0;x<map_width;x++)
	 {
       for (y=0;y<map_height;y++)
	   {
         for (xx=0;xx<inputs_width;xx++)
		 {
           for (yy=0;yy<inputs_height;yy++)
		   {
             fscanf(fp,"%f\n",&n);
			 unit[x][y][xx][yy] = n;
             fscanf(fp,"%d\n",&v);
			 image[x][y][xx][yy] = v;
		   }
		 }

		 fscanf(fp,"%d\n",&v);
		 hits[x][y] = v;
		 if (v>max_hits) max_hits = v;

         fscanf(fp,"%d\n",&v);
         classification[x][y]=v;
         for (i=0;i<10;i++)
		 {
           fscanf(fp,"%d\n",&v);
		   classificationMulti[x][y][i]=v;
		 }
	   }
	 }
	 
     for (i=0;i<20;i++)
	 {
       for (j=0;j<20;j++)
	   {
		 fscanf(fp,"%d\n",&c);
	     classificationName[i][j] = c;
	   }
	 }


     for (i=0;i<20;i++)
	 {
       for (j=0;j<150;j++)
	   {
		 fscanf(fp,"%d\n",&c);
	     portrait[i][j] = c;
	   }
	 }

	fclose(fp);
  }

}


/// <summary>
/// Returns the name for the given classification ID
/// </summary>
/// <param name="classID">Classification index or personID</param>
/// <returns>Name of the class or person</returns>
void Ttopmap::getClassificationName(int classID, char *className)
{
  int i;
  
  for (i = 0;i<17;i++)
    className[i] = classificationName[classID][i];
}


/// <summary>
/// Sets a name for the given classification ID
/// </summary>
/// <param name="classID">Classification index or personID</param>
/// <param name="className">Name of the class (person name)</param>
void Ttopmap::setClassificationName(int classID, char *className)
{
  int i;
  
  for (i = 0;i<17;i++)
    classificationName[classID][i] = className[i];
}


/// <summary>
/// Returns a filename associated with the given class index.  In this case the class index 
/// is the same as the personID - just a unique number corresponding to a particular classification.
/// </summary>
/// <param name="classID">Index number for the classification</param>
/// <param name="filename">Returned filename of the image</param>
void Ttopmap::getPortrait(int classID, char *filename)
{
  int i;
  
  for (i = 0;i<150;i++)
    filename[i] = portrait[classID][i];
}


/// <summary>
/// Stores a filename associated with the given class index.  In this case the class index 
/// is the same as the personID - just a unique number corresponding to a particular classification.
/// </summary>
/// <param name="classID">Index number for the classification</param>
/// <param name="filename">An image filename associated with the classification</param>
void Ttopmap::setPortrait(int classID, char *filename)
{
  int i;
  
  for (i = 0;i<150;i++)
    portrait[classID][i] = filename[i];
}

