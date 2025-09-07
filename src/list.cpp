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

  _size = List::layoutVector(Size(0, Sizes::listgap), _elements);
  _size.h += _elements.size() == 0 ? Sizes::listgap - 10 : Sizes::listgap;
  return _size;
  
}

void List::build(Renderer &renderer) {

  buildVector(renderer, _elements);
  
}

void List::render(Renderer &renderer, const Point &origin) {

  drawBorder(renderer, origin, _size, false);

  renderVector(renderer, origin + Point(Sizes::group_indent, Sizes::listgap), _elements);
  
//  renderer.renderRect(_r);

}

rfl::Generic List::getGeneric() { 

  return getGenericVector(_elements); 
  
}

Element *List::hitTest(const Point &origin, const Point &p) { 

  Element *hit = hitTestVector(origin + Size(Sizes::group_indent, Sizes::listgap), p, _elements);
  if (hit) {
    return hit;
  }

  return super::hitTest(origin, p);
  
};

Point List::localOrigin(int index) {

  return localOriginVector(_elements, index, false);
  
}

void List::setMode(Renderer &renderer, HUD *hud) {

  if (getParent() == 0) {
    renderer.setRootState();
  }
  else {
    hud->setMode(0);
  }
  
}

void List::processKey(Renderer &renderer, SDL_Keycode code) {

  if (getParent() == 0) {
    renderer.processRootKey(this, code);
  }
  
}

rfl::Generic List::getGenericVector(std::vector<std::unique_ptr<Element> > &list) { 

  vector<rfl::Generic> obj = vector<rfl::Generic>();

  for (auto&& i: list) {
    auto *wx = dynamic_cast<Writeable *>(i.get());
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

Element* List::hitTestVector(const Point &origin, const Point &p, std::vector<std::unique_ptr<Element> > &list) {

  Point o = origin;
  for (auto&& i: list) {
    Element *hit = i->hitTest(o, p);
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

Size List::layoutVector(const Size &size, std::vector<std::unique_ptr<Element> > &list) {

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

void List::buildVector(Renderer &renderer, std::vector<std::unique_ptr<Element> > &list) {

  for (auto&& i: list) {
    i->build(renderer);
  }

}

void List::renderVector(Renderer &renderer, const Point &origin, std::vector<std::unique_ptr<Element> > &list) {

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

Point List::localOriginVector(std::vector<std::unique_ptr<Element> > &list, int index, bool prop) {

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



