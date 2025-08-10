/*
  point.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "point.hpp"
#include "size.hpp"

Point& Point::operator+=(const Size &size) {
  x += size.w;
  y += size.h;
  return *this;
}
