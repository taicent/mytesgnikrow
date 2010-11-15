/***************************************************************************
                             segment.cpp

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


#include "stdafx.h"
//#include "segment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//


//Note:  Two image resolutions are used.  The original image accepted as a parameter within
//       RChbp_update is typically a fairly high resolution image.  This image is used
//       to detect faces and facial expressions.  Smaller sized (80x80) images are also
//       used in order to estimate body posture.  The smaller size is used because the segmentation
//       routine is computationally expensive.



const int FACE_IMAGE_SIZE = 60;
const int SMALL_IMAGE_SIZE = 80;  //width/height size of the small image used for detection of body posture

//image location of the last observed valid face
int prev_face_x=0;
int prev_face_y=0;
int current_face_x=0;
int current_face_y=0;

int initialised=0;

//the size of the small subsampled image (typically 80x80)
int imageWidth,imageHeight;

classimage *rawImage_normal[2];   //raw image from the camera (normal size)
classimage *rawImage_small[2];    //raw image from the camera (small size 80x80 pixels)
classimage *averageImage[2];      //average image from the camera - used to eliminate noise 
classimage *prev_averageImage[2]; //previous average image from the camera
classimage *filteredImage[2];     //edges image from the camera
classimage *objectImage[2];       //large observed object
classimage *segmentedImage[2];    //segmented image
classimage *skinImage[2];         //skin colour filter
classimage *cleanImage[2];        //got rid of some crap


//this isn't used and may soon be redundant
classimage *armImage;             //used to store arm image for classification

//variables which store data on the detected segments
int segment[2][5][500][12];       //segment area and colour
int No_Of_Segments[2][5];         //number of segments detected at each time step
int current_t[2];                 //current time step
int maxID=1;                      //maximum ID number for segments
int NoOfBodySegments=0;           //number of segments identified as belonging to the body
int bodySegmentIndex[100];        //stores the index number of segments identified as belonging to the body

//segmentation stuff
msImageProcessor *imageProcessor;
int *labels=NULL;
float *modes=NULL; 
int *NoOfPixels=NULL;

//some constants used for left/right arm array indexes
const int LEFTSIDE = 0;
const int RIGHTSIDE = 1;

//the humunculus object handles detection of facial features and stores body posture data
classHumunculus *person=NULL;

//the personalia object gathers data on specific individuals and how they behave with the robot
classPersonalia *people=NULL;

//colour group objects are used to classify the colours of clothes and the background
//Typically a colour group object contains a list of one or more colours sorted by the amount
//of surface area covered by each colour (biggest surface area is first in the list)
classColourClassifier *colourGroups;
classColourClassifier *colourGroups_background;


//face detection stuff
char *PATH_FACE_FRONTAL = "C:\\Develop\\hbp\\detectfrontal";
bool faceDetectionInitialised=false;
CvHaarClassifierCascade* cascade_frontal=NULL;  //face classifier
CvMemStorage* storage=NULL;
IplImage* video_image;                        //stores the scene
int NoOfFaces=0;
int face_location[10][4];                     //stores the locations of detected faces
classimage *faceImage=NULL;                   //image used to search for faces
const int faceSearchImageWidth=500;
const int faceSearchImageHeight=400;
bool face_detected=false;

//motion segmentation object for detecting movements of the hands
classMotionSegment *motionseg=NULL;

//stuff related to saving the arm silouette to a file - this may become obsolete soon
//dimensions of image used to store a picture of the left/right arms for classification
const int ARMIMAGE_WIDTH = 30;
const int ARMIMAGE_HEIGHT = 30;
int ArmFileIndex = 1;   //index number used for saving files containing arm data
const bool SAVE_ARMDATA = false;

//topological map for classifying faces
Ttopmap *topmap=NULL;
const int TOPMAP_WIDTH = 10;
const int TOPMAP_HEIGHT = 10;
const bool USE_NEURAL_CLASSIFIER = false;

//this constant sets the number of steps after which if one or more hands was detected held
//up, but then the arm wasn't detected for a while it's automatically assumed to be down again.
//Increasing the value waits for longer before putting the arm down.
const int HAND_UP = 5;
int hand_detected=0;

//face detection makes detection of the body more accurate, but slows things down a lot
bool USE_FACE_DETECTION = true;


//the system may be put into a number of different detection modes
//mode 0 - only detect visible motion
//mode 1 - look for human faces and do detection of features and person recognition
//mode 2 - fully body recognition
const DETECTIONMODE_MOTION = 0;
const DETECTIONMODE_FACES = 1;
const DETECTIONMODE_HUMANS = 2;
int DETECTION_MODE = 2;


//the minimum face width in pixels
const MIN_FACE_WIDTH = 10;

//unmanaged image which can be called from a .NET front end
BYTE *bmp_unmanaged=NULL;

/////////////////////////////////////////////////////////////////////////////
// ChbpApp

BEGIN_MESSAGE_MAP(ChbpApp, CWinApp)
	//{{AFX_MSG_MAP(ChbpApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ChbpApp construction

ChbpApp::ChbpApp()
{
  rawImage_small[0]=NULL;
  rawImage_small[1]=NULL;
  filteredImage[0]=NULL;
  filteredImage[1]=NULL;
  segmentedImage[0]=NULL;
  segmentedImage[1]=NULL;
  face_detected=false;
  person = new classHumunculus;
  people = new classPersonalia;
  colourGroups = new classColourClassifier;
  colourGroups->tollerance=80;
  colourGroups_background = new classColourClassifier;
  colourGroups_background->tollerance=80;
  NoOfFaces = 0;

  imageProcessor = new msImageProcessor;
}



//destructor
ChbpApp::~ChbpApp()
{
  if (rawImage_small[0]!=NULL)
  {
    delete rawImage_normal[0];
    delete rawImage_normal[1];
    delete rawImage_small[0];
    delete rawImage_small[1];
    delete averageImage[0];
    delete averageImage[1];
    delete prev_averageImage[0];
    delete prev_averageImage[1];
    delete filteredImage[0];
    delete filteredImage[1];
    delete segmentedImage[0];
    delete segmentedImage[1];
    delete objectImage[0];
    delete objectImage[1];
	delete skinImage[0];
	delete skinImage[1];
	delete cleanImage[0];
	delete cleanImage[1];
	delete imageProcessor;
	delete armImage;
	delete topmap;
  }

  if (faceDetectionInitialised)
  {
    cvReleaseImage( &video_image );
	delete faceImage;
  }

  delete person;
  delete people;
  delete colourGroups;
  delete colourGroups_background;
  if (motionseg!=NULL) delete motionseg;
}


ChbpApp theApp;



//load the cascade classifier from the given path
CvHaarClassifierCascade* ChbpApp::load_object_detector( const char* cascade_path )
{
    return (CvHaarClassifierCascade*)cvLoad( cascade_path );
}


int ChbpApp::findFaces(int cameraIndex)
//Is there anybody out there?...
//looks for human faces and returns the number of faces detected
{
  long n;
  int x,y,c,i,f;
  int face_tx,face_ty,face_bx,face_by;
  CvRect r;
  const ORIG_WIN_SIZE = 20;

  face_detected=false;
  if (!faceDetectionInitialised)
  {
    if (storage==NULL) storage = cvCreateMemStorage(0);

    //create the classifier
	cascade_frontal = cvLoadHaarClassifierCascade(PATH_FACE_FRONTAL,cvSize( ORIG_WIN_SIZE, ORIG_WIN_SIZE ));

    //create an image used to store the scene
    cvReleaseImage( &video_image );
    video_image = cvCreateImage( cvSize( faceSearchImageWidth , faceSearchImageHeight ), 8, 3 );
	faceImage = new classimage;
	faceImage->createImage(faceSearchImageWidth,faceSearchImageHeight);
	faceDetectionInitialised=true;
  }


  faceImage->sampleFromImage(rawImage_normal[cameraIndex],0,0,rawImage_normal[cameraIndex]->width,rawImage_normal[cameraIndex]->height);
  n=0;
  for (y=0;y<faceSearchImageHeight;y++)
  {
    for (x=0;x<faceSearchImageWidth;x++)
	{
	  for (c=0;c<3;c++)
	  {
	    video_image->imageData[n] = faceImage->image[x][y][2-c];
        n++;
	  }
	}
  }

  NoOfFaces=0;
  if( cascade_frontal && video_image )
  {
    int scale = 2;
    CvSize img_size = cvGetSize( video_image );
    IplImage* temp = cvCreateImage( cvSize(img_size.width/2,img_size.height/2), 8, 3 );

    cvPyrDown( video_image, temp );
    cvClearMemStorage( storage );

    //if( cascade )
    {
	  
      //CvSeq* faces = cvHaarDetectObjects( temp, hid_cascade, storage, 1.2, 2, 1 );

	  CvSeq* faces = cvHaarDetectObjects( temp, cascade_frontal, storage, 1.2, 2, CV_HAAR_DO_CANNY_PRUNING );

	  f=0;
      for( i = 0; i < (faces ? faces->total : 0); i++ )
      {
        if (i<10)
		{
          r = *(CvRect*)cvGetSeqElem( faces, i );
				
		  face_tx = (r.x)*imageWidth/(faceSearchImageWidth/2);
		  face_ty = (r.y)*imageHeight/(faceSearchImageHeight/2);
		  face_bx = ((r.x+r.width))*imageWidth/(faceSearchImageWidth/2);
		  face_by = ((r.y+r.height))*imageHeight/(faceSearchImageHeight/2);

		  if (face_bx-face_tx>MIN_FACE_WIDTH)
		  {
		    face_location[f][0] = face_tx;
		    face_location[f][1] = face_ty;
		    face_location[f][2] = face_bx;
		    face_location[f][3] = face_by;
		    f++;
		  }

          //detect key points on the face
          //detect_keypoints(i);

		}

      }
	  
	  NoOfFaces = f;
	  if (NoOfFaces>0) face_detected=true;
	  
    }

    cvReleaseImage( &temp );
  }

  return (NoOfFaces);
}



int ChbpApp::nearestFace()
//if multiple faces are detected this returns the index of the face which is closest to
//the previous valid face
{
  int i,dx,dy,dist,minDist,x,y,winner;

  if (NoOfFaces>1)
  {
	winner=0;
	minDist=99999;
    for (i=0;i<NoOfFaces;i++)
	{
	  x = face_location[i][0] + ((face_location[i][2]-face_location[i][0])/2);
	  y = face_location[i][1] + ((face_location[i][3]-face_location[i][1])/2);
	  dx = abs(prev_face_x - x);
	  dy = abs(prev_face_y - y);
	  dist = (int)sqrt((dx*dx)+(dy*dy));
      if (dist<minDist)
	  {
		minDist = dist;
        winner=i;
        current_face_x = x;
		current_face_y = y;
	  }
	}
	return(winner);
  }
  else
  {
    return(0);
  }
}



void ChbpApp::RChbp_update(int cameraIndex, SAFEARRAY **bmp, int width, int height, int &NoOfSegments)
//the main update routine called from the API
{
  int t;

  SafeArrayLock(*bmp);
  update(cameraIndex,bmp,width,height);
  SafeArrayUnlock(*bmp);

  t = current_t[cameraIndex]-1;
  if (t<0) t=4;
  NoOfSegments = No_Of_Segments[cameraIndex][t];
}


void ChbpApp::RChbp_setDetectionMode(int mode)
//sets the detection mode
{
  DETECTION_MODE = mode;
}


void ChbpApp::RChbp_getImage(SAFEARRAY **bmp, int imageIndex)
//returns an image of the specified type - used for debugging purposes to check the
//various stages of image processing
{
  int x,y,c;
  long n=0;
  BYTE *img;

  SafeArrayLock(*bmp);
  img = (BYTE *)(*bmp)->pvData;

  for (y=0;y<imageHeight;y++)
  {
    for (x=0;x<imageWidth;x++)
	{
	  for (c=0;c<3;c++)
	  {
		switch (imageIndex)
		{
		  case 0:
			  {
		        img[n] = rawImage_small[0]->image[x][y][2-c];
				break;
			  }
		  case 1:
			  {
		        img[n] = averageImage[0]->image[x][y][2-c];
				break;
			  }
		  case 2:
			  {
		        img[n] = segmentedImage[0]->image[x][y][2-c];
				break;
			  }
		  case 3:
			  {
		        img[n] = filteredImage[0]->image[x][y][2-c];
				break;
			  }
		  case 4:
			  {
		        img[n] = objectImage[0]->image[x][y][2-c];
				break;
			  }
		  case 5:
			  {
		        img[n] = cleanImage[0]->image[x][y][2-c];
				break;
			  }
		}
	    n++;
	  }
    }
  }
  
  SafeArrayUnlock(*bmp);
}



void ChbpApp::RChbp_getImagePixel(int imageIndex, int x, int y, BYTE &r, BYTE &g, BYTE &b)
//returns an image pixel of the specified type - used for debugging purposes to check the
//various stages of image processing
{
  switch (imageIndex)
  {
    case 0:
	{
	  r = rawImage_small[0]->image[x][y][0];
	  g = rawImage_small[0]->image[x][y][1];
	  b = rawImage_small[0]->image[x][y][2];
	  break;
	}

    case 1:
	{
	  r = averageImage[0]->image[x][y][0];
	  g = averageImage[0]->image[x][y][1];
	  b = averageImage[0]->image[x][y][2];
	  break;
	}

    case 2:
	{
	  r = segmentedImage[0]->image[x][y][0];
	  g = segmentedImage[0]->image[x][y][1];
	  b = segmentedImage[0]->image[x][y][2];
	  break;
	}

    case 3:
	{
	  r = filteredImage[0]->image[x][y][0];
	  g = filteredImage[0]->image[x][y][1];
	  b = filteredImage[0]->image[x][y][2];
	  break;
	}

    case 4:
	{
	  r = objectImage[0]->image[x][y][0];
	  g = objectImage[0]->image[x][y][1];
	  b = objectImage[0]->image[x][y][2];
	  break;
	}

    case 5:
	{
	  r = cleanImage[0]->image[x][y][0];
	  g = cleanImage[0]->image[x][y][1];
	  b = cleanImage[0]->image[x][y][2];
	  break;
	}

    case 6:
	{
	  r = skinImage[0]->image[x][y][0];
	  g = skinImage[0]->image[x][y][1];
	  b = skinImage[0]->image[x][y][2];
	  break;
	}

  }
}




void ChbpApp::RChbp_getPersonIdentity(int &personID, SAFEARRAY **personName)
{
  char *n;

  SafeArrayLock(*personName);
  n = (char *)(*personName)->pvData;
  if (person!=NULL)
  {
    if (!person->invalid_face)
	{
      personID = person->personID;
	  if (personID>0) person->getPersonName(personID,n);
	}
  }
  SafeArrayUnlock(*personName);
}



BYTE ChbpApp::RChbp_getPersonName(int personID, int charIndex)
{
  char *n;

  if (personID>0)
  {
	n = new char[20];
	person->getPersonName(personID,n);
	return(n[charIndex]);
	delete [] n;
  }
  else
  return(0);
}


BYTE ChbpApp::RChbp_getPortrait(int personID, int charIndex)
{
  char *n;

  if (personID>0)
  {
	n = new char[150];
	person->getPortrait(personID,n);
	return(n[charIndex]);
	delete [] n;
  }
  else
  return(0);
}




int ChbpApp::RChbp_getPersonID()
{
  if (person!=NULL)
  {
    if (!person->invalid_face)
      return(person->personID);
	  else
	  return(-1);
  }
  else
  return(-1);
}




void ChbpApp::RChbp_getFaceImage(SAFEARRAY **bmp)
//returns the detected face image
{
  int x,y,c;
  long n=0;
  BYTE *img;

  if ((person!=NULL) && (NoOfFaces>0))
  {
    SafeArrayLock(*bmp);
    img = (BYTE *)(*bmp)->pvData;

    for (y=0;y<FACE_IMAGE_SIZE;y++)
	{
      for (x=0;x<FACE_IMAGE_SIZE;x++)
	  {
	    for (c=0;c<3;c++)
		{
          img[n] = person->face->image[x][y][2-c];
	      n++;
		}
	  }
	}
  
    SafeArrayUnlock(*bmp);
  }
}


void ChbpApp::RChbp_getFaceImagePixel(int x, int y, BYTE &r, BYTE &g, BYTE &b)
//returns the detected face image
{
  r = person->face->image[x][y][0];
  g = person->face->image[x][y][1];
  b = person->face->image[x][y][2];	
}





int ChbpApp::RChbp_getNormalisedFaceImage(SAFEARRAY **bmp)
//retrieve the normalised face image which is used for recognition purposes
{
  int x,y,c;
  long n=0;
  BYTE *img;

  if ((person!=NULL) && (person->normalisedFace!=NULL))
  {
    SafeArrayLock(*bmp);
    img = (BYTE *)(*bmp)->pvData;

    for (y=0;y<FACE_IMAGE_SIZE/2;y++)
	{
      for (x=0;x<FACE_IMAGE_SIZE/2;x++)
	  {
	    for (c=0;c<3;c++)
		{
          img[n] = person->normalisedFace->image[x][y][2-c];
	      n++;
		}
	  }
	}
  
    SafeArrayUnlock(*bmp);
	return(1);
  }
  else
  return(0);
}





void ChbpApp::update_base(int cameraIndex, BYTE *img, int width, int height)
//update routine
{
  int x,y,c,i,t;
  int tx,ty,bx,by,window_width,window_height;
  int sigmaS = 2;
  float sigmaR = 4; //6.5;
  int minRegion;
  long n,p,pixels;
  RegionList *regions;
  int *regionPoints;
  classimage *temp_image;
  int mult_x,mult_y,faceID;

  mult_x = width / SMALL_IMAGE_SIZE;
  mult_y = height / SMALL_IMAGE_SIZE;
  imageWidth = width / mult_x;
  imageHeight = height / mult_y;

  if (rawImage_small[0]==NULL)
  {
	//create the image objects
	rawImage_normal[0] = new classimage;
	rawImage_normal[0]->createImage(width,height);
	rawImage_normal[1] = new classimage;
	rawImage_normal[1]->createImage(width,height);
	rawImage_small[0] = new classimage;
	rawImage_small[0]->createImage(imageWidth,imageHeight);
    rawImage_small[0]->reductionFactor_x = mult_x;
    rawImage_small[0]->reductionFactor_y = mult_y;
	rawImage_small[1] = new classimage;
	rawImage_small[1]->createImage(imageWidth,imageHeight);
    rawImage_small[1]->reductionFactor_x = mult_x;
    rawImage_small[1]->reductionFactor_y = mult_y;
	averageImage[0] = new classimage;
	averageImage[0]->createImage(imageWidth,imageHeight);
	averageImage[1] = new classimage;
	averageImage[1]->createImage(imageWidth,imageHeight);
	prev_averageImage[0] = new classimage;
	prev_averageImage[0]->createImage(imageWidth,imageHeight);
	prev_averageImage[1] = new classimage;
	prev_averageImage[1]->createImage(imageWidth,imageHeight);
	filteredImage[0] = new classimage;
	filteredImage[0]->createImage(imageWidth,imageHeight);
	filteredImage[1] = new classimage;
	filteredImage[1]->createImage(imageWidth,imageHeight);
	segmentedImage[0] = new classimage;
	segmentedImage[0]->createImage(imageWidth,imageHeight);
	segmentedImage[1] = new classimage;
	segmentedImage[1]->createImage(imageWidth,imageHeight);
	objectImage[0] = new classimage;
	objectImage[0]->createImage(imageWidth,imageHeight);
	objectImage[1] = new classimage;
	objectImage[1]->createImage(imageWidth,imageHeight);
	cleanImage[0] = new classimage;
	cleanImage[0]->createImage(imageWidth,imageHeight);
	cleanImage[1] = new classimage;
	cleanImage[1]->createImage(imageWidth,imageHeight);
	skinImage[0] = new classimage;
	skinImage[0]->createImage(imageWidth,imageHeight);
	skinImage[1] = new classimage;
	skinImage[1]->createImage(imageWidth,imageHeight);

	armImage = new classimage;
	armImage->createImage(ARMIMAGE_WIDTH,ARMIMAGE_HEIGHT);
	if (USE_NEURAL_CLASSIFIER)
	{
	  topmap = new Ttopmap(ARMIMAGE_WIDTH,ARMIMAGE_HEIGHT,TOPMAP_WIDTH,TOPMAP_HEIGHT);
	  topmap->load("c:\\develop\\segment\\training\\topmap.net");
	}

	labels = new int [imageWidth*imageHeight];
	modes = new float [1000*3];
    NoOfPixels = new int [1000];

    for (x=0;x<imageWidth;x++)
	{
      for (y=0;y<imageHeight;y++)
	  {
	    for (c=0;c<3;c++)
		{
		  rawImage_small[0]->image[x][y][c]=0;
		  rawImage_small[1]->image[x][y][c]=0;
		  averageImage[0]->image[x][y][c]=0;
		  averageImage[1]->image[x][y][c]=0;
		  prev_averageImage[0]->image[x][y][c]=0;
		  prev_averageImage[1]->image[x][y][c]=0;
		  filteredImage[0]->image[x][y][c]=0;
		  filteredImage[1]->image[x][y][c]=0;
		  segmentedImage[0]->image[x][y][c]=0;
		  segmentedImage[1]->image[x][y][c]=0;
		}
	  }
	}
  }

  minRegion = imageWidth * 5 / 2;

  //store the raw image at the same resolution as it was received
  //The original image resolution is reasonably high resolution, and so can be used
  //to detect faces and facial features
  rawImage_normal[cameraIndex]->updateFromBitmap(img,1,width,height);

  //store a smaller version of the raw image.
  //The smaller size is used because the segmentation algorithm is computationally
  //intensive and larger images slow it down a great deal
  rawImage_small[cameraIndex]->updateFromImage(rawImage_normal[cameraIndex]); //updateFromBitmap(img,1,width,height);

  //detect motion
  if (motionseg==NULL) motionseg = new classMotionSegment;
  motionseg->RCmseg_update(rawImage_small[cameraIndex]);

  if (DETECTION_MODE > 0)
  {

    if (USE_FACE_DETECTION)
	{
      //find faces
      findFaces(cameraIndex);
	}

    if (DETECTION_MODE == 1)
	{
	  //only detect faces
      if (NoOfFaces>0)
	  {
		faceID = nearestFace();
	    person->analyseFace(rawImage_normal[cameraIndex],face_location[faceID][0]*mult_x,face_location[faceID][1]*mult_y,face_location[faceID][2]*mult_x,face_location[faceID][3]*mult_y);
  	    detectSkin(cameraIndex,true);
	    //people->update(person);
	  }
	  else
	  {
	    detectSkin(cameraIndex,true);
	  }
	}
	else
	{

      //store the previous average image
      temp_image = prev_averageImage[cameraIndex];
      prev_averageImage[cameraIndex] = averageImage[cameraIndex];
      averageImage[cameraIndex] = temp_image;

      window_width = imageWidth/25;
      window_height = imageHeight/25;
      calcAverageImage(rawImage_small[cameraIndex],averageImage[cameraIndex],rawImage_small[cameraIndex],
  	               window_width,window_height,imageWidth,imageHeight);

      averageImage[cameraIndex]->saveToBitmap(img,1,imageWidth,imageHeight);
  

      //feed the bitmap into the segmentor
      imageProcessor->SetSpeedThreshold(100);
      imageProcessor->DefineImage(img,COLOR,imageWidth,imageHeight);
      imageProcessor->Segment(sigmaS,sigmaR,minRegion,HIGH_SPEEDUP);

      //current time step;
      t = current_t[cameraIndex];

      //get region info and the number of segments
      No_Of_Segments[cameraIndex][t] = imageProcessor->GetRegions(labels,modes,NoOfPixels);
      regions = imageProcessor->GetBoundaries();
      if (modes!=NULL)
	  {
        for (i=0;i<No_Of_Segments[cameraIndex][t];i++)
		{
	  
          //clear the body segment identifiers
          segment[cameraIndex][t][i][11]=0;

	      //get the bounding box for the segment
          tx=imageWidth-1;
	      ty=imageHeight-1;
	      bx=0;
	      by=0;
	      regionPoints = regions->GetRegionIndeces(i);
	      pixels = regions->GetRegionCount(i);
	      for (p=0;p<pixels;p++)
		  {
	        y = regionPoints[p] / imageWidth;
		    x = regionPoints[p] - (y*imageWidth);
		    if (x<tx) tx=x;
		    if (x>bx) bx=x;
		    if (y<ty) ty=y;
		    if (y>by) by=y;
		  }
	  
	      //set bounding box for the segment
          segment[cameraIndex][t][i][0] = tx;
	      segment[cameraIndex][t][i][1] = ty;
	      segment[cameraIndex][t][i][2] = bx;
	      segment[cameraIndex][t][i][3] = by;

	      //set segment colour
	      n = i*3;
          segment[cameraIndex][t][i][4] = (int)modes[n];
          segment[cameraIndex][t][i][5] = (int)modes[n+1];
          segment[cameraIndex][t][i][6] = (int)modes[n+2];

          //give the segment an ID number, used for tracking over time
	      segment[cameraIndex][t][i][7] = maxID;
	      maxID++;
	      if (maxID>32000) maxID=1;

		}
	  }

      //store the segmented image
      imageProcessor->GetResults(img);
      segmentedImage[cameraIndex]->updateFromBitmap(img,1,imageWidth,imageHeight);

      //crudely get the area of the image which is moving
      //getMovementBox(averageImage[cameraIndex],prev_averageImage[cameraIndex],imageWidth,imageHeight,tx,ty,bx,by);
  
      //match similar segments over time, using the movement box to identify moving segments
      //matchSegments(cameraIndex,tx,ty,bx,by);

      if (NoOfFaces>0)
	  {
		faceID = nearestFace();
        filterImage_WithFace(cameraIndex,segmentedImage[cameraIndex],faceID);
	    person->analyseFace(rawImage_normal[cameraIndex],face_location[faceID][0]*mult_x,face_location[faceID][1]*mult_y,face_location[faceID][2]*mult_x,face_location[faceID][3]*mult_y);
		detectSkin(cameraIndex,true);
	    detectHands(cameraIndex);
	    people->update(person);
	  }
      else
	  {
		detectSkin(cameraIndex,true);
        filterImage_NoFace(cameraIndex,segmentedImage[cameraIndex],person->bodyColour[0],person->bodyColour[1],person->bodyColour[2],person->bodyColourTollerance);
	  }

      //put the arm down if it hasn't been detected for a while
      if (hand_detected>0)
	  {
        hand_detected--;
	    if (hand_detected==0)
		{
	      person->ShoulderElevation[LEFTSIDE] = 100;
          person->ElbowAngle[LEFTSIDE] = 0;
	      person->ShoulderElevation[RIGHTSIDE] = 100;
          person->ElbowAngle[RIGHTSIDE] = 0;
		}
	  }

      //increment the overall time elapsed since the program started
      people->currentTime++;

      //increment the time step
      current_t[cameraIndex]++;
      if (current_t[cameraIndex]>4) current_t[cameraIndex]=0;
    }

  }

}





void ChbpApp::update(int cameraIndex, SAFEARRAY **bmp, int width, int height)
//update routine (suprisingly enough)
{
  BYTE *img;
  img = (BYTE *)(*bmp)->pvData;

  update_base(cameraIndex, img, width, height);
}


void ChbpApp::RChbp_getSegment(int cameraIndex, int index, int &tx, int &ty, int &bx, int &by, int &r, int &g, int &b, int &segmentID, int &inMotion, int &disparity)
//returns the dimensions and colour of a segment
{
  int t;

  t = current_t[cameraIndex]-1;
  if (t<0) t=4;

  if (index < No_Of_Segments[cameraIndex][t])
  {
    tx = segment[cameraIndex][t][index][0];
	ty = segment[cameraIndex][t][index][1];
	bx = segment[cameraIndex][t][index][2];
	by = segment[cameraIndex][t][index][3];
	r = segment[cameraIndex][t][index][4];
	g = segment[cameraIndex][t][index][5];
	b = segment[cameraIndex][t][index][6];
	segmentID = segment[cameraIndex][t][index][7];
	inMotion = segment[cameraIndex][t][index][8];
	disparity = segment[cameraIndex][t][index][9];
  }
}



void ChbpApp::getMovementBox(classimage *currentImage, classimage *prevImage, 
								 int width, int height, 
								 int top_x, int top_y, int bottom_x, int bottom_y,
								 int threshold, int &tx, int &ty, int &bx, int &by)
//calculates movement box between two successive images
{
  int x,y,p1,p2,thresh,dp;

  tx=width-1;
  ty=height-1;
  bx=0;
  by=0;

  thresh=threshold;

  for (x=top_x;x<bottom_x;x++)
  {
    for (y=top_y;y<bottom_y;y++)
	{
      p1 = prevImage->image[x][y][0];
	  p2 = currentImage->image[x][y][0];
	  dp = abs(p1-p2);
	  if (dp>thresh)
	  {
	    if (x<tx) tx=x;
	    if (y<ty) ty=y;
	    if (x>bx) bx=x;
	    if (y>by) by=y;
	  }
	}
  }

  if ((tx==width-1) && (ty==height-1) && (bx==0) && (by==0))
  {
    tx=0;
	ty=0;
	bx=0;
	by=0;
  }

}


void ChbpApp::matchSegments(int cameraIndex, int motion_tx, int motion_ty, int motion_bx, int motion_by)
//matches similar segments over time
{
  int t[3],i,s,s2,w[2],h[2],cx[2],cy[2],dx,dy,tx,ty,bx,by,isBodySegment;
  int max_dx,max_dy,min_dx,min_dy,dw,dh,segmentID,dr,dg,db,r[2],g[2],b[2],inMotion;
  long dist,min_dist;

  max_dx = imageWidth/3;
  max_dy = imageHeight/3;

  t[0] = current_t[cameraIndex]-2;
  t[1] = current_t[cameraIndex]-1;
  t[2] = current_t[cameraIndex];

  if (t[0]<0) t[0]+=5;
  if (t[1]<0) t[1]+=5;
  if (t[2]<0) t[2]+=5;

  for (s=0;s<No_Of_Segments[cameraIndex][t[2]];s++)
  {
    w[0] = segment[cameraIndex][t[2]][s][2] - segment[cameraIndex][t[2]][s][0];
	h[0] = segment[cameraIndex][t[2]][s][3] - segment[cameraIndex][t[2]][s][1];
	cx[0] = segment[cameraIndex][t[2]][s][0] + (w[0]/2);
	cy[0] = segment[cameraIndex][t[2]][s][1] + (h[0]/2);
	r[0] = segment[cameraIndex][t[2]][s][4];
	g[0] = segment[cameraIndex][t[2]][s][5];
	b[0] = segment[cameraIndex][t[2]][s][6];
	min_dist=999999999;
	segmentID=-1;
	isBodySegment = 0;

    for (i=0;i<2;i++)
	{
      for (s2=0;s2<No_Of_Segments[cameraIndex][t[i]];s2++)
	  {
        w[1] = segment[cameraIndex][t[i]][s2][2] - segment[cameraIndex][t[i]][s2][0];
	    h[1] = segment[cameraIndex][t[i]][s2][3] - segment[cameraIndex][t[i]][s2][1];
	    cx[1] = segment[cameraIndex][t[i]][s2][0] + (w[1]/2);
	    cy[1] = segment[cameraIndex][t[i]][s2][1] + (h[1]/2);
	    r[1] = segment[cameraIndex][t[i]][s2][4];
	    g[1] = segment[cameraIndex][t[i]][s2][5];
	    b[1] = segment[cameraIndex][t[i]][s2][6];

		dx = abs(cx[1] - cx[0]);
		dy = abs(cy[1] - cy[0]);

		if ((dx<max_dx) && (dy<max_dy))
		{
		  dw = abs(w[1] - w[0]);
		  dh = abs(h[1] - h[0]);
		  dr = abs(r[1] - r[0]);
		  dg = abs(g[1] - g[0]);
		  db = abs(b[1] - b[0]);

		  dist = ((dx + dy)*10) + (dw + dh) + (dr + dg + db);
		  if (dist < min_dist)
		  {
		    min_dist = dist;
			segmentID = segment[cameraIndex][t[i]][s2][7];
			inMotion = segment[cameraIndex][t[i]][s2][8];
			isBodySegment = segment[cameraIndex][t[i]][s2][11];
		  }
		}
	  }
	}

	if (segmentID > -1)
	{
	  segment[cameraIndex][t[2]][s][7] = segmentID;
	  segment[cameraIndex][t[2]][s][8] = inMotion;
	  if (segment[cameraIndex][t[2]][s][11]==0) segment[cameraIndex][t[2]][s][11] = isBodySegment;
	}

  }


  for (s=0;s<No_Of_Segments[cameraIndex][t[2]];s++)
  {
	segmentID = segment[cameraIndex][t[2]][s][7];
    for (s2=0;s2<No_Of_Segments[cameraIndex][t[2]];s2++)
	{
	  if (s!=s2)
	  {
	    if (segment[cameraIndex][t[2]][s2][7]==segmentID)
		{
		  segment[cameraIndex][t[2]][s2][7] = maxID;
		  maxID++;
		  if (maxID>32000) maxID=1;
		}
	  }
	}
  }


  //if a segment has moved more than a minimum distance then mark it as in motion
  min_dx = imageWidth/10;
  min_dy = imageHeight/10;
  for (s=0;s<No_Of_Segments[cameraIndex][t[2]];s++)
  {
	segmentID = segment[cameraIndex][t[2]][s][7];
	tx = segment[cameraIndex][t[2]][s][0];
	bx = segment[cameraIndex][t[2]][s][2];
	ty = segment[cameraIndex][t[2]][s][1];
	by = segment[cameraIndex][t[2]][s][3];
    w[0] = bx - tx;
	h[0] = by - ty;
	cx[0] = tx + (w[0]/2);
	cy[0] = ty + (h[0]/2);
	segment[cameraIndex][t[2]][s][8]--;
	if (segment[cameraIndex][t[2]][s][8]<0) segment[cameraIndex][t[2]][s][8]=0;

    if ((w[0]<imageWidth*3/4) && (h[0]<imageHeight*3/4))
	{
	  if ((cx[0]>motion_tx) && (cx[0]<motion_bx) && (cy[0]>motion_ty) && (cy[0]<motion_by))
	  {
		
        for (i=0;i<2;i++)
		{
          for (s2=0;s2<No_Of_Segments[cameraIndex][t[i]];s2++)
		  {
            if (segment[cameraIndex][t[i]][s2][7]==segmentID)
			{
              w[1] = segment[cameraIndex][t[i]][s2][2] - segment[cameraIndex][t[i]][s2][0];
	          h[1] = segment[cameraIndex][t[i]][s2][3] - segment[cameraIndex][t[i]][s2][1];
	          cx[1] = segment[cameraIndex][t[i]][s2][0] + (w[1]/2);
	          cy[1] = segment[cameraIndex][t[i]][s2][1] + (h[1]/2);

		      dx = abs(cx[1] - cx[0]);
		      dy = abs(cy[1] - cy[0]);

			  if ((dx>min_dx) || (dy>min_dy))
			  {
			    if ((cx[1]>motion_tx) && (cx[1]<motion_bx) && (cy[1]>motion_ty) && (cy[1]<motion_by))
		          segment[cameraIndex][t[2]][s][8]+=5; //=5;
			  }
			}
		  }
		}
		
	  }
	}
	else
	{
	  segment[cameraIndex][t[2]][s][8]=0;
	}
  }

}



void ChbpApp::calcAverageImage(classimage *inputImage, classimage *outputImage, classimage *prevImage,
										int window_width, int window_height,
										int width, int height)
//calculates the average image using the given window
{
  long window_pixels,tot_r,tot_g,tot_b,pixels;
  int x,y,tx,ty,bx,by,halfWidth,halfHeight,r,g,b;

  window_pixels = (long)window_width * (long)window_height;
  halfWidth = window_width / 2;
  halfHeight = window_height / 2;

  //inputImage->updateIntegralImage();

  for (x=0;x<width;x++)
  {
    for (y=0;y<height;y++)
	{
	  
	  tx = x - halfWidth;
	  ty = y - halfHeight;
	  bx = tx + window_width;
	  by = ty + window_height;

	  if (tx<0) tx=0;
	  if (bx>=width) bx=width-1;
	  if (ty<0) ty=0;
	  if (by>=height) by=height-1;

	  tot_r = inputImage->getIntegral(tx,ty,bx,by,0);
	  tot_g = inputImage->getIntegral(tx,ty,bx,by,2);
	  tot_b = inputImage->getIntegral(tx,ty,bx,by,3);

	  
	  if (((x>halfWidth) && (x<width-halfWidth-1)) && ((y>halfHeight) && (y<height-halfHeight-1)))
	    pixels = window_pixels;
	    else
	    pixels = ((long)(bx-tx) * (long)(by-ty));

	  r = tot_r / pixels;
	  g = tot_g / pixels;
	  b = tot_b / pixels;

	  prevImage->image[x][y][0] = outputImage->image[x][y][0];
	  prevImage->image[x][y][1] = outputImage->image[x][y][1];
	  prevImage->image[x][y][2] = outputImage->image[x][y][2];

	  outputImage->image[x][y][0] = (unsigned char)r;
	  outputImage->image[x][y][1] = (unsigned char)g;
	  outputImage->image[x][y][2] = (unsigned char)b;
	  
	}
  }

}


bool ChbpApp::RChbp_faceDetected()
//returns true if a face is detected
{
  static int prev_face_cx;
  static int prev_face_cy;
  int f_cx,f_cy,f_tx,f_ty,f_bx,f_by;
  int maxdist,faceID;
  bool valid_face;
	
  if (rawImage_small[0]!=NULL)
  {
    valid_face=false;
	  
    if (NoOfFaces>0)
	{
	  faceID = nearestFace();
      f_tx = face_location[faceID][0];
	  f_ty = face_location[faceID][1];
	  f_bx = face_location[faceID][2];
	  f_by = face_location[faceID][3];
	  f_cx = f_tx + ((f_bx - f_tx)/2);
	  f_cy = f_ty + ((f_by - f_ty)/2);

	  maxdist = imageWidth/5;
	  if ((abs(f_cx - prev_face_cx)<maxdist) && (abs(f_cy - prev_face_cy)<maxdist))
	    valid_face=true;

	  prev_face_cx = f_cx;
	  prev_face_cy = f_cy;	
	}

    //if ((valid_face) && (!person->invalid_face) && (face_detected))
    if ((!person->invalid_face) && (face_detected))
      return(true);
      else
	  return(false);
  }
  else return(false);
}



void ChbpApp::RChbp_getBodyPosture(int cameraIndex, int &leftShoulderElevation, 
										int &rightShoulderElevation, int &leftElbowElevation, 
										int &rightElbowElevation, int &leftHandSize, int &rightHandSize,
										int &face_tx, int &face_ty, 
										int &face_bx, int &face_by, int &bodyWidth, int &gazeDirection, 
										int &headTilt, int &mouthWidth, int &mouthOpen, int &eyebrowsVertical, 
										int &bodycolour_red, int &bodycolour_green, int &bodycolour_blue,
										int &background_red, int &background_green, int &background_blue,
										int &personID, bool &invalidFace)
//returns the detected body posture
{
  static int prev_face_cx;
  static int prev_face_cy;
  int face_cx,face_cy,faceID;
  int maxdist;
  bool valid_face;
	
  invalidFace=true;
  personID=0;
  valid_face=false;
  if (NoOfFaces>0)
  {
	faceID = nearestFace();
    face_tx = face_location[faceID][0];
	face_ty = face_location[faceID][1];
	face_bx = face_location[faceID][2];
	face_by = face_location[faceID][3];
	face_cx = face_tx + ((face_bx - face_tx)/2);
	face_cy = face_ty + ((face_by - face_ty)/2);

	maxdist = imageWidth/5;
	if ((abs(face_cx - prev_face_cx)<maxdist) && (abs(face_cy - prev_face_cy)<maxdist))
	  valid_face=true;

	prev_face_cx = face_cx;
	prev_face_cy = face_cy;	
  }
  else
  {
    face_tx = 0;
	face_ty = 0;
	face_bx = 0;
	face_by = 0;
  }

  if (valid_face) 
  {
    leftShoulderElevation = person->ShoulderElevation[LEFTSIDE];
    rightShoulderElevation = person->ShoulderElevation[RIGHTSIDE];
    leftElbowElevation = person->ElbowAngle[LEFTSIDE];
    rightElbowElevation = person->ElbowAngle[RIGHTSIDE];
	leftHandSize = person->handSize[LEFTSIDE];
	rightHandSize = person->handSize[RIGHTSIDE];
    bodyWidth = person->bodyWidth;
	bodycolour_red = person->bodyColour[0];
	bodycolour_green = person->bodyColour[1];
	bodycolour_blue = person->bodyColour[2];
	background_red = person->backgroundColour[0];
	background_green = person->backgroundColour[1];
	background_blue = person->backgroundColour[2];

	if (!person->invalid_face)
	{
	  gazeDirection = person->gazeDirection;
	  headTilt = person->headTilt;
	  mouthWidth = person->mouthWidth;
	  mouthOpen = person->mouthOpen;
	  eyebrowsVertical = person->eyebrows_vertical;
	  personID = person->personID;
	  invalidFace=false;
	  prev_face_x = current_face_x;
	  prev_face_y = current_face_y;
	}

  }
}



void ChbpApp::filterImage(int cameraIndex, classimage *sourceImage)
//filters the segmented image looking for large coloured blobs
{
  long pixels,max,max_pixels,min_pixels,p;
  int i,bb,c,winner,r[10],g[10],b[10],cx,cy,tx,ty,bx,by,w,x,y,xx,yy,region_index;
  int tollerance=45;
  RegionList *regions;
  int *regionPoints,border_x,top_y,x1,x2,left_top_y,right_top_y;

  r[0] = 45;
  g[0] = 45;
  b[0] = 45;

  r[1] = 210;
  g[1] = 210;
  b[1] = 210;

  r[2] = 210;
  g[2] = 45;
  b[2] = 45;

  r[3] = 150;
  g[3] = 45;
  b[3] = 45;

  r[4] = 45;
  g[4] = 210;
  b[4] = 45;

  r[5] = 45;
  g[5] = 150;
  b[5] = 45;

  r[6] = 45;
  g[6] = 45;
  b[6] = 210;

  r[7] = 45;
  g[7] = 45;
  b[7] = 150;

  NoOfBodySegments=0;
  w = imageWidth/4;
  border_x = imageWidth/3;
  max_pixels = (imageWidth*imageHeight)/2;
  min_pixels = (w*imageHeight)/10;
  max=-1;
  winner=-1;
  for (i=0;i<8;i++)
  {
    pixels = filteredImage[cameraIndex]->filterColour(sourceImage,r[i],g[i],b[i],tollerance,cx,cy);

	tx = cx - w;
	ty = 2;
	bx = cx + w;
	by = imageHeight-3;
	if (tx<2) tx=2;
	if (bx>=imageWidth-1) bx=imageWidth-2;
	
	if ((bx>tx) && (cx>border_x) && (cx<imageWidth-border_x))
	{
      filteredImage[cameraIndex]->replaceColourArea(255,255,255,0,0,0,0,ty,tx,by);
      filteredImage[cameraIndex]->replaceColourArea(255,255,255,0,0,0,bx,ty,imageWidth,by);

      pixels = filteredImage[cameraIndex]->getIntegral(tx,ty,bx,by,1);
	  pixels /= 255;

      if ((pixels>min_pixels) && (pixels<max_pixels))
	  {
	    if (pixels>max)
		{
	      max = pixels;
		  winner=i;
		}
	  }
	}
  }


  //clear object image
  for (x=0;x<imageWidth;x++)
  {
    for (y=0;y<imageHeight;y++)
	{
	  objectImage[cameraIndex]->image[x][y][0]=0;  
	  objectImage[cameraIndex]->image[x][y][1]=0;
	  objectImage[cameraIndex]->image[x][y][2]=0;
	}
  }


  if (winner>-1)
  {
    pixels = filteredImage[cameraIndex]->filterColour(sourceImage,r[winner],g[winner],b[winner],tollerance,cx,cy);

	tx = cx - w;
	ty = 2;
	bx = cx + w;
	by = imageHeight-3;
	if (tx<2) tx=2;
	if (bx>=imageWidth-1) bx=imageWidth-2;

	border_x = imageWidth/8;

    //filteredImage[cameraIndex]->replaceColourArea(255,255,255,0,0,0,0,ty,tx,by);
    //filteredImage[cameraIndex]->replaceColourArea(255,255,255,0,0,0,bx,ty,imageWidth,by);

	x1 = tx + ((bx-tx)/4);
	x2 = x1 + ((bx-tx)/2);
	top_y=0;
	left_top_y=0;
	right_top_y=0;
	regions = imageProcessor->GetBoundaries();
    for (y=ty;y<by;y++)
	{
	  for (x=tx;x<bx;x++)
	  {
		if (filteredImage[cameraIndex]->image[x][y][0]>0)
		{
		  if ((x>x1) && (x<x2))
		  {
		    if (top_y==0) top_y=y;
		  }
		  if ((x<cx) && (left_top_y==0)) left_top_y=y;
		  if ((x>cx) && (right_top_y==0)) right_top_y=y;
		}

	    if ((objectImage[cameraIndex]->image[x][y][0]==0) && (filteredImage[cameraIndex]->image[x][y][0]>0))
		{
          p = (y*imageWidth)+x;
		  region_index = labels[p];

		  //store a list of segment indexes associated with the body
		  bb=0;
		  while ((bb<NoOfBodySegments) && (bodySegmentIndex[bb]!=region_index)) bb++;
		  if ((bb==NoOfBodySegments) && (NoOfBodySegments<100))
		  {
            bodySegmentIndex[NoOfBodySegments] = region_index;
		    NoOfBodySegments++;  
		  }

		  //fill in the segment
	      regionPoints = regions->GetRegionIndeces(region_index);
	      pixels = regions->GetRegionCount(region_index);
	      for (p=0;p<pixels;p++)
		  {
	        yy = regionPoints[p] / imageWidth;
		    xx = regionPoints[p] - (yy*imageWidth);

			if ((xx>border_x) && (xx<imageWidth-border_x) && (yy>5) && (yy<imageHeight-5))
			{            
			  objectImage[cameraIndex]->image[xx][yy][0]=255;
			  objectImage[cameraIndex]->image[xx][yy][1]=255;
			  objectImage[cameraIndex]->image[xx][yy][2]=255;
			}
		  }
		  
		}
	  }
	}



    for (y=0;y<imageHeight;y++)
	{
      for (x=0;x<imageWidth;x++)
	  {
		//calculate array index for the given x,y coordinate
        p = (y*imageWidth)+x;
		region_index = labels[p];

		//if this is a blob segment fill it in
		for (bb=0;bb<NoOfBodySegments;bb++)
		{
		  if (bodySegmentIndex[bb]==region_index)
		  {
		    for (c=0;c<3;c++) objectImage[cameraIndex]->image[x][y][c]=255;
		  }
		}

		if ((x<border_x) || (x>imageWidth-border_x-1))
 		  for (c=0;c<3;c++) objectImage[cameraIndex]->image[x][y][c]=0;
	  }
	}

  }

}





void ChbpApp::filterImage_body(int cameraIndex, classimage *sourceImage, int cx, int cy)
//filters the segmented image using the given colour and tollerance
{
  long pixels,max_pixels,min_pixels,p;
  int bb,c,tx,ty,bx,by,w,x,y,xx,yy,region_index,t;
  int facesize,bodysearch_by,face_tx,face_ty,face_bx,face_by;
  RegionList *regions;
  int *regionPoints,border_x,wdth,av_wdth,top_y,x1,x2,left_top_y,right_top_y;

  NoOfBodySegments=0;
  w = imageWidth/4;
  border_x = imageWidth/3;
  max_pixels = (imageWidth*imageHeight)/2;
  min_pixels = (w*imageHeight)/10;

  //clear object image
  for (x=0;x<imageWidth;x++)
  {
    for (y=0;y<imageHeight;y++)
	{
	  objectImage[cameraIndex]->image[x][y][0]=0;  
	  objectImage[cameraIndex]->image[x][y][1]=0;
	  objectImage[cameraIndex]->image[x][y][2]=0;
	}
  }

  tx = cx - w;
  ty = 2;
  bx = cx + w;
  by = imageHeight-3;
  if (tx<2) tx=2;
  if (bx>=imageWidth-1) bx=imageWidth-2;

  border_x = imageWidth/8;

  t = current_t[cameraIndex];
  x1 = tx + ((bx-tx)/4);
  x2 = x1 + ((bx-tx)/2);
  top_y=0;
  left_top_y=0;
  right_top_y=0;
  av_wdth=0;
  regions = imageProcessor->GetBoundaries();
  for (y=ty;y<by;y++)
  {
    wdth=0;
    for (x=tx;x<bx;x++)
	{
	  if (sourceImage->image[x][y][0]>0)
	  {
	    if ((x>x1) && (x<x2))
		{
		  wdth++;
		  if (top_y==0) top_y=y;
		}
		if ((x<cx) && (left_top_y==0)) left_top_y=y;
		if ((x>cx) && (right_top_y==0)) right_top_y=y;
	  }
 
	  if ((objectImage[cameraIndex]->image[x][y][0]==0) && (sourceImage->image[x][y][0]>0))
	  {
        p = (y*imageWidth)+x;
	    region_index = labels[p];

        if (segment[cameraIndex][t][region_index][11]==1)
		{

	    //store a list of segment indexes associated with the body
	    bb=0;
	    while ((bb<NoOfBodySegments) && (bodySegmentIndex[bb]!=region_index)) bb++;
	    if ((bb==NoOfBodySegments) && (NoOfBodySegments<100))
		{
          bodySegmentIndex[NoOfBodySegments] = region_index;
		  NoOfBodySegments++;  
		}

		//fill in the segment
	    regionPoints = regions->GetRegionIndeces(region_index);
	    pixels = regions->GetRegionCount(region_index);
	    for (p=0;p<pixels;p++)
		{
	      yy = regionPoints[p] / imageWidth;
		  xx = regionPoints[p] - (yy*imageWidth);

		  if ((xx>border_x) && (xx<imageWidth-border_x) && (yy>5) && (yy<imageHeight-5))
		  {
		    objectImage[cameraIndex]->image[xx][yy][0]=255;
		    objectImage[cameraIndex]->image[xx][yy][1]=255;
		    objectImage[cameraIndex]->image[xx][yy][2]=255;
		  }
		}

		}
		  
	  }
	}
    av_wdth += wdth;
  }

  if (by-top_y>2)
    av_wdth /= by-top_y;
    else
    av_wdth=2;

  //calculate face size based upon body width
  facesize = (int)(av_wdth*0.7);
  bodysearch_by = top_y + (int)(facesize * 3.6);

  //calculate approximate position of the face
  face_tx = cx - (facesize/2);
  face_ty = top_y - facesize-2;
  face_bx = cx + (facesize/2);
  face_by = top_y-2;
  if (face_tx<0) face_tx=0;
  if (face_bx>=imageWidth) face_bx=imageWidth-1;
  if (face_ty<0) face_ty=0;
  if (face_by>=imageHeight) face_by=imageHeight-1;
  face_location[0][0]=face_tx;
  face_location[0][1]=face_ty;
  face_location[0][2]=face_bx;
  face_location[0][3]=face_by;
  NoOfFaces=1;


  for (y=0;y<imageHeight;y++)
  {
    for (x=0;x<imageWidth;x++)
	{
      //if this is the face region fill it in
	  if ((x>=face_tx) && (x<=face_bx) && (y>=face_ty) && (y<=face_by))
	  {
	    for (c=0;c<3;c++) objectImage[cameraIndex]->image[x][y][c]=254;
	  }

	  //calculate array index for the given x,y coordinate
      p = (y*imageWidth)+x;
	  region_index = labels[p];

	  //if this is a body segment fill it in
	  for (bb=0;bb<NoOfBodySegments;bb++)
	  {
	    if (bodySegmentIndex[bb]==region_index)
		{
		  for (c=0;c<3;c++) objectImage[cameraIndex]->image[x][y][c]=255;
		}
	  }

	  if ((x<border_x) || (x>imageWidth-border_x-1) || (y>bodysearch_by))
 	    for (c=0;c<3;c++) objectImage[cameraIndex]->image[x][y][c]=0;
	}
  }

  cleanup(objectImage[cameraIndex],cleanImage[cameraIndex],cx,cy);

  //estimate pose
  estimatePose(cameraIndex,0,cleanImage[cameraIndex],SAVE_ARMDATA);
  NoOfFaces=0;

}






void ChbpApp::filterImage_NoFace(int cameraIndex, classimage *sourceImage, int r, int g, int b, int tollerance)
//filters the segmented image using the given colour and tollerance
{
  int cx,cy;
  long pixels;

  pixels = filteredImage[cameraIndex]->filterColour(sourceImage,r,g,b,tollerance,cx,cy);
  filterImage_body(cameraIndex, filteredImage[cameraIndex],cx,cy);
}





void ChbpApp::cleanup(classimage *sourceImage, classimage *destImage, int seed_x,int seed_y)
//uses flood fill to extract the body area and ignore any peripheral blobs
{
  int x,y,c,tx,ty,bx,by;
  long pixels,av_r,av_g,av_b;

  //clear the destination image
  for (x=0;x<imageWidth;x++)
  {
    for (y=0;y<imageHeight;y++)
	{
	  for (c=0;c<3;c++)
	    destImage->image[x][y][c]=0;
	}
  }

  destImage->floodFill(seed_x,seed_y,255,255,255,0,tx,ty,bx,by,pixels,av_r,av_g,av_b,sourceImage);
}




void ChbpApp::filterImage_WithFace(int cameraIndex, classimage *sourceImage, int faceIndex)
//uses the location of the face to find the body
{
  long pixels,max_pixels,min_pixels,p,tot_pixels,body_pixels,background_pixels;
  int cx,cy,c,tx,ty,bx,by,w,x,y,xx,yy,region_index,x1,x2,y1,border_x,i,r2,g2,b2,coverage;
  RegionList *regions;
  int *regionPoints;
  int face_tx,face_ty,face_bx,face_by,facesize,face_cx,face_cy;
  int bodysearch_tx,bodysearch_ty,bodysearch_bx,bodysearch_by;
  int seg_cx,seg_cy,t,b;

  w = imageWidth/4;
  max_pixels = (imageWidth*imageHeight)/2;
  min_pixels = (w*imageHeight)/10;
  pixels=0;
  t = current_t[cameraIndex];
  if (t<0) t=4;
  NoOfBodySegments=0;
  border_x = imageWidth/25;
  
  if (NoOfFaces>0)
  {

    //clear object image
    for (x=0;x<imageWidth;x++)
	{
      for (y=0;y<imageHeight;y++)
	  {
		for (c=0;c<3;c++)
		{
	      objectImage[cameraIndex]->image[x][y][c]=0;  
		  filteredImage[cameraIndex]->image[x][y][c]=0;  
		}
	  }
	}

    //clear the body segment identifiers
    for (i=0;i<No_Of_Segments[cameraIndex][t];i++)
      segment[cameraIndex][t][i][11]=0;

	//get the dimensions and position of the face
    face_tx = face_location[faceIndex][0];
	face_ty = face_location[faceIndex][1];
	face_bx = face_location[faceIndex][2];
	face_by = face_location[faceIndex][3];
	facesize = ((face_bx-face_tx)+(face_by-face_ty))/2;
	face_cx = face_tx + ((face_bx-face_tx)/2);
	face_cy = face_ty + ((face_by-face_ty)/2);

	if (faceIndex==0)
	{
	  person->face_x = (face_cx * 100) / imageWidth;
	  person->face_y = (face_cy * 100) / imageHeight;
      person->bodyWidth = facesize * 2;  //default body width
	}

	//define an approximate region below the face in which to search for the body
	bodysearch_tx = face_cx - (int)(facesize * 0.3);
	bodysearch_bx = face_cx + (int)(facesize * 0.3);
	bodysearch_ty = face_by + (int)(facesize * 1.0);
	bodysearch_by = face_by + (int)(facesize * 3.6);

	//limit to within the size of the image
    if (bodysearch_tx<0) bodysearch_tx=0;
	if (bodysearch_bx>=imageWidth) bodysearch_bx=imageWidth-1;
    if (bodysearch_ty<0) bodysearch_ty=0;
	if (bodysearch_by>=imageHeight) bodysearch_by=imageHeight-1;


	if ((bodysearch_by>bodysearch_ty) && (bodysearch_bx>bodysearch_tx))
	{
	  //centre of the body
	  cx = face_cx;
	  cy = bodysearch_ty + ((bodysearch_by-bodysearch_ty)/2);

	  x1 = bodysearch_tx-(int)(facesize * 0.4);
	  if (x1<0) x1=0;
	  x2 = bodysearch_bx+(int)(facesize * 0.4);
	  if (x2>=imageWidth) x2=imageWidth-1;
	  y1 = bodysearch_ty+(int)(facesize * 0.1);
	  if (y1>=imageHeight) y1=imageHeight-1;

	  //define an area used to search for body segments
      filteredImage[cameraIndex]->replaceColourArea(0,0,0,255,255,255,bodysearch_tx,bodysearch_ty,bodysearch_bx,bodysearch_by);
      filteredImage[cameraIndex]->replaceColourArea(0,0,0,255,255,255,x1,bodysearch_ty,x2,y1);

	  //maximum number of pixels per segment - avoids selection of very large segments
	  max_pixels = ((bodysearch_bx-bodysearch_tx)*(bodysearch_by-bodysearch_ty))*2;

	  tx = cx - w;
	  ty = 2;
	  bx = cx + w;
	  by = imageHeight-3;
	  if (tx<2) tx=2;
	  if (bx>=imageWidth-1) bx=imageWidth-2;

	  tot_pixels=0;
	  regions = imageProcessor->GetBoundaries();
      for (y=ty;y<by;y++)
	  {
	    for (x=tx;x<bx;x++)
		{
	      if ((objectImage[cameraIndex]->image[x][y][0]==0) && (filteredImage[cameraIndex]->image[x][y][0]>1))
		  {
		    //calculate array index for the given x,y coordinate
            p = (y*imageWidth)+x;
		    region_index = labels[p];

		    //store a list of segment indexes associated with the body
		    b=0;
		    while ((b<NoOfBodySegments) && (bodySegmentIndex[b]!=region_index)) b++;
		    if ((b==NoOfBodySegments) && (NoOfBodySegments<100))
			{
		      //mark this segment as a body segment
			  segment[cameraIndex][t][region_index][11]=1;

			  //dump body segment indexes into an array
              bodySegmentIndex[NoOfBodySegments] = region_index;
		      NoOfBodySegments++;  
			}

		    //fill in the segment
	        regionPoints = regions->GetRegionIndeces(region_index);
	        pixels = regions->GetRegionCount(region_index);

		    //centre of the segment
            seg_cx = segment[cameraIndex][t][region_index][0] + (segment[cameraIndex][t][region_index][2] - segment[cameraIndex][t][region_index][0]);
            seg_cy = segment[cameraIndex][t][region_index][1] + (segment[cameraIndex][t][region_index][3] - segment[cameraIndex][t][region_index][1]);

		    if (pixels<max_pixels)
			{
	          for (p=0;p<pixels;p++)
			  {
			    //convert the index into an xx,yy coordinate
	            yy = regionPoints[p] / imageWidth;
		        xx = regionPoints[p] - (yy*imageWidth);

			    //fill
				if ((xx>0) && (xx<imageWidth) && (yy>0) && (yy<imageHeight))
				{
			      objectImage[cameraIndex]->image[xx][yy][0]=255;
			      objectImage[cameraIndex]->image[xx][yy][1]=255;
			      objectImage[cameraIndex]->image[xx][yy][2]=255;
			      tot_pixels++;
				}
			  }
			}
		   
		  }
		}
	  }


      if (tot_pixels>min_pixels)
	  {
        x1 = face_cx - (int)(facesize * 0.8);
        x2 = face_cx + (int)(facesize * 0.8);
		body_pixels=0;
		r2=255;
		g2=0;
		b2=0;
		colourGroups->clear();
		colourGroups_background->clear();
		background_pixels=0;

        for (y=0;y<imageHeight;y++)
		{
	      for (x=0;x<imageWidth;x++)
		  {
		    //calculate array index for the given x,y coordinate
            p = (y*imageWidth)+x;
		    region_index = labels[p];

		    //if this is a body segment fill it in
		    for (b=0;b<NoOfBodySegments;b++)
			{
		      if (bodySegmentIndex[b]==region_index)
			  {
		        for (c=0;c<3;c++) objectImage[cameraIndex]->image[x][y][c]=255;
				body_pixels++;
				colourGroups->update(sourceImage->image[x][y][0],sourceImage->image[x][y][1],sourceImage->image[x][y][2],x,y);
			  }
			  else
			  {
			    if (!((x>face_tx) && (x<face_bx) && (y>face_ty) && (y<face_by)))
				{
				  colourGroups_background->update(sourceImage->image[x][y][0],sourceImage->image[x][y][1],sourceImage->image[x][y][2],x,y);
				  background_pixels++;
				}
			  }
			}

		    if ((x<border_x) || (x>imageWidth-border_x-1) || (y>bodysearch_by))
 		      for (c=0;c<3;c++) objectImage[cameraIndex]->image[x][y][c]=0;

		  }
		}

		if (body_pixels>0)
		{
		  //get the most significant colour
		  colourGroups->getGroupColour(0,r2,g2,b2,coverage);
		  if (coverage>20)
		  {
            person->bodyColour[0] = ((person->bodyColour[0]*80) + (r2*20))/100;
            person->bodyColour[1] = ((person->bodyColour[1]*80) + (g2*20))/100;
            person->bodyColour[2] = ((person->bodyColour[2]*80) + (b2*20))/100;
		  }
		  person->bodyColourTollerance = 100; //45;
		}


		if (background_pixels>0)
		{
          colourGroups_background->getGroupColour(0,r2,g2,b2,coverage);
		  if (coverage>20)
		  {
            person->backgroundColour[0] = ((person->backgroundColour[0]*80) + (r2*20))/100;
            person->backgroundColour[1] = ((person->backgroundColour[1]*80) + (g2*20))/100;
            person->backgroundColour[2] = ((person->backgroundColour[2]*80) + (b2*20))/100;
		  }
		}

		cx = bodysearch_tx + ((bodysearch_bx-bodysearch_tx)/2);
		cy = bodysearch_ty + ((bodysearch_by-bodysearch_ty)/2);
		cleanup(objectImage[cameraIndex],cleanImage[cameraIndex],cx,cy);

		/*
        for (y=0;y<imageHeight;y++)
		{
	      for (x=0;x<imageWidth;x++)
		  {
		    //if this is the face region fill it in
		    if ((x>=face_tx) && (x<=face_bx) && (y>=face_ty) && (y<=face_by))
			{
	          for (c=0;c<3;c++) cleanImage[cameraIndex]->image[x][y][c]=254;
			}
		  }
		}
		*/

        //estimate pose
	    estimatePose(cameraIndex,faceIndex, cleanImage[cameraIndex],SAVE_ARMDATA);
	  }
	}
  }

}



