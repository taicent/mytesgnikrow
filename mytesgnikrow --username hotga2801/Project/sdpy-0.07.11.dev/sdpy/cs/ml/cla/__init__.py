#!/usr/bin/env python

from info import __doc__

from cla import *
from dataset import *
from toydata import *
from nb import *
from lda import *
from boosting import *
from cascade import *
from thresh1d import *

__all__ = filter(lambda s:not s.startswith('_'),dir())

from numpy.testing import NumpyTest
test = NumpyTest().test
