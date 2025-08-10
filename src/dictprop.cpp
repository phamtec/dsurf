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

Size DictProp::layout(Resources &res) {

  _size = List::layoutVector(res, _name.size() + Size(res.open_brace.size().w + Sizes::text_padding, 0), _objs);
  Size s = res.close_brace.size();
  _size += _objs.size() == 0 ? Size(s.w, 0) : Size(0, s.h);
  return _size;

}

void DictProp::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  List::buildVector(renderer, font, _objs);

}

void DictProp::render(Renderer &renderer, Resources &res, const Point &origin) {

  super::render(renderer, res, origin);
  
  res.open_brace.render(renderer, origin + Point(_name.size().w + Sizes::text_padding, 0));
  List::renderVector(renderer, res, origin + Point(Sizes::group_indent, res.open_brace.size().h), _objs);
  res.close_brace.render(renderer, origin + Point(
    _objs.size() == 0 ? _name.size().w + res.open_brace.size().w + (Sizes::text_padding * 2) : 0, 
    _objs.size() == 0 ? 0 : _size.h - res.close_brace.size().h));

//  renderer.renderRect(_r);
  
}
