#!/usr/bin/env python

def configuration(parent_package='',top_path=None):
    from numpy.distutils.misc_util import Configuration
    config = Configuration('interfaces',parent_package,top_path)
    config.add_subpackage('opencv')
    # We don't need sdcpp interface anymore
    #config.add_subpackage('sdcpp')
    return config

if __name__ == '__main__':
    print 'This is the wrong setup.py file to run'
