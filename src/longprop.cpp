/*
  longprop.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "longprop.hpp"
#include "sizes.hpp"
#include "spatial.hpp"
#include "renderer.hpp"

#include <sstream>

using namespace std;

LongProp::LongProp(const std::string &name, long value): _name(name, Colours::blue) {

  stringstream ss;
  ss << value;
  _value.set(ss.str(), Colours::green);
  
}

Size LongProp::layout() {

  _size = _value.size();
  _size.w += _name.size().w + Sizes::name_var_padding;
  return _size;
  
}

void LongProp::build(Renderer &renderer) {

  super::build(renderer);
  
  _name.build(renderer);
  _value.build(renderer);

}

void LongProp::render(Renderer &renderer, const Point &origin) {

  super::render(renderer, origin);
  
  _name.render(renderer, origin + Point(Sizes::group_indent, -Sizes::thickness));
  _value.render(renderer, Point(origin.x + _name.size().w + Sizes::name_var_padding, origin.y));
  
//  renderer.renderRect(_r);
  
}

rfl::Generic LongProp::getGeneric() { 
  
  stringstream ss(_value.str());
  long l;
  ss >> l;

  return l;

}

