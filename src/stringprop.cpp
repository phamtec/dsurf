/*
  stringprop.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "stringprop.hpp"
#include "sizes.hpp"
#include "spatial.hpp"
#include "renderer.hpp"

Size StringProp::layout(Resources &res, const Point &origin) {

  _r.origin = origin;
  _r.size = _value.size();
  _r.size.w += _name.size().w + Sizes::name_var_padding;
  return _r.size;
  
}

void StringProp::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  _value.build(renderer, font);

}

void StringProp::render(Renderer &renderer, Resources &res) {

  super::render(renderer, res);
  
  _value.render(renderer, Point(_r.origin.x + _name.size().w + Sizes::name_var_padding, _r.origin.y));
  
//  renderer.renderRect(_r);

}
