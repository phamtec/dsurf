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
#include "core.hpp"

#include <sstream>

using namespace std;

Long::Long(long value): _parent(0) {

  wstringstream ss;
  ss << value;
  _value.set(ss.str(), Colours::longE);
  
}

void Long::layout() {

  _size = _value.size();
  
}

void Long::build(Core &core) {

  _value.build(core);

}

void Long::render(Core &core, const Point &origin) {

  _value.render(core, origin);
  
//  core.renderRect(_r);
  
}

rfl::Generic Long::getGeneric() { 
  
  wstringstream ss(_value.str());
  long l;
  ss >> l;

  return l;

}

void Long::processKey(Core &core, SDL_Keycode code) {

  core.processTextKey(this, origin(), _size, code);
  
}

void Long::setString(Core &core, const wstring &s) {

  _value.set(s, Colours::longE);
  _value.build(core);
  core.setDirty(this);

}

void Long::setMode(Core &core, HUD *hud) {

  if (core.textTooSmall()) {
    hud->setHint(core, &_value);
    return;
  }

  core.setTextState();
  
}
