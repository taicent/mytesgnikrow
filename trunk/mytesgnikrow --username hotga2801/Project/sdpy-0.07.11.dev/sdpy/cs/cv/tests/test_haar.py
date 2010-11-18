import sys
from numpy.testing import *
from numpy import arange, array, ones, zeros

set_package_path()
from sdpy import tprint
from sdpy.cs import arrayd
from sdpy.cs.cg import isInsidePoly
from sdpy.cs.cv import *
from sdpy.cs.cv.haar import project_Haar
from sdpy.cs.ml.cla import *
# import xxx symbols
restore_path()

#Optional:
set_local_path()
# import modules that are located in the same directory as this file.
restore_path()

class test_haar(NumpyTestCase):
    def check_Haar3x5(self, level=1):
        # main idea is for testing
        M = 3
        N = 5
        z = generate_Haars(M,N)
        #print z
        
    def test_project_Haar(self, level=1):
        input_data = array([[1,2,30,4],[50,6,9,8]],'double')

        A1 = array([3,-1],'int')
        ind1 = array([3,0],'int')
        x = project_Haar(input_data,A1,ind1)
        assert_array_almost_equal(x,arrayd((11,-26)))
        
        A1 = array([5,7,2],'int')
        ind1 = array([3,1,2],'int')
        x = project_Haar(input_data,A1,ind1)
        assert_array_almost_equal(x,arrayd((94,100)))
        

    # def check_generate_and_subsample(self, level=1):
        # M = 10
        # N = 5
        # tprint("hi")
        # haars = generate_Haars(M,N)
        # ohaars = subsample_Haars(haars,0.5)
        # print haars
        # print ohaars

    # def test_train_Haar_GC(self, level=1): # still experimental
        
        # class ToyCDG(CDataGenerator):
            # def __init__(self):
                # CDataGenerator.__init__(self,(2,),2)
                # self.poly = arrayd((
                    # (10,10),
                    # (30,10),
                    # (30,20),
                    # (40,20),
                    # (40,40),
                    # (30,40),
                    # (30,30),
                    # (20,30),
                    # (20,20),
                    # (10,20)
                # ))

            # def generate(self,j):
                # while True:
                    # x = rand(2)*100-50
                    # if j == int(isInsidePoly(x[0],x[1],self.poly)):
                        # break
                # return x

        # haars = [(ones(1,'int'),array([[0],[1]],'int'))] # just dim 0 or dim 1

        # cdg = ToyCDG()
        # N = 200
        # input_data = [cdg.generate_set(0,N),cdg.generate_set(1,N)]
        # scd = ScoredWCDataset(input_data)

        # gc = train_Haar_GC(scd,cdg,haars,maxM=10)

        # N = 20
        # test_data = [cdg.generate_set(0,N),cdg.generate_set(1,N)]
        # res = [gc.test(x) for x in test_data]
        # self.failIf(res[0].sum() > 8, 'FP > 8, expecting 1 or 0 only')
        # self.failIf(N-res[1].sum() > 8, 'FN > 8, expecting less than or equal to 6 only')
        # #tprint("Class 0 has "+str(res[0].sum())+" FP")
        # #tprint("Class 1 has "+str(N-res[1].sum())+" FN")

    
if __name__ == "__main__":
    NumpyTest().run()
    