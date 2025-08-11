/*
  spatial.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Aug-2025
    
  Spatial tools.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_spatial
#define H_spatial

#include "point.hpp"
#include "size.hpp"
#include "rect.hpp"

class Spatial {

public:

  static void calcPan(const Point &p, Point *last, Size *offs, float scale);
    // calculate the pan. Takes the new point of a pan
    // last point of the pan, the offset to pan
    
  static Point center(const Size &wsize, const Size &size, float scale);
    // given a window size, center the object of size
    // qssuming the drawing scale
    // returns of the offset to draw the object at.
    
  static void scaleAndCenter(const Size &wsize, const Size &osize, const Point &mouse, float amount, float prescale, float *scale, Size *offset);
    // scale, but try to keep the object centered on the cursor while scaling.
    
};

#endif // H_spatial
