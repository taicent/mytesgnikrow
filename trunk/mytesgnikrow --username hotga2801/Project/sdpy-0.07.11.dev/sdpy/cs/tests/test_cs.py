import sys
from numpy.testing import *

set_package_path()
# import xxx symbols
from sdpy.cs import arrayd
restore_path()

#Optional:
set_local_path()
# import modules that are located in the same directory as this file.
restore_path()

class test_cs(NumpyTestCase):
    def check_arrayd(self, level=1):
        a = rand(1,2,3)
        self.failUnlessEqual(arrayd(a).dtype,'d')
        self.failUnlessEqual(arrayd(a.astype('int')).dtype,'d')
        self.failUnlessEqual(arrayd(a.astype('bool')).dtype,'d')

if __name__ == "__main__":
    NumpyTest().run()
    