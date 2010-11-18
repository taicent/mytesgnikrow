import sys
from numpy.testing import *
from numpy import array
from numpy.random import random

set_package_path()
# import xxx symbols
from sdpy.cs.opt import *
restore_path()

#Optional:
set_local_path()
# import modules that are located in the same directory as this file.
restore_path()

class test_de(NumpyTestCase):
    def check_DiffEvolver(self, level=1):
        def f(z):
            return (z[0]-3)**2 + (z[1]-20)**2
            
        s = DiffEvolver.frombounds(f,[-100,-100],[100,100],100)
        s.solve(100)
        assert_almost_equal(s.best_value,0,3)
        assert_array_almost_equal(s.best_vector,(3,20),3)
        

if __name__ == "__main__":
    NumpyTest().run()
    