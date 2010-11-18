import sys
from numpy.testing import *
from os.path import join

set_package_path()
from sdpy.cs.cv import *
# import xxx symbols
restore_path()

#Optional:
set_local_path()
# import modules that are located in the same directory as this file.
restore_path()

class test_cv(NumpyTestCase):
    def check_all(self, level=1):
        self.warn('This module still needs testing.')


if __name__ == "__main__":
    NumpyTest().run()
