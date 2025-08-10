/*
  listprop.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "listprop.hpp"
#include "renderer.hpp"
#include "resources.hpp"
#include "sizes.hpp"
#include "list.hpp"
#include "spatial.hpp"

#include <iostream>

using namespace std;

float ListProp::layout(Resources &res, const SDL_FPoint &origin) {

  Spatial::setOrigin(&_r, origin);
  Spatial::setDimensions(&_r, 100, List::layoutVector(res, origin, _name.height(), _objs) + 
    (_objs.size() > 0 ? res.close_bracket.height() : 0)); // closed bracket if not empty
  return _r.h;
}

void ListProp::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  List::buildVector(renderer, font, _objs);

}

void ListProp::render(Renderer &renderer, Resources &res) {

  super::render(renderer, res);
  
  res.open_bracket.render(renderer, Spatial::makePoint(_r.x + _name.width() + Sizes::text_padding, _r.y));
  List::renderVector(renderer, res, _objs);
  res.close_bracket.render(renderer, Spatial::calcOriginOffset(_r,
    _objs.size() == 0 ? _name.width() + res.open_bracket.width() + (Sizes::text_padding * 2) : 0, 
    _objs.size() == 0 ? 0 : _r.h - res.close_bracket.height()));
  
}
