# sdpy - SharkDolphin's Computer Science modules for Python
# Copyright (C) 2007  Minh-Tri Pham and Viet-Dung D. Hoang
# Copyright (C) 2007  Nanyang Technological University

# This program is free software: you can redistribute it and/or modify
# it under the following term, and the terms of the GNU General Public 
# License as published by the Free Software Foundation, either version 
# 3 of the License, or (at your option) any later version:
# 1. All submitted papers (or any publicly available text) that uses 
# or talks about sdpy must cite the following paper: 
# Minh-Tri Pham and Tat-Jen Cham. Fast Training and Selection of Haar 
# features using Statistics in Boosting-based Face Detection. In Proc. 
# 11th IEEE International Conference on Computer Vision (ICCV'07), Rio 
# de Janeiro, Brazil, 2007.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# ---------------------------------------------------------------------
#!/usr/bin/env python

"""
This file defines a set of system_info classes for getting
information about various resources (libraries, library directories,
include directories, etc.) in the system. It is an extension of
numpy's system_info.py. Currently, the following classes are 
available:

  scipack_info
  sdcpp_info
  paypack_info
  <all other classes supported by numpy>

Usage:
    info_dict = get_info(<name>)
  where <name> is a string 'scipack', 'atlas', 'blas', 'lapack', etc. 
  For a complete list of allowed names, see the definition of get_info() 
  function below.

  Returned info_dict is a dictionary which is compatible with
  distutils.setup keyword arguments. If info_dict == {}, then the
  asked resource is not available (system_info could not find it).

  Several *_info classes specify an environment variable to specify
  the locations of software. When setting the corresponding environment
  variable to 'None' then the software will be ignored, even when it
  is available in system.

Global parameters:
  system_info.search_static_first - search static libraries (.a)
             in precedence to shared ones (.so, .sl) if enabled.
  system_info.verbosity - output the results to stdout if enabled.

The file 'site.cfg' is looked for in

1) Directory of main setup.py file being run.
2) Home directory of user running the setup.py file as ~/.numpy-site.cfg
3) System wide directory (location of this file...)

The first one found is used to get system configuration options The
format is that used by ConfigParser (i.e., Windows .INI style). The
section DEFAULT has options that are the default for each section. The
available sections are fftw, atlas, and x11. Appropiate defaults are
used if nothing is specified.

The order of finding the locations of resources is the following:
 1. environment variable
 2. section in site.cfg
 3. DEFAULT section in site.cfg
Only the first complete match is returned.

Example:
----------
[DEFAULT]
library_dirs = /usr/lib:/usr/local/lib:/opt/lib
include_dirs = /usr/include:/usr/local/include:/opt/include
src_dirs = /usr/local/src:/opt/src
# search static libraries (.a) in preference to shared ones (.so)
search_static_first = 0

[scipack]
library_dirs = M:/programming/packages/scipack/build/lib
include_dirs = M:/programming/packages/scipack/build/stlport;M:/programming/packages/scipack/build/include
scipack_libs = stlport.5.1.dll, atlapack.dll, cephesd.dll, stdc++

[paypack]
library_dirs = M:/programming/packages/paypack/build/lib
include_dirs = M:/programming/packages/paypack/build/include
paypack_libs =

[sdcpp]
library_dirs = M:/programming/workspace/CB/sdcpp
include_dirs = M:/programming/workspace/CB/sdcpp
----------

Author:
  Minh-Tri Pham <mtpham@ntu.edu.sg>, July 2007

Copyright 2007 Minh-Tri Pham all rights reserved,
Minh-Tri Pham <mtpham@ntu.edu.sg>
Permission to use, modify, and distribute this software is given under the
terms of the SdPy (BSD style) license.  See LICENSE.txt that came with
this distribution for specifics.

NO WARRANTY IS EXPRESSED OR IMPLIED.  USE AT YOUR OWN RISK.
"""

__all__ = ['get_info']

import sys
import os
import re
import copy
import warnings
from glob import glob
import ConfigParser

from distutils.errors import DistutilsError
from distutils.dist import Distribution
import distutils.sysconfig
from distutils import log

from numpy.distutils.exec_command import \
    find_executable, exec_command, get_pythonexe
from numpy.distutils.misc_util import is_sequence, is_string
from numpy.distutils.command.config import config as cmd_config

from numpy.distutils import system_info as npsi

def get_info(name,notfound_action=0):
    """
    notfound_action:
      0 - do nothing
      1 - display warning message
      2 - raise error
    """
    cl = {
        'scipack':scipack_info,
        'sdcpp':sdcpp_info,
        'paypack':paypack_info,
          }.get(name.lower())
    if cl is None:
        return npsi.get_info(name,notfound_action)
    return cl().get_info(notfound_action)

class ScipackNotFoundError(npsi.NotFoundError):
    """
    Scipack (http://pmtri.sharkdolphin.com) libraries not found.
    Directories to search for the libraries can be specified in the
    site.cfg file (section [scipack]) or by setting
    the ROOT_SCIPACK environment variable."""

