/*
	Author : Binh Nguyen - Bioz
	Last Update : 21 Oct 2008	 
	License : Free
	Official site : www.sectic.com
	Description : detect face by using haar model supplied by OpenCV lib. 
*/

#pragma warning( disable : 4996 )
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include "Constant.h"
#include <stdafx.h>

// facedetect.cpp : Defines the initialization routines for the DLL.
//
#include "OpenCVFaceDetection.h"

/////////////////////////////////////////////////////////////////////////////
// CFacedetectApp construction

//#ifdef _EiC
//#define WIN32
//#endif

CvMemStorage* storage = 0;
CvHaarClassifierCascade* cascade;

int OpenCVFaceDetection::init_cascade(const char* cascade_name)
{
	/*
	* Reading the haar file from disk.
	*/
	cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
	if( !cascade )
	{
		fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
		fprintf( stderr, "Usage: facedetect --cascade=\"<cascade_path>\" [filename|camera_index]\n" );
		return -1;
	}

	/*
	* Initialize storage in order to store the found faces.
	*/
	storage = cvCreateMemStorage(0);
	if(storage == NULL){
		fprintf( stderr, "ERROR: Could not create memory storage.\n" );
		return -1;
	}

	/*
	* All went well.
	*/
	return 0;
}

//
// markType = 0 -> around; 1 -> rectangle;
//

CvSeq* OpenCVFaceDetection::detect_face(IplImage* img, double scale, double &time)
{
	static CvScalar colors[] = 
	{
		{{0,0,255}},
		{{0,128,255}},
		{{0,255,255}},
		{{0,255,0}},
		{{255,128,0}},
		{{255,255,0}},
		{{255,0,0}},
		{{255,0,255}}
	};
	
	/*
	* Prepare the image and clean the storage.
	*/
	IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
	IplImage* small_img = cvCreateImage(cvSize( cvRound (img->width/scale), cvRound (img->height/scale)), 8, 1 );
	cvCvtColor( img, gray, CV_BGR2GRAY );
	cvResize( gray, small_img, CV_INTER_LINEAR );
	cvEqualizeHist( small_img, small_img );
	cvClearMemStorage( storage );

	/*
	* When a cascade is present do the detection.
	*/

	/*
	* Do the actual detection using the haar file here.
	*/
	time = (double)cvGetTickCount();
	CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage, 1.1, 2, 0|CV_HAAR_DO_CANNY_PRUNING, cvSize(30, 30));
	time = (double)cvGetTickCount() - time;

	time = time/(double)((double)cvGetTickFrequency()*1000.);
	
	/*
	* Draw ovals around the detected faces.
	*/
	for(int i = 0; i < (faces ? faces->total : 0); i++ )
	{
		CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
		CvPoint center;
		int radius;
		center.x = cvRound((r->x + r->width*0.5)*scale);
		center.y = cvRound((r->y + r->height*0.5)*scale);
		radius = cvRound((r->width + r->height)*0.25*scale);

		cvCircle( img, center, radius, colors[i % 8], 3, 8, 0 );
	    //cvRectangle(img, cvPoint(r->x * scale, r->y * scale), cvPoint((r->x + r->width)*scale, (r->y + r->height)*scale), cvScalar(0, 0, 255), 2, 8, 0);
	}	

	/*
	* Removed, because SDL is doing the showing of the image.
	*/

	cvReleaseImage( &gray );
	cvReleaseImage( &small_img );

	/*
	* Return faces struct.
	*/
	return faces;
}