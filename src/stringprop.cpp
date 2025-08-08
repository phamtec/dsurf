/*
  stringprop.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dead
*/

#include "stringprop.hpp"

void StringProp::render(Renderer &renderer, Font &font, float x, float *y) {

  float origy = *y;
  _name.render(renderer, font, x, y);
  _value.render(renderer, font, x + 100, &origy);
  
}
