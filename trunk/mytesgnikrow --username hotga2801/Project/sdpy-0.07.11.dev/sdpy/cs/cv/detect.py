#-------------------------------------------------------------------------------
# Name:        detect
# Purpose:     Detect objects based on Haar features
#
# Author:      minhtri
#
# Created:     06/05/2007
# Copyright:   (c) minhtri 2007
# Licence:     Lesser GPL
#-------------------------------------------------------------------------------
#!/usr/bin/env python

__all__ = ['ObjectDetector']


from cPickle import load
from ctypes import pointer
from numpy import array, zeros, concatenate
from scipy.weave import inline
from time import time
from cv import fromIplImage
from ..ml.cla import DiscreteBoostedClassifier, SimpleCascade, GeneralizedCascade
#from ... import tprint
from ...interfaces.opencv import cvLoadCast, CvHaarClassifierCascade, cvCreateMemStorage, \
    cvReleaseMemStorage, cvReleaseHaarClassifierCascade, cvHaarDetectObjects, \
    CvSize, cvGetSeqElem, CvRect, haar_cascades, cvReleaseImage, cvCopy, cvResize, \
    CV_INTER_NN, IPL_DEPTH_8U, IPL_DEPTH_32S, IPL_DEPTH_64F, cvIntegral, \
    cvSet, cvSetZero, CvScalar, cvCreateImage, cvConvertImage, \
    cvSetImagesForHaarClassifierCascade, cvRunHaarClassifierCascade, \
    cvSetImageROI, cvResetImageROI, \
    CvPoint
from haar import THaarClassifier, integral

