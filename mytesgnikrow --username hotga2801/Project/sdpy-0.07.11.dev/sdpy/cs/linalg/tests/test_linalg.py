import sys
from numpy.testing import *
from numpy import array

set_package_path()
# import xxx symbols
from sdpy.cs.linalg import symm_eig, ordered_eigh
restore_path()

#Optional:
set_local_path()
# import modules that are located in the same directory as this file.
restore_path()

class test_linalg(NumpyTestCase):
    def check_symm_eig(self, level=1, verbose=True):
        a = array([[1,2],[3,4],[5,6],[7,8]])
        s, v = symm_eig(a)
        assert_array_almost_equal(s,array([0.39291363,203.60708637]),verbose)
        assert_array_almost_equal(v,array([[-0.7671874,0.64142303],[0.64142303,0.7671874]]),verbose)
        s, v = symm_eig(a.T)
        assert_array_almost_equal(s,array([0.39291363,203.60708637]),verbose)
        assert_array_almost_equal(v,array([[0.82264747,0.15248323],[0.42137529,0.34991837],
            [0.0201031,0.54735351],[-0.38116908,0.74478865]]),verbose)


    def check_ordered_eigh(self, level=1, verbose=True):
        a = array([[1,2],[3,4]])
        s, v = ordered_eigh(a)
        assert_array_almost_equal(s,array([ 5.85410197, -0.85410197]),verbose)
        assert_array_almost_equal(v,array([[ 0.52573111, -0.85065081],[ 0.85065081,  0.52573111]]),verbose)

if __name__ == "__main__":
    NumpyTest().run()
    