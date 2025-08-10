/*
  spatial.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "spatial.hpp"

void Spatial::calcPan(const Point &p, Point *last, Size *offs) {

  Point d = *last - p;
  *last = p;
  *offs += Size(d);
  
}

Point Spatial::center(float width, float height, const Size &size, float scale) {

  return {
    .x = ((width - (size.w * scale)) * (1/scale)) / 2, 
    .y = ((height - (size.h * scale)) * (1/scale)) / 2 
  };
    
}
