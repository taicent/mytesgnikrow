#-------------------------------------------------------------------------------
# Module:      __init__
# Package:     sd.cs.cv
# Purpose:     Initiate the 'cv' package -- computer vision
#
# Author:      minhtri
#
# Created:     26/01/2007
# Copyright:   (c) minhtri 2007
# Licence:     Lesser GPL
#-------------------------------------------------------------------------------
#!/usr/bin/env python

__all__ = ['fromIplImage','fromCvMat','datapath']

# import the necessary things
from os.path import dirname, join
from numpy import frombuffer, dtype
from ctypes import POINTER
from ...interfaces.opencv import IplImage, cvImageAsBuffer, IPL_DEPTH_1U, \
    IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16U, IPL_DEPTH_16S, IPL_DEPTH_32S, \
    IPL_DEPTH_32F, IPL_DEPTH_64F, CvMat, CV_8U, CV_8S, CV_16U, CV_16S, \
    CV_32S, CV_32F, CV_64F, CV_CN_SHIFT, cvCvMatAsBuffer


datapath = join(dirname(__file__), 'data')

_dict_opencvdepth2dtype = {
    IPL_DEPTH_1U: 'bool',
    IPL_DEPTH_8U: 'uint8',
    IPL_DEPTH_8S: 'int8',
    IPL_DEPTH_16U: 'uint16',
    IPL_DEPTH_16S: 'int16',
    IPL_DEPTH_32S: 'int32',
    IPL_DEPTH_32F: 'float32',
    IPL_DEPTH_64F: 'float64',
}

def fromIplImage(img):
    """Convert a POINTER(IplImage) into numpy.array
    
    Input:
        img: a POINTER(IplImage)
    Output:
        img2: a numpy.array
    """
    if not isinstance(img,POINTER(IplImage)):
        raise TypeError('img is not of type POINTER(IplImage)')
    
    dtypename = _dict_opencvdepth2dtype[img.contents.depth]
    data = frombuffer(cvImageAsBuffer(img),dtype=dtypename)
    
    w = img.contents.width
    ws = img.contents.widthStep / dtype(dtypename).itemsize
    h = img.contents.height
    nc = img.contents.nChannels

    if nc > 1:
        return data.reshape(h,ws)[:,:w*nc].reshape(h,w,nc)
    else:
        return data.reshape(h,ws)[:,:w]

_dict_opencvmat2dtype = {
    CV_8U: 'uint8',
    CV_8S: 'int8',
    CV_16U: 'uint16',
    CV_16S: 'int16',
    CV_32S: 'int32',
    CV_32F: 'float32',
    CV_64F: 'float64',
}

def fromCvMat(mat):
    """Convert a POINTER(CvMat) into numpy.array

    Input:
        mat: a POINTER(CvMat)
    Output:
        mat2: a numpy.array
    """
    if not isinstance(mat,POINTER(CvMat)):
        raise TypeError('mat is not of type POINTER(CvMat)')

    typedepth = mat[0].type & 0x0FFF
    thetype = typedepth & ((1 << CV_CN_SHIFT)-1)
    nc = (typedepth >> CV_CN_SHIFT) + 1
    dtypename = _dict_opencvmat2dtype[thetype]
    data = frombuffer(cvCvMatAsBuffer(mat),dtype=dtypename)

    w = mat[0].cols
    ws = mat[0].step / dtype(dtypename).itemsize
    h = mat[0].rows

    if nc > 1:
        return data.reshape(h,ws)[:,:w*nc].reshape(h,w,nc)
    else:
        return data.reshape(h,ws)[:,:w]