void ChbpApp::estimatePose(int cameraIndex, int faceIndex, classimage *sourceImage, bool saveArmData)
//estimates the positions of limbs
{
  int tx,ty,bx,by,x,y,w,h,coverage_percent,centre_x,extra,bodywidth,shoulders_y,c;
  int arm_tx,arm_ty,arm_bx,arm_by,armpixels,neutral_threshold=200;
  long pixels,p,av_wdth,wdth,max,val;
  char filename[80],buffer[10];

  tx=imageWidth-1;
  ty=imageHeight-1;
  bx=0;
  by=0;
  p=0;
  av_wdth=0;

  for (y=0;y<imageHeight-2;y++)
  {
	wdth=0;
    for (x=0;x<imageWidth;x++)
	{
	  if (sourceImage->image[x][y][0]>0)
	  {
	    if (x<tx) tx=x;
		if (y<ty) ty=y;
		if (x>bx) bx=x;
		if (y>by) by=y;
		p++;
		wdth++;

	  }
	}
    av_wdth += wdth;
  }

  if ((p>0) && (bx>tx) && ((by-ty)>imageHeight*4/10))
  {
    w = bx-tx;
    h = by-ty;
	pixels = w*h;
	av_wdth /= h;
	if (av_wdth<1) av_wdth=1;
    coverage_percent = p * 100 / pixels;
    if (coverage_percent < neutral_threshold)
	{
	  //find the body centre line
	  max=0;
	  extra=0;
      for (x=tx;x<bx;x++)
	  {
		val=0;
        for (y=ty;y<by;y++)
		  if (sourceImage->image[x][y][0]==255) val++;

		if (val>max)
		  max = val;
	  }

      //estimate the vertical position of the shoulders
	  //if (USE_FACE_DETECTION)
	  if (face_detected)
	    shoulders_y = face_location[faceIndex][3];
	    else
	    shoulders_y = by - max;

	  centre_x=tx;
      for (x=tx;x<bx;x++)
	  {
		val=0;
        for (y=ty;y<by;y++)
		  if (sourceImage->image[x][y][0]==255) val++;

		if (val>max-5)
		{
		  if (centre_x==tx) centre_x = x;
		  extra++;
		}
	  }
	  centre_x += extra/2;
	  
	  //estimate the body width
	  bodywidth = av_wdth/2;

	  //update body width as a percentage of the image width
	  if (!face_detected) person->bodyWidth = (person->bodyWidth*2/3) + (((av_wdth * 100) / imageWidth)/3);

	  //bounding box for the left arm
	  arm_tx = centre_x - (bodywidth*3);
	  arm_ty = shoulders_y - ((by - shoulders_y)/2); 
	  arm_bx = centre_x - (bodywidth*8/10);
	  arm_by = by;

	  //save arm data if necessary
	  armpixels = updateArmImage(cameraIndex,true,arm_tx,arm_ty,arm_bx,arm_by);
      if (saveArmData)
	  {        
		if (armpixels > 5) 
		{
		  strcpy(filename,"c:\\develop\\segment\\training\\arm");
		  itoa(ArmFileIndex,buffer,10);
		  strcat(filename,buffer);
		  strcat(filename,".dat");
		  saveArmImage(filename);
		  ArmFileIndex++;
		  if (ArmFileIndex>999) ArmFileIndex=1;
		}
	  }

	  //estimate the position of the left arm
	  if (USE_NEURAL_CLASSIFIER)
		estimateArmPositionNeural(cameraIndex,true,armpixels);
	    else
        estimateArmPosition(cameraIndex,true,arm_tx,arm_ty,arm_bx,arm_by,shoulders_y);

	  //bounding box for the right arm
	  arm_tx = centre_x + (bodywidth*8/10);
	  arm_ty = shoulders_y - ((by - shoulders_y)/2); 
	  arm_bx = centre_x + (bodywidth*3);
	  arm_by = by;

	  //save arm data if necessary
	  armpixels = updateArmImage(cameraIndex,false,arm_tx,arm_ty,arm_bx,arm_by);
      if (saveArmData)
	  {
		if (armpixels > 5) 
		{
		  strcpy(filename,"c:\\develop\\segment\\training\\arm");
		  itoa(ArmFileIndex,buffer,10);
		  strcat(filename,buffer);
		  strcat(filename,".dat");
		  saveArmImage(filename);
		  ArmFileIndex++;
		  if (ArmFileIndex>999) ArmFileIndex=1;
		}
	  }

	  //estimate the position of the right arm
	  if (USE_NEURAL_CLASSIFIER)
		estimateArmPositionNeural(cameraIndex,false,armpixels);
	    else
        estimateArmPosition(cameraIndex,false,arm_tx,arm_ty,arm_bx,arm_by,shoulders_y);

	  //show the centre line
      for (y=shoulders_y;y<by;y++)
	  {
        
		if (centre_x-bodywidth>0)
		{
	      sourceImage->image[centre_x-bodywidth][y][0]=0;
		  sourceImage->image[centre_x-bodywidth][y][1]=255;
		  sourceImage->image[centre_x-bodywidth][y][2]=0;
		}

		if (centre_x-(bodywidth*3)>0)
		{
	      sourceImage->image[centre_x-(bodywidth*3)][y][0]=255;
		  sourceImage->image[centre_x-(bodywidth*3)][y][1]=255;
		  sourceImage->image[centre_x-(bodywidth*3)][y][2]=0;
		}

		if (centre_x+(bodywidth*3)<imageWidth)
		{
	      sourceImage->image[centre_x+(bodywidth*3)][y][0]=255;
		  sourceImage->image[centre_x+(bodywidth*3)][y][1]=255;
		  sourceImage->image[centre_x+(bodywidth*3)][y][2]=0;
		}

		if (centre_x+bodywidth<imageWidth)
		{
		  sourceImage->image[centre_x+bodywidth][y][0]=0;
		  sourceImage->image[centre_x+bodywidth][y][1]=255;
		  sourceImage->image[centre_x+bodywidth][y][2]=0;
		}
		
	  }

	}
	else
	{
	  //neutral posture
	  person->ShoulderElevation[LEFTSIDE] = 100;
	  person->ShoulderElevation[RIGHTSIDE] = 100;
	  person->ElbowAngle[RIGHTSIDE] = 0;
	  person->ElbowAngle[LEFTSIDE] = 0;
	}


	//if a face wasn't explicitly detected then estimate the head position
    if (!face_detected)
	{
      face_location[faceIndex][0] = centre_x - (bodywidth/2);
      face_location[faceIndex][1] = 4 + shoulders_y - (bodywidth);
      face_location[faceIndex][2] = centre_x + (bodywidth/2);
      face_location[faceIndex][3] = 4 + shoulders_y;
	}

    for (y=0;y<imageHeight;y++)
	{
	  for (x=0;x<imageWidth;x++)
	  {
	    //if this is the face region fill it in
	    if ((x>=face_location[faceIndex][0]) && (x<=face_location[faceIndex][2]) && (y>=face_location[faceIndex][1]) && (y<=face_location[faceIndex][3]))
		{
	      for (c=0;c<3;c++) cleanImage[cameraIndex]->image[x][y][c]=254;
		}
	  }
	}

  }
  else
  {
    face_location[faceIndex][0] = 0;
    face_location[faceIndex][1] = 0;
    face_location[faceIndex][2] = 1;
    face_location[faceIndex][3] = 1;
  }

}



