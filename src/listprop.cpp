/*
  listprop.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "listprop.hpp"
#include "renderer.hpp"
#include "sizes.hpp"
#include "list.hpp"
#include "spatial.hpp"

#include <iostream>

using namespace std;

Size ListProp::layout() {

  _size = List::layoutVector(_name.size() + Size(Sizes::text_padding, Sizes::listgap), _objs);
  _size.h += _objs.size() == 0 ? 10 : -10;
  return _size;

}

void ListProp::build(Renderer &renderer) {

  super::build(renderer);
  
  _name.build(renderer);
  List::buildVector(renderer, _objs);

}

void ListProp::render(Renderer &renderer, const Point &origin) {

  List::drawBorder(renderer, origin, _size, true);

  super::render(renderer, origin);
  
  _name.render(renderer, origin + Point(Sizes::group_indent, -Sizes::thickness));
  List::renderVector(renderer, origin + Point(Sizes::group_indent, 60), _objs);

//  renderer.renderRect(_r);

}

rfl::Generic ListProp::getGeneric() { 

  vector<rfl::Generic> obj = vector<rfl::Generic>();

  for (auto&& i: _objs) {
    obj.push_back(i->getGeneric());
  }
  
  return obj; 
  
}
