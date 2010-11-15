// motionsegment.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "classfilter.h"
#include "motionsegment.h"
#include "classmotion.h"
#include "classflow.h"


classfilter *filter=NULL;
classimage *sourceImage=NULL;
classimage *sourceImage2=NULL;
classmotion *motion=NULL;
classflow *flow=NULL;
int currDist=0;
int disparity=0;
float currActivity=0;
long **testresult;



/////////////////////////////////////////////////////////////////////////////
// classMotionSegment construction

classMotionSegment::classMotionSegment()
{
  testresult=NULL;
}


classMotionSegment::~classMotionSegment()
{
  if (sourceImage!=NULL)
  {
	delete filter;
	delete sourceImage;
    delete motion;
    delete flow;
	delete [] testresult;
  }

  if (sourceImage2!=NULL) delete sourceImage2;
}





void classMotionSegment::RCmseg_update(classimage *img)
{
  update(img);
}



void classMotionSegment::update(classimage *img)
{
  if (img!=NULL)
  {
    if (sourceImage==NULL)
	{
	  sourceImage = new classimage;
	  sourceImage->createImage(img->width/2,img->height/2);
	  sourceImage->reductionFactor_x = 2;
	  sourceImage->reductionFactor_y = 2;
	  filter = new classfilter;
      motion = new classmotion;
      flow = new classflow;
	  flow->motion = motion;
	}

    sourceImage->updateFromImage(img);

    filter->update(sourceImage);
    motion->update(filter);
    flow->update(filter);
    flow->segment(filter,currDist);
  }
}



void classMotionSegment::RCmseg_getFeature(int index, int &fx, int &fy, int &featureType, int &featureWidth, int &featureHeight)
{
  
  if (sourceImage->topFeature!=NULL)
  {
    fx = (int)(sourceImage->topFeature[index][1]);
    fy = (int)(sourceImage->topFeature[index][2]);
    featureType = (int)(sourceImage->topFeature[index][3]);
    featureWidth = (int)(sourceImage->topFeature[index][4]);
    featureHeight = (int)(sourceImage->topFeature[index][5]);  
  }

  /*
  else
  {
  
    fx=0;
	fy=0;
	featureType=0;
	featureWidth=1;
	featureHeight=1;
  }
  */
}



void classMotionSegment::getCoefficients(classimage *img, int tx, int ty, int bx, int by, int *values)
{
  int p,p2,w,h;

  w = (bx-tx)/3;
  h = (by-ty)/3;

  p = img->getIntegral(tx+w,ty+h,tx+w+w,ty+h+h,0);
  p2 = img->getIntegral(tx,ty,tx+w,ty+h,0);
  values[0] = abs(p-p2);

  p2 = img->getIntegral(tx+w,ty,tx+w+w,ty+h,0);
  values[1] = abs(p-p2);

  p2 = img->getIntegral(bx-w,ty,bx,ty+h,0);
  values[2] = abs(p-p2);

  p2 = img->getIntegral(tx,ty+h,tx+w,ty+h+h,0);
  values[3] = abs(p-p2);

  p2 = img->getIntegral(bx-w,ty+h,bx,ty+h+h,0);
  values[4] = abs(p-p2);

  p2 = img->getIntegral(tx,by-h,tx+w,by,0);
  values[5] = abs(p-p2);

  p2 = img->getIntegral(tx+w,by-h,tx+w+w,by,0);
  values[6] = abs(p-p2);

  p2 = img->getIntegral(bx-w,by-h,bx,by,0);
  values[7] = abs(p-p2);
}







unsigned char classMotionSegment::RCmseg_getPixel(int x, int y, int col)
{
  if (flow!=NULL)
    return(flow->image[x][y][col]);
    else
	return(0);
}



int classMotionSegment::RCmseg_getFlowHistogram(int x, int y)
{
  if (flow!=NULL)
    return(flow->flowHistogram[x][y]);
    else
	return(0);
}



