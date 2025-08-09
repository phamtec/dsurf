/*
  dict.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "dict.hpp"
#include "renderer.hpp"
#include "resources.hpp"
#include "sizes.hpp"
#include "list.hpp"

#include <iostream>

using namespace std;

float Dict::layout(Resources &res, float x, float y) {

  _r = { .x = x, .y = y, .w = 100, .h = List::layoutVector(res, x, y, res.open_brace.height(), _objs) + res.close_brace.height() };
  return _r.h;
  
}

void Dict::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  List::buildVector(renderer, font, _objs);
  
}

void Dict::render(Renderer &renderer, Resources &res) {

  super::render(renderer, res);
  
  res.open_brace.render(renderer, _r.x, _r.y);
  List::renderVector(renderer, res, _objs);
  res.close_brace.render(renderer, 
    _r.x + (_objs.size() == 0 ? res.open_brace.width() + Sizes::text_padding : 0), 
    _r.y + (_objs.size() == 0 ? 0 : _r.h - res.close_brace.height()));

//  renderer.renderRect(_r);

}
