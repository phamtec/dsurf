/*
  rect.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "rect.hpp"

std::ostream& operator <<(std::ostream& stream, const Rect& rect) {

  stream << rect.origin << ", " << rect.size;
  return stream;
  
}

bool Rect::contains(const Point &p) {

  SDL_FPoint np = p.spoint();
  SDL_FRect nr = srect();
  return SDL_PointInRectFloat(&np, &nr);

}
