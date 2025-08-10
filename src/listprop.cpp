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

Size ListProp::layout(Resources &res) {

  _size = List::layoutVector(res, _name.size() + Size(res.open_bracket.size().w + Sizes::text_padding, 0), _objs);
  Size s = res.close_bracket.size();
  _size += _objs.size() == 0 ? Size(s.w, 0) : Size(0, s.h);
  return _size;

}

void ListProp::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  List::buildVector(renderer, font, _objs);

}

void ListProp::render(Renderer &renderer, Resources &res, const Point &origin) {

  super::render(renderer, res, origin);
  
  res.open_bracket.render(renderer, Point(origin.x + _name.size().w + Sizes::text_padding, origin.y));
  List::renderVector(renderer, res, origin + Point(Sizes::group_indent, res.open_bracket.size().h), _objs);
  res.close_bracket.render(renderer, origin + Point(
    _objs.size() == 0 ? _name.size().w + res.open_bracket.size().w + (Sizes::text_padding * 2) : 0, 
    _objs.size() == 0 ? 0 : _size.h - res.close_bracket.size().h));
  
//  renderer.renderRect(_r);

}
