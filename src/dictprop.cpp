/*
  dictprop.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "dictprop.hpp"
#include "renderer.hpp"

#include <iostream>

using namespace std;

float DictProp::layout(float x, float y) {

//  cout << "layout dictprop" << endl;
  
  _x = x;
  _y = y;
  _width = 640;
  _height = 20; // name and open brace
  y += 20;
  for (auto&& i: _objs) {
    long h = i->layout(x + 20, y);
    _height += h;
    y += h;
  }
  return _height + (_objs.size() > 0 ? 20 : 0); // closed bracket if not empty
}

void DictProp::render(Renderer &renderer, Font &font) {

  _name.render(renderer, font, _x, _y);
  renderer.pool.open_brace.render(renderer, font, _x + 100, _y);
  for (auto&& i: _objs) {
    i->render(renderer, font);
  }
  renderer.pool.close_brace.render(renderer, font, _x + (_objs.size() == 0 ? 110 : 0), _y + (_objs.size() == 0 ? 0 : _height));

}
