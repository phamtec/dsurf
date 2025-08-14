/*
  list.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "list.hpp"
#include "renderer.hpp"
#include "resources.hpp"
#include "sizes.hpp"
#include "spatial.hpp"

#include <iostream>

using namespace std;

Size List::layout(Resources &res) {

  _size = List::layoutVector(res, Size(0, Sizes::listgap), _objs);
  _size.h += _objs.size() == 0 ? Sizes::listgap - 10 : Sizes::listgap;
  return _size;
  
}

void List::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  buildVector(renderer, font, _objs);
  
}

void List::render(Renderer &renderer, Resources &res, const Point &origin) {

  renderer.renderFilledRect(Rect(origin + Size(Sizes::group_indent / 2, 0), Size(Sizes::toplinelength, Sizes::thickness)), Colours::orange);
  renderer.renderFilledRect(Rect(origin + Size(Sizes::group_indent / 2, 0), Size(Sizes::thickness, _size.h - Sizes::thickness)), Colours::orange);
  renderer.renderFilledRect(Rect(origin + Size(Sizes::group_indent / 2, 0) + Size(0, _size.h - Sizes::thickness), Size(Sizes::bottomlinelength, Sizes::thickness)), Colours::orange);

  super::render(renderer, res, origin);

  renderVector(renderer, res, origin + Point(Sizes::group_indent, Sizes::listgap), _objs);
  
//  renderer.renderRect(_r);

}

Size List::layoutVector(Resources &res, const Size &size, std::vector<std::unique_ptr<Box> > &list) {

  Size newsize = size;
  for (auto&& i: list) {
    Size s = i->layout(res);
    newsize.h += s.h + Sizes::listgap;
    if ((Sizes::group_indent + s.w) > newsize.w) {
      newsize.w = Sizes::group_indent + s.w;
    }
  }
  return newsize;
  
}

void List::buildVector(Renderer &renderer, Font &font, std::vector<std::unique_ptr<Box> > &list) {

  for (auto&& i: list) {
    i->build(renderer, font);
  }

}

void List::renderVector(Renderer &renderer, Resources &res, const Point &origin, std::vector<std::unique_ptr<Box> > &list) {

  float y = origin.y;
  for (auto&& i: list) {
    i->render(renderer, res, Point(origin.x, y));
    y += i->_size.h + Sizes::listgap;
  }

}


