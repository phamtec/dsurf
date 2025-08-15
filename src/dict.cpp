/*
  dict.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "dict.hpp"
#include "renderer.hpp"
#include "colours.hpp"
#include "sizes.hpp"
#include "list.hpp"
#include "spatial.hpp"

#include <iostream>

using namespace std;

Size Dict::layout() {

  _size = List::layoutVector(Size(0, Sizes::listgap), _objs);
  _size.h += _objs.size() == 0 ? Sizes::listgap - 10 : 0;
  return _size;
  
}

void Dict::build(Renderer &renderer) {

  super::build(renderer);
  
  List::buildVector(renderer, _objs);
  
}

void Dict::render(Renderer &renderer, const Point &origin) {

  drawBorder(renderer, origin, _size, false);

  super::render(renderer, origin);
  
  List::renderVector(renderer, origin + Point(Sizes::group_indent, Sizes::listgap), _objs);

//  renderer.renderRect(Rect(origin, _size));

}

void Dict::drawBorder(Renderer &renderer, const Point &origin, const Size &size, bool prop) {

  renderer.renderFilledRect(Rect(origin + Size(Sizes::group_indent / 2, 0), Size(Sizes::toplinelength, Sizes::thickness)), Colours::plum);
  renderer.renderFilledRect(Rect(origin + Size(Sizes::group_indent / 2, 0), Size(Sizes::thickness, size.h - (prop ? 0 : Sizes::thickness))), Colours::plum);
  renderer.renderFilledRect(Rect(origin + Size(Sizes::group_indent / 2, 0) + Size(0, size.h - Sizes::thickness), Size(Sizes::bottomlinelength, Sizes::thickness)), Colours::plum);

}

rfl::Generic Dict::getGeneric() { 

  rfl::Object<rfl::Generic> obj = rfl::Object<rfl::Generic>();

  for (auto&& i: _objs) {
    string name = i->getName();
    obj[name] = i->getGeneric();
  }
  
  return obj; 
  
}
