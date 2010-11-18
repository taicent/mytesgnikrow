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
"""sdpy - SharkDolphin's Computer Science modules for Python

sdpy is an umbrella package of C++ and Python modules useful for
computer science. The package have been mainly being developed by 
Minh-Tri Pham, as part of his Ph.D. research. Since his research area
is about face detection, you will find many useful modules for computer
vision and machine learning, especially boosting techniques,
Haar-like features, and face detection (i.e. (rotation-invariant) frontal 
face).

(To be updated)
"""

DOCLINES = __doc__.split("\n")


import os
import sys
from numpy.distutils.misc_util import Configuration
from numpy.distutils.core import setup


CLASSIFIERS = """\
Development Status :: 3 - Alpha
Intended Audience :: Science/Research
Intended Audience :: Developers
Programming Language :: C
Programming Language :: Python
Topic :: Software Development
Topic :: Scientific/Engineering
Operating System :: Microsoft :: Windows
Operating System :: POSIX
Operating System :: Unix
Operating System :: MacOS
"""



def configuration(parent_package='',top_path=None):
    config = Configuration(None,parent_package,top_path)
    config.add_subpackage('sdpy')
    config.add_data_files(('sdpy','*.txt'),
                          ('sdpy','site.cfg.example'))
    config.get_version('sdpy/version.py') # sets config.version
    return config

def setup_package():
    old_path = os.getcwd()
    local_path = os.path.dirname(os.path.abspath(sys.argv[0]))
    os.chdir(local_path)
    sys.path.insert(0,local_path)

    try:
        setup(
            name = 'sdpy',
            maintainer = "Minh-Tri Pham et. al.",
            maintainer_email = "mtpham@ntu.edu.sg",
            description = DOCLINES[0],
            url = "http://code.google.com/p/sdpy",
            license = "GNU Public License version 3",
            author = "Minh-Tri Pham et. al.",
            author_email = "mtpham@ntu.edu.sg",
            platforms = 'Windows, Linux, MacOS',
            classifiers = filter(None, CLASSIFIERS.split('\n')),
            long_description = "\n".join(DOCLINES[2:]),
            configuration = configuration,
            )
    finally:
        del sys.path[0]
        os.chdir(old_path)
    return

    
if __name__ == '__main__':
    setup_package()