class ObjectDetector:

    def _convert_Haar(self,lc,patch_len,c,is_improver,to_reset,reset_value=0):
        """Convert from THaarClassifier to a new classifier with scale s so that I can detect from image with different scales.

        Input:
            lc: a THaarClassifier
            patch_len: the length of the patch to be used by lc
            c: the voting weight (double)
            is_improver: (boolean) an improver or a filter?
            to_reset: (boolean) to reset previous sum?
            reset_value: (double) value to be reset to
        Output:
            all the parameters of the haar classifier will be stored in an int array and a double array

            New reference:

                ivec: 64 items, format as follows
                    - ivec[0]: the number of indices
                    - ivec[1]: flag, in which:
                        - bit 0: set if it is an improver, otherwise it is a filter
                        - bit 1: set if the previous sum is to be reset
                    - starting from ivec[4]: tuples of 3 integers:
                        - x position w.r.t. the top-left corner
                        - y position w.r.t. the top-left corner
                        - coefficient/weight

                dvec: 4 items, format as follows
                    - dvec[0]: threshold b
                    - dvec[1]: voting weight c
                    - dvec[2]: reset value -- only meaningful if bit 1 of ivec[1] is set

            Old reference:

                ivec: format as follows
                    - first 32 ints: (vectorized) indices w.r.t. the location of the top-left corner
                    - ivec[32]: the number of indices
                    - ivec[33]: flag, in which:
                        - bit 0: set if it is an improver, otherwise it is a filter
                        - bit 1: set if the previous sum is to be reset

                dvec: format as follows
                    - first 32 doubles: weights
                    - dvec[32]: threshold b
                    - dvec[33]: voting weight c
                    - dvec[34]: reset value -- only meaningful if bit 1 of ivec[33] is set
        """
        ivec = zeros(64,'int')
        dvec = zeros(4,'double')

        # for debugging
    ##    ivec[2] = patch_len
        
        N = len(lc.p)
        ivec[0] = N
        for n in xrange(N):
            ivec[n*3+4] = lc.p[n] % patch_len
            ivec[n*3+5] = lc.p[n] / patch_len
            ivec[n*3+6] = int(round(lc.w[n]))

        dvec[0] = lc.b
        dvec[1] = c
        dvec[2] = reset_value

        if is_improver: ivec[1] |= 1   # set bit 0
        if to_reset: ivec[1] |= 2      # set bit 1
        
        return ivec, dvec

    def _convert_Haars(self,cc,patch_len):
        """Convert from composite to a new classifier with scale s so that I can detect from image with different scales.

        Input:
            cc: a composite classifier, which can be:
                - A THaarClassifier
                - A SimpleCascade of composite classifiers
                - A GeneralizedCascade of composite classifiers
                - A DiscreteBoostedClassifier of composite classifiers
            patch_len: the length of the patch to be used by lc
        Output:
            ivecs: a 2D int array representing the 'int' parameters of the composite classifier
            dvecs: a 2D double array representing the 'double' parameters of the composite classifier
        """
        if isinstance(cc,THaarClassifier):
            ivec, dvec = self._convert_Haar(cc,patch_len,1,False,True)
            return ivec.reshape(1,64), dvec.reshape(1,4)

        if isinstance(cc,DiscreteBoostedClassifier):
            for weak in cc.weaks:
                if not isinstance(weak,THaarClassifier):
                    raise ValueError("cc is a DiscreteBoostedClassifier, but one of its weak classifiers is not THaarClassifier")
            N = len(cc.weaks)
            ivecs = zeros((N,64),'int')
            dvecs = zeros((N,4),'double')
            for n in xrange(N):
                ivecs[n], dvecs[n] = self._convert_Haar(cc.weaks[n],patch_len,cc.c[n],n != N-1,n == 0,0)
            return ivecs, dvecs

        if isinstance(cc,SimpleCascade):
            for weak in cc.bclassifiers:
                if not isinstance(weak,THaarClassifier) and \
                    not isinstance(weak,DiscreteBoostedClassifier) and \
                    not isinstance(weak,GeneralizedCascade):
                    raise ValueError("cc is a SimpleCascade, but one of its weak \
                classifiers is neither a THaarClassifier, nor a \
                DiscreteBoostedClassifier, nor a GeneralizedCascade")

            z = [self._convert_Haars(x,patch_len) for x in cc.bclassifiers]
            ivecs = concatenate([x[0] for x in z])
            dvecs = concatenate([x[1] for x in z])
            return ivecs, dvecs

        if isinstance(cc,GeneralizedCascade):
            N = len(cc.weaks)
            for n in xrange(1,N):
                if not isinstance(cc.weaks[n],THaarClassifier):
                    raise ValueError("cc is a GeneralizedCascade, but one of its weak classifiers is not a THaarClassifier")
            ivecs = zeros((N-1,64),'int')
            dvecs = zeros((N-1,4),'double')
            ivecs[0], dvecs[0] = self._convert_Haar(cc.weaks[1],patch_len,cc.c[1],cc.z[1] and 1 != N,True,1)
            for n in xrange(2,N):
                ivecs[n-1], dvecs[n-1] = self._convert_Haar(cc.weaks[n],patch_len,cc.c[n],cc.z[n] and n != N-1,False,0)
            return ivecs, dvecs
        
        raise ValueError("Unknown class type of cc")

    def save_to_file(self,filepath):
        if not self.has_BinaryClassifier:
            raise SyntaxError('The object detector was created without a BinaryClassifier')
            
        f = open(filepath,'w')
        M = len(self.ivecs)
        f.write(str(M)+' '+str(self.patch_len)+'\n')
        for m in xrange(M):
            for i in xrange(64):
                f.write(str(self.ivecs[m,i])+' ')
            for i in xrange(4):
                f.write(str(self.dvecs[m,i])+' ')
            f.write('\n')

    def _load_from_file(self,filepath):
        if not self.has_textfile:
            raise SyntaxError('The object detector was created without using a cascade text file')

        f = open(filepath,'r')
        s = f.read().split()
        M = int(s.pop(0)) # len(self.ivecs)
        patch_len = int(s.pop(0))
        ivecs = zeros((M,64),'int')
        dvecs = zeros((M,4),'double')
        for m in xrange(M):
            for i in xrange(64):
                ivecs[m,i] = int(s.pop(0))
            for i in xrange(4):
                dvecs[m,i] = float(s.pop(0))
        return ivecs, dvecs, patch_len
        
        
    def _unsetup_resolution(self):
        if self.N is not None:
            if self.use_opencv:
                cvReleaseImage(self.img_tlsum)
                cvReleaseImage(self.img_sqsum)
                cvReleaseImage(self.img_sum)
            
            for n in xrange(self.N):
                cvReleaseImage(self.mask[n])
                cvReleaseImage(self.img[n])
                
            self.N = None
        
    def setup_resolution(self, width, height, scale_factor):
        """Prepare the object detector for fast object detection.
        
        Input:
            width: the width of the intended image
            height: the height of the intended image
            scale_factor: The factor by which the search window is scaled 
                between the subsequent scans, for example, 1.1 means increasing window by 10%.
        Output:
            nothing, internal variables are allocated
        """
        if self.N is not None:
            if self.ww[0] == width and self.hh[0] == height and self.sc[1] == scale_factor:
                return
            self._unsetup_resolution()
            
        w = width
        h = height
        N = 0
        while w >= 30 and h >= 30:
            w = int(w/scale_factor)
            h = int(h/scale_factor)
            N += 1
            
        self.N = N
        self.ww = ww = zeros(N,'int')
        self.hh = hh = zeros(N,'int')
        self.sc = sc = zeros(N,'double')
        ww[0] = width
        hh[0] = height
        sc[0] = 1.0
        for n in xrange(1,N):
            ww[n] = int(ww[n-1]/scale_factor)
            hh[n] = int(hh[n-1]/scale_factor)
            sc[n] = sc[n-1]*scale_factor
            
        self.img = [cvCreateImage( CvSize(ww[n],hh[n]), IPL_DEPTH_8U, 1 ) for n in xrange(N)]
        self.mask = [cvCreateImage( CvSize(ww[n],hh[n]), IPL_DEPTH_8U, 1 ) for n in xrange(N)]
        self.img_cum = [zeros((hh[n],ww[n])) for n in xrange(N)]
        
        # convert to array, possibly non-contiguous
        self.pimg = [fromIplImage(x) for x in self.img]
        self.pmask = [fromIplImage(x) for x in self.mask]
        
        if self.use_opencv:
            self.img_sum = cvCreateImage( CvSize(ww[0]+1,hh[0]+1), IPL_DEPTH_32S, 1 )
            self.img_sqsum = cvCreateImage( CvSize(ww[0]+1,hh[0]+1), IPL_DEPTH_64F, 1 )
            self.img_tlsum = cvCreateImage( CvSize(ww[0]+1,hh[0]+1), IPL_DEPTH_32S, 1 )
            

    def __init__(self, object_type, flag = 0, param1 = None, param2 = None):
        """Initialize an ObjectDetector.
        
        Input:
            object_type: the type of object to detect,
                in ('frontal_face','profile_face','upper_body','lower_body','full_body')
            flag: the type of operation of the object detector
                0 = use opencv's default haar cascade
                1 = use the cascade stored in a BinaryClassifier, see `param1`, `param2`
                2 = same as 1 but use opencv's default haar cascade to post-verify
                3 = use the cascade stored in a text file named by `param1`
                4 = same as 3 but use opencv's default haar cascade to post-verify
            param1: [optional] a strong classifier to classify a patch into object/non-object                
                If flag is 1 or 2: This holds a BinaryClassifier
                If flag is 3 or 4: This holds a path to the text file storing the cascade
            param2: [optional] If flag is 1 or 2: 
                this holds the length of the image patch (e.g. 20-by-20 or 24-by-24)
                
        """
        self.has_classifier = flag != 0
        self.has_BinaryClassifier = flag in (1,2)
        self.has_textfile = flag in (3,4)
        self.use_opencv = flag in (0,2,4)
        
        self.N = None
        
        if self.has_BinaryClassifier: # load the classifier from BinaryClassifier
            self.patch_len = param2
            self.ivecs, self.dvecs = self._convert_Haars(param1,param2)
            self.ivecs2 = zeros(self.ivecs.shape, 'int')
        elif self.has_textfile: # load the classifier from file
            self.ivecs, self.dvecs, self.patch_len = self._load_from_file(param1)
            self.ivecs2 = zeros(self.ivecs.shape, 'int')
        if self.use_opencv:
            self.haar_cascade = cvLoadCast(haar_cascades[object_type],CvHaarClassifierCascade)
            self.storage = cvCreateMemStorage(0)
                        
    def __del__(self):
        self._unsetup_resolution()
        if self.haar_cascade:
            cvReleaseMemStorage(pointer(self.storage))
            cvReleaseHaarClassifierCascade(pointer(self.haar_cascade))


    def _detect(self,n):
        # initialize variables
        mask = self.pmask[n]
        mask_ws = mask.strides[0] / 1 # sizeof(uint8)
        img = self.img_cum[n]
        img_ws = img.strides[0] / 8 # sizeof(double)
        w = int(self.ww[n])
        h = int(self.hh[n])
        ww0 = int(self.ww[0])
        hh0 = int(self.hh[0])
        M = len(self.ivecs)
        ivecs = self.ivecs
        ivecs2 = self.ivecs2
        dvecs = self.dvecs
        min_neighbors = int(self.min_neighbors)
        patch_len = int(self.patch_len)
        
        qx = zeros(10000,'int')
        qy = zeros(10000,'int')
        outx = zeros(1000,'int')
        outy = zeros(1000,'int')

        code = """
        
        // declaring a function
        class g_
        {
        public:
            int scan( double* topleft, int M, int* ivec2, double* dvec )
            {
                double sum = 0.0, val, v;
                int n;

                while(--M >= 0)
                {
                    if(ivec2[61]&2) // to reset
                        sum = dvec[2];

                    v = 0;
                    n = ivec2[60];
                    while(--n>=0)
                        v += ivec2[n+30]*topleft[ivec2[n]];

                    val = (v >= dvec[0])? dvec[1]: -dvec[1];
                    if(ivec2[61]&1) // is an improver
                        sum += val;
                    else if(sum+val < 0) return 0;

                    ivec2 += 64;
                    dvec += 4;
                }

                return 1;
            }
        } g;
        
        int i, ni, m, x, y;
        int *ivec, *ivec2;
        unsigned char *dmsk;

        // prepare indices
        for( ivec = (int*)ivecs, ivec2 = (int*)ivecs2, m = 0; m < M; ++m, ivec += 64, ivec2 += 64 )
        {
            ni = ivec2[60] = ivec[0];
            ivec2[61] = ivec[1];

            for( i = 0; i < ni; ++i )
            {
                ivec2[i] = ivec[i*3+4] + ivec[i*3+5]*img_ws;
                ivec2[i+30] = ivec[i*3+6];
            }
        }

        for( y = 0; y < h-patch_len; ++y )
        {
            dmsk = &mask[y*mask_ws];
            for( x = 0; x < w-patch_len; ++x )
                if(dmsk[x] && g.scan(&img[y*img_ws+x], M, (int*)ivecs2, dvecs))
                    dmsk[x] = 2;
        }

        // post-processing, group pixels of the same component into one
        int j, x1, y1, x2, y2, z1, nout = 0;
        double mx, my;
        int mq, nq;
        int dx[4] = {-1,0,1,0};
        int dy[4] = {0,-1,0,1};

        if(min_neighbors > 0) for( y = 0; y < h; ++y )
        {
            for( x = 0; x < w; ++x ) if(mask[y*mask_ws+x] == 2)
            {
                mx = 0;
                my = 0;

                mq = 0; nq = 1;
                qx[0] = x;
                qy[0] = y;
                mask[y*mask_ws+x] = 1;

                while(mq < nq)
                {
                    x1 = qx[mq];
                    y1 = qy[mq];
                    ++mq;

                    mx += x;
                    my += y;

                    for( i = 0; i < 4; ++i )
                    {
                        x2 = x1 + dx[i];
                        if(x2 < 0 || x2 >= w) continue;
                        y2 = y1 + dy[i];
                        if(y2 < 0 || y2 >= h) continue;
                        if(mask[y2*mask_ws+x2] == 2)
                        {
                            qx[nq] = x2;
                            qy[nq] = y2;
                            mask[y2*mask_ws+x2] = 1;
                            ++nq;
                        }
                    }
                }


                x1 = (int)floor(mx/nq);
                y1 = (int)floor(my/nq);

                if(nq < min_neighbors) continue;
                
                x1 = x1 * ww0 / w;
                y1 = y1 * hh0 / h;
                z1 = patch_len * ww0 / w;

                outx[nout] = x1;
                outy[nout] = y1;
                ++nout;
            }
        }
        
        return_val = nout;
        """
        nout = inline(code,['mask','mask_ws','img','img_ws','w','h','ww0','hh0','ivecs','ivecs2','dvecs',
            'min_neighbors','patch_len','qx','qy','outx','outy','M'])
            
        # finalize the candidates
        to_set_img = True
        tmask = self.mask[n]
        for i in xrange(nout):
            x1 = outx[i]
            y1 = outy[i]
            
            # true object location
            x2 = x1 * (ww0 / w)
            y2 = y1 * (hh0 / h)
            z2 = patch_len * (ww0 / w)

            # check if a candidate passes the default opencv's detector
            if self.use_opencv:
                if to_set_img:
                    cvSetImagesForHaarClassifierCascade(self.haar_cascade, \
                        self.img_sum,self.img_sqsum,self.img_tlsum,self.sc[n])
                    to_set_img = False

                if cvRunHaarClassifierCascade(self.haar_cascade, CvPoint(x2,y2)) <= 0:
                    continue

            # clear a few neighboring candidates
            x1min = max(0, x1-patch_len/2)
            x1max = min(w, x1+patch_len/2)
            y1min = max(0, y1-patch_len/2)
            y1max = min(h, y1+patch_len/2)
            if y1max > y1min and x1max > x1min:
                cvSetImageROI(tmask, CvRect(x1min,y1min,x1max-x1min,y1max-y1min))
                cvSetZero(tmask)
                cvResetImageROI(tmask)

            self.faces.append((x2,y2,z2,z2))

        
    def detect(self, img, scale_factor = 1.1, min_neighbors = 3):
        """Detect objects from a grayscale image.
        
        Input:
            img: a POINTER(IplImage) of depth IPL_DEPTH_8U and 1 or 3 channels
            scale_factor: The factor by which the search window is scaled 
                between the subsequent scans, for example, 1.1 means increasing window by 10%.
            min_neighbors: Minimum number (minus 1) of neighbor rectangles that makes up an object. 
                All the groups of a smaller number of rectangles than min_neighbors-1 are rejected. 
                If min_neighbors is 0, the function does not any grouping at all and returns all 
                the detected candidate rectangles, which may be useful if the user wants to apply 
                a customized grouping procedure.
        Output:
            z: an 'int' numpy.array of tuples (x,y,w,h) representing the object locations
        """
        if self.has_classifier: # got a classifier
            self.min_neighbors = min_neighbors
            self.setup_resolution(img.contents.width, img.contents.height, scale_factor)

            if img.contents.nChannels > 1:
                if img.contents.origin > 0:
                    cvConvertImage( img, self.img[0], CV_CVTIMG_FLIP )
                else:
                    cvConvertImage( img, self.img[0] )
            else:
                if img.contents.origin > 0:
                    cvFlip( img, self.img[0] )
                else:
                    cvCopy( img, self.img[0] )

            for n in xrange(self.N-1):
                cvResize( self.img[n], self.img[n+1], CV_INTER_NN )
            if self.use_opencv:
                cvIntegral( self.img[0], self.img_sum, self.img_sqsum, self.img_tlsum )
            
            cvSet(self.mask[self.N-1], CvScalar(1,1,1,1))
            self.faces = []
            for n in xrange(self.N-1,0,-1):
                integral(self.pimg[n],self.img_cum[n])
                self._detect(n)
                if n > 0:
                    cvResize(self.mask[n], self.mask[n-1], CV_INTER_NN)
                    
            z = array(self.faces)
                
        else: # use opencv's default slow but accurate haar_cascade
            faces = cvHaarDetectObjects( img, self.haar_cascade, self.storage, scale_factor,
                min_neighbors, 0, CvSize(0,0) )
                
            z = zeros((len(faces),4),'int')
            for i in xrange(len(faces)):
                z[i,0] = faces[i].x
                z[i,1] = faces[i].y
                z[i,2] = faces[i].width
                z[i,3] = faces[i].height
                
        return z
