/*
  null.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 3-Nov-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "null.hpp"
#include "sizes.hpp"
#include "spatial.hpp"
#include "core.hpp"

#include <sstream>

using namespace std;

Null::Null(): _parent(0) {

  wstringstream ss;
  ss << "null";
  _value.set(ss.str(), Colours::nullE);
  
}

void Null::layout() {

  _size = _value.size();
  
}

void Null::build(Core &core) {

  _value.build(core);

}

void Null::render(Core &core, const Point &origin) {

  _value.render(core, origin);
  
//  core.renderRect(_r);
  
}

rfl::Generic Null::getGeneric() { 
  
  rfl::Generic empty;
  return empty;

}

void Null::processKey(Core &core, SDL_Keycode code) {
  
}

void Null::setString(Core &core, const wstring &s) {

  cerr << "tried to set a null" << endl;
  
}

void Null::setMode(Core &core, HUD *hud) {

}
