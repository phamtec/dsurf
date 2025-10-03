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
#include "hud.hpp"
#include "unicode.hpp"

using namespace std;

String::String(const std::wstring &value): _parent(0) {

  _value.set(value, Colours::stringE);
  
}

std::string String::describe() {

  stringstream ss;
  ss << "String (";
  ss << Unicode::convert(_value.str());
  ss << ")";
  return ss.str();
  
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

  return Unicode::convert(getString());
  
}

void String::setString(Renderer &renderer, const wstring &s) {

  _value.set(s, Colours::stringE);
  _value.build(renderer);
  renderer.setDirty(this);

}

void String::setMode(Renderer &renderer, HUD *hud) {

  if (renderer.textTooSmall()) {
    hud->setHint(renderer, &_value);
    return;
  }

  renderer.setTextState();

}

void String::processKey(Renderer &renderer, SDL_Keycode code) {

  renderer.processTextKey(this, origin(), _size, code);
  
}

