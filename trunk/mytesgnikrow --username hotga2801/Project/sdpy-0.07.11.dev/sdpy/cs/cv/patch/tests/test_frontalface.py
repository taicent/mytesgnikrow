import sys
from numpy.testing import *

set_package_path()
# import xxx symbols
from sdpy import tprint
from sdpy.cs.cv.patch import FrontalFaceGenerator
restore_path()

#Optional:
set_local_path()
# import modules that are located in the same directory as this file.
restore_path()

class test_frontalface(NumpyTestCase):
    def check_frontalface(self):
        from sdpy.interfaces import opencv
        ff = FrontalFaceGenerator('m:/facedata/frontal-face')
        tprint("Press a key to continue...")
        patch = opencv.cvCreateImage( opencv.CvSize(400,400), opencv.IPL_DEPTH_8U, 1 )
        opencv.cvNamedWindow('test_frontalface',opencv.CV_WINDOW_AUTOSIZE)

        for i in xrange(10):
            ff.generate(patch)
            opencv.cvShowImage('test_frontalface',patch)
            if opencv.cvWaitKey(100) > 0:
                break

        opencv.cvReleaseImage(patch)
        opencv.cvDestroyWindow('test_frontalface')

if __name__ == "__main__":
    NumpyTest().run()
