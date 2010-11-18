#!/usr/bin/env python

__all__ = ['FrontalFaceGenerator']

# import the necessary things
from dircache import listdir
from math import atan2, pi, hypot, cos, sin
from numpy import array
from os.path import exists, join, dirname
from string import split

from objectlocation import ObjectLocation
from ..imageview import ImageView
from .... import tprint
from ... import approx

from ....interfaces.opencv import cvLoadImage, CV_LOAD_IMAGE_GRAYSCALE, \
    cvReleaseImage, cvWarpAffine, CV_INTER_LINEAR, CV_WARP_FILL_OUTLIERS, CvScalar


class FrontalFaceGenerator:
    """A generator that generates frontal face patches."""

    def _convert_frontal_face_info_to_ObjectLocation(self,left_eye,right_eye,nose,
        left_mouth_corner,center_mouth_corner,right_mouth_corner):
        """
        Create an ObjectLocation representing a frontal face from its parts' locations.

        :Parameters:
            From the user's view point.

            left_eye : array([x,y])
                The left eye's location.
            right_eye: array([x,y])
                The right eye's location.
            nose: array([x,y])
                The nose's location.
            left_mouth_corner: array([x,y])
                The left mouth corner's location.
            center_mouth_corner: array([x,y])
                The center mouth (lower) corner's location.
            right_mouth_corner: array([x,y])
                The right mouth corner's location.

        :Returns:
            ol: ObjectLocation
                The frontal face's location.
        """
        ol = ObjectLocation()
        pts = array([left_eye,right_eye,nose,left_mouth_corner,center_mouth_corner,right_mouth_corner])

        pt0 = (pts[0]+pts[1]+pts[4]) / 3
        ol.x = pt0[0]
        ol.y = pt0[1]

        r2 = 0
        for i in xrange(6):
            p = pts[i] - pt0
            r = hypot(p[0],p[1])
            if r2 < r: r2 = r

        ol.shape_type = 0
        ol.rx = r2*1.4
        ol.ry = ol.rx

        p = pts[1]+pts[5]-pts[0]-pts[3]
        ol.angle = atan2(p[1],p[0])

        return ol

    def _convertHS2MyFaceDB(self,facedatapath):
        filepath = join( facedatapath, 'HandySolution' )
        filenames = listdir(filepath)

        return [[join(filepath,x),
                [30.0,30.0],[70.0,30.0],[50.0,55.0],
                [35.0,75.0],[50.0,75.0],[65.0,75.0]] for x in filenames]

    def _convertBioID2MyFaceDB(self,facedatapath):
        biolist = xrange(0,1521)
        listcontent = []

        for i in biolist:
            filename = join( facedatapath, 'BioID', 'Faces', 'BioID_%(#)04d' % { "#": i } )
            filecontent = []

            f = open(filename+'.pts')
            if f is None: break
            try: filecontent = f.readlines()
            except IOError: break
            finally: f.close()

            locations = [filename+'.pgm']
            locations = locations + [split(filecontent[3+0])]
            locations = locations + [split(filecontent[3+1])]
            locations = locations + [split(filecontent[3+14])]
            locations = locations + [split(filecontent[3+2])]
            locations = locations + [split(filecontent[3+18])]
            locations = locations + [split(filecontent[3+3])]
            listcontent = listcontent + [locations]

            for i in xrange(1,7):
                locations[i][0] = float(locations[i][0])
                locations[i][1] = float(locations[i][1])

    ##        left eye 1
    ##        right eye 0
    ##        nose tip 14
    ##        left mouth corner 3
    ##        center mouth corner (below) 18
    ##        right mouth corner 7

        return listcontent

    def _convertARFaces2MyFaceDB(self,facedatapath):
        arlist = listdir(join(facedatapath, 'arfaces'))
        arlist = [join(facedatapath, 'arfaces', x) for x in arlist if '.pts' in x]
        listcontent = []

        for filename in arlist:
            filecontent = []

            f = open(filename)
            try: filecontent = f.readlines()
            except IOError: break
            finally: f.close()

            locations = [filename[:-3]+'png']
            locations = locations + [split(filecontent[3+0])]
            locations = locations + [split(filecontent[3+1])]
            locations = locations + [split(filecontent[3+14])]
            locations = locations + [split(filecontent[3+2])]
            locations = locations + [split(filecontent[3+18])]
            locations = locations + [split(filecontent[3+3])]
            listcontent = listcontent + [locations]

            for i in xrange(1,7):
                locations[i][0] = float(locations[i][0])
                locations[i][1] = float(locations[i][1])

    ##        left eye 1
    ##        right eye 0
    ##        nose tip 14
    ##        left mouth corner 3
    ##        center mouth corner (below) 18
    ##        right mouth corner 7

        return listcontent

    def _convertCMU2MyFaceDB(self,facedatapath):
        listcontent = []

        f = open(join(facedatapath,'CMU','FRONTAL','myfacelist.txt'))
        if f is None: return []

        for line in f:
            words = split(line)
            locations = [join(facedatapath,'CMU','FRONTAL','images',words[0])]
            for i in xrange(1,13): words[i] = float(words[i])
            locations.append([words[1],words[2]])
            locations.append([words[3],words[4]])
            locations.append([words[5],words[6]])
            locations.append([words[7],words[8]])
            locations.append([words[9],words[10]])
            locations.append([words[11],words[12]])
            listcontent = listcontent + [locations]

        return listcontent

    def __init__(self,database_path):
        """Initialize the generator.

        :Parameters:
            database_path : string
                the path to the folder containing the frontal face images
        """
        faceinfolist = \
            self._convertHS2MyFaceDB(database_path) + \
            self._convertBioID2MyFaceDB(database_path) + \
            self._convertCMU2MyFaceDB(database_path) + \
            self._convertARFaces2MyFaceDB(database_path)
        nfaces = len(faceinfolist)
        tprint("There are "+str(nfaces)+" faces...")

        # list of files
        self.facefiles = list(set([x[0] for x in faceinfolist]))

        # list of faces per file
        self.facerecs = [
                [self._convert_frontal_face_info_to_ObjectLocation(
                    array(z[1]),array(z[2]),array(z[3]),
                    array(z[4]),array(z[5]),array(z[6])
                    ) for z in faceinfolist if z[0] == x]
            for x in self.facefiles]

        self.fileid = 0
        self.img = cvLoadImage(self.facefiles[0],CV_LOAD_IMAGE_GRAYSCALE)
        self.faceid = len(self.facerecs[0])-1
        
        self.iview = ImageView()

    def __del__(self):
        cvReleaseImage(self.img)

    def _inc(self):
        if self.faceid == 0:
            cvReleaseImage(self.img)
            if self.fileid == len(self.facefiles)-1:
                self.fileid = 0
            else:
                self.fileid += 1
            self.img = cvLoadImage(self.facefiles[self.fileid],CV_LOAD_IMAGE_GRAYSCALE)
            self.faceid = len(self.facerecs[self.fileid])-1
        else:
            self.faceid -= 1
            

    def generate(self, patch, perturb_func = None, *args, **kwds):
        """Fill in a patch with a (perturbed) frontal face patch.

        :Parameters:
            patch : POINTER(IplImage)
                a POINTER(IplImage) that points to a uint8 IplImage. The
                patch is filled after calling generate()
            perturb_func : a function [optional]
                This function takes an ImageView corresponding to the current view
                that encapsulates the frontal face. You can perturb the view before
                it is warped to the patch. It also returns a tuple expressing the 
                purturbed parameters. If None is given, no pertubation is made.

                Format:
                    def perturb_func(iview, *args, **kwds)
                Input:
                    iview: ImageView
                        a view encapsulating the frontal face, to be supplied
                        by generate()
                Output:
                    perturbed_theta: tuple
                        a tuple of parameters expressing how the transformation
                        matrix is perturbed
            other arguments or keywords:
                all passed to perturb_func, if it exists
                
        :Returns:
            theta : a tuple/array
                a vector of perturbed parameters.
        """
        self._inc()

        faceloc = self.facerecs[self.fileid][self.faceid]

        # copy the patch over
        self.iview.iparam = array((patch[0].width, patch[0].height, 0), 'i')
        self.iview.dparam = array((faceloc.angle,
            faceloc.rx * 2 / min(self.iview.w, self.iview.h),
            faceloc.x, faceloc.y), 'd')
##        self.iview.w = patch[0].width
##        self.iview.h = patch[0].height
##        self.iview.x0 = faceloc.x
##        self.iview.y0 = faceloc.y
##        self.iview.scale = faceloc.rx * 2 / min(self.iview.w, self.iview.h)
##        self.iview.angle = faceloc.angle
##        self.iview.flip = False

        if perturb_func is not None:
            theta = perturb_func(self.iview, *args, **kwds)
        else:
            theta = None
        
        cvWarpAffine(self.img,patch,self.iview.get_transformation_matrix(),
            CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS,CvScalar(0,0,0,0))            
        
        return theta
