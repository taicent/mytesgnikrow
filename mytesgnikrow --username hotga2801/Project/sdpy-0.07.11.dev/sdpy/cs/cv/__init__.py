#!/usr/bin/env python

from info import __doc__

from cv import *
from imageview import *
from integralimage import *
from haar import *
from detect import *

__all__ = filter(lambda s:not s.startswith('_'),dir())

from numpy.testing import NumpyTest
test = NumpyTest().test
