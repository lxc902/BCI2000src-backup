# -*- coding: utf-8 -*-

# ///////////////////////////////////////////////////////////////////////////
# $Id: Plotting.py 6642 2022-03-29 03:17:13Z jhill $
# Author: jeremy.hill@neurotechcenter.org
# Description: graphical helper functions (matplotlib wrappers)
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
	'load_plt',
	'plot',
	'imagesc', 'scatterplot', 'scatcmp', 'stem',
	'colorbar', 'rmcolorbar',
	'make_cmap', 'transform_cmap', 'show_cmaps',
	'subplots',
	'curve',
	'cax',
	'superimpose', 'look', 'hotfig', 
]

import sys

import numpy

if sys.version < '3': bytes = str
else: unicode = str; basestring = ( unicode, bytes )

def load_plt():
	try:
		import matplotlib,sys
		set_interactive = ('matplotlib.backends' not in sys.modules and 'IPython' in sys.modules)
		if set_interactive: matplotlib.interactive(True)
		import matplotlib.pyplot as plt
	except:
		print( __name__ + " module failed to import matplotlib.pyplot: plotting methods will not work" )
	else:
		if set_interactive and hasattr(plt, 'ion'): plt.ion()
		return plt

def project(a, maxdim):
	"""
	Return a view of the numpy array <a> that has at least <maxdim>+1
	dimensions.
	"""
	if isinstance(a, numpy.matrix) and maxdim > 1: a = numpy.asarray(a)
	else: a = a.view()
	a.shape += type( a.shape )( (1,) * (maxdim-len(a.shape)+1) )
	return a

def filterdict(d, exact=(), startswith=None, endswith=None):
	f = dict([(k,v) for k,v in d.items() if k in exact or (startswith is not None and k.startswith(startswith)) or (endswith is not None and k.endswith(endswith))])
	for k in f: d.pop(k)
	return f

def plot(*pargs,**kwargs):
	"""
	A wrapper around plt.plot that reduces dimensionality-related fiddliness.
	
	plot(x, y)
	plot(y)
	
	where either x or y, independent of each other, can specify multiple lines.
	
	Additional options and their defaults:
		axis = 0         Along which dimension can one step while staying on
		                 the same graphical line?
		stem = False     If True, do a stem plot instead of an ordinary line
		grid = True      Whether to turn on the grid.
		balance = None   If None, do nothing. If a numeric value, balance the
		                 y axis limits around this value.
		aspect = None    If 'auto', 'image' or 'equal', apply that aspect mode
		hold = False     If False, clear the axes before plotting.
		drawnow = True   If True, execute plt.draw() after plotting.
	"""
	hold = kwargs.pop('hold', False)
	axis = kwargs.pop('axis', 0)
	drawnow = kwargs.pop('drawnow', True)
	grid = kwargs.pop('grid', True)
	balance = kwargs.pop('balance', None)
	stem = kwargs.pop('stem', False)
	aspect = kwargs.pop('aspect', None)
	axkwargs = filterdict(kwargs, ['title', 'xlabel', 'ylabel', 'ylim', 'xlim'])

	pargs = list(pargs) # makes a copy, at least of the list container
	allvec = True
	for i in range(len(pargs)):
		if isinstance(pargs[i], (tuple,list)):
			pargs[i] = numpy.array(pargs[i],dtype=numpy.float64)
		if isinstance(pargs[i], numpy.ndarray):
			allvec &= (max(pargs[i].shape) == numpy.prod(pargs[i].shape))
			if len(pargs[i].shape) > 1:
				pargs[i] = project(pargs[i],axis).swapaxes(0,axis)
			isinf = numpy.isinf(pargs[i])
			if numpy.any(isinf):
				pargs[i] = pargs[i].copy()
				pargs[i][isinf] = numpy.nan # crude workaround---plt.plot can't cope with infinite values
	if allvec:
		for i in range(len(pargs)):
			if isinstance(pargs[i], numpy.matrix):  pargs[i] = pargs[i].A
			if isinstance(pargs[i], numpy.ndarray): pargs[i] = pargs[i].flatten()	
	pargs = tuple(pargs)

	plt = load_plt()
	if not hold: # undo a few known hangovers before clearing
		ax = plt.gca()
		ax.set_aspect('auto') # image aspect ratio
		ax.set_ylim(sorted(ax.get_ylim())) # ydir reversed
		rmcolorbar(drawnow=False) # colorbar
		plt.cla()
	
	if stem:
		if len(pargs) == 1: pargs = (range(pargs[0].shape[axis]),) + pargs
		p = plt.stem(*pargs, **kwargs)
		x = pargs[0]
		xl = numpy.r_[float(x[0]), float(x[-1])]
		if len(x) == 1: xl += [-0.5, 0.5]
		else: xl += numpy.r_[float(x[0]-x[1]), float(x[-1]-x[-2])] / 2.0
		plt.gca().set_xlim(xl)
	else:
		p = plt.plot(*pargs,**kwargs)
	plt.grid(grid)
	ax = plt.gca()
	if balance is not None:
		yl = numpy.array(ax.get_ylim())
		yl = balance + numpy.array([-1,+1]) * max(abs(yl - balance))
		ax.set_ylim(yl)
	if aspect is not None: ax.set_aspect({'image':'equal'}.get(aspect, aspect))
	ax.set(**axkwargs)
	if drawnow: plt.draw()
	return p
	

