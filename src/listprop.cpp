/*
  listprop.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "listprop.hpp"
#include "renderer.hpp"
#include "resources.hpp"
#include "sizes.hpp"

#include <iostream>

using namespace std;

float ListProp::layout(Resources &res, float x, float y) {

//  cout << "layout listprop" << endl;
  
  _x = x;
  _y = y;
  _width = 100;
  _height = _name.height(); // open bracket and name
  y += _name.height();
  for (auto&& i: _objs) {
    long h = i->layout(res, x + Sizes::group_indent, y);
    _height += h;
    y += h;
  }
  return _height + (_objs.size() > 0 ? res.close_bracket.height() : 0); // closed bracket if not empty
}

void ListProp::build(Renderer &renderer, Font &font) {

  _name.build(renderer, font);
  for (auto&& i: _objs) {
    i->build(renderer, font);
  }

}

void ListProp::render(Renderer &renderer, Resources &res) {

  _name.render(renderer, _x, _y);
  res.open_bracket.render(renderer, _x + _name.width() + 4, _y);
  for (auto&& i: _objs) {
    i->render(renderer, res);
  }
  res.close_bracket.render(renderer, 
    _x + (_objs.size() == 0 ? _name.width() + res.open_bracket.width() + (Sizes::text_padding * 2): 0), 
    _y + (_objs.size() == 0 ? 0 : _height));
  
}
