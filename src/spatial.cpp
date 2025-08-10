/*
  spatial.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "spatial.hpp"

void Spatial::calcPan(const SDL_FPoint &p, SDL_FPoint *last, SDL_FPoint *offs) {

  SDL_FPoint d = Spatial::makePoint(last->x - p.x, last->y - p.y);
  *last = p;
  Spatial::add(offs, d);
  
}
