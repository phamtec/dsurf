/*
  string.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "string.hpp"
#include "sizes.hpp"
#include "spatial.hpp"
#include "renderer.hpp"

using namespace std;

String::String(const std::string &value): _parent(0), _index(0) {

  stringstream ss;
  ss << "\"" << value << "\"";
  _value.set(ss.str(), Colours::green);
}

Size String::layout() {

  _size = _value.size();
  return _size;
  
}

void String::build(Renderer &renderer) {

  _value.build(renderer);

}

void String::render(Renderer &renderer, const Point &origin) {

  _value.render(renderer, origin);

//  renderer.renderRect(_r);

}

rfl::Generic String::getGeneric() { 

  return _value.str().substr(1, _value.str().size() - 2); 
  
}

void String::edit(TextEditor *editor) {

  editor->focus(origin(), _size, this);
  
}

string String::getString() {

  return _value.str().substr(1, _value.str().size() - 2);
  
}

void String::setString(Renderer &renderer, const string &s) {

  stringstream ss;
  ss << "\"" << s << "\"";
  _value.set(ss.str(), Colours::red);
  _value.build(renderer);

}
