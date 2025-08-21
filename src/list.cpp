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
#include "sizeable.hpp"

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

  return getGenericVector(_objs); 
  
}

Box *List::hitTest(const Point &origin, const Point &p) { 

  Box *hit = hitTestVector(origin + Size(Sizes::group_indent, Sizes::listgap), p, _objs);
  if (hit) {
    return hit;
  }

  return super::hitTest(origin, p);
  
};

Point List::localOrigin(int index) {

  return localOriginVector(_objs, index, false);
  
}

rfl::Generic List::getGenericVector(std::vector<std::unique_ptr<Box> > &list) { 

  vector<rfl::Generic> obj = vector<rfl::Generic>();

  for (auto&& i: list) {
    Writeable *wx = dynamic_cast<Writeable *>(i.get());
    if (wx) {
      obj.push_back(wx->getGeneric());
    }
    else {
      stringstream ss;
      ss << typeid(i.get()).name() << " not Writeable";
      obj.push_back(ss.str());
    }
  }
  
  return obj; 
  
}

Box* List::hitTestVector(const Point &origin, const Point &p, std::vector<std::unique_ptr<Box> > &list) {

  Point o = origin;
  for (auto&& i: list) {
    Box *hit = i->hitTest(o, p);
    if (hit) {
      return hit;
    }
    Sizeable *sz = dynamic_cast<Sizeable *>(i.get());
    if (sz) {
      o.y += sz->getSize().h;
    }
    else {
    }
    o.y += Sizes::listgap;
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
    Sizeable *sz = dynamic_cast<Sizeable *>(i.get());
    if (sz) {
      o.y += sz->getSize().h;
    }
    o.y += Sizes::listgap;
  }

}

Point List::localOriginVector(std::vector<std::unique_ptr<Box> > &list, int index, bool prop) {

  int j=0;
  int y=Sizes::listgap + (prop ? 40 : 0);
  for (auto&& i: list) {
    if (j == index) {
      return Point(Sizes::group_indent, y);
    }
    Sizeable *sz = dynamic_cast<Sizeable *>(i.get());
    if (sz) {
      y += sz->getSize().h;
    }
    y += Sizes::listgap;
    j++;
  }

  return Point(0, 0);
  
}

void List::drawBorder(Renderer &renderer, const Point &origin, const Size &size, bool prop) {

  renderer.renderFilledRect(Rect(origin + Size(Sizes::thickness, 0), Size(Sizes::toplinelength, Sizes::thickness)), Colours::orange);
  renderer.renderFilledRect(Rect(origin, Size(Sizes::thickness, Sizes::leftlinelength + (prop ? 40 : 0))), Colours::orange);
  renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::leftlinelength), Size(Sizes::thickness, Sizes::leftlinelength)), Colours::orange);
  renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::thickness), Size(Sizes::bottomlinelength, Sizes::thickness)), Colours::orange);

}