def imagesc(img, x=None, y=None, hold=False, drawnow=True, aspect='image', balance=None, colorbar=False, grid=False, ydir='reverse', **kwargs):
	kwargs['interpolation'] = kwargs.get('interpolation', 'nearest')
	kwargs['cmap'] = kwargs.get('cmap', 'kelvin_i')
	cbkwargs = filterdict(kwargs, startswith='colorbar')
	cbkwargs = dict([(k[len('colorbar'):],v) for k,v in cbkwargs.items()])
	axlabelkwargs = filterdict(kwargs, ['title', 'xlabel', 'ylabel'])
	auto_aspect = {'equal':False, 'image':False, 'auto':True}.get(aspect)
	if auto_aspect is None: raise ValueError('aspect should be "image" or "auto"')
	plt = load_plt()
	ax = plt.gca()
	if not hold: ax.cla()
	
	img = numpy.asarray(img)

	xlab,ylab = None,None
	if isinstance(x, (tuple,list,numpy.ndarray)) and isinstance(x[0], basestring): xlab,x = x,None
	if isinstance(y, (tuple,list,numpy.ndarray)) and isinstance(y[0], basestring): ylab,y = y,None
	if x is None: x = numpy.arange(img.shape[1], dtype=numpy.float64)
	if y is None: y = numpy.arange(img.shape[0], dtype=numpy.float64)
	x = numpy.asarray(x).flatten()
	y = numpy.asarray(y).flatten()
	if len(x) == 1: xl = [ x[ 0 ] - 0.5, x[ 0 ] + 0.5 ]	
	else: xl = [x[0] - 0.5 * (x[1]-x[0]),   x[-1] + 0.5 * (x[-1]-x[-2])]
	if len(y) == 1: yl = [ y[ 0 ] - 0.5, y[ 0 ] + 0.5 ]	
	else: yl = [y[0] - 0.5 * (y[1]-y[0]),   y[-1] + 0.5 * (y[-1]-y[-2])]
	tol = 1e-10
	regular = ( len(x) == 1 or (numpy.diff(x).ptp() < tol) ) and ( len(y) == 1 or (numpy.diff(y).ptp() < tol) )
	if ydir == 'reverse': yl = yl[::-1]; origin='upper'
	else: origin='lower'
	if not regular: raise ValueError('x and y data must be regularly spaced') # NonUniformImage just doesn't play nice, with ydir or with clim scaling
	h = plt.imshow(img, extent=xl+yl, origin=origin, **kwargs)		
	if xlab is not None: ax.set_xticks(x); ax.set_xticklabels(list(xlab))
	if ylab is not None: ax.set_yticks(y); ax.set_yticklabels(list(ylab))
	if auto_aspect: ax.set_aspect('auto')
	else: ax.set_aspect('equal')
	plt.grid(grid)
	ax.set_xlim(xl)
	ax.set_ylim(yl)
	ax.set(**axlabelkwargs)
	if balance is not None:
		c = numpy.array(h.get_clim())
		c = balance + numpy.array([-1,+1]) * max(abs(c - balance))
		h.set_clim(c)
	if colorbar: _colorbar(parent_axes=ax, **cbkwargs)
	else: rmcolorbar(parent_axes=ax)
	if drawnow: plt.draw()
	return h