void ChbpApp::estimateArmPosition(int cameraIndex, bool leftArm,int arm_tx,int arm_ty,int arm_bx,int arm_by,int shoulder_y)
{
  int x,xx,y,horizontal_extension_x,horizontal_extension_y,val,top_extension_x,top_extension_y,armwidth,aw,armlines;
  int horizontal_x,dx,dy,similarity_thresh,bottom_extension_x,bottom_extension_y,bottom_horizontal_x;
  long p,pixelPercent,w,h;
  bool found;
  int ShoulderElevation, ElbowAngle;


  ShoulderElevation = 90;
  ElbowAngle = 10;

  w = arm_bx-arm_tx;
  h = arm_by-arm_ty;
  if (w<5) w=5;
  if (h<5) h=5;

  //get the percentage of active pixels
  armwidth=0;
  armlines=0;
  p=0;
  for (x=arm_tx;x<arm_bx;x++)
  {
	aw=0;
    for (y=arm_ty;y<arm_by;y++)
	{
	  if ((x>0) && (x<imageWidth-1) && (y>0) && (y<imageHeight-1))
	    if (objectImage[cameraIndex]->image[x][y][0]>0) 
		{
		  p++;  //count active pixels
		  aw++; //count arm pixels
		}
	}

	//update the average vertical arm width and count 
	//the number of vertical lines containing arm pixels
	if (aw>0)
	{
	  armwidth += aw;
	  armlines++;
	}
  }
  //calculate the percentage pixel coverage between 0 and 100
  pixelPercent = (p * 100) / (w*h);

  //calculate the average vertical arm width
  if (armlines>0) armwidth /= armlines;

  if (arm_tx<1) arm_tx=1;
  if (arm_ty<1) arm_ty=1;
  if (arm_bx>=imageWidth-1) arm_bx=imageWidth-2;
  if (arm_by>=imageHeight-1) arm_by=imageHeight-2;

  horizontal_extension_y=arm_by;
  if (leftArm)
    horizontal_extension_x=arm_bx;
    else
	horizontal_extension_x=arm_tx;

  //detect the point at which extension from the body is greatest
  found=false;
  x=arm_tx;
  horizontal_x=100;
  while ((x<arm_bx) && (!found))
  {
	if (leftArm)
      xx = x;
	  else
	  xx = arm_tx + (arm_bx-x);

	val=0;
    for (y=arm_ty;y<arm_by;y++)
	{
	  if (objectImage[cameraIndex]->image[xx][y][0]==255)
	  {
	    val++;
		horizontal_extension_y = y;
	  }
	}

	if (val>1)
	{
	  found=true;
	  horizontal_extension_x = xx;
	  horizontal_x = (x-arm_tx)*100/w;
	}

    x++;
  }

  //detect the top point of the arm
  if (leftArm)
    top_extension_x = arm_bx;
    else
    top_extension_x = arm_tx;
  top_extension_y = shoulder_y;

  found=false;
  y=arm_ty;
  while ((y<arm_by) && (!found))
  {
	val=0;
	if (leftArm)
	{
      for (x=arm_tx;x<arm_bx;x++)
	    if (objectImage[cameraIndex]->image[x][y][0]==255)
		{
	      val++;
		  if (val==1) top_extension_x = x;
		}
	}
	else
	{
      for (x=arm_bx;x>arm_tx;x--)
	    if (objectImage[cameraIndex]->image[x][y][0]==255)
		{
	      val++;
		  if (val==1) top_extension_x = x;
		}
	}

	if (val>1)
	{
	  found=true;
	  top_extension_y = y;
	}

    y++;
  }



  //detect the bottom point of the arm
  if (leftArm)
    bottom_extension_x = arm_bx;
    else
    bottom_extension_x = arm_tx;
  bottom_extension_y = arm_by;

  found=false;
  y=arm_by;
  while ((y>arm_ty) && (!found))
  {
	val=0;
	if (leftArm)
	{
      for (x=arm_tx;x<arm_bx;x++)
	  {
	    if (objectImage[cameraIndex]->image[x][y][0]==255)
		{
	      val++;
		  if (val==1) 
		  {
		    bottom_extension_x = x;
			bottom_horizontal_x = (x-arm_tx)*100/w;
		  }
		}
	  }
	}
	else
	{
      for (x=arm_bx;x>arm_tx;x--)
	  {
	    if (objectImage[cameraIndex]->image[x][y][0]==255)
		{
	      val++;
		  if (val==1) 
		  {
		    bottom_extension_x = x;
			bottom_horizontal_x = (arm_bx-x)*100/w;
		  }
		}
	  }
	}

	if (val>1)
	{
	  found=true;
	  bottom_extension_y = y;
	}

    y--;
  }


  similarity_thresh = w/5;
  if (top_extension_y<shoulder_y)
  {
	//the hand is at or above shoulder level
    dx = abs(top_extension_x - horizontal_extension_x);
    dy = abs(top_extension_y - horizontal_extension_y);

    if ((dx<similarity_thresh) && (dy<similarity_thresh))
	{
      //if both points are the same then the hand must be at or above above shoulder level
      ShoulderElevation = (top_extension_y - shoulder_y)*100/h;
	  ElbowAngle = horizontal_x;
	}

	
    if ((dy>=similarity_thresh) && (top_extension_y<horizontal_extension_y))
	{
      //forearm vertical with hand moving closer to head
      ShoulderElevation = (horizontal_extension_y - shoulder_y)*100/h;
	  ElbowAngle = horizontal_x;
	}
	
  }
  else
  {
	if (leftArm)
	{
	  if ((armwidth<h/4) && (horizontal_x<70) && (bottom_horizontal_x<70))
	  {
		ShoulderElevation = (horizontal_extension_y - shoulder_y)*100/h;
	    ElbowAngle = 20 + ((bottom_extension_y - (h/3) - horizontal_extension_y)*100/h);
	  }
	  else
	  {
		ShoulderElevation = 80 - ((100-horizontal_x)/2) - (armwidth / h * 5);
        ElbowAngle = -10 - ((100-horizontal_x)/2) - (armwidth / h * 5);
	  }
	}
	else
	{
	  if ((armwidth<h/4) && (horizontal_x<70) && (bottom_horizontal_x<70))
	  {
		ShoulderElevation = (horizontal_extension_y - shoulder_y)*100/h;
	    ElbowAngle = 20 + ((bottom_extension_y - (h/3) - horizontal_extension_y)*100/h);
	  }
	  else
	  {
		ShoulderElevation = 80 - ((100-horizontal_x)/2) - (armwidth / h * 5);
        ElbowAngle = -10 - ((100-horizontal_x)/2) - (armwidth / h * 5);
	  }
		
	}

  }

  //if there are too many pixels or too few
  if ((pixelPercent<4) || (pixelPercent>50))
  {
    //neutral posture
    ShoulderElevation = 100;
    ElbowAngle = 0;
  }

  //update the position of the arm  
  person->updateArmPosition(leftArm,ShoulderElevation,ElbowAngle,0);
}




