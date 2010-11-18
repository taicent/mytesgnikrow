import sys
from numpy.testing import *
from numpy import arange, zeros

set_package_path()
# import xxx symbols
from sdpy import tprint
from sdpy.cs.ml.cla import ToyBalancedDataGenerator, WeightedCDataset, NBClassifier, train_NBClassifier
restore_path()

#Optional:
set_local_path()
# import modules that are located in the same directory as this file.
restore_path()

class test_nb(NumpyTestCase):
    def check_nb(self, level=1):
        a = ToyBalancedDataGenerator()
        N = 5000
        N2 = 1000

        train_set = [a.generate_set(0,N),a.generate_set(1,N)]

        nb1 = train_NBClassifier(WeightedCDataset([train_set[0][:10],train_set[1][:10]]))
        for i in xrange(10,N):
            for j in xrange(2):
                nb1.learn(train_set[j][i],j)
        
        nb2 = train_NBClassifier(WeightedCDataset(train_set))

        test_set = [a.generate_set(0,N2),a.generate_set(1,N2)]
        err1 = map( nb1.test, test_set )
        err2 = map( nb2.test, test_set )
        assert_array_equal(err1,err2)
        
        err = err1[0].sum() + N2 - err1[1].sum()
        self.failIf(err > 330)

if __name__ == "__main__":
    NumpyTest().run()
    