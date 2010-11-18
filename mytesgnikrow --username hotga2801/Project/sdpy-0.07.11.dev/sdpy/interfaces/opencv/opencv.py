#!/usr/bin/env python

__all__ = ['haar_cascades']

from os.path import dirname, join

haarpath = join( dirname(__file__), 'haarcascades' )

haar_cascades = {
    'frontal_face': join( haarpath, 'haarcascade_frontalface_alt.xml' ),
    'profile_face': join( haarpath, 'haarcascade_profileface.xml' ),
    'upper_body': join( haarpath, 'haarcascade_upperbody.xml' ),
    'lower_body': join( haarpath, 'haarcascade_lowerbody.xml' ),
    'full_body': join( haarpath, 'haarcascade_fullbody.xml' ),    
}
