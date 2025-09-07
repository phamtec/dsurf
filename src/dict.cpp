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

  _size = List::layoutVector(Size(0, Sizes::listgap), _elements);
  _size.h += _elements.size() == 0 ? Sizes::listgap - 10 : 0;
  return _size;
  
}

void Dict::build(Renderer &renderer) {

  List::buildVector(renderer, _elements);
  
}

void Dict::render(Renderer &renderer, const Point &origin) {

  drawBorder(renderer, origin, _size, false);

  List::renderVector(renderer, origin + Point(Sizes::group_indent, Sizes::listgap), _elements);

//  renderer.renderRect(Rect(origin, _size));

}

rfl::Generic Dict::getGeneric() { 

  return getGenericVector(_elements);
  
}

Element *Dict::hitTest(const Point &origin, const Point &p) { 

  Element *hit = List::hitTestVector(origin + Point(Sizes::group_indent, Sizes::listgap), p, _elements);
  if (hit) {
    return hit;
  }

  return super::hitTest(origin, p);
  
};

Point Dict::localOrigin(int index) {

  return List::localOriginVector(_elements, index, false);
  
}

void Dict::setMode(Renderer &renderer, HUD *hud) {

  if (getParent() == 0) {
    renderer.setRootState();
  }
  else {
    hud->setMode(0);
  }
  
}

void Dict::processKey(Renderer &renderer, SDL_Keycode code) {

  if (getParent() == 0) {
    renderer.processRootKey(this, code);
  }
  
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

rfl::Generic Dict::getGenericVector(std::vector<std::unique_ptr<Element> > &list) { 

  rfl::Object<rfl::Generic> obj = rfl::Object<rfl::Generic>();

  for (auto&& i: list) {
    auto *wx = dynamic_cast<Writeable *>(i.get());
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
