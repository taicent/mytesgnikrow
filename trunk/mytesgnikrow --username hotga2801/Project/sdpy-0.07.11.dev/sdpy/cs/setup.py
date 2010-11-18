#!/usr/bin/env python

from os.path import join

def configuration(parent_package='',top_path=None):
    from numpy.distutils.misc_util import Configuration
    config = Configuration('cs',parent_package,top_path)
    config.add_subpackage('cg')
    config.add_subpackage('cv')
    config.add_subpackage('dm')
    config.add_subpackage('ml')
    config.add_subpackage('opt')
    config.add_subpackage('linalg')
    config.add_subpackage('stats')
    config.add_data_dir('tests')
    return config

if __name__ == '__main__':
    from numpy.distutils.core import setup
    setup(configuration=configuration)
