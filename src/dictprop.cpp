/*
  dictprop.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "dictprop.hpp"
#include "renderer.hpp"
#include "resources.hpp"
#include "sizes.hpp"
#include "list.hpp"

#include <iostream>

using namespace std;

float DictProp::layout(Resources &res, float x, float y) {

  _r = { .x = x, .y = y, .w = 100, .h = List::layoutVector(res, x, y, _name.height(), _objs) };
  return _r.h + (_objs.size() > 0 ? res.close_brace.height() : 0); // closed bracket if not empty
}

void DictProp::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  List::buildVector(renderer, font, _objs);

}

void DictProp::render(Renderer &renderer, Resources &res) {

  super::render(renderer, res);
  
  res.open_brace.render(renderer, _r.x + _name.width() + Sizes::text_padding, _r.y);
  List::renderVector(renderer, res, _objs);
  res.close_brace.render(renderer, 
    _r.x + (_objs.size() == 0 ? _name.width() + res.open_brace.width() + (Sizes::text_padding * 2) : 0), 
    _r.y + (_objs.size() == 0 ? 0 : _r.h));

}
