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

__all__ = ['sdpath','datapath','tprint','speak','ordinal']

from win32com.client import Dispatch
from time import ctime
from os.path import join, dirname

sdpath = dirname(__file__)
datapath = join( sdpath, 'data' )

_logfile = open(join( sdpath, 'mylog.txt' ), 'w')
_speaker = Dispatch("SAPI.SpVoice")

def tprint(x):
    """Print a message to stdout but also fprint it to the global log file."""
    s2 = ctime()+": "+str(x)
    print s2
    _logfile.write(s2+'\n')

def speak(s):
    """Speak a message using text-to-speech (windows-dependent)."""
    _speaker.Speak(str(s))

def ordinal(n):
    """Return the string representing the n-th position of a series, e.g. 0th, 1st, 2nd, 3rd, etc."""
    b = (n/10)%10
    if b == 1:
        suffix = 'th'
    else:
        a = n%10
        if a == 1:
            suffix = 'st'
        elif a == 2:
            suffix = 'nd'
        elif a == 3:
            suffix = 'rd'
        else:
            suffix = 'th'
    return str(n)+suffix
