# -*- coding: utf-8 -*-

# ///////////////////////////////////////////////////////////////////////////
# $Id: Container.py 6642 2022-03-29 03:17:13Z jhill $
# Author: jeremy.hill@neurotechcenter.org
# Description: helper container class and pretty-printing routine
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

__all__ = [
	'Summarize',
	'Bunch',
]

import sys
import copy
import types

try: import numpy
except ImportError: numpy = None

if sys.version < '3': bytes = str
else: unicode = str; basestring = ( unicode, bytes )

def Summarize(a, appendBeforeComment='', limit=50 ):
	"""
	Returns a somewhat easier-to-swallow string representation of
	an object, when the full repr() would otherwise be too long.
	In particular, summarize the interesting atrributes of numpy
	arrays and matrices.
	"""
	if a is None: return "None" + appendBeforeComment

	if   numpy and isinstance(a, numpy.matrix): atype = 'numpy.matrix'
	elif numpy and isinstance(a, numpy.ndarray): atype = 'numpy.array'
	elif isinstance(a, (types.MethodType, types.BuiltinMethodType)):
		obj = getattr(a, '__self__', None)
		if obj is None: atype = repr( a )
		else: atype = 'method %s of %s instance 0x%08x' % (a.__name__,obj.__class__.__name__, id(obj))
		return atype + appendBeforeComment
	elif isinstance(a, (types.FunctionType, types.BuiltinFunctionType)):
		atype = 'function ' + '.'.join(x for x in (a.__module__, a.__name__) if x)
		return atype + appendBeforeComment
	else: atype = a.__class__.__name__
	
	if hasattr(a, 'shape'):
		shape = 'shape='+str(a.shape).replace(' ', '')
		nels = 1
		for extent in a.shape: nels *= extent
	elif hasattr(a, '__len__'):
		shape = 'length='+str(len(a))
		nels = len(a)
	else:
		shape = ''
		nels = 1
	
	if nels <= limit:
		s = repr(a) if isinstance(a, basestring) else ','.join(str(a).split()).replace('[,','[').replace(',]',']').replace(',,',',')
		if len(s) <= limit and type(a) in [float,int,str,bytes,unicode,tuple,list,dict]: atype = s
		elif len(s) <= limit - len(atype) - 2: atype += '(' + s + ')'
		else: atype += '(...)'
	else: atype += '(...)'

	if isinstance(a, (float,int,bytes,unicode,tuple)): addr = ''
	else: addr = 'id=0x%08x' % id(a)
	
	if numpy and isinstance(a, numpy.ndarray): base = a.base
	else: base = None
	if base is None: base = ''
	else: base = 'base=0x%08x' % id(base)
	
	dtype = getattr(getattr(a, 'dtype', ''), 'name', '')
	if dtype != '': dtype = 'dtype=numpy.'+dtype
	
	try: order = '' if a.ndim < 2 else "order='F'" if a.flags.farray else "order='C'"
	except: order = ''
	
	s =  ', '.join( x for x in ( addr, shape, dtype, order, base ) if len( x ) )
	s = ' # '.join( x for x in ( atype + appendBeforeComment, s )  if len( x ) )
	return s	

class Bunch( dict ):
	"""
	A `dict` subclass whose members can be addressed lazily `like.this` as well as
	`like['this']` provided the key does not start with an underscore.
	
	Optionally set the `._summarize` attribute to a number of characters to invoke
	`Summarize()` when converting the object to `str`.
	
	Useful methods include `._read(filename)`, `._write(filename)` and also
	`._convert(d)` which hierarchically converts any dict `d` (and its values, where
	appropriate) to this class.
	"""
	def __getattr__( self, name ):
		b = self
		for name in name.split( '.' ): b = b[ name ] if name in b else getattr( super( b.__class__, b ), name ) if isinstance( b, Bunch ) else getattr( b, name )
		return b
	def __setattr__( self, name, value ):
		if name.startswith( '_' ): return dict.__setattr__( self, name, value )
		container = self
		parts = name.split( '.' )
		for name in parts[ :-1 ]:
			try: container = getattr( container, name )
			except: sub = container[ name ] = self.__class__(); container = sub
		container.__setitem__( parts[ -1 ], value )
	def __dir__( self ): return self.keys()
	_getAttributeNames = __dir__
	def __repr__( self ): return self._report()
	_display_sorted = tuple( sys.version_info ) < ( 3, 0 )
	_summarize = False
	def _report( self, indent=0, minColonPosition=0, sortUnknownKeys=None, summarize=None ):
		s = ' ' * indent + '{\n'
		keys = list( self.keys() )
		order = getattr( self, '_fieldOrder', '' )
		known = order.replace( '/', '' ).split()
		order = order.split()
		unknown = [ key for key in keys if key not in known ]
		if sortUnknownKeys or ( sortUnknownKeys is None and self._display_sorted ): unknown.sort()
		keys = [ key for key in order if key.strip( '/' ) in keys ] + unknown
		maxLen = max( len( repr( key ) ) for key in keys ) if keys else 0
		indentIncrement = 4
		minColonPosition = max( minColonPosition - indentIncrement, maxLen + indent + indentIncrement + 1 )
		#minColonPosition = max( minColonPosition, maxLen + indent + indentIncrement + 1 )
		#minColonPosition = maxLen + indent + indentIncrement + 1
		if summarize is None: summarize = self._summarize
		for key in keys:
			if isinstance( key, basestring ):
				if key.startswith( '//' ): s += '\n'; key = key[ 2: ]
			krepr = repr( key )
			spaces = minColonPosition - len( krepr ) - 1
			spacesBefore = indent + indentIncrement
			#spacesBefore = spaces
			spacesAfter = spaces - spacesBefore
			s += ' ' * spacesBefore + krepr + ' ' * spacesAfter + ' : '
			value = self[ key ]
			if hasattr( value, '_report' ):
				s += '\n' + value._report( indent=indent + indentIncrement, minColonPosition=minColonPosition + indentIncrement, sortUnknownKeys=sortUnknownKeys, summarize=summarize ).rstrip() + ','
			elif summarize:
				s += Summarize( value, appendBeforeComment=',', limit=summarize if summarize > 1 else 50 )
			else:
				vrepr = repr( value ).strip()
				if '\n' in vrepr: vrepr = ( '\n' + vrepr ).replace( '\n', '\n' + ' ' * ( spacesBefore + indentIncrement ) )
				s += vrepr + ','
			s += '\n'
		s += ' ' * ( indent ) + '}'
		return s
	def _copy( self, deep=True ):
		return copy.deepcopy( self ) if deep else copy.copy( self )
	def _set( self, **kwargs ):
		for k, v in kwargs.items(): setattr( self, k, v )
		return self
	def _write( self, filename ):
		open( filename, 'wt' ).write( self._report( summarize=False ) )
	@classmethod
	def _read( cls, filename ):
		return cls._convert( ast.literal_eval( open( filename, 'rt' ).read() ) )
	@classmethod
	def _convert( cls, d ):
		return cls( { k : cls._convert( v ) for k, v in d.items() } ) if isinstance( d, dict ) and not isinstance( d, cls ) else d
