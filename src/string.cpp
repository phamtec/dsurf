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

String::String(const std::wstring &value): _parent(0), _index(0) {

  wstringstream ss;
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

  return Unicode::convert(getString());
  
}

void String::processKey(Renderer &renderer, SDL_Keycode code) {

  switch (code) {
    case SDLK_A:
      renderer.editText(this, origin(), _size);
      break;
  }
  
}

wstring String::getString() {

  return _value.str().substr(1, _value.str().size() - 2);
  
}

void String::setString(Renderer &renderer, const wstring &s) {

  wstringstream ss;
  ss << "\"" << s << "\"";
  _value.set(ss.str(), Colours::red);
  _value.build(renderer);

}

void String::setMode(Renderer &renderer, HUD *hud) {

  renderer.setTextState();

}
