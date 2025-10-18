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

Bool::Bool(bool value): _parent(0) {

  _value.set(value ? L"true" : L"false", Colours::boolE);
  
}

void Bool::layout() {

  _size = _value.size();
  
}

void Bool::build(Renderer &renderer) {

  _value.build(renderer);

}

void Bool::render(Renderer &renderer, const Point &origin) {

  _value.render(renderer, origin);
  
//  renderer.renderRect(_r);
  
}

void Bool::processKey(Renderer &renderer, SDL_Keycode code) {

  renderer.processTextKey(this, origin(), _size, code);
  
}

void Bool::setString(Renderer &renderer, const wstring &s) {

  _value.set(s, Colours::boolE);
  _value.build(renderer);
  renderer.setDirty(this);
  
}

void Bool::setMode(Renderer &renderer, HUD *hud) {

  if (renderer.textTooSmall()) {
    hud->setHint(renderer, &_value);
    return;
  }

  renderer.setTextState();
  
}
