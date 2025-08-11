/*
  size.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Aug-2025
    
  A dimension class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_size
#define H_size

#include <iostream>

class Point;

class Size {

public:
  Size(): w(0), h(0) {}
  Size(float w1, float h1): w(w1), h(h1) {};
  Size(const Point &point);
  
  float w;
  float h;
  
  inline Size& operator-=(float offset) {
    w -= offset;
    h -= offset;
    return *this;
  }

  inline Size& operator+=(const Size &other) {
    w += other.w;
    h += other.h;
    return *this;
  }
  
  inline Size& operator-=(const Size &other) {
    w -= other.w;
    h -= other.h;
    return *this;
  }
  
  inline Size& operator*=(float d) {
    w *= d;
    h *= d;
    return *this;
  }

  inline Size& operator/=(float d) {
    w /= d;
    h /= d;
    return *this;
  }

  inline Size operator+(const Size& other) const {
    return Size(w + other.w, h + other.h);
  }

  friend std::ostream& operator<< (std::ostream& stream, const Size& matrix);
  
};

#endif // H_size
