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

  _x = x;
  _y = y;
  _width = 100;
  _height = _name.height();
  return _height;
  
}

void StringProp::build(Renderer &renderer, Font &font) {

  _name.build(renderer, font);
  _value.build(renderer, font);

}

void StringProp::render(Renderer &renderer, Resources &res) {

  _name.render(renderer, _x, _y);
  _value.render(renderer, _x + _name.width() + Sizes::text_padding, _y);
  
}
