/*
  stringprop.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "stringprop.hpp"

float StringProp::layout(float x, float y) {

  _x = x;
  _y = y;
  _width = 640;
  _height = 20;
  return _height;
  
}

void StringProp::render(Renderer &renderer, Font &font) {

  _name.render(renderer, font, _x, _y);
  _value.render(renderer, font, _x + 100, _y);
  
}
