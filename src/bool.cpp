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
#include "core.hpp"
#include "generic.hpp"

#include <sstream>

using namespace std;
using flo::Generic;

Bool::Bool(bool value): _parent(0) {

  _value.set(value ? L"true" : L"false", Colours::boolE);
  
}

void Bool::layout() {

  _size = _value.size();
  
}

void Bool::build(Core &core) {

  _value.build(core);

}

void Bool::render(Core &core, const Point &origin) {

  _value.render(core, origin);
  
//  core.renderRect(_r);
  
}

void Bool::processKey(Core &core, SDL_Keycode code) {

  core.processTextKey(this, origin(), _size, code);
  
}

void Bool::setString(Core &core, const wstring &s) {

  _value.set(s, Colours::boolE);
  _value.build(core);
  core.setDirty(this);
  
}

void Bool::setMode(Core &core, HUD *hud) {

  if (core.textTooSmall()) {
    hud->setHint(core, &_value);
    return;
  }

  core.setTextState();
  
}
