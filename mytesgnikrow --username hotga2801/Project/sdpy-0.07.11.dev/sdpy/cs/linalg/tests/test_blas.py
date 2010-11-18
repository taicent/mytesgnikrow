import sys
from numpy.testing import *

set_package_path()
# import xxx symbols
from sdpy.cs.linalg import dger, daxpy
restore_path()

#Optional:
set_local_path()
# import modules that are located in the same directory as this file.
restore_path()

class test_blas(NumpyTestCase):
    def check_none(self, level=1):
        self.warn('This module needs testing.')

if __name__ == "__main__":
    NumpyTest().run()
    