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
#include "core.hpp"
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

void String::layout() {

  _size = _value.size();
//  cout << "String size " << _size << endl;
  
}

void String::build(Core &core) {

  _value.build(core);

}

void String::render(Core &core, const Point &origin) {

  _value.render(core, origin);

//  core.renderRect(_r);

}

DictG String::getGeneric() { 

  return Unicode::convert(getString());
  
}

void String::setString(Core &core, const wstring &s) {

  _value.set(s, Colours::stringE);
  _value.build(core);
  core.setDirty(this);

}

void String::setMode(Core &core, HUD *hud) {

  if (core.textTooSmall()) {
    hud->setHint(core, &_value);
    return;
  }

  core.setTextState();

}

void String::processKey(Core &core, SDL_Keycode code) {

  core.processTextKey(this, origin(), _size, code);
  
}

