/*
  bool.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 22-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "bool.hpp"
#include "sizes.hpp"
#include "spatial.hpp"
#include "renderer.hpp"

#include <sstream>

using namespace std;

Bool::Bool(bool value): _parent(0), _index(0) {

  _value.set(value ? L"true" : L"false", Colours::green);
  
}

Size Bool::layout() {

  _size = _value.size();
  return _size;
  
}

void Bool::build(Renderer &renderer) {

  _value.build(renderer);

}

void Bool::render(Renderer &renderer, const Point &origin) {

  _value.render(renderer, origin);
  
//  renderer.renderRect(_r);
  
}

void Bool::processKey(Renderer &renderer, SDL_Keycode code) {

  switch (code) {
    case SDLK_A:
      renderer.editText(this, origin(), _size);
      break;
  }
  
}

void Bool::setString(Renderer &renderer, const wstring &s) {

  _value.set(s, Colours::red);
  _value.build(renderer);
  
}

void Bool::setState(HUD *hud) {

  hud->setState(Text);
  
}