void ChbpApp::estimateArmPositionNeural(int cameraIndex, bool leftArm, int armpixels)
//classify arm position using topological map
{
  int x,y;

  if (armpixels > 5)
  {

    for (x=0;x<ARMIMAGE_WIDTH;x++)
	{
      for (y=0;y<ARMIMAGE_HEIGHT;y++)
	  {
	    if (armImage->image[x][y][0]>0)
 	      topmap->setInput(x,y,255);
	      else
		  topmap->setInput(x,y,0);
	  }
	}

    topmap->update();

    x = topmap->WinnerX;
    y = topmap->WinnerY;

    if (leftArm)
	{
	  person->ShoulderElevation[LEFTSIDE] = topmap->getClassificationMulti(x,y,0);
      person->ElbowAngle[LEFTSIDE] = topmap->getClassificationMulti(x,y,1);
	}
    else
	{
	  person->ShoulderElevation[RIGHTSIDE] = topmap->getClassificationMulti(x,y,0);
      person->ElbowAngle[RIGHTSIDE] = topmap->getClassificationMulti(x,y,1);
	}
  }
  else
  {
    if (leftArm)
	{
	  person->ShoulderElevation[LEFTSIDE] = 80;
	  person->ElbowAngle[LEFTSIDE] = -10;
	}
	else
    {
	  person->ShoulderElevation[RIGHTSIDE] = 80;
	  person->ElbowAngle[RIGHTSIDE] = -10;
	}
  }
}





