#!/usr/bin/env python

from os.path import join, dirname
from scipy.weave import ext_tools
from numpy.distutils.misc_util import Configuration, dot_join

from numpy import array

# ==============================================================================
# Building a C++ extension module
# ==============================================================================
def build_extension(module_name,location='.',**kw):
    """ Build an extension module for this package.
    """
    mod = ext_tools.ext_module(module_name)

    #----------------------------------------------------------
    # cg_slide(iparam, dparam, x, y)
    #----------------------------------------------------------
    # slide the view along its x- and y- axes
    #----------------------------------------------------------

    # this is effectively a type declaration for function arguments
    iparam = array((3,), 'i')
    dparam = array((4,), 'd')
    x = 1.0
    y = 1.0

    # code
    code = """
        double cA, sA;

        cA = cos(dparam[0]) * dparam[1];
        sA = sin(dparam[0]) * dparam[1];
        
        // slide via x-axis
        dparam[2] += cA * x;
        dparam[3] += sA * x;
        
        // slide via y-axis
        if(iparam[2])
        {
            dparam[2] += sA * y;
            dparam[3] -= cA * y;
        }
        else
        {
            dparam[2] -= sA * y;
            dparam[3] += cA * y;
        }
        
        return_val = 1;
    """
    func = ext_tools.ext_function('cg_slide',code,
        ['iparam', 'dparam', 'x', 'y'])
    # func.customize.add_support_code(fib_code)
    mod.add_function(func)


    #----------------------------------------------------------
    # cg_transform_pairs(iparam, dparam, src, dst)
    #----------------------------------------------------------
    # get 3 tranformation coordinate pairs
    #----------------------------------------------------------

    # this is effectively a type declaration for function arguments
    iparam = array((3,), 'i')
    dparam = array((4,), 'd')
    src = array((6,), 'd')
    dst = array((6,), 'd')

    # code
    code = """
        double cA, sA, x0, y0, x1, y1;
        
        cA = cos(dparam[0]) * dparam[1];
        sA = sin(dparam[0]) * dparam[1];
        x0 = dparam[2];
        y0 = dparam[3];
        x1 = 0.5 * iparam[0];
        y1 = 0.5 * iparam[1];

        src[0] = x0;
        src[1] = y0;
        src[2] = x0 + cA;
        src[3] = y0 + sA;
        if(iparam[2])
        {
            src[4] = x0 + sA;
            src[5] = y0 - cA;
        }
        else
        {
            src[4] = x0 - sA;
            src[5] = y0 + cA;
        }

        dst[0] = x1;
        dst[1] = y1;
        dst[2] = x1+1;
        dst[3] = y1;
        dst[4] = x1;
        dst[5] = y1+1;

        return_val = 1;
    """
    func = ext_tools.ext_function('cg_transform_pairs',code,
        ['iparam', 'dparam', 'src', 'dst'])
    mod.add_function(func)

    #----------------------------------------------------------
    # return the module
    #----------------------------------------------------------
    return mod.setup_extension(location,**kw)


# ==============================================================================
# Configuration
# ==============================================================================
def configuration(parent_package='',top_path=None):
    from numpy.distutils.misc_util import Configuration, dot_join
    config = Configuration('cg',parent_package,top_path)
    
    config.add_data_dir('tests')

    ext = build_extension('extension', config.local_path)
    ext.name = dot_join(config.name,ext.name)
    config.ext_modules.append(ext)    
    
    return config

if __name__ == '__main__':
    print 'This is the wrong setup.py file to run'
