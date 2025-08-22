/*
  property.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 22-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "property.hpp"

#include "sizes.hpp"
#include "texteditor.hpp"

#include <iostream>

using namespace std;

Size Property::layout() {

  _size = _obj->layout();
  _size.w += _name.size().w + Sizes::name_var_padding;
  if (_container) {
    _size.h += 70;
  }
  return _size;

}

void Property::build(Renderer &renderer) {

  super::build(renderer);
  
  _name.build(renderer);
  _obj->build(renderer);

}

void Property::render(Renderer &renderer, const Point &origin) {

  super::render(renderer, origin);
  
  if (_container) {
    _name.render(renderer, origin + Point(0, -10));
    _obj->render(renderer, origin + Point(0, 60));
  }
  else {
    _name.render(renderer, origin);
    _obj->render(renderer, origin + Size(_name.size().w + Sizes::name_var_padding, 0));
  }
  
//  renderer.renderRect(_r);

}

rfl::Generic Property::getGeneric() { 

  auto wx = dynamic_cast<Writeable *>(_obj.get());
  if (wx) {
    return wx->getGeneric();
  }
  return "not writeable";
  
}

Box *Property::hitTest(const Point &origin, const Point &p) { 

  Point o = origin + (_container ? Size(0, 60) : Size(_name.size().w + Sizes::name_var_padding, 0));

  Box *hit = _obj->hitTest(o, p);
  if (hit) {
    return hit;
  }

  return super::hitTest(origin, p);
  
}

Point Property::localOrigin(int index) {

  if (index != 0) {
    cerr << "property values are always at index 0!" << endl;
    return Point(0, 0);
  }
  
  return Point(_name.size().w + Sizes::name_var_padding, 0);
  
}

void Property::edit(TextEditor *editor) {

  editor->focus(origin() , _name.size(), _name.str());
  
}
