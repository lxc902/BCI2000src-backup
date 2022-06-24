# -*- coding: utf-8 -*-

# ///////////////////////////////////////////////////////////////////////////
# $Id: Remote.py 6648 2022-03-30 16:51:49Z jhill $
# Author: jeremy.hill@neurotechcenter.org
# Description: import helper for prog/BCI2000Remote.py
#
# $BEGIN_BCI2000_LICENSE$
#
# This file is part of BCI2000, a platform for real-time bio-signal research.
# [ Copyright (C) 2000-2022: BCI2000 team and many external contributors ]
#
# BCI2000 is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# BCI2000 is distributed in the hope that it will be useful, but
#                         WITHOUT ANY WARRANTY
# - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.
#
# $END_BCI2000_LICENSE$
# ///////////////////////////////////////////////////////////////////////////

"""
For historical reasons and modularity-related reasons the BCI2000RemoteLib
dynamic library, and its wrapper BCI2000Remote.py, live in the `prog` directory
of the BCI2000 distribution, and there is no bootstrapping---from Python,
you would have to manually find it and import/execute it.

The current submodule is a convenience wrapper for finding `BCI2000Remote.py`,
importing the `BCI2000Remote` class from it, and creating a singular
instance of it called, quite simply, `BCI2000`.

Example::

	from BCI2000Tools.Remote import BCI2000
	
	if not BCI2000.Connect(): raise OSError( "failed to connect to BCI2000" )
	BCI2000.StartupModules(['SignalGenerator', 'DummySignalProcessing', 'DummyApplication'])
	BCI2000.Start()

It will only work if the `BCI2000Tools` Python package has been editably-
installed (i.e. with `pip install -e`) in the expected location (i.e. in
the `tools/python` directory of the relevant BCI2000 distribution), as
recommended in our `setup.py` file.

NB: if you have installed the `BCI2000Tools` Python package editably from
one BCI2000 distribution, but temporarily want to access a *different*
BCI2000 distribution, you can say `from BCI2000Tools import bci2000root`
and use `bci2000root()` to configure the location of your desired BCI2000
distribution *before* importing `BCI2000Tools.Remote`.
"""

__all__ = [
	'BCI2000',
	'bci2000root', 'bci2000path',
]

import os
import sys

from . import Bootstrapping; from .Bootstrapping import bci2000root, bci2000path

class BCI2000LayoutError( ImportError ): pass
	
if bci2000root() is None: # it shouldn't be, if the BCI2000Tools package is in the expected place in tools/python
	raise BCI2000LayoutError( "need to set bci2000root() before importing %s" % __name__ )
prog = bci2000path( 'prog' )
if prog is None or not os.path.isabs( prog ):
	raise BCI2000LayoutError( "something went wrong while looking for the `prog` directory" )
if not os.path.isdir( prog ):
	raise BCI2000LayoutError( "directory not found: %s" % prog )
target = os.path.join( prog, 'BCI2000Remote.py' )
if not os.path.isfile( target ):
	raise BCI2000LayoutError( "file not found: %s" % target )
try:
	sys.path.insert( 0, prog )  # or you could navigate the whole version-dependent labyrinth of https://stackoverflow.com/a/67692
	from BCI2000Remote import BCI2000Remote
finally:
	try: sys.path.remove( prog )
	except: pass
BCI2000 = BCI2000Remote()