def scatterplot(*pargs, **kwargs):
	"""
	scatterplot(xy)         # scatterplots xy[:,1].flat against xy[:,0].flat
	scatterplot(xy1, 'r*')
	scatterplot(xy1, 'r*', xy2, 'bs')
	scatterplot(xy1, 'r*', xy2, 'bs', markersize=10, hold=False, drawnow=True)
	
	scatterplot(x, y)      # scatterplots y.flat against x.flat
	scatterplot(x1, y1, 'r*')
	scatterplot(x1, y1, 'r*', x2, y2, 'bs')
	scatterplot(x1, y1, 'r*', x2, y2, 'bs', markersize=10, hold=False, drawnow=True)
	"""
	x = None
	y = None
	plotargs = []
	connected = []
	if not isinstance(pargs[-1], basestring):
		pargs = list(pargs) + ['*']
	for i,arg in enumerate(pargs):
		if isinstance(arg, basestring):
			if y is None:
				if max( x.shape ) == x.size: x, y = numpy.arange( x.size ), x.flatten()
				else: x,y = x[:, 0], x[:, 1]
			plotargs += [x.flatten(), y.flatten(), arg]			
			connected.append('-' in arg)
			x = y = None
		elif x is None: x = arg
		elif y is None: y = arg
	h = plot(*plotargs, **kwargs)
	for hi in zip(h, connected):
		if not connected: hi.set_linestyle('None')
	return h
	
def scatcmp(a, b, hold=False, drawnow=True, markerorder=None, **kwargs):
	kwargs['linestyle'] = kwargs.get('linestyle', 'None')
	grid = kwargs.pop('grid', True)
	grid = {'on':True, 'off':False}.get(grid,grid)
	plt = load_plt()
	if not hold: plt.cla()
	h = plot(a, b, drawnow=False, hold=hold, **kwargs)
	import matplotlib
	if 'marker' not in kwargs:
		if markerorder is None:
			mm = matplotlib.lines.lineMarkers
			keyfn = lambda x: 0 if x is None else x
			markerorder = sorted([m for m in mm if isinstance(m, basestring) and len(m.strip())], key=keyfn) + sorted([m for m in mm if not isinstance(m, basestring)], key=keyfn)
		for i,hi in enumerate(h): hi.set_marker(markerorder[i % len(markerorder)])
	ax = plt.gca()
	lim = ax.get_xlim() + ax.get_ylim()
	lim = (min(lim),max(lim))
	ax.set_xlim(lim)
	ax.set_ylim(lim)
	ax.grid(grid)
	plt.plot(lim, lim, linestyle='-', color=(0.6,0.6,0.6), scalex=False, scaley=False)
	if drawnow: plt.draw()
	return h

def stem(*pargs, **kwargs):
	kwargs['stem'] = True
	return plot(*pargs, **kwargs)


def colorbar(mappable=None, parent_axes=None, **kwargs):
	"""
	Create a colorbar in such a way that it can be found again and reused, or removed.
	Or, if one already exists associated with the specified parent_axes, use and
	update the existing one.  Use rmcolorbar() to remove.  matplotlib, y u no do this?
	"""
	plt = load_plt()
	if mappable is None:
		if parent_axes is None: mappable = plt.gci()
		else: mappable = parent_axes.get_images()[-1]  # NB: doesn't find patch, surf, etc - only images. So to use this with non-images, supply the handle explicitly instead of relying on gca()
	ax = parent_axes
	if ax is None: ax = mappable.get_axes()
	cb = getattr(ax, 'colorbar', None)
	if cb is None:
		kwargs['ax'] = ax
		ax.oldposition = ax.get_position()
	else:
		kwargs['cax'] = cb.ax
		cb.ax.cla()
	title = kwargs.pop('title', None)
	ax.colorbar = plt.colorbar(mappable, **kwargs)
	if title is not None: ax.colorbar.ax.set_title(title)
	plt.draw()
	return ax.colorbar
_colorbar = colorbar

def rmcolorbar(parent_axes=None, drawnow=True):
	"""
	Remove a colorbar created with colorbar() from this module
	"""
	plt = load_plt()
	ax = parent_axes
	if ax is None: ax = plt.gca()
	cb = getattr(ax, 'colorbar', None)
	if cb is None: return
	cb.ax.get_figure().delaxes(cb.ax)
	ax.colorbar = None
	if getattr(ax, 'oldposition', None) is not None:
		ax.set_position(ax.oldposition)
	if drawnow: plt.draw()

