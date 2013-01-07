MyAddon
=====================================

Introduction
------------
C++ openFrameworks addon with a set of template classes for doing various types of interpolations on data with any number of dimensions. You can feed the system an arbitrary number of data, then resample at any resolution, or ask for the value at any percentage along the data. Input data can be floats (for 1D splines, Vec2f (for 2D splines), Vec3f (for 3D splines), or even matrices, or custom data types (e.g. biped pose).
Demo at [www.memo.tv/msainterpolator](http://www.memo.tv/msainterpolator)


Licence
-------
The code in this repository is available under the [MIT License](https://secure.wikimedia.org/wikipedia/en/wiki/Mit_license).  
Copyright (c) 2008-2012 Memo Akten, [www.memo.tv](http://www.memo.tv)  
The Mega Super Awesome Visuals Company


Installation
------------
Copy to your openFrameworks/addons folder.

Dependencies
------------
- MSACore

Compatibility
------------
openFrameworks 0072  
I am generally testing only with [openFrameworks](www.openframeworks.cc), however it should work with [Cinder](www.libcinder.org) too. If it doesn't, please file an issue.


Known issues
------------
none

Version history
------------
### v2.1    23/09/2012
- compatible with OF0072
- renamed (uppercase) MSA namespace to (lowercase) msa. (kept MSA as an alias for backwards compatibility)
- interpolating by distance seems broken

### v2.0
- move to centralized MSALibs (requires MSACore)
- everything is msa:: namespace
- renamed useDistance to useLength (and all relevant functions)
- using length now uses length of interpolated spline, not linear
- fixed rare crash bug

### v1.1	07/04/2009
- changed license to revised BSD (a lot more more permissive than GPL)

### v1.0	03/10/08
- initial version



