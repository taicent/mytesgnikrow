import sys
from numpy.testing import *
from numpy import arange, zeros

set_package_path()
# import xxx symbols
from sdpy.cs.stats import *
from sdpy.cs.ml.cla import ToyImbalancedDataGenerator, WeightedCDataset
restore_path()

#Optional:
set_local_path()
# import modules that are located in the same directory as this file.
restore_path()

class test_dataset(NumpyTestCase):
    def check_none(self, level=1):
        self.warn('This module still needs more testing.')

    def check_Stats2(self, level=1):
        a = ToyImbalancedDataGenerator()
        b = [a.generate_set(0,100),a.generate_set(1,10)]
        w = [arange(100)+1.0,arange(10)+1.0]
        s2 = WeightedCDataset(b,w).compute_Stats2()
        AA = s2.A
        s2.A = zeros(s2.A.shape)
        for i in xrange(100):
            s2.learn(b[0][i],0,i+1.0)
        for i in xrange(10):
            s2.learn(b[1][i],1,i+1.0)
        assert_array_almost_equal(AA,s2.A)

    def check_Stats2e(self, level=1):
        a = ToyImbalancedDataGenerator()
        b = [a.generate_set(0,100),a.generate_set(1,10)]
        w = [arange(100)+1.0,arange(10)+1.0]
        s2 = WeightedCDataset(b,w).compute_Stats2e()
        AA = s2.A
        s2.A = zeros(s2.A.shape)
        for i in xrange(100):
            s2.learn(b[0][i],0,i+1.0)
        for i in xrange(10):
            s2.learn(b[1][i],1,i+1.0)
        assert_array_almost_equal(AA,s2.A)

    def check_Stats2m(self, level=1):
        a = ToyImbalancedDataGenerator()
        b = [a.generate_set(0,100),a.generate_set(1,10)]
        w = [arange(100)+1.0,arange(10)+1.0]
        s2 = WeightedCDataset(b,w).compute_Stats2m()
        AA = s2.A
        s2.A = zeros(s2.A.shape)
        for i in xrange(100):
            s2.learn(b[0][i],0,i+1.0)
        for i in xrange(10):
            s2.learn(b[1][i],1,i+1.0)
        assert_array_almost_equal(AA,s2.A)

if __name__ == "__main__":
    NumpyTest().run()
    