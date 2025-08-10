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

  static inline Point calcOriginOffset(const Rect &r, float width, float height) {
  // calculate the offset to the origin of a rectangle.
    return Point(r.origin.x + width, r.origin.y + height);
  }
  
  static void calcPan(const Point &p, Point *last, Size *offs);
    // calculate the pan. Takes the new point of a pan
    // last point of the pan, the offset to pan
    
};

#endif // H_spatial
