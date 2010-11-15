/***************************************************************************
                          classHumunculus.cpp

  Humunculus - the little man inside your head

  Comments are included in machine readable XML format, for possible future 
  production of program documentation

    begin                : Mon Jan 26 2004
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
//#include <stdio.h>
//#include <math.h>
//#include "classHumunculus.h"

//size of the image used to analyse features of the face
#define FACE_IMAGE_SIZE       60

#define DETECTED_EYE_WIDTH    40
#define DETECTED_EYE_HEIGHT   40

#define DETECTED_NOSE_WIDTH   10
#define DETECTED_NOSE_HEIGHT  20

#define DETECTED_MOUTH_WIDTH  20
#define DETECTED_MOUTH_HEIGHT 15

//face classifier dimensions
#define FACE_CLASSIFIER_WIDTH 20
#define FACE_CLASSIFIER_HEIGHT 17


/// <summary>
/// Constructor
/// </summary>
classHumunculus::classHumunculus()
{
  personID=-1;

  //allow recognition of specific individuals
  FaceRecognition = true;

  //default body position - arms down at sides
  ShoulderElevation[0] = 100;
  ShoulderElevation[1] = 100;
  ElbowAngle[0] = 0;
  ElbowAngle[1] = 0;
  prevShoulderElevation[0] = ShoulderElevation[0];
  prevShoulderElevation[1] = ShoulderElevation[1];
  prevElbowAngle[0] = ElbowAngle[0];
  prevElbowAngle[1] = ElbowAngle[1];
  prevHandSize[0]=0;
  prevHandSize[1]=0;
  prev_gaze_direction=0;
  prev_head_tilt=0;
  prev_mouth_Open=0;
  bodyWidth=0;
  face_x=0;
  face_y=0;
  gazeDirection=0;
  headTilt=0;
  invalid_face=false;
  mouthWidth=0;
  mouthOpen=0;

  //define default body colour (black)
  bodyColour[0] = 45;
  bodyColour[1] = 45;
  bodyColour[2] = 45;
  bodyColourTollerance = 45;

  //default background colour (white)
  backgroundColour[0]=255;
  backgroundColour[1]=255;
  backgroundColour[2]=255;

  //define images used for analysis of the face
  face = new classimage;
  face->createImage(FACE_IMAGE_SIZE,FACE_IMAGE_SIZE);
  normalisedFace = NULL;
  imgEye[0] = new classimage;
  imgEye[0]->createImage(DETECTED_EYE_WIDTH,DETECTED_EYE_HEIGHT);
  imgEye[1] = new classimage;
  imgEye[1]->createImage(DETECTED_EYE_WIDTH,DETECTED_EYE_HEIGHT);
  imgNose = new classimage;
  imgNose->createImage(DETECTED_NOSE_WIDTH,DETECTED_NOSE_HEIGHT);
  imgMouth = new classimage;
  imgMouth->createImage(DETECTED_MOUTH_WIDTH,DETECTED_MOUTH_HEIGHT);

  //initialise face classifier
  classifier = new Ttopmap(FACE_IMAGE_SIZE/2,FACE_IMAGE_SIZE/2,FACE_CLASSIFIER_WIDTH,FACE_CLASSIFIER_HEIGHT);
  //load("c:\\develop\\hbp\\faces.net");
  classifier->learningRate = 0.5f;
  enableLearning=true;
}

/// <summary>
/// Destructor
/// </summary>
classHumunculus::~classHumunculus()
{
  delete face;
  delete normalisedFace;
  delete imgEye[0];
  delete imgEye[1];
  delete imgNose;
  delete imgMouth;

  //save("c:\\develop\\hbp\\faces.net");
  delete classifier;
}


/// <summary>
/// save the classifier data
/// </summary>
/// <param name="filename">File name to save learned faces to</param>
void classHumunculus::save(char *filename)
{
  //if (enableLearning) classifier->save("c:\\develop\\hbp\\faces.net");
  if (enableLearning) classifier->save(filename);
}


/// <summary>
/// load the classifier data
/// </summary>
/// <param name="filename">File name to load learned faces from</param>
void classHumunculus::load(char *filename)
{
  //classifier->load("c:\\develop\\hbp\\faces.net");
  classifier->load(filename);
  classifier->learningRate = 0.5f;
}


/// <summary>
/// detect facial features using the given image and face coordinates
/// </summary>
/// <param name="img">image object containing the face</param>
/// <param name="face_tx">The top left x coordinate of the detected face within the image object</param>
/// <param name="face_ty">The top left y coordinate of the detected face within the image object</param>
/// <param name="face_bx">The bottom right x coordinate of the detected face within the image object</param>
/// <param name="face_by">The bottom right y coordinate of the detected face within the image object</param>
void classHumunculus::analyseFace(classimage *img, int face_tx, int face_ty, int face_bx, int face_by)
{
  int x,y,c,xx,yy,w,h;
  int lateral_symetry,lefteye_x,righteye_x,lefteye_y,righteye_y,mouth_y,mouth_width;

  //dimensions of the face within the original image
  w = face_bx - face_tx;
  h = face_by - face_ty;

  //dump the face region into a separate image of fixed dimensions
  for (x=0;x<FACE_IMAGE_SIZE;x++)
  {
	xx = face_tx + ((x * w)/ FACE_IMAGE_SIZE);
    for (y=0;y<FACE_IMAGE_SIZE;y++)
	{
	  yy = face_ty + ((y * h) / FACE_IMAGE_SIZE);
	  for (c=0;c<3;c++)
	    face->image[x][y][c] = img->image[xx][yy][c];
	}
  }
  //face->updateIntegralImage();

  //detect the positions of features within the face image
  detectFeatures(lateral_symetry, lefteye_x, righteye_x, lefteye_y, righteye_y, mouth_y, mouth_width);
  detectKeypoints(lateral_symetry, lefteye_x, righteye_x, lefteye_y, righteye_y, mouth_y, mouth_width);

}



/// <summary>
/// Detect the locations of facial features
/// </summary>
/// <param name="lateral_symetry">Returned x coordinate for the lateral symetry of the face</param>
/// <param name="lefteye_x">Returned x coordinate for the centre of the left eye</param>
/// <param name="righteye_x">Returned x coordinate for the centre of the right eye</param>
/// <param name="lefteye_y">Returned y coordinate for the centre of the left eye</param>
/// <param name="righteye_y">Returned y coordinate for the centre of the right eye</param>
/// <param name="mouth_y">Returned y coordinate for the centre of the mouth</param>
/// <param name="mouth_width">Returned x width of the mouth in pixels</param>
void classHumunculus::detectFeatures(int &lateral_symetry, int &lefteye_x, int &righteye_x, int &lefteye_y, int &righteye_y, int &mouth_y, int &mouth_width)
{ 
  int x,y,halfwidth,quarterwidth,symetry_x;
  int halfheight,quarterheight,eyes_separation,eyes_centre;
  long tot,tot_w,w,xpos;
  float mouth_ratio = (float)1.4;

  //check symetry within the edges image
  halfwidth = FACE_IMAGE_SIZE/2;
  quarterwidth = halfwidth/2;
  halfheight = FACE_IMAGE_SIZE/2;
  quarterheight = halfheight/2;

  symetry_x = halfwidth;
  lateral_symetry = (symetry_x * 100) / FACE_IMAGE_SIZE;

  //search for the left eye horizontal position
  tot=0;
  tot_w=0;
  for (x=symetry_x-(FACE_IMAGE_SIZE/3);x<symetry_x;x++)
  {
	w=0;
	for (y=-2;y<3;y++)
	{
	  w += 255 - face->image[x][quarterheight+y][0];
	}

	if (w>200)
	{
	  tot_w += w;
      tot += (x * w);
	}
  }
  if (tot_w>0) tot /= tot_w; else tot=halfwidth;
  xpos=tot;
  lefteye_x = xpos; //(xpos * 100) / FACE_IMAGE_SIZE;

  //search for the left eye vertical position
  tot=0;
  tot_w=0;
  for (y=quarterheight;y<halfheight;y++)
  {
	w=0;
	for (x=-2;x<3;x++)
	  w += 255 - face->image[xpos+x][y][0];

	if (w>200)
	{
  	  tot_w += w;
      tot += y * w;
	}
  }
  if (tot_w>0) tot /= tot_w; else tot=halfheight;
  lefteye_y = tot; //(tot * 100) / FACE_IMAGE_SIZE;


  //search for the right eye horizontal position
  tot=0;
  tot_w=0;
  for (x=symetry_x;x<symetry_x+(FACE_IMAGE_SIZE/3);x++)
  {
	w=0;
	for (y=-2;y<3;y++)
	{
	  w += 255 - face->image[x][quarterheight+y][0];
	}

	if (w>200)
	{
	  tot_w += w;
      tot += (x * w);
	}
  }
  if (tot_w>0) tot /= tot_w; else tot=halfwidth;
  xpos=tot;
  righteye_x = xpos; //(xpos * 100) / FACE_IMAGE_SIZE;

  //search for the right eye vertical position
  tot=0;
  tot_w=0;
  for (y=quarterheight;y<halfheight;y++)
  {
	w=0;
	for (x=-2;x<3;x++)
	  w += 255 - face->image[xpos+x][y][0];

	if (w>200)
	{
	  tot_w += w;
      tot += y * w;
	}
  }
  if (tot_w>0) tot /= tot_w; else tot=halfheight;
  righteye_y = tot; //(tot * 100) / FACE_IMAGE_SIZE;



  //mouth vertical position
  eyes_separation = righteye_x - lefteye_x;                             //separation between the eyes
  eyes_centre = lefteye_x + (eyes_separation/2);                        //horizontal centre point between the eyes
  mouth_y = ((lefteye_y + righteye_y)/2) + (int)(eyes_separation*mouth_ratio);  //mouth vertical position found using fixed ratio
  if (mouth_y>99) mouth_y=99;
  mouth_width = (eyes_separation*120)/100;                                //mouth width is a little wider than the separation between the eyes
}


/// <summary>
/// Detects the presence of a horizontal line between the given start and end y coordinates
/// </summary>
/// <param name="start_y">The y coordinate from which to begin the search</param>
/// <param name="end_y">The y coordinate at which to end the search</param>
int classHumunculus::detectHorizintalLine(int start_y, int end_y)
{ 
  int x,y,winner,start_x,end_x;
  long p[2],max,diff;

  start_x = FACE_IMAGE_SIZE / 3;
  end_x = FACE_IMAGE_SIZE - (FACE_IMAGE_SIZE/3);

  winner=-1;
  max=(end_x-start_x)*10;
  for (y=start_y+2;y<end_y-3;y++)
  {
	p[0]=0;
	p[1]=0;
    for (x=start_x;x<end_x;x++)
	{
	  p[0] += face->image[x][y-2][0];
	  p[0] += face->image[x][y-1][0];
	  p[0] += face->image[x][y][0];
	  p[1] += face->image[x][y+1][0];
	  p[1] += face->image[x][y+2][0];
	  p[1] += face->image[x][y+3][0];
	}
	diff = abs(p[0] - p[1]);
	if (diff>max)
	{
	  max = diff;
	  winner = y;
	}
  }

  return(winner);
}


/// <summary>
/// Detects the presence of a vertical line between the given start and end x coordinates.
/// A bias value may be used in order to favour detections either to the left or right.  This
/// bias is typically used for detection of the left and right sides of the face.
/// </summary>
/// <param name="start_x">The x coordinate from which to begin the search</param>
/// <param name="end_x">The x coordinate at which to end the search</param>
/// <param name="leftBias">This is used in order to bias the decision either to the left or to the right</param>
/// <param name="biasValue">The value of the bias to be applied during the search</param>
int classHumunculus::detectVerticalLine(int start_x, int end_x, bool leftBias, int biasValue)
{ 
  int x,y,winner,start_y,end_y;
  long p[2],max,diff;

  //there is no need to scan the entire column, just the area around the eyes will do
  start_y = FACE_IMAGE_SIZE / 20; //6;
  end_y = FACE_IMAGE_SIZE - (FACE_IMAGE_SIZE/3);

  winner=-1;
  max=(end_y-start_y)*20;
  for (x=start_x+3;x<end_x-3;x++)
  {
	p[0]=0;
	p[1]=0;
    for (y=start_y+1;y<end_y;y++)
	{
	  p[0] += face->image[x-3][y][0];
	  p[0] += face->image[x-2][y][0];
	  p[0] += face->image[x-1][y][0];
	  p[0] += face->image[x][y][0];

	  p[0] += face->image[x-3][y-1][0];
	  p[0] += face->image[x-2][y-1][0];
	  p[0] += face->image[x-1][y-1][0];
	  p[0] += face->image[x][y-1][0];

	  p[1] += face->image[x][y][0];
	  p[1] += face->image[x+1][y][0];
	  p[1] += face->image[x+2][y][0];
	  p[1] += face->image[x+3][y][0];

	  p[1] += face->image[x][y-1][0];
	  p[1] += face->image[x+1][y-1][0];
	  p[1] += face->image[x+2][y-1][0];
	  p[1] += face->image[x+3][y-1][0];
	}
	diff = abs(p[0] - p[1]);

	//bias the difference value either to the left or right
	//this helps to ensure that the first peak value in the given search direction is detected
	if (leftBias)
	  diff *= 1 + ((x * biasValue) / (end_x-start_x));
	  else
	  diff *= 1 + (((end_x-x) * biasValue) / (end_x-start_x));

	if (diff>max)
	{
	  max = diff;
	  winner = x;
	}
  }

  return(winner);
}


/// <summary>
/// Create a normalised face image which can be used for recognition purposes
/// </summary>
/// <param name="lefteye_x">x coordinate for the centre of the left eye</param>
/// <param name="righteye_x">x coordinate for the centre of the right eye</param>
/// <param name="lefteye_y">y coordinate for the centre of the left eye</param>
/// <param name="righteye_y">y coordinate for the centre of the right eye</param>
/// <param name="leftFaceSide">x coordinate for the left side of the face</param>
/// <param name="rightFaceSide">x coordinate for the right side of the face</param>
/// <param name="eyebrows_y">y coordinate for the eyebrows</param>
void classHumunculus::updateNormalisedFace(int lefteye_x, int lefteye_y, int righteye_x, int righteye_y, int leftFaceSide, int rightFaceSide, int eyebrows_y)
{
  int x,y,c,w,h,xx,yy,yyy,facewidth,p,diff;
  int lefteye_x2,righteye_x2,top_y,bottom_y,leftSide,rightSide;
  long av_illumination=0;

  w = FACE_IMAGE_SIZE/2;
  h = FACE_IMAGE_SIZE/2;
  facewidth = rightFaceSide - leftFaceSide;

  //create the face object if necessary
  if (normalisedFace == NULL)
  {
    normalisedFace = new classimage;
    normalisedFace->createImage(w,h);
  }


  lefteye_x2 = w/4;
  righteye_x2 = w*3/4;
  top_y = eyebrows_y - (FACE_IMAGE_SIZE/8);
  if (top_y<0) top_y=0;
  bottom_y = FACE_IMAGE_SIZE-1;

  for (y=0;y<h;y++)
  {
	if (y<h*3/5)
      yy = top_y + ((y * ((bottom_y - top_y)*3/4)) / h);
	  else
	  yy = top_y + ((y * (bottom_y - top_y)) / h);

    for (x=0;x<w;x++)
	{
	  //correct for vertical differences between positions of left and right eyes
      yyy = yy + (((righteye_y - lefteye_y)*5/2*x)/w);

      leftSide = ((lefteye_x - leftFaceSide) * (h-(y*3/4))) / h;
      rightSide = ((rightFaceSide - righteye_x) * (h-(y*3/4))) / h;

	  //correct for rotation of the eyes about the vertical head axis
	  if (x<lefteye_x2)
	    xx = (lefteye_x - leftSide) + ((x * leftSide) / lefteye_x2);
	    else
	    if (x<righteye_x2)
		  xx = lefteye_x + (((x-lefteye_x2) * (righteye_x - lefteye_x)) / (righteye_x2 - lefteye_x2));
		  else
		  xx = righteye_x + (((x-righteye_x2) * rightSide) / (w - righteye_x2));
	  

	  //make sure values aren't out of range
	  if (xx>=FACE_IMAGE_SIZE) xx = FACE_IMAGE_SIZE-1;
	  if (yyy<0) yyy=0;
	  if (yyy>=FACE_IMAGE_SIZE) yyy = FACE_IMAGE_SIZE-1;

	  //update the normalised face image
	  for (c=0;c<3;c++) normalisedFace->image[x][y][c] = face->image[xx][yyy][c];
	  av_illumination += normalisedFace->image[x][y][0];
	}
  }

  //adjust for lighting
  av_illumination /= w*h;
  for (y=0;y<h;y++)
  {
    for (x=0;x<w;x++)
	{
	  for (c=0;c<3;c++)
	  {
	    p = normalisedFace->image[x][y][c];
		diff = normalisedFace->image[x][y][c] - av_illumination;
        diff = (diff * 127) / av_illumination;
		p = diff + 127;
		if (p<0) p=0;
		if (p>255) p=255;
		normalisedFace->image[x][y][c] = (unsigned char)p;
	  }
	}
  }

}


/// <summary>
/// Classify (recognise) the normalised face image and update the person ID
/// </summary>
void classHumunculus::classifyFace()
{
  int x,y,w,h;

  //dimensions of the normalised face image
  w = FACE_IMAGE_SIZE/2;
  h = FACE_IMAGE_SIZE/2;

  //feed the image into the classifier
  for (y=0;y<h;y++)
    for (x=0;x<w;x++)
	  classifier->setInput(x,y,normalisedFace->image[x][y][0]);

  //update the classifier
  classifier->update();
  if (enableLearning) classifier->learn();

  //get the ID number for the person recognised
  personID = classifier->getNearestClassification(classifier->WinnerX,classifier->WinnerY);
}



/// <summary>
/// Uses the points detected earlier with the function detectfeatures to do a more detailed
/// search for specific points on the eyes, eyebrows, nose and mouth
/// </summary>
/// <param name="lateral_symetry">x coordinate for lateral symetry of the face</param>
/// <param name="lefteye_x">x coordinate for the centre of the left eye</param>
/// <param name="righteye_x">x coordinate for the centre of the right eye</param>
/// <param name="lefteye_y">y coordinate for the centre of the left eye</param>
/// <param name="righteye_y">y coordinate for the centre of the right eye</param>
/// <param name="mouth_y">y coordinate for the centre of the mouth</param>
/// <param name="mouth_width">width of the mouth in pixels</param>
void classHumunculus::detectKeypoints(int lateral_symetry, int lefteye_x, int righteye_x, int lefteye_y, int righteye_y, int mouth_y, int mouth_width)
{ 
  int eye_width,eye_height,nose_width,nose_height,mouth_height;
  int x,y,x1,x2,y1,y2,xx,yy,lateral,lefteye,righteye,lefteyeY,righteyeY,c;
  int noseY,noseX,point_x,point_y,px;
  int eyesY[2];
  int smile,mouth_Open,minY,maxY,mouthOpenWidth;
  int pxx,rightEyeCorner[2],leftEyeCorner[2],eyeCentre[2],eyebrow_y[2];
  int threshold_mouthopen = 20; //threshold for detecting whether the mouth is open
  int threshold_smile = 20;     //threshold for detecting smiling
  int leftSide,rightSide,faceCentre,d1,d2,head_tilt,gaze_direction,eyes_separation,eyeb_y;

  lateral    = FACE_IMAGE_SIZE * lateral_symetry / 100;
  lefteye    = FACE_IMAGE_SIZE * lefteye_x / 100;
  righteye   = FACE_IMAGE_SIZE * righteye_x / 100;
  lefteyeY   = FACE_IMAGE_SIZE * lefteye_y / 100;
  righteyeY  = FACE_IMAGE_SIZE * righteye_y / 100;
  noseY      = FACE_IMAGE_SIZE / 2;
  noseX      = FACE_IMAGE_SIZE / 2;

  invalid_face = true;
  leftSide=-1;
  rightSide=-1;

  //left eye
  eye_height = (int)(FACE_IMAGE_SIZE / 2.5);
  eye_width = (int)(FACE_IMAGE_SIZE / 2.6);
  for (y=0;y<DETECTED_EYE_HEIGHT;y++)
  {
    yy = lefteye_y - (eye_height/2) + ((y * eye_height) / DETECTED_EYE_HEIGHT);
	if (yy<0) yy=0;
	if (yy>=face->height-1) yy=face->height-1;
    for (x=0;x<DETECTED_EYE_WIDTH;x++)
	{
      xx = lefteye_x - (eye_width/2) + ((x * eye_width) / DETECTED_EYE_WIDTH);
	  if (xx<0) xx=0;
	  if (xx>=face->width-1) xx=face->width-1;
      
	  for (c=0;c<3;c++)
        imgEye[0]->image[x][y][c] = face->image[xx][yy][c];
	}
  }

    
  //find the centre of the left eye
  imgEye[0]->CG((DETECTED_EYE_WIDTH/8), DETECTED_EYE_HEIGHT / 2, DETECTED_EYE_WIDTH - (DETECTED_EYE_WIDTH/8), DETECTED_EYE_HEIGHT - 1, 0, 0, 0, point_x, point_y, 255);
  eyesY[0] = point_y;
    
  //inner left eye
  px = point_x;
  imgEye[0]->CG(px, DETECTED_EYE_HEIGHT / 2, DETECTED_EYE_WIDTH - 1, DETECTED_EYE_HEIGHT - 1, 0, 0, 0, point_x, point_y, 255 * 3);
  rightEyeCorner[0] = point_x;
    
  //outer left eye
  pxx = px - 1;
  if (pxx < 0) pxx = 0;
  imgEye[0]->CG(0, DETECTED_EYE_HEIGHT / 2, pxx, DETECTED_EYE_HEIGHT - 1, 0, 0, 0, point_x, point_y, 255 * 2);
  leftEyeCorner[0] = point_x;
    
  minY = eyesY[0] - (DETECTED_EYE_HEIGHT / 8);
  maxY = eyesY[0] + (DETECTED_EYE_HEIGHT / 8);
  if (minY < 0) minY = 0;
  if (maxY >= DETECTED_EYE_HEIGHT) maxY = DETECTED_EYE_HEIGHT - 1;
    
  //left eye centre
  imgEye[0]->CG(leftEyeCorner[0], minY, rightEyeCorner[0], maxY, 0, 0, 0, point_x, point_y, 255);
  if (point_x < leftEyeCorner[0] + 1) point_x = leftEyeCorner[0] + 1;
  if (point_x > rightEyeCorner[0] - 1) point_x = rightEyeCorner[0] - 1;
  eyeCentre[0] = point_x;
    
  //left eye direction
  imgEye[0]->CG(leftEyeCorner[0], minY, rightEyeCorner[0], maxY, 255, 255, 255, point_x, point_y, 200 * 3);
    
  //centre of left eyebrow
  imgEye[0]->CG(0, 0, DETECTED_EYE_WIDTH - 1, DETECTED_EYE_HEIGHT / 2, 0, 0, 0, point_x, point_y, 255);
  eyebrow_y[0] = point_y;
    
  //right eye
  for (y=0;y<DETECTED_EYE_HEIGHT;y++)
  {
    yy = righteye_y - (eye_height/2) + ((y * eye_height) / DETECTED_EYE_HEIGHT);
	if (yy<0) yy=0;
	if (yy>=face->height-1) yy=face->height-1;
    for (x=0;x<DETECTED_EYE_WIDTH;x++)
	{
      xx = righteye_x - (eye_width/2) + ((x * eye_width) / DETECTED_EYE_WIDTH);
	  if (xx<0) xx=0;
	  if (xx>=face->width-1) xx=face->width-1;
      
	  for (c=0;c<3;c++)
        imgEye[1]->image[x][y][c] = face->image[xx][yy][c];
	}
  }

  //centre of right eye
  imgEye[1]->CG((DETECTED_EYE_WIDTH/8), DETECTED_EYE_HEIGHT / 2, DETECTED_EYE_WIDTH - (DETECTED_EYE_WIDTH/8), DETECTED_EYE_HEIGHT - 1, 0, 0, 0, point_x, point_y, 255);
  eyesY[1] = point_y;
    
  //inner right eye
  px = point_x;
  imgEye[1]->CG(0, DETECTED_EYE_HEIGHT / 2, px, DETECTED_EYE_HEIGHT - 1, 0, 0, 0, point_x, point_y, 255 * 3);
  leftEyeCorner[1] = point_x;
    
  //outer right eye
  pxx = px + 1;
  if (pxx >= DETECTED_EYE_WIDTH) pxx = DETECTED_EYE_WIDTH - 1;
  imgEye[1]->CG(pxx, DETECTED_EYE_HEIGHT / 2, DETECTED_EYE_WIDTH - 1, DETECTED_EYE_HEIGHT - 1, 0, 0, 0, point_x, point_y, 255 * 2);
  rightEyeCorner[1] = point_x;
    
  minY = eyesY[1] - (DETECTED_EYE_HEIGHT / 8);
  maxY = eyesY[1] + (DETECTED_EYE_HEIGHT / 8);
  if (minY < 0) minY = 0;
  if (maxY >= DETECTED_EYE_HEIGHT) maxY = DETECTED_EYE_HEIGHT - 1;
    
  //centre of right eye
  imgEye[1]->CG(leftEyeCorner[1], minY, rightEyeCorner[1], maxY, 0, 0, 0, point_x, point_y, 255);
  //if (point_x == 0) point_x = (eyeCentre[0] - leftEyeCorner[0]) + leftEyeCorner[1];
  if (point_x < leftEyeCorner[1] + 1) point_x = leftEyeCorner[1] + 1;
  if (point_x > rightEyeCorner[1] - 1) point_x = rightEyeCorner[1] - 1;
  eyeCentre[1] = point_x;
    
  //right eye direction
  imgEye[1]->CG(leftEyeCorner[1], minY, rightEyeCorner[1], maxY, 255, 255, 255, point_x, point_y, 200 * 3);
        
  //centre of right eyebrow
  imgEye[1]->CG(0, 0, DETECTED_EYE_WIDTH - 1, DETECTED_EYE_HEIGHT / 2, 0, 0, 0, point_x, point_y, 255);
  eyebrow_y[1] = point_y;

  
  //nose
  nose_height = FACE_IMAGE_SIZE / 5;
  nose_width = FACE_IMAGE_SIZE / 6;
  for (y=0;y<DETECTED_NOSE_HEIGHT;y++)
  {
    yy = noseY - (nose_height/2) + ((y * nose_height) / DETECTED_NOSE_HEIGHT);
	if (yy<0) yy=0;
	if (yy>=face->height-1) yy=face->height-1;
    for (x=0;x<DETECTED_NOSE_WIDTH;x++)
	{
      xx = noseX - (nose_width/2) + ((x * nose_width) / DETECTED_NOSE_WIDTH);
	  if (xx<0) xx=0;
	  if (xx>=face->width-1) xx=face->width-1;
      
	  for (c=0;c<3;c++)
        imgNose->image[x][y][c] = face->image[xx][yy][c];
	}
  }

  //nose tip
  imgNose->CG(0, DETECTED_NOSE_HEIGHT / 2, DETECTED_NOSE_WIDTH - 1, DETECTED_NOSE_HEIGHT - 1, 0, 0, 0, point_x, point_y, 255);
  
  //mouth
  mouth_height = FACE_IMAGE_SIZE / 3;
  for (y=0;y<DETECTED_MOUTH_HEIGHT;y++)
  {
    yy = mouth_y - (mouth_height/2) + ((y * mouth_height) / DETECTED_MOUTH_HEIGHT);
	if (yy<0) yy=0;
	if (yy>=face->height-1) yy=face->height-1;
    for (x=0;x<DETECTED_MOUTH_WIDTH;x++)
	{
      xx = lateral - (mouth_width/2) + ((x * mouth_width) / DETECTED_MOUTH_WIDTH);
	  if (xx<0) xx=0;
	  if (xx>=face->width-1) xx=face->width-1;
      
	  for (c=0;c<3;c++)
        imgMouth->image[x][y][c] = face->image[xx][yy][c];
	}
  }

  
  mouth_Open = imgMouth->relativeThreshold(threshold_mouthopen,DETECTED_MOUTH_WIDTH/4,0,(DETECTED_MOUTH_WIDTH*3)/4,DETECTED_MOUTH_HEIGHT,mouthOpenWidth);
  if (mouth_Open>10)
  {
    smile = 0;	
  }
  else
  {
	smile = imgMouth->relativeThreshold(threshold_smile,DETECTED_MOUTH_WIDTH/4,0,DETECTED_MOUTH_WIDTH/2,DETECTED_MOUTH_HEIGHT,mouthOpenWidth);
	smile += imgMouth->relativeThreshold(threshold_smile,DETECTED_MOUTH_WIDTH*3/4,0,DETECTED_MOUTH_WIDTH,DETECTED_MOUTH_HEIGHT,mouthOpenWidth);
	smile -= mouth_Open/2;
	mouth_Open = 0;
	mouthOpenWidth = 0;
	if (smile<4) smile=0;
  }


  
  //show pupils
  x1 = lefteye_x + (((eyeCentre[0] - (DETECTED_EYE_WIDTH/2)) * eye_width) / DETECTED_EYE_WIDTH);
  y1 = lefteye_y + (((eyesY[0] - (DETECTED_EYE_HEIGHT/2)) * eye_height) / DETECTED_EYE_HEIGHT);
  x2 = righteye_x + (((eyeCentre[1] - (DETECTED_EYE_WIDTH/2)) * eye_width) / DETECTED_EYE_WIDTH);
  y2 = righteye_y + (((eyesY[1] - (DETECTED_EYE_HEIGHT/2)) * eye_height) / DETECTED_EYE_HEIGHT);

  if ((x1>(FACE_IMAGE_SIZE/5)) && (x2<FACE_IMAGE_SIZE-(FACE_IMAGE_SIZE/5)))
  {
	eyes_separation = x2 - x1;
	if (eyes_separation>0)
	{
	  //calculate the head tilt from side to side using the vertical positions of the eyes
	  head_tilt = ((y1 - y2)*100)/(x2 - x1);

      //detect the left and right sides of the face
      leftSide = detectVerticalLine(0,x1,true,5);
	  if (leftSide==-1) leftSide=0;
      rightSide = detectVerticalLine(x2,FACE_IMAGE_SIZE,false,5);
	  if (rightSide==-1) rightSide=FACE_IMAGE_SIZE-1;
	  if (leftSide>x1-2) leftSide = x1-2;
	  if (leftSide<0) leftSide=0;
	  if (rightSide<x2+2) rightSide = x2+2;
	  if (rightSide>=FACE_IMAGE_SIZE) rightSide=FACE_IMAGE_SIZE-1;

	  //calculate the apparent centre position of the face
	  faceCentre = leftSide + ((rightSide - leftSide)/2);

	  //calculate the distance of left and right eyes from the centre of the face
	  //in order to estimate gaze direction
	  d1 = x1 - faceCentre;
	  d2 = faceCentre - x2;
      gaze_direction = ((d1-d2) * 500) / (rightSide - leftSide);
	  if (gaze_direction>100) gaze_direction=100;
	  if (gaze_direction<-100) gaze_direction=-100;
	}
	else
	{
	  //gazeDirection = 0;
	  head_tilt = 0;
	}

    
	//if the geometry of the detected features doesn't make sense then assume it isn't a face
	if ((abs(head_tilt)<40) && (eyes_separation>FACE_IMAGE_SIZE/6) && (eyes_separation<FACE_IMAGE_SIZE*2/3)) 
	{

      eyeb_y = lefteye_y + (((eyebrow_y[0] - (DETECTED_EYE_HEIGHT/2)) * eye_height) / DETECTED_EYE_HEIGHT);
      eyeb_y += righteye_y + (((eyebrow_y[1] - (DETECTED_EYE_HEIGHT/2)) * eye_height) / DETECTED_EYE_HEIGHT);
      eyeb_y += y1 + y2;
      eyeb_y /= 4;

      if (FaceRecognition)
	  {
		//perform operations necessary for recognising specific individuals
	    updateNormalisedFace(x1,y1,x2,y2,leftSide,rightSide,eyeb_y);
		classifyFace();
	  }

      //detect the mouth
	  mouth_y = detectVerticalLine(FACE_IMAGE_SIZE/2,FACE_IMAGE_SIZE-4,false,0);
	  if (mouth_y>-1) face->DrawBox(FACE_IMAGE_SIZE/2,mouth_y,FACE_IMAGE_SIZE/8,1,90);

	  //show eyes
      face->DrawBox(x1,y1,FACE_IMAGE_SIZE/10,FACE_IMAGE_SIZE/10,90);
      face->DrawBox(x2,y2,FACE_IMAGE_SIZE/10,FACE_IMAGE_SIZE/10,90);

      //show mouth open
      if (mouth_Open>0)
        face->DrawBox(lateral,mouth_y,mouth_width,(FACE_IMAGE_SIZE/4)*mouth_Open/DETECTED_MOUTH_HEIGHT,90);

      //show inner corners
      x = lefteye_x + (((rightEyeCorner[0] - (DETECTED_EYE_WIDTH/2)) * eye_width) / DETECTED_EYE_WIDTH);
      y = lefteye_y + (((eyesY[0] - (DETECTED_EYE_HEIGHT/2)) * eye_height) / DETECTED_EYE_HEIGHT);
      face->DrawBox(x,y,1,FACE_IMAGE_SIZE/15,90);
      x = righteye_x + (((leftEyeCorner[1] - (DETECTED_EYE_WIDTH/2)) * eye_width) / DETECTED_EYE_WIDTH);
      y = righteye_y + (((eyesY[1] - (DETECTED_EYE_HEIGHT/2)) * eye_height) / DETECTED_EYE_HEIGHT);
      face->DrawBox(x,y,1,FACE_IMAGE_SIZE/15,90);

      //show outer corners
      x = lefteye_x + (((leftEyeCorner[0] - (DETECTED_EYE_WIDTH/2)) * eye_width) / DETECTED_EYE_WIDTH);
      y = lefteye_y + (((eyesY[0] - (DETECTED_EYE_HEIGHT/2)) * eye_height) / DETECTED_EYE_HEIGHT);
      face->DrawBox(x,y,1,FACE_IMAGE_SIZE/15,90);
      x = righteye_x + (((rightEyeCorner[1] - (DETECTED_EYE_WIDTH/2)) * eye_width) / DETECTED_EYE_WIDTH);
      y = righteye_y + (((eyesY[1] - (DETECTED_EYE_HEIGHT/2)) * eye_height) / DETECTED_EYE_HEIGHT);
      face->DrawBox(x,y,1,FACE_IMAGE_SIZE/15,90);

	  //show the left and right sides of the face
	  if (leftSide>-1) face->DrawBox(leftSide,FACE_IMAGE_SIZE/2,1,FACE_IMAGE_SIZE/4,90);
	  if (rightSide>-1) face->DrawBox(rightSide,FACE_IMAGE_SIZE/2,1,FACE_IMAGE_SIZE/4,90);

      //eyebrows
      x = lefteye_x + (((eyeCentre[0] - (DETECTED_EYE_WIDTH/2)) * eye_width) / DETECTED_EYE_WIDTH);
      y = lefteye_y + (((eyebrow_y[0] - (DETECTED_EYE_HEIGHT/2)) * eye_height) / DETECTED_EYE_HEIGHT);
      face->DrawBox(x,y,eye_width/5,1,90);
      x = righteye_x + (((eyeCentre[1] - (DETECTED_EYE_WIDTH/2)) * eye_width) / DETECTED_EYE_WIDTH);
      y = righteye_y + (((eyebrow_y[1] - (DETECTED_EYE_HEIGHT/2)) * eye_height) / DETECTED_EYE_HEIGHT);
      face->DrawBox(x,y,eye_width/5,1,90);

      //dump the results into the appropriate public properties
	  headTilt = (prev_head_tilt + head_tilt)/2;
	  prev_head_tilt = headTilt;
	  gazeDirection = (prev_gaze_direction + gaze_direction)/2;
	  prev_gaze_direction = gazeDirection;
      EyeOuter[0] = leftEyeCorner[0];
      EyeOuter[1] = rightEyeCorner[1];
      EyePupil_x[0] = eyeCentre[0];
      EyePupil_x[1] = eyeCentre[1];
      EyePupil_y[0] = eyesY[0];
      EyePupil_y[1] = eyesY[1];
      Eyebrow_y[0] = eyebrow_y[0];
      Eyebrow_y[1] = eyebrow_y[1];
      mouthOpen = (prev_mouth_Open + ((mouth_Open * 100)/DETECTED_MOUTH_HEIGHT))/2;
	  prev_mouth_Open = mouthOpen;
      mouthWidth = mouthOpenWidth;
      eyebrows_vertical = (((eyesY[1] - Eyebrow_y[1]) + (eyesY[0] - Eyebrow_y[0]))*100)/DETECTED_EYE_HEIGHT;
	  invalid_face = false;
	}
  }
}


/// <summary>
/// Returns the name for the person with the given ID
/// </summary>
/// <param name="classID">The ID number corresponding to the person</param>
/// <param name="className">The returned name of the person</param>
void classHumunculus::getPersonName(int classID, char *className)
{
  classifier->getClassificationName(classID,className);
}

/// <summary>
/// Returns the filename for an image of the person with the given ID
/// </summary>
/// <param name="classID">The ID number corresponding to the person</param>
/// <param name="filename">A returned image filename</param>
void classHumunculus::getPortrait(int classID, char *filename)
{
  classifier->getPortrait(classID,filename);
}


/// <summary>
/// Update the position of an arm.
/// There is some faffing about within this function, with the aim of eliminating
/// occasional single frame errors
/// </summary>
/// <param name="leftArm">Indicates whether this is the left or right arm</param>
/// <param name="Shoulder_Elevation">shoulder elevation in the range 0-100</param>
/// <param name="Elbow_Angle">elbow angle in the range 0-100</param>
/// <param name="hand_size">Hand size in pixels</param>
void classHumunculus::updateArmPosition(bool leftArm, int Shoulder_Elevation, int Elbow_Angle, int hand_size)
{
  int index,ds,de,dh,diff;

  if (leftArm) index=0; else index=1;
  if (hand_size==0) hand_size = bodyWidth / 4;

  ds = Shoulder_Elevation - prevShoulderElevation[index];
  de = Elbow_Angle - prevElbowAngle[index];
  dh = hand_size - prevHandSize[index];
  
  diff = abs(ds) + abs(de);
  if (diff<80)
  {
    ShoulderElevation[index] = prevShoulderElevation[index] + (ds/2);
    ElbowAngle[index] = prevElbowAngle[index] + (de/2);
	handSize[index] = prevHandSize[index] + (dh/2);
  }

  prevShoulderElevation[index] = Shoulder_Elevation;
  prevElbowAngle[index] = Elbow_Angle;
  prevHandSize[index] = hand_size;
}

