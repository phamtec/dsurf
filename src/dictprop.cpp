/*
  dictprop.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "dictprop.hpp"
#include "renderer.hpp"
#include "resources.hpp"
#include "sizes.hpp"

#include <iostream>

using namespace std;

float DictProp::layout(Resources &res, float x, float y) {

//  cout << "layout dictprop" << endl;
  
  _x = x;
  _y = y;
  _width = 100;
  _height = _name.height(); // name and open brace
  y += _name.height();
  for (auto&& i: _objs) {
    long h = i->layout(res, x + Sizes::group_indent, y);
    _height += h;
    y += h;
  }
  return _height + (_objs.size() > 0 ? res.close_brace.height() : 0); // closed bracket if not empty
}

void DictProp::build(Renderer &renderer, Font &font) {

  _name.build(renderer, font);
  for (auto&& i: _objs) {
    i->build(renderer, font);
  }

}
void DictProp::render(Renderer &renderer, Resources &res) {

  _name.render(renderer, _x, _y);
  res.open_brace.render(renderer, _x + _name.width() + Sizes::text_padding, _y);
  for (auto&& i: _objs) {
    i->render(renderer, res);
  }
  res.close_brace.render(renderer, 
    _x + (_objs.size() == 0 ? _name.width() + res.open_brace.width() + (Sizes::text_padding * 2) : 0), 
    _y + (_objs.size() == 0 ? 0 : _height));

}
