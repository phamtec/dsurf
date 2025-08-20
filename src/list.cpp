/*
  list.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "list.hpp"
#include "renderer.hpp"
#include "colours.hpp"
#include "sizes.hpp"
#include "spatial.hpp"

#include <iostream>

using namespace std;

Size List::layout() {

  _size = List::layoutVector(Size(0, Sizes::listgap), _objs);
  _size.h += _objs.size() == 0 ? Sizes::listgap - 10 : Sizes::listgap;
  return _size;
  
}

void List::build(Renderer &renderer) {

  super::build(renderer);
  
  buildVector(renderer, _objs);
  
}

void List::render(Renderer &renderer, const Point &origin) {

  drawBorder(renderer, origin, _size, false);

  super::render(renderer, origin);

  renderVector(renderer, origin + Point(Sizes::group_indent, Sizes::listgap), _objs);
  
//  renderer.renderRect(_r);

}

rfl::Generic List::getGeneric() { 

  vector<rfl::Generic> obj = vector<rfl::Generic>();

  for (auto&& i: _objs) {
    obj.push_back(i->getGeneric());
  }
  
  return obj; 
  
}

Box *List::hitTest(const Point &origin, const Point &p) { 

  Box *hit = hitTestVector(origin + Size(Sizes::group_indent, Sizes::listgap), p, _objs);
  if (hit) {
    return hit;
  }

  return super::hitTest(origin, p);
  
};

Box* List::hitTestVector(const Point &origin, const Point &p, std::vector<std::unique_ptr<Box> > &list) {

  Point o = origin;
  for (auto&& i: list) {
    if (i->hitTest(o, p)) {
      return i.get();
    }
    o.y += i->_size.h + Sizes::listgap;
  }
  return nullptr;
  
}

Size List::layoutVector(const Size &size, std::vector<std::unique_ptr<Box> > &list) {

  Size newsize = size;
  for (auto&& i: list) {
    Size s = i->layout();
    newsize.h += s.h + Sizes::listgap;
    if ((Sizes::group_indent + s.w) > newsize.w) {
      newsize.w = Sizes::group_indent + s.w;
    }
  }
  return newsize;
  
}

void List::buildVector(Renderer &renderer, std::vector<std::unique_ptr<Box> > &list) {

  for (auto&& i: list) {
    i->build(renderer);
  }

}

void List::renderVector(Renderer &renderer, const Point &origin, std::vector<std::unique_ptr<Box> > &list) {

  Point o = origin;
  for (auto&& i: list) {
    i->render(renderer, o);
    o.y += i->_size.h + Sizes::listgap;
  }

}

void List::drawBorder(Renderer &renderer, const Point &origin, const Size &size, bool prop) {

  renderer.renderFilledRect(Rect(origin + Size(Sizes::thickness, 0), Size(Sizes::toplinelength, Sizes::thickness)), Colours::orange);
  renderer.renderFilledRect(Rect(origin, Size(Sizes::thickness, Sizes::leftlinelength + (prop ? 40 : 0))), Colours::orange);
  renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::leftlinelength), Size(Sizes::thickness, Sizes::leftlinelength)), Colours::orange);
  renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::thickness), Size(Sizes::bottomlinelength, Sizes::thickness)), Colours::orange);

}

