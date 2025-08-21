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

rfl::Generic Dict::getGeneric() { 

  return getGenericVector(_objs);
  
}

Box *Dict::hitTest(const Point &origin, const Point &p) { 

  Box *hit = List::hitTestVector(origin + Point(Sizes::group_indent, Sizes::listgap), p, _objs);
  if (hit) {
    return hit;
  }

  return super::hitTest(origin, p);
  
};

Point Dict::localOrigin(int index) {

  return List::localOriginVector(_objs, index, false);
  
}

void Dict::drawBorder(Renderer &renderer, const Point &origin, const Size &size, bool prop) {

  // top left corner
  renderer.resources.topleft.render(renderer, origin);
     
  renderer.renderFilledRect(Rect(origin + Size(Sizes::thickness, 0), Size(Sizes::toplinelength, Sizes::thickness)), Colours::plum);
  renderer.renderFilledRect(Rect(origin + Size(0, Sizes::thickness), Size(Sizes::thickness, Sizes::leftlinelength + (prop ? 20 : 0) - Sizes::thickness)), Colours::plum);

  // bottom left corner
   renderer.resources.bottomleft.render(renderer, origin + Size(0, size.h - Sizes::thickness));
  
  renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::leftlinelength), Size(Sizes::thickness, Sizes::leftlinelength - Sizes::thickness)), Colours::plum);
  renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::thickness) + Size(Sizes::thickness, 0), Size(Sizes::bottomlinelength - Sizes::thickness, Sizes::thickness)), Colours::plum);

}

rfl::Generic Dict::getGenericVector(std::vector<std::unique_ptr<Box> > &list) { 

  rfl::Object<rfl::Generic> obj = rfl::Object<rfl::Generic>();

  for (auto&& i: list) {
    Writeable *wx = dynamic_cast<Writeable *>(i.get());
    if (wx) {
      string name = wx->getName();
      obj[name] = wx->getGeneric();
    }
    else {
      obj[typeid(i.get()).name()] = "not Writeable";
    }
  }
  
  return obj; 
  
}
