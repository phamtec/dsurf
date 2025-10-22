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

void Root::setParent(Element *parent) {
  cerr << "setParent on Root!" << endl;
}

string Root::getFilename() { 

  return Unicode::convert(_filename.str()); 
  
}

void Root::setDirty(Renderer &renderer, bool state) {

  _filename.set(_filename.str(), state ? Colours::red : Colours::black);
  _filename.build(renderer);

}

RectList Root::calcLayout() {

  RectList layout;
  
  auto s = _filename.size();
  auto o = Point();
  layout.push_back(Rect(o, s));
  o.y += s.h + Sizes::listgap;
  s = _obj->size();
  layout.push_back(Rect(o, s));
  o.y += s.h;
  Layout::addSize(&layout, Size(s.w, o.y));
  
  return layout;
  
}

void Root::layout() {

  _obj->layout();

  // calculate the layout.
  _layout = calcLayout();
  _size = Layout::size(_layout);
  
}

void Root::build(Renderer &renderer) {

  _filename.build(renderer);

}

void Root::render(Renderer &renderer, const Point &origin) {

//  renderer.renderLayout(origin, _layout);
  
  auto i = _layout.begin();
  _filename.render(renderer, origin + (*i).origin);
  i++;
  _obj->render(renderer, origin + (*i).origin);
  
}

std::string Root::getName() {

  return Writeable::cast(_obj.get())->getName();
  
}

rfl::Generic Root::getGeneric() { 

  return Writeable::cast(_obj.get())->getGeneric();
  
}

Element *Root::hitTest(const Point &origin, const Point &p) { 

  auto i = _layout.begin();
  // skip name
  i++;
  
  auto hit = _obj->hitTest(origin + i->origin, p);
  if (hit) {
    return hit;
  }
  
  return super::hitTest(origin, p);
  
}

Point Root::localOrigin(Element *elem) {

  auto i = _layout.begin();
  // skip name
  i++;
  
  return _obj->localOrigin(elem) + i->origin;
  
}

bool Root::visit(std::function<bool (Element *)> f) {

  if (!f(this)) {
    return false;
  }
  return _obj->visit(f);
  
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
