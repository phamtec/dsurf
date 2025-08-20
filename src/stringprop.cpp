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

#include <iostream>

using namespace std;

Size StringProp::layout() {

  _size = _value.size();
  _size.w += _name.size().w + Sizes::name_var_padding;
  return _size;
  
}

void StringProp::build(Renderer &renderer) {

  super::build(renderer);
  
  _name.build(renderer);
  _value.build(renderer);

}

void StringProp::render(Renderer &renderer, const Point &origin) {

  super::render(renderer, origin);
  
  _name.render(renderer, origin);
  _value.render(renderer, origin + Size(_name.size().w + Sizes::name_var_padding, 0));
  
//  renderer.renderRect(_r);

}

rfl::Generic StringProp::getGeneric() { 

  return _value.str().substr(1, _value.str().size() - 2); 
  
}

Point StringProp::parentOrigin(int index) {

  cout << "StringProp parent " << typeid(*_parent).name() << " index " << _index << endl;
  
  return _parent->origin(_index);
  
}

void StringProp::edit(TextEditor *editor) {
  editor->_origin = origin(_index);
  editor->_size = _size;
}
