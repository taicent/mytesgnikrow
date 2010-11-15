/***************************************************************************
                          classHumunculus.cpp

  Humunculus - the little man inside your head

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


#ifndef CLASS_HUMUNCULUS_H
#define CLASS_HUMUNCULUS_H


/**
  *@author Bob Mottram
  */

#include "stdafx.h"
#include <afxwin.h>

/// <summary>
/// This class is used to classify (recognise) faces, detect facial features and gaze direction
/// </summary>
class classHumunculus {
private:
  
  classimage *imgEye[2];        //used for detection of points around the eyes
  classimage *imgNose;          //used for detection of tip of the nose
  classimage *imgMouth;         //used for detection of mouth
  Ttopmap *classifier;          //used for classifying faces

  void detectFeatures(int &lateral_symetry, int &lefteye_x, int &righteye_x, int &lefteye_y, int &righteye_y, int &mouth_y, int &mouth_width);
  void detectKeypoints(int lateral_symetry, int lefteye_x, int righteye_x, int lefteye_y, int righteye_y, int mouth_y, int mouth_width);
  int detectVerticalLine(int start_x, int end_x, bool leftBias, int biasValue);
  int detectHorizintalLine(int start_y, int end_y);
  void updateNormalisedFace(int lefteye_x, int lefteye_y, int righteye_x, int righteye_y, int leftFaceSide, int rightFaceSide, int eyebrows_y);
  void classifyFace();

public:
  bool initialised;
  bool enableLearning;

  //previous data
  int prevShoulderElevation[2]; //previous left and right shoulder elevations
  int prevElbowAngle[2];        //previous left and right elbow angles
  int prevHandSize[2];          //previous left and right hand sizes
  int prev_gaze_direction,prev_head_tilt,prev_mouth_Open;
  
  //body posture
  int gazeDirection;        //left/right gaze direction
  int face_x,face_y;        //coordinates of the face as a percentage of the image width/height
  int bodyWidth;            //body width as a percentage of the image width
  int body_Tilt;            //tilt to the left or right
  int ShoulderElevation[2]; //left and right shoulder elevations
  int ElbowAngle[2];        //left and right elbow angles
  int ArmExtension[2];      //how far the arm extends out horizontally from the body
  int bodyColour[3];        //red/green/blue average colour of the body area
  int backgroundColour[3];  //red/green/blue average colour of the background surrounding the body
  int bodyColourTollerance; //tollerance for colour-based detection of the body
  int handSize[2];          //percentage size of the left/right hands within the image 0-100

  //facial features
  int personID;             //ID number for the specific individual recognised
  bool FaceRecognition;     //enable or dissable recognition of faces belonging to specific individuals
  bool invalid_face;        //set to true if the detected facial features don't make sense
  int EyeOuter[2];          //horizontal position of the outer eye corner (towards the ear)
  int EyeInner[2];          //horizontal position of the inner eye corner (towards the nose)
  int EyePupil_x[2];        //horizintal position of the pupil
  int EyePupil_y[2];        //vertical position of the pupil
  int Eyebrow_y[2];         //vertical position of the eyebrows
  int mouthOpen;            //how open is the mouth
  int mouthWidth;           //how wide is the mouth
  int headTilt;             //tilt from side to side
  int eyebrows_vertical;    //average vertical elevation of the eyebrows

  classimage *face;          //stores the face image
  classimage *normalisedFace;//normalised version of the face image used for recognition purposes
  void analyseFace(classimage *img, int face_tx, int face_ty, int face_bx, int face_by);
  void getPersonName(int classID, char *className);
  void getPortrait(int classID, char *filename);
  void updateArmPosition(bool leftArm, int Shoulder_Elevation, int Elbow_Angle, int hand_size);
  void save(char *filename);
  void load(char *filename);

  classHumunculus();
  ~classHumunculus();
};

#endif