def make_cmap(cmap, name, n=256):
	plt = load_plt(); import matplotlib
	cmap = matplotlib.colors.LinearSegmentedColormap(name, cmap, n)
	try: matplotlib.cm.register_cmap(name=name, cmap=cmap)
	except: print( "failed to register colormap '%s'" % name )
	return cmap

def transform_cmap(cmap, name=None, n=256, reverse=False, complement=False):
	plt = load_plt()
	if isinstance(cmap, basestring): cmap = plt.cm.get_cmap(cmap)
	cmap = getattr(cmap, '_segmentdata', cmap)
	out = {}
	for k in ('red', 'green', 'blue'):
		v = numpy.array(cmap[k])
		if complement: v[:, 1:] = 1.0 - v[:, 1:]
		if reverse:    v[:, 1:] = v[::-1, 1:]
		out[k] = v
	if name is not None: out = make_cmap(out, name=name, n=n)
	return out

def show_cmaps(*pargs):
	plt = load_plt()
	cmaps = []
	for arg in pargs:
		if isinstance(arg, basestring): arg = arg.split()
		if isinstance(arg, (tuple,list)): cmaps += arg
		else: cmaps.append(arg)
	if len(cmaps) == 0: cmaps=sorted([m for m in plt.cm.cmap_d if not m.endswith("_r")])
	plt.clf()
	for i,cmap in enumerate(cmaps):
		plt.subplot(len(cmaps), 1, i+1)
		if isinstance(cmap, basestring): cmap = plt.cm.get_cmap(cmap)
		plt.imshow([numpy.linspace(0.0,1.0,256,endpoint=True)], cmap=cmap)
		plt.gca().set(aspect='auto', yticks=[0.0], yticklabels=[cmap.name])
	plt.draw()

def subplots(r, c=None, fig=None, **kwargs):
	plt = load_plt()
	if fig is None: fig = plt.gcf()
	if c is None:
		if isinstance(r, int):
			nPlots = r
			ar = fig.get_size_inches()
			ar = float(ar[0])/float(ar[1])
			layout = numpy.r_[nPlots/ar, nPlots*ar] ** 0.5
			i = numpy.argmin(abs(layout - numpy.round(layout)))
			r = int(round(layout[i]))
			c = int(numpy.ceil(nPlots/float(r)))
			if i == 1: r,c = c,r
			while r * (c-1) >= nPlots: c -= 1
			while (r-1) * c >= nPlots: r -= 1
		else:
			r,c = r
	i = 0
	ax = []
	for ri in range(r):
		row = []; ax.append(row)
		for ci in range(c):
			row.append(plt.subplot(r, c, i+1, **kwargs))
			i += 1
	return numpy.array(ax)
	
def make_kelvin():
	kelvin_i = {
		'red': (
			(0.000, 0.0, 0.0, ),
			(0.350, 0.0, 0.0, ),
			(0.500, 1.0, 1.0, ),
			(0.890, 1.0, 1.0, ),
			(1.000, 0.5, 0.5, ),
		),
		'green': (
			(0.000, 0.0, 0.0, ),
			(0.125, 0.0, 0.0, ),
			(0.375, 1.0, 1.0, ),
			(0.640, 1.0, 1.0, ),
			(0.910, 0.0, 0.0, ),
			(1.000, 0.0, 0.0, ),
		),
		'blue': (
			(0.000, 0.5, 0.5, ),
			(0.110, 1.0, 1.0, ),
			(0.500, 1.0, 1.0, ),
			(0.650, 0.0, 0.0, ),
			(1.000, 0.0, 0.0, ),
		),
	}
	kelvin_i = make_cmap(kelvin_i, 'kelvin_i', 256)
	kelvin_r = transform_cmap(kelvin_i, 'kelvin_r', 256, complement=True)
	kelvin   = transform_cmap(kelvin_i, 'kelvin',   256, complement=True, reverse=True)

make_kelvin()

def curve(x,y=None, hold=False, drawnow=True, **kwargs):
	plt = load_plt(); import matplotlib, matplotlib.path
	if not hold: plt.cla()
	kwargs['facecolor'] = kwargs.get('facecolor', 'None')
	if y is None: xy = x
	else: xy = numpy.c_[x,y]
	codes = [matplotlib.path.Path.MOVETO] + [matplotlib.path.Path.CURVE4] * (len(xy)-1)
	path = matplotlib.path.Path(xy,codes)
	patch = matplotlib.patches.PathPatch(path, lw=2, **kwargs)
	plt.gca().add_patch(patch)
	if drawnow: plt.draw()

