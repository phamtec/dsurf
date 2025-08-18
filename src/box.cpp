/*
  box.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "box.hpp"

Box *Box::hitTest(const Point &origin, const Point &p) { 

  if (Rect(origin, _size).contains(p)) {
    return this; 
  }
  
  return nullptr;
  
};
