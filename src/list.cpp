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

//  cout << "layout list" << endl;
  
  _x = x;
  _y = y;
  _width = 100;
  _height = res.open_bracket.height();
  y += _height;
  for (auto&& i: _objs) {
    long h = i->layout(res, x + Sizes::group_indent, y);
    _height += h;
    y += h;
  }
  return _height + res.close_bracket.height();
}

void List::build(Renderer &renderer, Font &font) {

  for (auto&& i: _objs) {
    i->build(renderer, font);
  }

}

void List::render(Renderer &renderer, Resources &res) {

  res.open_bracket.render(renderer, _x, _y);
  for (auto&& i: _objs) {
    i->render(renderer, res);
  }
  res.close_bracket.render(renderer, 
    _x + (_objs.size() == 0 ? res.open_bracket.width() + Sizes::text_padding : 0), 
    _y + (_objs.size() == 0 ? 0 : _height));

}