int ChbpApp::updateArmImage(int cameraIndex, bool leftArm,int arm_tx,int arm_ty,int arm_bx,int arm_by)
//dump the arm region into an image which can then be classified
{
  int x,y,xx,yy,w,h,c;
  long pixels,p=0;

  w = arm_bx - arm_tx;
  h = arm_by - arm_ty;
  if (w<5) w=5;
  if (h<5) h=5;
  pixels = w*h;

  //clear the arm image
  for (x=0;x<ARMIMAGE_WIDTH;x++)
  {
    for (y=0;y<ARMIMAGE_HEIGHT;y++)
	  for (c=0;c<3;c++) armImage->image[x][y][c] = 0;
  }

  //populate the arm image
  for (x=arm_tx;x<arm_bx;x++)
  {
	xx = ((x-arm_tx) * ARMIMAGE_WIDTH) / w;

	//mirror the image if it is the right arm, so that the classifier works
	//the same way for either arm
    if (!leftArm) xx = ARMIMAGE_WIDTH - 1 - xx;

    for (y=arm_ty;y<arm_by;y++)
	{
	  yy = ((y-arm_ty) * ARMIMAGE_HEIGHT) / h;
  
	  if ((x>0) && (x<imageWidth-1) && (y>0) && (y<imageHeight-1) && (xx>=0) && (yy>=0) && (xx<ARMIMAGE_WIDTH) && (yy<ARMIMAGE_HEIGHT))
	  {
        if (objectImage[cameraIndex]->image[x][y][0]>0)
		{
		  for (c=0;c<3;c++) armImage->image[xx][yy][c] = 255;
		  p++;
		}
	  }
	}
  }

  return((int)(p * 100 / pixels));
}



