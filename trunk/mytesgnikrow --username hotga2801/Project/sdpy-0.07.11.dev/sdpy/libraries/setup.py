#!/usr/bin/env python

import cpuidpy
import re
import os
from copy import copy
from os.path import join
from scipy.weave import inline
from numpy.distutils.system_info import get_info
from numpy.distutils.ccompiler import get_default_compiler
from numpy.distutils.misc_util import Configuration
from distutils.version import StrictVersion

def get_gcc_version():
    out = os.popen('gcc -dumpversion','r')
    out_string = out.read()
    out.close()
    result = re.search('(\d+\.\d+)',out_string)
    if result:
        return StrictVersion(result.group(1))
    return None
    

def configuration(parent_package='',top_path=None):
    config = Configuration('libraries',parent_package,top_path)
    
    config.add_include_dirs('plat_det')
    config.add_library('plat_det', # platform detection
        sources=[join('plat_det', '*.c')],
        headers=[join('plat_det', '*.h')])
        
    extra_args = []
    dcomp = get_default_compiler()
    if cpuidpy.has_SSE2 and (dcomp == 'mingw32' or dcomp == 'gcc'):
        extra_args = ['-O3', '-msse2', '-mfpmath=sse', '-malign-double']
        if get_gcc_version() >= '4.2':
            extra_args.append('-mtune=generic')
        else:
            extra_args.append('-march=pentium4')
            extra_args.append('-mtune=pentium4')

    config.add_include_dirs('cephesd')
    config.add_library('cephesd',
        sources=[join('cephesd', '*.c')],
        headers=[join('cephesd', '*.h')],
        include_dirs=['plat_det'])

    atlas_info = get_info('atlas')
    build_info = {}
    if atlas_info.has_key('define_macros'):
        build_info['macros'] = copy(atlas_info['define_macros'])
    else:
        build_info['macros'] = []
    build_info['macros'].append(('HAS_ATLAS', None))
    
    if atlas_info.has_key('include_dirs'):
        build_info['include_dirs'] = copy(atlas_info['include_dirs'])
    else:
        build_info['include_dirs'] = []
    build_info['include_dirs'].append('plat_det')
    
    build_info['extra_compiler_args'] = extra_args
    build_info['language'] = 'C'
    
    config.add_include_dirs('cblas_ext')
    config.add_library('cblas_ext',
        sources=[join('cblas_ext', '*.c')],
        headers=[join('cblas_ext', '*.h')],
        **build_info)
        
    build_info2 = copy(build_info)
    build_info2['include_dirs'].append('cblas_ext')
    
    config.add_include_dirs('sdcpp')
    config.add_library('sdcpp',
        sources=[join('sdcpp', '*.cpp')],
        headers=[join('sdcpp', '*.h')],
        **build_info2)
        
    return config

if __name__ == '__main__':
    print 'This is the wrong setup.py file to run'
