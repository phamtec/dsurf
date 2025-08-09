/*
  stringprop.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "stringprop.hpp"
#include "sizes.hpp"

float StringProp::layout(Resources &res, float x, float y) {

  _r = { .x = x, .y = y, .w = 100, .h = _name.height() };
  return _r.h;
  
}

void StringProp::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  _value.build(renderer, font);

}

void StringProp::render(Renderer &renderer, Resources &res) {

  super::render(renderer, res);
  
  _value.render(renderer, _r.x + _name.width() + Sizes::name_var_padding, _r.y);
  
}