void ChbpApp::saveArmImage(char *filename)
//saves the memory
{
  int x,y,dummy=0;
  FILE *fp;
 
  fp = fopen(filename, "w");

  if (fp != NULL)
  {
	//save dimensions of the image
    fprintf(fp,"%d\n",ARMIMAGE_WIDTH);
    fprintf(fp,"%d\n",ARMIMAGE_HEIGHT);

	//save image pixels
    for (x=0;x<ARMIMAGE_WIDTH;x++)
	{
      for (y=0;y<ARMIMAGE_HEIGHT;y++)
	  {		
		//note: this is a binary image - only 1 or 0 need be written
	    if (armImage->image[x][y][0]>0)
		  fprintf(fp,"%d\n",1);
		  else
		  fprintf(fp,"%d\n",0);
	  }
	}

    for (x=0;x<9;x++) fprintf(fp,"%d\n",dummy);

	fclose(fp);
  }
}



void ChbpApp::RChbp_enableLearning(bool state)
//enable or dissable learning of faces
{
  person->enableLearning = state;
}


void ChbpApp::detectSkin(int cameraIndex, bool useMotion)
//try to detect skin colours within the segmented image, based upon the
//average colour of the given face
{
  int tollerance,centre_x,centre_y,x,y,tx,ty,bx,by,c,tx2,ty2,bx2,by2,w,mode,min_w,w2,x1,x2,y1,y2;  
  static int prev_tx,prev_ty,prev_bx,prev_by;
  classimage *img;
  bool fillgaps = true;

  img = rawImage_small[cameraIndex];
  tollerance = (img->getAverageIntensity(0,0,imageWidth-1,imageHeight-1) * 25) / 100;
  skinImage[cameraIndex]->filterSkinColour(img,tollerance,centre_x,centre_y);

  if (useMotion)
  {
	motionseg->RCmseg_getBoundingBox(tx,ty,bx,by);
    tx = ((tx * imageWidth * 2) / SMALL_IMAGE_SIZE) - (imageWidth/10);
    ty = ((ty * imageHeight * 2) / SMALL_IMAGE_SIZE) - (imageHeight/10);
    bx = ((bx * imageWidth * 2) / SMALL_IMAGE_SIZE) + (imageWidth/10);
    by = ((by * imageHeight * 2) / SMALL_IMAGE_SIZE) + (imageWidth/5);

	if ((bx-tx>imageWidth/5) && (by-ty>imageHeight/7))
	{
	  tx2 = tx;
	  ty2 = ty;
	  bx2 = bx;
	  by2 = by;
	  if (tx2<0) tx2=0;
	  if (bx2>=imageWidth) bx2=imageWidth-1;
	  if (ty2<0) ty2=0;
	  if (by2>=imageHeight) by2=imageHeight-1;
	}
	else
	{
	  tx2 = prev_tx;
	  ty2 = prev_ty;
	  bx2 = prev_bx;
	  by2 = prev_by;
	}

    for (x=0;x<imageWidth;x++)
    {
	  for (y=0;y<imageHeight;y++)
	  {
		if (!((x>tx2) && (x<bx2) && (y>ty2) && (y<by2)))
		{
		  if (skinImage[cameraIndex]->image[x][y][0]>0)
		    for (c=0;c<3;c++) skinImage[cameraIndex]->image[x][y][c]=0;
		}
	    else
		{
	      if (skinImage[cameraIndex]->image[x][y][0]>0)
		    for (c=0;c<3;c++) skinImage[cameraIndex]->image[x][y][c] = rawImage_small[cameraIndex]->image[x][y][c];
		}
	  }
    }

    prev_tx = tx2;
    prev_ty = ty2;
    prev_bx = bx2;
    prev_by = by2;

  }

  //attempt to fill in any gaps
  if (fillgaps)
  {
	min_w = 1; //imageHeight/40;
	if (min_w < 1) min_w = 1;

	for (x=0;x<imageWidth-2;x++)
	{
	  
	  mode=-1;
	  w2=0;
	  w=0;
	  y1=-1;
      for (y=0;y<imageHeight;y++)
	  {
        if (mode==-1)
		{
		  if (skinImage[cameraIndex]->image[x][y][0]<2) mode=0;
		}

		if (mode==0)
		{
	      if (skinImage[cameraIndex]->image[x][y][0]>1)
		  { 
		    w++; 
		  }
		  else
		  {
			if (w > min_w)
			{
			  w=0;
			  mode=1; 
			  y1=y;
			  w2=0;
			}
		  }
		}

		if (mode==1)
		{
	      if (skinImage[cameraIndex]->image[x][y][0]<2)
		  { 
		    w2++; 
		  }
		  else
		  {
			mode=0;
			if ((w2 > min_w * 2) && (w2 < imageHeight/4))
			{
			  for (y2=y1;y2<y;y2++)
			  {
				for (c=0;c<3;c++)
				{
			      skinImage[cameraIndex]->image[x][y2][c] = rawImage_small[cameraIndex]->image[x][y2][c];
			      skinImage[cameraIndex]->image[x+1][y2][c] = rawImage_small[cameraIndex]->image[x+1][y2][c];
			      skinImage[cameraIndex]->image[x+2][y2][c] = rawImage_small[cameraIndex]->image[x+2][y2][c];
				}
			  }
			}
		  }
		}
	  }
	}



	min_w = imageWidth/50;
	if (min_w < 1) min_w = 1;

	for (y=1;y<imageHeight-1;y++)
	{
	  
	  mode=-1;
	  w2=0;
	  w=0;
	  x1=-1;
      for (x=0;x<imageWidth;x++)
	  {
        if (mode==-1)
		{
		  if (skinImage[cameraIndex]->image[x][y][0]<2) mode=0;
		}

		if (mode==0)
		{
	      if (skinImage[cameraIndex]->image[x][y][0]>1)
		  { 
		    w++; 
		  }
		  else
		  {
			if (w > min_w)
			{
			  w=0;
			  mode=1; 
			  x1=x;
			  w2=0;
			}
		  }
		}

		if (mode==1)
		{
	      if (skinImage[cameraIndex]->image[x][y][0]<2)
		  { 
		    w2++; 
		  }
		  else
		  {
			mode=0;
			if ((w2 > min_w * 3) && (w2 < imageWidth/4))
			{
			  for (x2=x1;x2<x;x2++)
			  {
				for (c=0;c<3;c++)
				{
  			      skinImage[cameraIndex]->image[x2][y][c] = 1; //rawImage_small[cameraIndex]->image[x2][y][c];
				  skinImage[cameraIndex]->image[x2][y+1][c] = 1; //rawImage_small[cameraIndex]->image[x2][y][c];
				}
			  }
			}
		  }
		}
	  }
	  


	  mode=-1;
	  w2=0;
	  w=0;
	  x1=-1;
      for (x=imageWidth-1;x>=0;x--)
	  {
        if (mode==-1)
		{
		  if (skinImage[cameraIndex]->image[x][y][0]<2) mode=0;
		}

		if (mode==0)
		{
	      if (skinImage[cameraIndex]->image[x][y][0]>1)
		  { 
		    w++; 
		  }
		  else
		  {
			if (w > min_w)
			{
			  w=0;
			  mode=1; 
			  x1=x;
			  w2=0;
			}
		  }
		}

		if (mode==1)
		{
	      if (skinImage[cameraIndex]->image[x][y][0]<2)
		  { 
		    w2++; 
		  }
		  else
		  {
			mode=0;
			if ((w2 > min_w * 3) && (w2 < imageWidth/4))
			{
			  for (x2=x1;x2>x;x2--)
			  {
				for (c=0;c<3;c++)
				{
				  if (skinImage[cameraIndex]->image[x2][y][0] == 1)
				  {
  			        skinImage[cameraIndex]->image[x2][y][c] = rawImage_small[cameraIndex]->image[x2][y][c];
				    skinImage[cameraIndex]->image[x2][y+1][c] = rawImage_small[cameraIndex]->image[x2][y+1][c];
				  }
				}
			  }
			}
		  }
		}
	  }

	}

	for (x=0;x<imageWidth;x++)
	{
      for (y=0;y<imageHeight;y++)
	  {
        if ((skinImage[cameraIndex]->image[x][y][0]>0) ||
		    (skinImage[cameraIndex]->image[x][y][0]==1) ||
			(skinImage[cameraIndex]->image[x][y][1]==1) ||
			(skinImage[cameraIndex]->image[x][y][2]==1))
		{
		  for (c=0;c<3;c++)
		  {
		    skinImage[cameraIndex]->image[x][y][c] = rawImage_small[cameraIndex]->image[x][y][c];
		  }
		}
	  }
	}
  }
}


