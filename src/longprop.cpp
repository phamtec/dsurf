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

#include <sstream>

using namespace std;

LongProp::LongProp(const std::string &name, long value): Prop(name) {

  stringstream ss;
  ss << value;
  _value.set(ss.str(), Colours::green);
  
}

float LongProp::layout(Resources &res, const Point &origin) {

  _r.origin = origin;
  _r.size = Size(100, _value.size().h);
  return _r.size.h;
  
}

void LongProp::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  _value.build(renderer, font);

}

void LongProp::render(Renderer &renderer, Resources &res) {

  super::render(renderer, res);
  
  _value.render(renderer, Spatial::calcOriginOffset(_r, _name.size().w + Sizes::name_var_padding, 0));
  
}
