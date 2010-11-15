/***************************************************************************
                          classflow.cpp 

    optical flow

    begin                : Sun Nov 24 2002
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

#include "classimage.h"
#include "classflow.h"

/// <summary>
/// Constructor
/// </summary>
classflow::classflow()
{
  int i,j;

  initialised=false;
  motion_threshold = 20;
  optical_flow_search_region = 5;
  optical_flow_patch = 3;

  //clear the optical flow history
  noflow=0;
  flowHistoryCtr=0;
  for (i=0;i<100;i++)
    for (j=0;j<7;j++) flowHistory[i][j]=0;
}


/// <summary>
/// Destructor
/// </summary>
classflow::~classflow()
{
}


/// <summary>
/// Initialise
/// </summary>
void classflow::init()
{
  int x,y;

  flow = new unsigned char**[width];
  for (x=0;x<width;x++)
  {
    flow[x] = new unsigned char*[height];
    for (y=0;y<height;y++)
      flow[x][y] = new unsigned char[2];
  }

  segmented = new unsigned char**[width];
  for (x=0;x<width;x++)
  {
    segmented[x] = new unsigned char*[height];
    for (y=0;y<height;y++)
      segmented[x][y] = new unsigned char[3];
  }

  createImage(width,height);

  initialised=true;
}



/// <summary>
/// Calculates optical flow
/// </summary>
/// <param name="img">A bitmap image</param>
/// <param name="wdth">Width of the bitmap</param>
/// <param name="hght">Height of the bitmap</param>
void classflow::update(unsigned char ***img, int wdth, int hght)
{
  int x,y,xx,yy,xxx,yyy,c;
  int closest_x,closest_y;
  int px,py,dx,dy;
  long closest,dp;
  long avflow_x,avflow_y,pixels;
  int bin_x,bin_y;

  width = wdth;
  height = hght;
  if (initialised==false) init();

  //enable secondary update of motion
  motion->enableSecondaryUpdate=true;

  //variables used to calculate average flow over the entire image
  avflow_x=0;
  avflow_y=0;
  pixels=1;

  if (motion->averageIntensity >= 0)
  {

    //clear the flow histogram votes and boxes
    flowHistogramMax=1;
    for (x=0;x<14;x++)
    {
      for (y=0;y<14;y++)
        flowHistogram[x][y]=0;
    }


    //step through the image
    //note that increasing the step size for x and y increases the speed (fewer calculations)
    for (x = 0;x<width;x+=1)
    {
      for (y = 0;y<height;y+=1)
      {
        for (c=0;c<3;c++) image[x][y][c]=0;

        //only examine areas of the image where some movement has taken place
        if ((motion->image[x][y][0] > motion_threshold) 
			&& (x>optical_flow_search_region) && (x<width-optical_flow_search_region) 
			&& (y>optical_flow_search_region) && (y<height-optical_flow_search_region))
		{
          //for each point examine a small surrounding region
          closest_x = x;
          closest_y = y;
          closest=99999;
          for (xx=x-optical_flow_search_region;xx<x+optical_flow_search_region;xx+=2)
          {
            if ((xx>0) && (xx<width-1))
            {
              for (yy=y-optical_flow_search_region;yy<y+optical_flow_search_region;yy+=2)
			  {
                if ((yy>0) && (yy<height-1))
				{

                  //compare the patch in the current image to that in the previous one
                  dp=0;
                  for (xxx=xx-optical_flow_patch;xxx<xx+optical_flow_patch;xxx++)
				  {
                    if ((xxx>0) && (xxx<width-1))
                    {
                      for (yyy=yy-optical_flow_patch;yyy<yy+optical_flow_patch;yyy++)
                      {
                        if ((yyy>0) && (yyy<height-1))
                        {
                          px = x + (xxx-xx);
                          py = y + (yyy-yy);
                          if ((px>=0) && (px<width-1) && (py>=0) && (py<height-1))
						  {
                            for (c=0;c<3;c++)
                              dp += abs(img[xxx][yyy][c] - motion->image2[px][py][c]);
                          }
                        }
                      }
                    }
                  }

                  //if this patch is the closest match then take note of its position
                  if (dp<closest)
                  {
                    closest = dp;
                    closest_x = xx;
                    closest_y = yy;
                  }

                }
              }
            }
          }

          //get the displacement
          dx = x - closest_x;
          dy = y - closest_y;

          //update average flow figures
          avflow_x = avflow_x + dx;

          avflow_y = avflow_y + dy;
          pixels++;

          //squash the displacement values into a 0-255 range so that it can be stored in a byte array
          flow[x][y][IMAGE_FLOW_X] = (unsigned char)((255 * (dx + optical_flow_search_region)) / (2 * optical_flow_search_region));
          flow[x][y][IMAGE_FLOW_Y] = (unsigned char)((255 * (dy + optical_flow_search_region)) / (2 * optical_flow_search_region));

          image[x][y][0] = flow[x][y][IMAGE_FLOW_X];
          image[x][y][1] = flow[x][y][IMAGE_FLOW_Y];
		  image[x][y][2] = 0;

          xx = x+dx;
          yy = y+dy;
          if ((xx>0) && (xx<width) && (yy>0) && (yy<height))
            img[xx][yy][0] = img[x][y][0];

          //update the flow histogram
          bin_x = flow[x][y][IMAGE_FLOW_X] / 20;
          bin_y = flow[x][y][IMAGE_FLOW_Y] / 20;
          flowHistogram[bin_x][bin_y]++;
          if (flowHistogram[bin_x][bin_y]>flowHistogramMax) flowHistogramMax=flowHistogram[bin_x][bin_y];
        }
      }
    }
  }

  //perform secondary update of motion object
  motion->update_secondary(img);
  
  average_flow_x = (float)(avflow_x / pixels);
  average_flow_y = (float)(avflow_y / pixels);

}


/// <summary>
/// Calculates optical flow
/// </summary>
/// <param name="img">Image object</param>
void classflow::update(classimage *img)
{
  update(img->image,img->width,img->height);
}



/// <summary>
/// segment the image based upon optical flow
/// </summary>
/// <param name="img">Image object</param>
/// <param name="distance">Distance</param>
void classflow::segment(classimage *img, int distance)
{
  int x,y,xx,disp,disp2,c,i,j;
  int start_pos,hits,pixels;
  int max_segwidth = width/2;
  long cx;

  for (y=1;y<height-1;y++)
  {
    for (x=0;x<width;x++)
	{
	  segmented[x][y][0]=0;
	  segmented[x][y][1]=0;
	}
  }
  
  boundingbox_tx=width;
  boundingbox_bx=0;
  boundingbox_ty=height;
  boundingbox_by=0;

  pixels=1;
  cx=0;
  for (y=0;y<height;y++)
  {
	start_pos=-1;
	disp=0;
    for (x=1;x<width-1;x++)
	{
	  if (motion->image[x][y][0]>10) hits++;

	  if ((((image[x][y][0]>0) && (image[x-1][y][0]==0)) || ((image[x][y][0]==0) && (image[x-1][y][0]>0)))
		  //(((image[x][y][0]>0) && (image[x-1][y-1][0]==0)) || ((image[x][y][0]==0) && (image[x-1][y-1][0]>0))) ||
		  //(((image[x][y][0]>0) && (image[x-1][y+1][0]==0)) || ((image[x][y][0]==0) && (image[x-1][y+1][0]>0)))
		  )
	  {

          if (x<boundingbox_tx) boundingbox_tx=x;
          if (x>boundingbox_bx) boundingbox_bx=x;
          if (y<boundingbox_ty) boundingbox_ty=y;
          if (y>boundingbox_by) boundingbox_by=y;


		if ((start_pos>-1) && (hits>-1) && (x-start_pos<max_segwidth))
		{
		  disp2 = (disp + image[x][y][0])/2;
		  disp = image[x][y][1];
		  for (xx=start_pos;xx<x;xx++)
		  {
		    segmented[xx][y][0]=255;
			segmented[xx][y][1]=disp2;
	        pixels++;
			cx += xx;
		  }

		}
		start_pos=x;
		hits=0;
      
	  }
	}
  }

  
  for (x=0;x<width;x++)
  {
	start_pos=-1;
	disp=0;
    for (y=1;y<height-1;y++)
	{
	  if (motion->image[x][y][0]>10) hits++;

	  if (((image[x][y][0]>0) && (image[x][y-1][0]==0)) || ((image[x][y][0]==0) && (image[x][y-1][0]>0)))
	  {
		if ((start_pos>-1) && (hits>-1) && (y-start_pos<max_segwidth))
		{
		  disp2 = (disp + image[x][y][0] + segmented[x][y][1])/3;
		  disp = image[x][y][1];
		  for (xx=start_pos;xx<y;xx++) 
		  {
		    segmented[x][xx][0]=255;
			segmented[x][xx][1]=disp2;
	        pixels++;
			cx += x;
		  }

		}
		start_pos=y;		
		hits=0;
      
	  }
	}
  }
  


  for (y=boundingbox_ty;y<boundingbox_by-1;y++)
  {
    for (x=boundingbox_tx;x<boundingbox_bx;x++)
	{
      for (c=0;c<3;c++)
	  {
	    if (segmented[x][y][0]>0)
		  image[x][y][c] = img->image[x][y][c];
		  else
		  image[x][y][c] = img->image[x][y][c]/3;
	  }
    }
  }


  if ((pixels>1) && (boundingbox_bx>boundingbox_tx+1) && (boundingbox_by>boundingbox_ty+1))
  {
    centre_x = (centre_x + ((cx/pixels) + (boundingbox_tx + ((boundingbox_bx-boundingbox_tx)/2)))/2)/2;
    centre_y = boundingbox_ty + ((boundingbox_by-boundingbox_ty)/2);
    aspectRatio = (boundingbox_by-boundingbox_ty) / (float)(boundingbox_bx-boundingbox_tx) * width / height;
  }
  else
  aspectRatio = 0;


  //store the optical flow history, for possible gesture recognition
  if ((pixels>1) && ((abs((int)average_flow_x)>=1) || (abs((int)average_flow_y)>=1)))
  {
	noflow=0;
    flowHistory[flowHistoryCtr][0] = (int)average_flow_x;
	flowHistory[flowHistoryCtr][1] = (int)average_flow_y;
	flowHistory[flowHistoryCtr][2] = boundingbox_tx;
	flowHistory[flowHistoryCtr][3] = boundingbox_ty;
	flowHistory[flowHistoryCtr][4] = boundingbox_bx;
	flowHistory[flowHistoryCtr][5] = boundingbox_by;
	flowHistory[flowHistoryCtr][6] = distance;

    flowHistoryCtr++;
	if (flowHistoryCtr>99) flowHistoryCtr=0;
  }
  else
  {
	//if there has been no movement for a while then erase the optical flow history
    noflow++;
	if (noflow==10)
	{
      flowHistoryCtr=0;
      for (i=0;i<100;i++)
        for (j=0;j<6;j++) flowHistory[i][j]=0;
	}
	if (noflow>11) noflow=11;
  }

}



/// <summary>
/// Update stereo disparity - this function is not presently used
/// </summary>
/// <param name="f">Optical flow object</param>
void classflow::updateDisparity(classflow *f)
{
  int x,y,max;

  averageDisparity = abs(centre_x - f->centre_x) * 4;
  if (averageDisparity > 255) averageDisparity=255;

  //show a bar indicating distance from the object
  max=(height*averageDisparity)/255*1;
  if (max>height-1) max=height-1;
  for (y=0;y<height;y++)
  {
    for (x=0;x<width/20;x++)
	{
	  if (y<max)
	  {
	    image[x][y][0]=0;
	    image[x][y][1]=255;
	    image[x][y][2]=0;
	  }
	  else
	  {
	    image[x][y][0]=0;
	    image[x][y][1]=0;
	    image[x][y][2]=0;
	  }
	  
	}
  }

}



/// <summary>
/// match the flow with another image - this function is not presently used
/// </summary>
/// <param name="img">Image object</param>
/// <param name="rawimgLeft">Object storing the left image</param>
/// <param name="rawimgRight">Object storing the right image</param>
void classflow::matchFlow(classimage *img, classimage *rawimgLeft, classimage *rawimgRight)
{
  int x,y,p1[2],p2[2],r1[3],xx,yy,c;
  int max_disparity = width/4;
  int similarity,min,disp;

  for (y=1;y<height-1;y++)
  {
    for (x=0;x<width;x++)
	{
	  //get the flow vector in the left image
      p1[0] = image[x][y][0];
      p1[1] = image[x][y][1];
	  for (c=0;c<3;c++) r1[c] = rawimgLeft->image[x][y][c];
	  if (p1[0]>0)
	  {
		min=99999;
		disp=0;
        for (xx = x-max_disparity;xx<x+max_disparity;xx++)
		{
		  if ((xx>0) && (xx<width-1))
		  {
            for (yy=y-1;yy<y+1;yy++)
			{
			  //get the flow vector in the right image
		      p2[0] = img->image[xx][yy][0];
			  p2[1] = img->image[xx][yy][1];
			  if (p2[0]>0)
			  {
                //compare the optical flow vectors
				similarity = abs(p1[0]-p2[0]) + abs(p1[1]-p2[1]);

				//include some colour information to improve matching
				for (c=0;c<3;c++) similarity += abs(r1[c] - rawimgRight->image[xx][yy][c])/2;

				//bias depending upon distance from the current x position
				//this prevents a lot of spurious high disparity matches by making
				//smaller disparities more probable
				similarity += abs(xx-x)*1;

				//square
				similarity *= similarity;

                //if the match is close store the disparity value
				if (similarity<min)
				{
			      min=similarity;
				  disp = abs(xx-x);
				}
			  }
			}
		  }
		}


        disp = 10+(disp*4);
		if (disp>255) disp=255;

		image[x][y][0]=disp;
		image[x][y][1]=disp;
		image[x][y][2]=disp;
		segmented[x][y][1]=disp;

	  }


	}
  }
}



/// <summary>
/// Returns the optical flow history
/// </summary>
/// <param name="t">Time</param>
/// <param name="index">Index</param>
int classflow::getFlowHistory(int t, int index)
{
  int i;
  
  if (t>99) t=99;
  i = flowHistoryCtr - t;
  if (i<0) i += 100;

  return(flowHistory[i][index]);
}