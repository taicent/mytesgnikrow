import sys
from math import pi
from numpy.testing import *
from os.path import join

set_package_path()
from sdpy.cs import arrayd
from sdpy.cs.cv import *
# import xxx symbols
restore_path()

#Optional:
set_local_path()
# import modules that are located in the same directory as this file.
restore_path()

class test_imageview(NumpyTestCase):
    def check_simple(self, level=1):
        a = ImageView(20,20,10,10)
        b = a.get_transformation_matrix()
        c = fromCvMat(b)
        assert_array_almost_equal(c,arrayd(((1,0,0),(0,1,0))))
        
        a = ImageView(100,200,100,100,1,0,True)
        b = a.get_transformation_matrix()
        c = fromCvMat(b)
        assert_array_almost_equal(c,arrayd(((1,0,-50),(0,-1,200))))
        

    def check_scale(self, level=1):
        a = ImageView(10,20,30,40,2,0)
        b = a.get_transformation_matrix()
        c = fromCvMat(b)
        assert_array_almost_equal(c,arrayd(((0.5,0,-10),(0,0.5,-10))))

    def check_angle(self, level=1):
        a = ImageView(20,10,0,0,2,pi/2)
        b = a.get_transformation_matrix()
        c = fromCvMat(b)
        assert_array_almost_equal(c,arrayd(((0,0.5,10),(-0.5,0,5))))
        
    def check_hard(self, level=1):
        a = ImageView(1,2,3,4,5,pi/2,True)
        b = a.get_transformation_matrix()
        c = fromCvMat(b)
        assert_array_almost_equal(c,arrayd(((0,0.2,-0.3),(0.2,0,0.4))))

    def check_slide(self, level=1):
        a = ImageView(angle=pi/2,flip=True)
        a.slide(0,1)
        assert_array_almost_equal(a.dparam,arrayd((pi/2,1,1,0)))
        a.slide(1,-1)
        assert_array_almost_equal(a.dparam,arrayd((pi/2,1,0,1)))

if __name__ == "__main__":
    NumpyTest().run()
