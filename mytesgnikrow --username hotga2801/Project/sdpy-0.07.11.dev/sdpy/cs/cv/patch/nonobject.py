#!/usr/bin/env python

__all__ = ['NonObjectGenerator']

# import the necessary things
from dircache import listdir
from numpy import array
from os.path import exists, join, dirname
from random import randrange

from ....interfaces.opencv import cvLoadImage, CV_LOAD_IMAGE_GRAYSCALE, \
    cvReleaseImage, CV_64FC1, cvGetAffineTransform, cvWarpAffine, \
    CvPoint2D32f, cvCreateMat, cvReleaseMat, CV_INTER_LINEAR, \
    CV_WARP_FILL_OUTLIERS, CvScalar
from .... import tprint


class NonObjectGenerator:
    """A generator that generates non-object patches."""

    def __init__(self,database_path,maxCnt = 100):
        """Initialize the generator.

        :Parameters:
            database_path : string
                the path to the folder containing the non-object images
        """
        self.filenames = listdir(database_path)
        self.filenames = [join(database_path,x) for x in self.filenames]
        self.maxCnt = maxCnt
        self.mat = cvCreateMat(2,3,CV_64FC1)
        self.img = None
        self._reset()

    def __del__(self):
        self._clear()
        cvReleaseMat(self.mat)

    def _clear(self):
        if self.img is not None:
            cvReleaseImage(self.img)
            self.img = None

    def _reset(self):
        self._clear()
        afile = self.filenames[randrange(len(self.filenames))]
        self.img = cvLoadImage(afile,CV_LOAD_IMAGE_GRAYSCALE)
        self.cnt = self.maxCnt

    def _inc(self):
        if self.cnt <= 0:
            self._reset()
        else:
            self.cnt -= 1

    def generate(self,patch):
        """Fill in a patch of size N-by-N with a non-object patch.

        :Parameters:
            patch : POITNER(IplImage)
                a POINTER(IplImage) that points to a N-by-N uint8 IplImage. The
                patch is filled after calling generate()
        """
        self._inc()

        # get a random patch
        w = self.img[0].width
        h = self.img[0].height

        maxr = min(w,h)
        while True:
            x = randrange(w)
            y = randrange(h)
            r = randrange(16,maxr)
            if x-r >= 0 and x+r <= w and y-r >= 0 and y+r <= h:
                break

        # copy the patch over
        N = float(patch[0].height)
        C3 = CvPoint2D32f * 3
        src = C3()
        dst = C3()

        src[0].x = x
        src[0].y = y
        dst[0].x = N/2
        dst[0].y = N/2
        
        src[1].x = x+r
        src[1].y = y
        dst[1].x = N-1
        dst[1].y = N/2
        
        src[2].x = x
        src[2].y = y+r
        dst[2].x = N/2
        dst[2].y = N-1

        cvGetAffineTransform(src,dst,self.mat)
        cvWarpAffine(self.img,patch,self.mat,
            CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS,CvScalar(0,0,0,0))