class scipack_info(npsi.system_info):
    #variables to override
    section = 'scipack'
    dir_env_var = 'ROOT_SCIPACK'
    notfounderror = ScipackNotFoundError
    ver_info  = [ { 'name':'scipack',
                    'libs':['atlapack.dll', 'cephesd.dll', 'asmlib', 'stdc++_s']},
                ]

    def __init__(self):
        npsi.system_info.__init__(self)

    def calc_ver_info(self,ver_param):
        """Returns True on successful version detection, else False"""
        args = ['-O3','-mtune=generic','-msse2','-mfpmath=sse','-malign-double','-std=c99']
        lib_dirs = self.get_lib_dirs()
        incl_dirs = self.get_include_dirs()
        incl_dir = None
        libs = self.get_libs(self.section+'_libs', ver_param['libs'])
        info = None
        for d in lib_dirs:
            r = self.check_libs(d,libs)
            if r is not None:
                info = r
                break
        if info is not None:
            npsi.dict_append(info,include_dirs=incl_dirs)
            npsi.dict_append(info,extra_compile_args=args)
            self.set_info(**info)
            return True
        else:
            log.info('  %s not found' % (ver_param['name']))
            return False

    def calc_info(self):
        for i in self.ver_info:
            if self.calc_ver_info(i):
                break


class PaypackNotFoundError(npsi.NotFoundError):
    """
    Paypack (http://pmtri.sharkdolphin.com) libraries not found.
    Directories to search for the libraries can be specified in the
    site.cfg file (section [scipack]) or by setting
    the ROOT_PAYPACK environment variable."""

class paypack_info(npsi.system_info):
    #variables to override
    section = 'paypack'
    dir_env_var = 'ROOT_PAYPACK'
    notfounderror = PaypackNotFoundError
    ver_info  = [ { 'name':'paypack',
                    'libs':['CGAL'],
                    'macros':[('HAVE_MACSTL',None)]},
                ]

    def __init__(self):
        npsi.system_info.__init__(self)

    def calc_ver_info(self,ver_param):
        """Returns True on successful version detection, else False"""
        args = ['-O3','-mtune=generic','-msse2','-mfpmath=sse','-mstackrealign',
            '-mpreferred-stack-boundary=4','-m128bit-long-double']
        lib_dirs = self.get_lib_dirs()
        incl_dirs = self.get_include_dirs()
        incl_dir = None
        libs = self.get_libs(self.section+'_libs', ver_param['libs'])
        info = None
        for d in lib_dirs:
            r = self.check_libs(d,libs)
            if r is not None:
                info = r
                break
        if info is not None:
            npsi.dict_append(info,include_dirs=incl_dirs)
            npsi.dict_append(info,extra_compile_args=args)
            npsi.dict_append(info,define_macros=ver_param['macros'])
            self.set_info(**info)
            return True
        else:
            log.info('  %s not found' % (ver_param['name']))
            return False

    def calc_info(self):
        for i in self.ver_info:
            if self.calc_ver_info(i):
                break


class SdCppNotFoundError(npsi.NotFoundError):
    """
    SdCpp (http://pmtri.sharkdolphin.com) library not found.
    Directories to search for the librart can be specified in the
    site.cfg file (section [sdcpp]) or by setting
    the ROOT_SDCPP environment variable."""

class sdcpp_info(npsi.system_info):
    #variables to override
    section = 'sdcpp'
    dir_env_var = 'ROOT_SDCPP'
    notfounderror = SdCppNotFoundError
    ver_info  = [ { 'name':'sdcpp',
                    'libs':['sdcpp.dll'],
                    'includes':['sdcpp.h'],
                    'macros':[('SDPY_SDCPP_H',None)]},
                ]

    def __init__(self):
        npsi.system_info.__init__(self)

    def calc_ver_info(self,ver_param):
        """Returns True on successful version detection, else False"""
        args = ['-O3','-mtune=generic','-msse2','-mfpmath=sse','-mstackrealign',
            '-mpreferred-stack-boundary=4','-m128bit-long-double','-lstdc++_s']
        lib_dirs = self.get_lib_dirs()
        incl_dirs = self.get_include_dirs()
        incl_dir = None
        libs = self.get_libs(self.section+'_libs', ver_param['libs'])
        info = None
        for d in lib_dirs:
            r = self.check_libs(d,libs)
            if r is not None:
                info = r
                break
        if info is not None:
            flag = 0
            for d in incl_dirs:
                if len(self.combine_paths(d,ver_param['includes']))==len(ver_param['includes']):
                    npsi.dict_append(info,include_dirs=[d])
                    npsi.dict_append(info,extra_compile_args=args)
                    flag = 1
                    incl_dirs = [d]
                    incl_dir = d
                    break
            if flag:
                npsi.dict_append(info,define_macros=ver_param['macros'])
            else:
                info = None
        if info is not None:
            self.set_info(**info)
            return True
        else:
            log.info('  %s not found' % (ver_param['name']))
            return False

    def calc_info(self):
        for i in self.ver_info:
            if self.calc_ver_info(i):
                break


#--------------------------------------------------------------------

if __name__ == "__main__":
    npsi.show_all()
