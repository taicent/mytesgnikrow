import sys
from numpy.testing import *

set_package_path()
# import xxx symbols
restore_path()

#Optional:
set_local_path()
# import modules that are located in the same directory as this file.
restore_path()

class test_frontalface(NumpyTestCase):
    def check_all(self, level=1):
        self.warn('This module still needs testing.')

##    def check_notworking(self):
##        from sdpy.interfaces import opencv
##        ff = FrontalFaceGenerator('m:/facedata/frontal-face')
##        patch = opencv.cvCreateImage( opencv.CvSize(400,400), opencv.IPL_DEPTH_8U, 1 )
##        opencv.cvNamedWindow('test',opencv.CV_WINDOW_AUTOSIZE)
##
##        while opencv.cvWaitKey(100) < 0:
##            ff.generate(patch)
##            opencv.cvShowImage('test',patch)
##
##        cvReleaseImage(patch)
##
if __name__ == "__main__":
    NumpyTest().run()
