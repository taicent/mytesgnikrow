import sys
from numpy.testing import *
from numpy import arange, zeros

set_package_path()
# import xxx symbols
from sdpy import tprint
from sdpy.cs.ml.cla import *
restore_path()

#Optional:
set_local_path()
# import modules that are located in the same directory as this file.
restore_path()

class test_boosting(NumpyTestCase):
    def check_boost_online_Oza(self, level=1): # looks ok
        if level < 3: return
        a = ToyBalancedDataGenerator()
        N = 5000
        N2 = 1000

        train_set = [a.generate_set(0,N),a.generate_set(1,N)]
        test_set = [a.generate_set(0,N2),a.generate_set(1,N2)]
        
        cd = WeightedCDataset([train_set[0][:10],train_set[1][:10]])

        boost1 = train_DBC( cd, train_NBClassifier, 5, 1.0, 0, 0 )
        for i in xrange(10,N):
            boost1.learn(train_set[0][i],0)
            boost1.learn(train_set[1][i],1)
            err1 = map( boost1.test, test_set )
            print [err1[0].sum(),N2-err1[1].sum()]

    def check_boost_online_Pham(self, level=1): # looks ok, too
        if level < 3: return
        a = ToyBalancedDataGenerator()
        N = 5000
        N2 = 1000

        train_set = [a.generate_set(0,N),a.generate_set(1,N)]
        test_set = [a.generate_set(0,N2),a.generate_set(1,N2)]
        
        cd = WeightedCDataset([train_set[0][:10],train_set[1][:10]])

        boost1 = train_DBC( cd, train_NBClassifier, 5, 1.0, 0, 1 )
        for i in xrange(10,N):
            boost1.learn(train_set[0][i],0)
            boost1.learn(train_set[1][i],1)
            err1 = map( boost1.test, test_set )
            print [err1[0].sum(),N2-err1[1].sum()]

    def check_boost_online_Pham_asym(self, level=1): # looks good, too
        if level < 3: return
        a = ToyImbalancedDataGenerator()
        N = 5000
        N2 = 1000

        train_set = [a.generate_set(0,N),a.generate_set(1,N/10)]
        test_set = [a.generate_set(0,N2),a.generate_set(1,N2/10)]
        
        cd = WeightedCDataset([train_set[0][:10],train_set[1][:1]])

        boost1 = train_DBC( cd, train_NBClassifier, 5, 10.0, 0, 1 )
        for i in xrange(1,N/10):
            for j in xrange(10):
                boost1.learn(train_set[0][i*10+j],0)
            boost1.learn(train_set[1][i],1)
            err1 = map( boost1.test, test_set )
            print [err1[0].sum(),N2/10-err1[1].sum()]

    def test_boost_offline(self, level=1):
        a = ToyBalancedDataGenerator()
        N = 5000
        N2 = 1000

        train_set = [a.generate_set(0,N),a.generate_set(1,N)]
        
        cd = WeightedCDataset(train_set)

        boost1 = train_AdaBoost( cd, train_NBClassifier, 5 )
        boost2 = train_DBC( cd, train_NBClassifier, 5, 1.0, 0 )

        test_set = [a.generate_set(0,N2),a.generate_set(1,N2)]
        err1 = map( boost1.test, test_set )
        err2 = map( boost2.test, test_set )
        assert_array_equal(err1,err2)
        
        err = err1[0].sum() + N2 - err1[1].sum()
        self.failIf(err > 330)

    def test_boost_offline2(self, level=1):
        a = ToyImbalancedDataGenerator()
        N = 5000
        N2 = 1000

        train_set = [a.generate_set(0,N),a.generate_set(1,N/10)]
        
        cd = WeightedCDataset(train_set)

        boost1 = train_VJ( cd, train_NBClassifier, 5, 3.0 )
        boost2 = train_DBC( cd, train_NBClassifier, 5, 3.0, 1 )

        test_set = [a.generate_set(0,N2),a.generate_set(1,N2)]
        err1 = map( boost1.test, test_set )
        err2 = map( boost2.test, test_set )
        assert_array_equal(err1,err2)
        
        #err = err1[0].sum() + N2 - err1[1].sum()
        #self.failIf(err > 330)

    def test_boost_offline3(self, level=1):
        a = ToyImbalancedDataGenerator()
        N = 5000/10
        N2 = 1000

        train_set = [a.generate_set(0,N),a.generate_set(1,N)]
        
        cd = WeightedCDataset(train_set)

        boost1 = train_PC( cd, train_NBClassifier, 5, 9.0 )
        boost2 = train_DBC( cd, train_NBClassifier, 5, 9.0, 2 )

        test_set = [a.generate_set(0,N2),a.generate_set(1,N2)]
        err1 = map( boost1.test, test_set )
        err2 = map( boost2.test, test_set )
        assert_array_equal(err1,err2)
        
        #err = err1[0].sum() + N2 - err1[1].sum()
        #self.failIf(err > 330)

if __name__ == "__main__":
    NumpyTest().run()
    