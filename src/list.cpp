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

#include <iostream>

using namespace std;

float List::layout(Resources &res, float x, float y) {

  _r = { .x = x, .y = y, .w = 100, .h = List::layoutVector(res, x, y, res.open_bracket.height(), _objs) };
  return _r.h + res.close_bracket.height();
  
}

void List::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  buildVector(renderer, font, _objs);
  
}

void List::render(Renderer &renderer, Resources &res) {

  super::render(renderer, res);
  
  res.open_bracket.render(renderer, _r.x, _r.y);
  renderVector(renderer, res, _objs);
  res.close_bracket.render(renderer, 
    _r.x + (_objs.size() == 0 ? res.open_bracket.width() + Sizes::text_padding : 0), 
    _r.y + (_objs.size() == 0 ? 0 : _r.h));

}

float List::layoutVector(Resources &res, float x, float y, float height, std::vector<std::unique_ptr<Box> > &list) {

  y += height;
  for (auto&& i: list) {
    long h = i->layout(res, x + Sizes::group_indent, y);
    height += h;
    y += h;
  }
  return height;
  
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


