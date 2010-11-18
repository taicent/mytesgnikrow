#!/usr/bin/env python

__all__ = ['ImageView']

from math import sin, cos
from numpy import array, zeros

from ...interfaces.opencv import CvPoint2D32f, CV_64FC1, cvGetAffineTransform, \
    cvWarpAffine, cvCreateMat, cvReleaseMat, CV_INTER_LINEAR, \
    CV_WARP_FILL_OUTLIERS, cvSetIdentity, CvScalar, cvGetReal2D, \
    cvSetReal2D

from extension import imageview_transform_pairs, imageview_slide

class ImageView:
    """A rotated rectangular view of an image.

    :Parameters:
        iparam : array((3,),'i')
            iparam[0] : width of the output patch
            iparam[1] : height of the output patch
            iparam[2] : flipping flag
                Non-zero if in the view the direction from x-axis to
                    y-axis is counter-clockwise
                Zero if in the view the direction from x-axis to
                    y-axis is clockwise
        dparam : array((4,),'d')
            dparam[0] : angle of rotation from the image's x-axis to the view's
                x-axis, clockwise, (in radians)
            dparam[1] : scaling ratio, or the view's one unit length
                (in image pixels)
            dparam[2] : x-coordinate of the center point (in image pixels)
            dparam[3] : y-coordinate of the center point (in image pixels)
                the center point of the view on the image is geometrically
                transformed into location (0.5*w,0.5*h) on the output patch
    """

    def __init__(self,w = 10,h = 10,x0 = 0,y0 = 0,scale = 1,angle = 0,flip = False):
        """
        Initialize an image view.

        :Parameters:
            w : int
                width of the output patch
            h : int
                height of the output patch
            x0, y0 : double, double
                center point, location (x0,y0) on the image is geometrically
                transformed to location (0.5*w,0.5*h) on the output patch
            scale : double
                scaling ratio, or the view's one unit length (in image pixel)
            angle : double
                angle of rotation from the image's x-axis to the view's x-axis,
                clockwise, (in radian)
            flip : boolean
                to flip the view's y-axis or not
                Non-zero if in the view the direction from x-axis to
                    y-axis is counter-clockwise
                Zero if in the view the direction from x-axis to
                    y-axis is clockwise
        """
        self.map_matrix = cvCreateMat(2,3,CV_64FC1)

        self.iparam = array((w,h,flip),'i')
        self.dparam = array((angle, scale, x0, y0),'d')

    def __del__(self):
        cvReleaseMat(self.map_matrix)
        
    def slide(self, x=0, y=0):
        """Slide the view along its x-axis and y-axis in the image.
        
        :Parameters:
            x : double
                number of (patch) pixels to slide along the x-axis
            y : double
                number of (patch) pixels to slide along the y-axis
                
        :Returns:
            The dparam array is updated.
        """
        imageview_slide(self.iparam, self.dparam, float(x), float(y))
        

    def get_transformation_matrix(self):
        A = zeros(6)
        B = zeros(6)
        imageview_transform_pairs(self.iparam, self.dparam, A, B)
        C3 = CvPoint2D32f * 3
        src = C3((A[0],A[1]),(A[2],A[3]),(A[4],A[5]))
        dst = C3((B[0],B[1]),(B[2],B[3]),(B[4],B[5]))
        cvGetAffineTransform(src,dst,self.map_matrix)
        return self.map_matrix
