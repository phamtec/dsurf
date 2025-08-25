/*
  long.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "long.hpp"
#include "sizes.hpp"
#include "spatial.hpp"
#include "renderer.hpp"

#include <sstream>

using namespace std;

Long::Long(long value): _parent(0), _index(0) {

  stringstream ss;
  ss << value;
  _value.set(ss.str(), Colours::green);
  
}

Size Long::layout() {

  _size = _value.size();
  return _size;
  
}

void Long::build(Renderer &renderer) {

  _value.build(renderer);

}

void Long::render(Renderer &renderer, const Point &origin) {

  _value.render(renderer, origin);
  
//  renderer.renderRect(_r);
  
}

rfl::Generic Long::getGeneric() { 
  
  stringstream ss(_value.str());
  long l;
  ss >> l;

  return l;

}

void Long::edit(TextEditor *editor) {

  editor->focus(origin(), _size, this);
  
}

void Long::setString(Renderer &renderer, const string &s) {

  _value.set(s, Colours::red);
  _value.build(renderer);

}
