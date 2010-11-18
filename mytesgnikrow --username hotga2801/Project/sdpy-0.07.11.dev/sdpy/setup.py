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

def configuration(parent_package='',top_path=None):
    from numpy.distutils.misc_util import Configuration
    config = Configuration('sdpy',parent_package,top_path)
    config.add_subpackage('cs')
    config.add_subpackage('interfaces')
    config.add_subpackage('libraries')
    config.add_data_dir('tests')
    return config

if __name__ == '__main__':
    print 'This is the wrong setup.py file to run'
