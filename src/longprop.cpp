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

LongProp::LongProp(const std::string &name, long value): Prop(name) {

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
  
  _value.build(renderer);

}

void LongProp::render(Renderer &renderer, const Point &origin) {

  super::render(renderer, origin);
  
  _value.render(renderer, origin + Point(_name.size().w + Sizes::name_var_padding, 0));
  
//  renderer.renderRect(_r);
  
}