void ChbpApp::detectHands(int cameraIndex)
//try to detect the position of hands
{
  int face_tx, face_ty, face_bx, face_by, face_cx, face_cy;
  int face_width,ww,hh,tx,ty,bx,by,w,h,cx,cy;
  int top_x,top_y,bottom_x,bottom_y;
  int vertical_diff,horizontal_diff,handSize;

  motionseg->RCmseg_getBoundingBox(tx,ty,bx,by);
  tx *= 2;
  ty *= 2;
  bx *= 2;
  by *= 2;
  w = bx-tx;
  h = by-ty;

  
  if ((w>0) && (w<imageWidth/2))
  {
    face_tx = face_location[0][0];
    face_ty = face_location[0][1];
    face_bx = face_location[0][2];
    face_by = face_location[0][3];

    ww = face_bx - face_tx;
    hh = face_by - face_ty;
    face_width = (ww*100)/imageWidth;

    //the face must be bigger than a minimum size
    if (face_width>20)
	{
      face_cx = face_tx + ((face_bx - face_tx)/2);
      face_cy = face_ty + ((face_by - face_ty)/2);

      //look for movement on the left hand side
	  top_x = 2;
	  top_y = 2;
	  bottom_x = face_cx - (face_bx - face_tx);
	  bottom_y = imageHeight-2;
	  cx = tx + (w/2);
	  cy = ty; //+ (h/4);
	  if ((bottom_x - top_x > (imageWidth/6)) && (cx<bottom_x))
	  {
		//difference between the head and hand positions
		vertical_diff = ((face_cy - cy)*100)/hh;
		horizontal_diff = ((face_cx - cx)*100)/ww;
		handSize = (((w+h)/2)*100)/imageWidth;
		if (handSize<face_width/2) handSize=face_width/2;

		person->updateArmPosition(true,(-vertical_diff/3)+20,(100-(horizontal_diff/3))+5,handSize);
        cleanImage[cameraIndex]->DrawBox(cx,cy,w,h,0);
		hand_detected = HAND_UP;
	  }

      //look for movement on the left hand side
	  top_x = face_cx + (face_bx - face_tx);
	  top_y = 2;
	  bottom_x = imageWidth-2;
	  bottom_y = imageHeight-2;
	  if ((bottom_x - top_x > (imageWidth/6)) && (cx>top_x))
	  {
		//difference between the head and hand positions
		vertical_diff = ((face_cy - cy)*100)/hh;
		horizontal_diff = ((face_cx - cx)*100)/ww;
		handSize = (((w+h)/2)*100)/imageWidth;
		if (handSize<face_width/2) handSize=face_width/2;

		person->updateArmPosition(false,(-vertical_diff/3)+20,-(100-(horizontal_diff/3))+5,handSize);
        cleanImage[cameraIndex]->DrawBox(cx,cy,w,h,0);
		hand_detected = HAND_UP;
	  }
	
	}
  }
}



