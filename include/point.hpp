/*
  point.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Aug-2025
    
  A point class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_point
#define H_point

#include <SDL3/SDL_rect.h>
#include <iostream>

class Size;

class Point {

public:
  Point(): x(0), y(0) {}
  Point(float x1, float y1): x(x1), y(y1) {};
  Point(const Size &size);

  float x;
  float y;
  
  inline Point& operator+=(float offset) {
    x += offset;
    y += offset;
    return *this;
  }

  inline Point operator-(const Point& other) const {
    return Point(x - other.x, y - other.y);
  }
  
  inline Point operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
  }
  
  inline Point operator-(float offset) const {
    return Point(x - offset, y - offset);
  }

  inline Point operator+(float offset) const {
    return Point(x + offset, y + offset);
  }

  inline Point operator*(float offset) const {
    return Point(x * offset, y * offset);
  }

  friend std::ostream& operator<< (std::ostream& stream, const Point& matrix);
  
  inline SDL_FPoint spoint() const {
    return { .x = x, .y = y };
  }

};

#endif // H_point
