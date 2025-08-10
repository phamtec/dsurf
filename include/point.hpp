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

#include <iostream>

class Size;

class Point {

public:

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
  
  Point& operator+=(const Size &size);
  
  friend std::ostream& operator<< (std::ostream& stream, const Point& matrix);
  
};

#endif // H_point
