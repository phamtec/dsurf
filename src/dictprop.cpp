/*
  dictprop.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "dictprop.hpp"
#include "renderer.hpp"
#include "sizes.hpp"
#include "list.hpp"
#include "dict.hpp"
#include "spatial.hpp"

#include <iostream>

using namespace std;

Size DictProp::layout() {

  _size = List::layoutVector(_name.size() + Size(Sizes::text_padding, 0), _objs);
  _size.h += _objs.size() == 0 ? Sizes::listgap - 10 : 0;
  return _size;

}

void DictProp::build(Renderer &renderer) {

  super::build(renderer);
  
  _name.build(renderer);
  List::buildVector(renderer, _objs);

}

void DictProp::render(Renderer &renderer, const Point &origin) {

  Dict::drawBorder(renderer, origin, _size, true);

  super::render(renderer, origin);
  
  _name.render(renderer, origin);
  List::renderVector(renderer, origin + Point(Sizes::group_indent, 60), _objs);

//  renderer.renderRect(_r);
  
}

rfl::Generic DictProp::getGeneric() { 

  rfl::Object<rfl::Generic> obj = rfl::Object<rfl::Generic>();

  for (auto&& i: _objs) {
    string name = i->getName();
    obj[name] = i->getGeneric();
  }
  
  return obj; 
  
}
