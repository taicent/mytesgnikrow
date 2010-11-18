import sys
from numpy.testing import *

set_package_path()
# import xxx symbols
from sdpy import tprint, speak
restore_path()

#Optional:
set_local_path()
# import modules that are located in the same directory as this file.
restore_path()

class test_tprint(NumpyTestCase):
    def check_speak(self, level=1):
        speak("Testing sdpy's speaking ability...")

if __name__ == "__main__":
    NumpyTest().run()
    