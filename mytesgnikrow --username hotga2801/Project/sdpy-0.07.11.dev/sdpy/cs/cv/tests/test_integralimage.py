import sys
from numpy.testing import *
from numpy import arange, array, ones, zeros

set_package_path()
from sdpy.cs import arrayd
from sdpy.cs.cv import *
# import xxx symbols
restore_path()

#Optional:
set_local_path()
# import modules that are located in the same directory as this file.
restore_path()

class test_integralimage(NumpyTestCase):
    def check_integral(self, level=1):
        N = 4
        a = arange(N*N,dtype='uint8').reshape(N,N)
        b = zeros([N,N])
        integral(a,b)
        assert_array_almost_equal(b,arrayd(((0,1,3,6),(4,10,18,28),(12,27,45,66),(24,52,84,120))))
        
    def check_make_vi(self, level=1):
        a = array([[[1,2],[3,4]],[[2,2],[3,4]]],'uint8')
        b = make_vi(a)
        assert_array_almost_equal(b,arrayd(((1,3,4,10),(2,4,5,11))))
        
    
if __name__ == "__main__":
    NumpyTest().run()
    