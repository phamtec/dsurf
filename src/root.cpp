/*
  root.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 3-Oct-2025
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "root.hpp"

#include "renderer.hpp"
#include "unicode.hpp"
#include "sizes.hpp"

using namespace std;

Root::Root(const std::string &filename, Element *obj): 
  _filename(Unicode::convert(filename), Colours::black), _obj(obj) {

  obj->setParent(this);
      
}

string Root::getFilename() { 

  return Unicode::convert(_filename.str()); 
  
}

void Root::setDirty(Renderer &renderer, bool state) {

  _filename.set(_filename.str(), state ? Colours::red : Colours::black);
  _filename.build(renderer);

}

Size Root::layout() {

  _size = _obj->layout();
  return _size;
  
}

void Root::build(Renderer &renderer) {

  _filename.build(renderer);
  _obj->build(renderer);

}

void Root::destroy(Renderer &renderer) {

  _obj->destroy(renderer);
  
}

void Root::render(Renderer &renderer, const Point &origin) {

  _filename.render(renderer, origin);
  _obj->render(renderer, origin + Size(0, _filename.size().h+Sizes::listgap));

}

std::string Root::getName() {

  return Writeable::cast(_obj.get())->getName();
  
}

rfl::Generic Root::getGeneric() { 

  return Writeable::cast(_obj.get())->getGeneric();
  
}

Element *Root::hitTest(const Point &origin, const Point &p) { 

  auto hit = _obj->hitTest(origin + Size(0, _filename.size().h+Sizes::listgap), p);
  if (hit) {
    return hit;
  }
  
  return super::hitTest(origin, p);
  
}

Point Root::localOrigin(Element *elem) {

  return _obj->localOrigin(elem) + Size(0,  _filename.size().h+Sizes::listgap);
  
}

void Root::processKey(Renderer &renderer, SDL_Keycode code) {

  Commandable::cast(_obj.get())->processKey(renderer, code);
  
}

std::wstring Root::getString() {

  return Editable::cast(_obj.get())->getString();
  
}

void Root::setString(Renderer &renderer, const wstring &s) {

  Editable::cast(_obj.get())->setString(renderer, s);
  
}

void Root::initHUD(HUD *hud) {

  Commandable::cast(_obj.get())->initHUD(hud);

}

void Root::setMode(Renderer &renderer, HUD *hud) {

  Commandable::cast(_obj.get())->setMode(renderer, hud);
  
}
