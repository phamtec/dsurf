/*
  string.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "string.hpp"
#include "sizes.hpp"
#include "spatial.hpp"

float String::layout(Resources &res, const SDL_FPoint &origin) {

  Spatial::setOrigin(&_r, origin);
  Spatial::setDimensions(&_r, 100, _value.height());
  return _r.h;
  
}

void String::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  _value.build(renderer, font);

}

void String::render(Renderer &renderer, Resources &res) {

  super::render(renderer, res);
  
  _value.render(renderer, Spatial::origin(_r));
  
}
