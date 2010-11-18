import sys
from numpy.testing import *
from numpy import array

set_package_path()
# import xxx symbols
from sdpy import tprint
from sdpy.cs.dm import *
restore_path()

#Optional:
set_local_path()
# import modules that are located in the same directory as this file.
restore_path()

class test_dm(NumpyTestCase):
    def test_argsort(self, level=1):
        a = array([3,1,2,4])
        assert_array_almost_equal(argsort(a),array([1,2,0,3]))

    def test_argsort_and_cut(self, level=1):
        a = array([1.2,3,1.2,4,1.2,3])
        b, c = argsort_and_cut(a)
        assert_array_almost_equal(b,array([0,2,4,1,5,3]))
        assert_array_almost_equal(c,array([0.2,2.1,3.5,5]))


if __name__ == "__main__":
    NumpyTest().run()
    
