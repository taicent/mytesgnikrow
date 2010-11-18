import sys
from numpy.testing import *
from numpy import array
from numpy.random import random
#from Polygon import Polygon

set_package_path()
# import xxx symbols
from sdpy.cs.cg import *
from sdpy.cs import arrayd
restore_path()

#Optional:
set_local_path()
# import modules that are located in the same directory as this file.
restore_path()

class test_cg(NumpyTestCase):
    def check_poly(self, level=1):
        pass # its' ok
        # polyarray = ((2,8),(6,8),(7,10),(8,8),(9,8),(8,7),(11,2),(6,5),(4,4),(5,6))
        # poly = Polygon(polyarray)
        # poly2 = arrayd(polyarray)
        
        # for i in xrange(100000):
            # x, y = random(2)*20-10
            # self.failUnlessEqual( isInsidePoly(x,y,poly2), poly.isInside(x,y) )
            
    def check_smallestEnclosingBall(self, level=1):
        a = array([[0,0,0],[1,5,3],[2,4,5],[4,2,1],[4,6,3],[1,1,3],
            [0,0,5],[0,5,0],[0,0,5],[0,5,5],
            [5,0,5],[5,5,0],[5,0,5],[5,5,5],
            ],'d')
        z = smallestEnclosingBall(a)
        assert_array_almost_equal(z[0],array((2.5,2.5,2.5)))
        assert_almost_equal(z[1],18.75)
        a = array([
            [0,0],[0,5]#,[0,5],[5,5],
            ],'d')
        z = smallestEnclosingBall(a)
        assert_array_almost_equal(z[0],array((0,2.5)))
        assert_almost_equal(z[1],6.25)

    def check_distanceToPlane(self, level=1):
        a = array([[1,2,3],[2,3,4]],'d')
        plane = array([1,1,1,0],'d')
        assert_array_almost_equal(distanceToPlane(a,plane),
        array([ 3.46410162,  5.19615242]))

        

if __name__ == "__main__":
    NumpyTest().run()
