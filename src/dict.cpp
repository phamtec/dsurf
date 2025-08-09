/*
  dict.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "dict.hpp"
#include "renderer.hpp"
#include "resources.hpp"
#include "sizes.hpp"

#include <iostream>

using namespace std;

float Dict::layout(Resources &res, float x, float y) {

//  cout << "layout dict" << endl;
  
  _x = x;
  _y = y;
  _width = 100;
  _height = res.open_brace.height();
  y += _height;
  for (auto&& i: _objs) {
    long h = i->layout(res, x + Sizes::group_indent, y);
    _height += h;
    y += h;
  }
  return _height + res.close_brace.height();
}

void Dict::build(Renderer &renderer, Font &font) {

  for (auto&& i: _objs) {
    i->build(renderer, font);
  }

}

void Dict::render(Renderer &renderer, Resources &res) {

  res.open_brace.render(renderer, _x, _y);
  for (auto&& i: _objs) {
    i->render(renderer, res);
  }
  res.close_brace.render(renderer, 
    _x + (_objs.size() == 0 ? res.open_brace.width() + Sizes::text_padding : 0), 
    _y + (_objs.size() == 0 ? 0 : _height));

}
