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

Size List::layout(Resources &res, const Point &origin) {

  _r.origin = origin;
  _r.size = List::layoutVector(res, origin, res.open_bracket.size(), _objs);
  Size s = res.close_bracket.size();
  _r.size += _objs.size() == 0 ? Size(s.w, 0) : s;
  return _r.size;
  
}

void List::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  buildVector(renderer, font, _objs);
  
}

void List::render(Renderer &renderer, Resources &res) {

  super::render(renderer, res);
  
  res.open_bracket.render(renderer, _r.origin);
  renderVector(renderer, res, _objs);
  res.close_bracket.render(renderer, Spatial::calcOriginOffset(_r,
    _objs.size() == 0 ? res.open_bracket.size().w + Sizes::text_padding : 0, 
    _objs.size() == 0 ? 0 : _r.size.h - res.open_bracket.size().h));

//  renderer.renderRect(_r);

}

Size List::layoutVector(Resources &res, const Point &origin, const Size &size, std::vector<std::unique_ptr<Box> > &list) {

  Point p = origin;
  Size newsize = size;
  p.y += size.h;
  for (auto&& i: list) {
    Size s = i->layout(res, Point(p.x + Sizes::group_indent, p.y));
    newsize.h += s.h;
    p.y += s.h;
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

void List::renderVector(Renderer &renderer, Resources &res, std::vector<std::unique_ptr<Box> > &list) {

  for (auto&& i: list) {
    i->render(renderer, res);
  }

}


