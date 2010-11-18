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

class test_detect(NumpyTestCase):
    def check_all(self, level=1):
        self.warn('This module "detect" still needs testing.')

    # def test_overall(self, level=1):
        # z = load(open(join(sdpath,'cs','cv','iccv_train.ssd'),'r'))
        # cc = z[0]
        # ivecs, dvecs = convert_Haars(cc,20,1.0,320)

    # def _detect():
    #    cvNamedWindow("screen",CV_WINDOW_AUTOSIZE)
    #    img = cvLoadImage(datapath+"testface.jpg",0)
    #    cvShowImage("screen",img)

        # cc = load(open('new_uac_filter_dim20.sdd','r'))
        # patch_len = 20

        # ivecs, dvecs = convert_Haars(cc,patch_len)
        # write_to_file(ivecs,dvecs,patch_len)



if __name__ == "__main__":
    NumpyTest().run()
    