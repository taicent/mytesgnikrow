#-------------------------------------------------------------------------------
# Module:      facedata
# Package:     sd.cs.cv
# Purpose:     My 2-stage face and face generator modules.
#
# Author:      minhtri
#
# Created:     26/03/2007
# Copyright:   (c) minhtri 2007
# Licence:     Lesser GPL
#-------------------------------------------------------------------------------
#!/usr/bin/env python

__all__ = ['ObjectLocation']

# import the necessary things
from numpy import array


#-------------------------------------------------------------------------------
# Object Location class
#-------------------------------------------------------------------------------
class ObjectLocation:
    """The location of an object in an image.

    :Parameters:
        x : double
            The object's center point's location's x component.
        y : double
            The object's center point's location's y component.
        shape_type : integer
            Type of the shape bounding the object, this can be:
                0: a square/circle
                1: a rectangle/ellipse
        rx : double
            The x-axis radius.
        ry : double
            The y-axis radius.
        The values of rx and ry depend on shape_type:
            shape_type=0: rx = ry = l/2, where l is the length of the square, or
                the radius of the circle
            shape_type=1: rx = w/2, ry = h/2, where (w,h) is the size of the rectangle,
                or the two radiuses (fix this) of the ellipse
        angle : double
            Angle of rotation, clock-wise, w.r.t. x-axis (in radian)
    """
    x = 0.0
    y = 0.0
    shape_type = 0
    rx = 0.0
    ry = 0.0
    angle = 0.0

