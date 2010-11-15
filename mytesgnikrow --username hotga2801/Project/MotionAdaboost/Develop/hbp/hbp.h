/***************************************************************************
                             segment.h

  Image segmentation and detection of human body forms.  The segmentation 
  code is freeware and was written by Chris M. Christoudias and Bogdan Georgescu.

    begin                : Thu Jan 01 2004
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

#if !defined(AFX_SEGMENT_H__2A4829C4_5C66_479C_B0B8_60D8A0FEB11A__INCLUDED_)
#define AFX_SEGMENT_H__2A4829C4_5C66_479C_B0B8_60D8A0FEB11A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// ChbpApp
// See segment.cpp for the implementation of this class
//


/// <summary>
/// Human Body Project main class
/// </summary>
class ChbpApp : public CWinApp
{
private:
  CvHaarClassifierCascade* load_object_detector( const char* cascade_path );
  void update(int cameraIndex, SAFEARRAY **bmp, int width, int height);
  void getMovementBox(classimage *currentImage, classimage *prevImage, 
					  int width, int height, 
					  int top_x, int top_y, int bottom_x, int bottom_y,
					  int threshold, int &tx, int &ty, int &bx, int &by);
  void matchSegments(int cameraIndex, int motion_tx, int motion_ty, int motion_bx, int motion_by);
  void calcAverageImage(classimage *inputImage, classimage *outputImage, classimage *prevImage,
						int window_width, int window_height,
						int width, int height);
  void filterImage(int cameraIndex, classimage *sourceImage);
  void filterImage_body(int cameraIndex, classimage *sourceImage, int cx, int cy);
  void filterImage_NoFace(int cameraIndex, classimage *sourceImage, int r, int g, int b, int tollerance);
  void filterImage_WithFace(int cameraIndex, classimage *sourceImage, int faceIndex);
  int findFaces(int cameraIndex);
  void estimateArmPosition(int cameraIndex, bool leftArm,int arm_tx,int arm_ty,int arm_bx,int arm_by,int shoulder_y);
  void estimateArmPositionNeural(int cameraIndex, bool leftArm, int armpixels);
  int updateArmImage(int cameraIndex, bool leftArm,int arm_tx,int arm_ty,int arm_bx,int arm_by);
  void saveArmImage(char *filename);
  void estimatePose(int cameraIndex, int faceIndex, classimage *sourceImage, bool saveArmData);
  void cleanup(classimage *sourceImage, classimage *destImage, int seed_x,int seed_y);
  void detectHands(int cameraIndex);
  void update_base(int cameraIndex, BYTE *img, int width, int height);
  int nearestFace();
  void detectSkin(int cameraIndex, bool useMotion);

public:
  void RChbp_update(int cameraIndex, SAFEARRAY **bmp, int width, int height, int &NoOfSegments);
  void RChbp_updateImage(int x, int y, int width, int height, BYTE r, BYTE g, BYTE b);
  void RChbp_update2(int cameraIndex, int width, int height);
  void RChbp_getImage(SAFEARRAY **bmp, int imageIndex);
  void RChbp_getImagePixel(int imageIndex, int x, int y, BYTE &r, BYTE &g, BYTE &b);
  void RChbp_getSegment(int cameraIndex, int index, int &tx, int &ty, int &bx, int &by, int &r, int &g, int &b, int &segmentID, int &inMotion, int &disparity);
  void RChbp_getBodyPosture(int cameraIndex, int &leftShoulderElevation, int &rightShoulderElevation, 
	                         int &leftElbowElevation, int &rightElbowElevation, 
							 int &leftHandSize, int &rightHandSize,
							 int &face_tx, int &face_ty, int &face_bx, int &face_by, 
							 int &bodyWidth, int &gazeDirection, int &headTilt, 
							 int &mouthWidth, int &mouthOpen, int &eyebrowsVertical, 
							 int &bodycolour_red, int &bodycolour_green, int &bodycolour_blue, 
							 int &background_red, int &background_green, int &background_blue,
							 int &personID, bool &invalidFace);
  void RChbp_getFaceImage(SAFEARRAY **bmp);
  void RChbp_getFaceImagePixel(int x, int y, BYTE &r, BYTE &g, BYTE &b);
  int RChbp_getNormalisedFaceImage(SAFEARRAY **bmp);
  void RChbp_getPersonIdentity(int &personID, SAFEARRAY **personName);
  int RChbp_getPersonID();
  BYTE RChbp_getPersonName(int personID, int charIndex);
  BYTE RChbp_getPortrait(int personID, int charIndex);
  void RChbp_setDetectionMode(int mode);
  void RChbp_enableLearning(bool state);
  void RChbp_getMotionBox(int &tx, int &ty, int &bx, int &by, float &flow_x, float &flow_y);
  bool RChbp_faceDetected();
  void RChbp_save(char *filename);
  void RChbp_load(char *filename);

  ChbpApp();
  ~ChbpApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ChbpApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(ChbpApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEGMENT_H__2A4829C4_5C66_479C_B0B8_60D8A0FEB11A__INCLUDED_)
