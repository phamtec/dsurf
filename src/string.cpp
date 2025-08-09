/*
  string.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "string.hpp"
#include "sizes.hpp"

float String::layout(Resources &res, float x, float y) {

  _r = { .x = x, .y = y, .w = 100, .h = _value.height() };
  return _r.h;
  
}

void String::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  _value.build(renderer, font);

}

void String::render(Renderer &renderer, Resources &res) {

  super::render(renderer, res);
  
  _value.render(renderer, _r.x, _r.y);
  
}
