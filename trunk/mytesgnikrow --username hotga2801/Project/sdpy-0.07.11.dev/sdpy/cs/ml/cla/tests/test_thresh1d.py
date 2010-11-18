import sys
from numpy.testing import *
from numpy import array

set_package_path()
# import xxx symbols
from sdpy.cs.ml.cla import WeightedCDataset, histogram_1d
restore_path()

#Optional:
set_local_path()
# import modules that are located in the same directory as this file.
restore_path()

class test_thresh1d(NumpyTestCase):
    def check_all(self, level=1):
        print "Warning: this thresh1d still needs more testing functions"
        
    def check_histogram_1d(self, level=1):
        a = [array([1.0,2,3,4,5],'d'),
            array([1,1.0,2,2,3,4,5],'d')]
        w = [array([1.0,1,1,1,1],'d'),
            array([1,1.0,1,1,1,1,1],'d')]

        wcd = WeightedCDataset(a,w)
        hist, bins = histogram_1d(wcd, 10)
        assert_array_almost_equal( hist,
            array([[ 0.2       ,  0.        ,  0.2       ,  0.        ,  0,
        0.2       ,  0.        ,  0.2       ,  0.        ,  0.2       ],
      [ 0.28571429,  0.        ,  0.28571429,  0.        ,  0.        ,
        0.14285714,  0.        ,  0.14285714,  0.        ,  0.14285714]] ))
        assert_array_almost_equal(bins, array([[ 1. ,  1.4],
          [ 1.4,  1.8],
          [ 1.8,  2.2],
          [ 2.2,  2.6],
          [ 2.6,  3. ],
          [ 3. ,  3.4],
          [ 3.4,  3.8],
          [ 3.8,  4.2],
          [ 4.2,  4.6],
          [ 4.6,  5. ]]))

        wcd = WeightedCDataset(a)
        hist, bins = histogram_1d(wcd, 10)
        assert_array_almost_equal( hist,
            array([[ 0.2       ,  0.        ,  0.2       ,  0.        ,  0,
        0.2       ,  0.        ,  0.2       ,  0.        ,  0.2       ],
      [ 0.28571429,  0.        ,  0.28571429,  0.        ,  0.        ,
        0.14285714,  0.        ,  0.14285714,  0.        ,  0.14285714]] ))
        assert_array_almost_equal(bins, array([[ 1. ,  1.4],
          [ 1.4,  1.8],
          [ 1.8,  2.2],
          [ 2.2,  2.6],
          [ 2.6,  3. ],
          [ 3. ,  3.4],
          [ 3.4,  3.8],
          [ 3.8,  4.2],
          [ 4.2,  4.6],
          [ 4.6,  5. ]]))


if __name__ == "__main__":
    NumpyTest().run()