def cax( handles=None, whichAx='c', lim=None, min=None, max=None, include=None, common=False, balance=None, midpoint=None, auto=False ):
	plt = load_plt()
	
	def search_handles( handles, reclevel=0 ):
		out = []
		if reclevel >= 10: zark # allows `debug` of apparent indefinite recursion
		try: handles = list( handles )
		except: handles = [ handles ]
		for h in handles:
			if isinstance( h, ( int, float ) ):
				if h == 0: h = plt.get_fignums()
				elif plt.fignum_exists( h ): h = plt.figure( h )
			while hasattr( h, 'axes' ) and h.axes is not h: h = h.axes   # this was intended to grab the children of a figure, but it can also grab the parent of an AxesImage....
			try: h = list( h )
			except: out.append( h )
			else: out.extend( search_handles( h, reclevel=reclevel+1 ) )
		return out
	if handles in [ 'gca' ]: handles = plt.gca()
	elif handles in [ None, 'gcf' ]: handles = plt.gcf()
	handles = search_handles( handles )
	if whichAx == 'c': handles = sum( [ list( ax.images ) for ax in handles ], [] )  #... with the unintended side-effect that if you pass an AxesImage handle, the function will affect it and any of its siblings on the same axes
	
	all = []
	
	for handle in handles:
		getter = getattr( handle, 'get_' + whichAx + 'lim' )
		setter = getattr( handle, 'set_' + whichAx + 'lim' )
		if auto:
			if whichAx == 'c': handle.autoscale()
			else: handle.autoscale( enable=True, axis=whichAx, tight=True )
		v = list( getter() )
		if lim is not None: v = [ numpy.min( lim ), numpy.max( lim ) ]
		if min is not None: v[ 0 ] = min
		if max is not None: v[ 1 ] = max
		if include is not None: v = v + list( include ); v = [ numpy.min( v ), numpy.max( v ) ]
		if balance and midpoint is None: midpoint = 0.0
		if balance == False: midpoint = None
		if midpoint is not None:
			hw = numpy.max( [ abs( x - midpoint ) for x in v ] )
			v = [ midpoint - hw, midpoint + hw ]
		setter( v )
		all.extend( v )
		
	if common:
		v = [ numpy.min( all ), numpy.max( all ) ]
		for handle in handles:
			setter = getattr( handle, 'set_' + whichAx + 'lim' )
			setter( v )
	plt.draw()

def superimpose( figs='all' ):
	plt = load_plt()
	if figs == 'all': figs = plt.get_fignums()
	if not isinstance( figs, ( tuple, list ) ): figs = [ figs ]
	figs = list( figs )
	for i, f in enumerate( figs ):
		if isinstance( f, int ): figs[ i ] = plt.figure( f )
	geom = figs[ 0 ].canvas.manager.window.geometry()
	for fig in figs[ 1: ]: fig.canvas.manager.window.geometry( geom )
	plt.draw()

def look( fig=None ):
	plt = load_plt()
	if fig is None and len( plt.get_fignums() ): fig = plt.gcf()
	if isinstance( fig, int ): fig = plt.figure( fig )
	try: fig.canvas.manager.show()
	except: pass
	else: return
	try: fig.canvas._tkcanvas.focus_force() # requires   matplotlib.use('tkagg')
	except: pass
	else: return
	try: fig.canvas.manager.window.focus_force() # older matplotlib, qtagg; works but then removes focus from whichever part of the figure is listening to keystrokes :-(
	except: pass
	else: return

def hotfig( figs='all', align=False ):
	plt = load_plt()
	if figs == 'all': figs = plt.get_fignums()
	if not isinstance( figs, ( tuple, list ) ): figs = [ figs ]
	figs = list( figs )
	for i, f in enumerate( figs ):
		if isinstance( f, int ): figs[ i ] = plt.figure( f )
	if align == True: align = figs
	if align: superimpose( align )
	def kp(event):
		if not isinstance( event.key, basestring ): return
		key = event.key.lower()
		if key.startswith( 'alt+' ): key = key[ 4: ]
		codes = list( '1234567890QWERTYUIOP'.lower() )
		if key in codes: target = int( codes.index( key ) ) + 1
		elif key == '[': target = event.canvas.figure.number - 1
		elif key == ']': target = event.canvas.figure.number + 1
		else: target = None
		if target is not None and plt.fignum_exists( target ): look( target )
		
	for fig in figs: fig.canvas.mpl_connect( 'key_press_event', kp )
	look()
