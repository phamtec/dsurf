/*
  size.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "size.hpp"
#include "point.hpp"

Size::Size(const Point &point): w(point.x), h(point.y) {
}
