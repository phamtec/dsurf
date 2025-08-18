/*
  rect.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Aug-2025
    
  A rectangle class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_rect
#define H_rect

#include <SDL3/SDL_rect.h>
#include <iostream>

#include "point.hpp"
#include "size.hpp"

class Rect {

public:
  Rect(): origin(0, 0), size(0, 0) {}
  Rect(const Point &o, const Size &d): origin(o), size(d) {}
  Rect(const SDL_Rect &rect): origin(rect.x, rect.y), size(rect.w, rect.h) {}
  
  Point origin;
  Size size;
  
  inline SDL_FRect srect() const {
    return { .x = origin.x, .y = origin.y, .w = size.w, .h = size.h };
  }
  
  inline Rect& operator-=(float offset) {
    origin += offset;
    size -= offset * 2;
    return *this;
  }

  inline Rect operator+(const Size& dist) const {
    return Rect(origin + dist, size);
  }

  friend std::ostream& operator<< (std::ostream& stream, const Rect& matrix);
  
  bool contains(const Point &p);
  
};

#endif // H_rect
