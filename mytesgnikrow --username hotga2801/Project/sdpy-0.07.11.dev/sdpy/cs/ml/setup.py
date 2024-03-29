#!/usr/bin/env python

from os.path import join

def configuration(parent_package='',top_path=None):
    from numpy.distutils.misc_util import Configuration
    config = Configuration('ml',parent_package,top_path)
    config.add_subpackage('cla')
    config.add_subpackage('reg')
    config.add_data_dir('tests')
    return config

if __name__ == '__main__':
    from numpy.distutils.core import setup
    setup(configuration=configuration)
