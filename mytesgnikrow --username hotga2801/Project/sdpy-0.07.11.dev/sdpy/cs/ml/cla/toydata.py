#-------------------------------------------------------------------------------
# Module:      toydata
# Package:     sd.cs.ml.cla
# Name:        Toy data
# Purpose:     Create set of toy data for an asymmetric binary classification
#              problem.
#
# Author:      minhtri
#
# Created:     24/01/2007
# Copyright:   (c) minhtri 2007
# Licence:     <your licence>
#-------------------------------------------------------------------------------
#!/usr/bin/env python

__all__ = ['ToyImbalancedDataGenerator','ToyBalancedDataGenerator']

from numpy import array
from numpy.random import random
from random import randint
from cPickle import load, dump
from dataset import CDataGenerator
from ...cg import isInsidePoly
from ... import arrayd

class ToyImbalancedDataGenerator(CDataGenerator):

    def __init__(self):
        CDataGenerator.__init__(self,(2,),2)
        self.poly = arrayd(((2,8),(6,8),(7,10),(8,8),(9,8),(8,7),
            (11,2),(6,5),(4,4),(5,6)))

    def generate(self,j):
        """
        Generate a random sample of class j.

        Input:
            j: class j
        Output:
            input_point: a point of class j.
        """
        while True:
            z = random(2)*100-50
            if isInsidePoly(z[0],z[1],self.poly) == j:
                return z

class ToyBalancedDataGenerator(CDataGenerator):

    def __init__(self):
        CDataGenerator.__init__(self,(2,),2)
        self.poly = arrayd(((-10,-10),(0,10),(10,-10),(-10,0),(10,10),(0,-10),(-10,10),(10,0)))

    def generate(self,j):
        """
        Generate a random sample of class j.

        Input:
            j: class j
        Output:
            input_point: a point of class j.
        """
        while True:
            z = random(2)*30-15
            if isInsidePoly(z[0],z[1],self.poly) == j:
                return z