void ChbpApp::RChbp_getMotionBox(int &tx, int &ty, int &bx, int &by, float &flow_x, float &flow_y)
//returns the bounding box for observed motion and average optical flow direction
{
  int mult_x,mult_y;

  if (rawImage_small[0]!=NULL)
  {
    mult_x = rawImage_small[0]->reductionFactor_x*2;
    mult_y = rawImage_small[0]->reductionFactor_y*2;
  
    flow_x = motionseg->RCmseg_getAverageFlow_x() * mult_x;
    flow_y = motionseg->RCmseg_getAverageFlow_y() * mult_y;
    motionseg->RCmseg_getBoundingBox(tx,ty,bx,by);
	tx *= mult_x;
	ty *= mult_y;
	bx *= mult_x;
	by *= mult_y;
  }
}



void ChbpApp::RChbp_updateImage(int x, int y, int width, int height, BYTE r, BYTE g, BYTE b)
{
  const int pts = width*height*3;

  if (bmp_unmanaged==NULL)
    bmp_unmanaged = new BYTE[pts];

  int p = ((width*y)+x)*3;
  
  if (p+2<pts)
  {
    bmp_unmanaged[p] = b;
    bmp_unmanaged[p+1] = g;
    bmp_unmanaged[p+2] = r;
  }
}


void ChbpApp::RChbp_update2(int cameraIndex, int width, int height)
{
  if (bmp_unmanaged!=NULL) update_base(cameraIndex,bmp_unmanaged,width,height);
}


void ChbpApp::RChbp_save(char *filename)
//save the classifier data
{
  if (person!=NULL) person->save(filename);
}


void ChbpApp::RChbp_load(char *filename)
//load the classifier data
{
  if (person!=NULL) person->load(filename);
}


