#!/usr/bin/env python

__all__ = ['FrontalFace_CDataGenerator']

# import the necessary things
from math import pi
from numpy import zeros
from os.path import join

from ... import arrayd
from .. import fromIplImage, integral
from ..patch import FrontalFaceGenerator, NonObjectGenerator
from ...ml.cla import CDataGenerator
from ....interfaces.opencv import cvCreateImage, CvSize, IPL_DEPTH_8U, \
    cvReleaseImage

class FrontalFace_CDataGenerator(CDataGenerator):
    """Generate face and non-face image patches of size N-by-N.
    """

    def __init__(self,database_path,N):
        CDataGenerator.__init__(self,(N*N,),2,'float64')
        self.N = N*2
        self.patch = cvCreateImage(CvSize(N,N),IPL_DEPTH_8U,1)
        self.patch_array = fromIplImage(self.patch)
        self.patch_vi = zeros((N,N))
        self.ffgenerator = FrontalFaceGenerator(join(database_path,'frontal-face'))
        self.nogenerator = NonObjectGenerator(join(database_path,'non-object'))
        
        self.cnt = 2
        self.toflip = True

    def __del__(self):
        cvReleaseImage(self.patch)

    def generate(self,j):
        """
        Generate a random face or non-face, vectorized-integrated.

        Input:
            j: class j
        Output:
            input_point: a point of class j.
        """
        j = int(j)
        if j < 0 or j > 1:
            raise IndexError('j must be 0 or 1')
        elif j > 0: # frontal face
            def perturb_func(iview):
                # randomly flip
                if self.cnt > 0:
                    self.cnt -= 1
                else:
                    self.cnt = 2
                    self.toflip = not self.toflip

                iview.flip = self.toflip
                if self.toflip:
                    iview.angle = pi

            self.ffgenerator.generate(self.patch,perturb_func)
            
        else: # non-face
            self.nogenerator.generate(self.patch)
        
        integral(self.patch_array,self.patch_vi)
        return self.patch_vi.ravel()