float classMotionSegment::RCmseg_getAverageFlow_x()
{
  if (flow!=NULL)
    return(flow->average_flow_x);
    else
	return(0);
}


float classMotionSegment::RCmseg_getAverageFlow_y()
{
  if (flow!=NULL)
    return(flow->average_flow_y);
    else
	return(0);
}



float classMotionSegment::RCmseg_getAspectRatio()
{
  if (flow!=NULL)
    return(flow->aspectRatio);
    else
	return(0);
}





void classMotionSegment::RCmseg_getBoundingBox(int &tx, int &ty, int &bx, int &by)
{
  if (flow!=NULL)
  {
    tx = flow->boundingbox_tx;
    ty = flow->boundingbox_ty;
    bx = flow->boundingbox_bx;
    by = flow->boundingbox_by;
  }
}



int classMotionSegment::RCmseg_getFlowHistory(int t, int index)
{
  if (flow!=NULL)
    return(flow->getFlowHistory(t,index));
    else
	return(0);
}



float classMotionSegment::RCmseg_faceProbability()
{
  int tx,ty,bx,by,w,h;
  static float prob=0;
  long average_illumination,r,g,b,skinTone,pixels;

  if (flow!=NULL)
  {
    tx = flow->boundingbox_tx;
    ty = flow->boundingbox_ty;
    bx = flow->boundingbox_bx;
    by = flow->boundingbox_by;
	w = bx-tx;
	h = by-ty;

	//check that the aspect ratio is roughly square
	if ((w>2) && (w < flow->width/2) && (h < flow->height/2) && (flow->aspectRatio > 0.8) && (flow->aspectRatio < 1.5))
	{
	  //find the average illumination
      average_illumination = flow->getIntegral(0,0,flow->width-1,flow->height-1,0);
      average_illumination /= ((flow->width -1) * (flow->height -1));

	  //if (average_illumination > 0)
	  {
		pixels= w*h;
	    r = flow->getIntegral(tx,ty,bx,by,0) / pixels;
		g = flow->getIntegral(tx,ty,bx,by,2) / pixels;
		b = flow->getIntegral(tx,ty,bx,by,3) / pixels;

		skinTone = (2*r) - g - b;
		if ((skinTone > 0) && (skinTone <480))
		{
		  prob = (float)(((skinTone + (disparity/(flow->width*2)))*0.2) + (prob*0.8));
		}
	  }
	}
  }

  prob = prob * (float)0.9;
  if (prob<0.1) prob=0;

  return(prob);
}




float classMotionSegment::RCmseg_personProbability()
{
  int tx,ty,bx,by,w,h;
  static float prob=0;
  float personProb;

  if (flow!=NULL)
  {
    tx = flow->boundingbox_tx;
    ty = flow->boundingbox_ty;
    bx = flow->boundingbox_bx;
    by = flow->boundingbox_by;
	w = bx-tx;
	h = by-ty;

	//check that the aspect ratio is roughly square
	if ((w>2) && (w < flow->width/2) && (h > flow->height/2) && (flow->aspectRatio > 1.5))
	{
	  personProb = (flow->aspectRatio - (float)1.5);
	  prob = (float)((personProb*0.2) + (prob*0.8));
	}

  }

  prob = prob * (float)0.9;
  if (prob<0.1) prob=0;
  return(prob);
}



float classMotionSegment::RCmseg_activityLevel()
{
  int tx,ty,bx,by,w,h,pixels;

  if (flow!=NULL)
  {
	if ((flow->average_flow_x!=0) || (flow->average_flow_y!=0))
	{
      tx = flow->boundingbox_tx;
      ty = flow->boundingbox_ty;
      bx = flow->boundingbox_bx;
      by = flow->boundingbox_by;
      w = bx-tx;
      h = by-ty;

      pixels = flow->width * flow->height;
      currActivity = (float)((currActivity * 0.8) + (((w*h)/(float)pixels)*0.5));
	}
	else
	currActivity=(float)(currActivity*0.8);

    if (currActivity<0.01) currActivity=0;
  }

  return(currActivity);
}