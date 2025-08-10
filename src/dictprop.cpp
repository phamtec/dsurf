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
#include "spatial.hpp"

#include <iostream>

using namespace std;

float DictProp::layout(Resources &res, const Point &origin) {

  _r.origin = origin;
  _r.size = Size(100, List::layoutVector(res, origin, 
    _name.size().h, _objs) + 
    (_objs.size() > 0 ? res.close_brace.size().h : 0));
  return _r.size.h;

}

void DictProp::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  List::buildVector(renderer, font, _objs);

}

void DictProp::render(Renderer &renderer, Resources &res) {

  super::render(renderer, res);
  
  res.open_brace.render(renderer, Point(_r.origin.x + _name.size().w + Sizes::text_padding, _r.origin.y));
  List::renderVector(renderer, res, _objs);
  res.close_brace.render(renderer, Spatial::calcOriginOffset(_r,
    _objs.size() == 0 ? _name.size().w + res.open_brace.size().w + (Sizes::text_padding * 2) : 0, 
    _objs.size() == 0 ? 0 : _r.size.h - res.close_brace.size().h));

}
