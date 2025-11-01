/*
  root.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 3-Oct-2025
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "root.hpp"

#include "core.hpp"
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

optional<string> Root::getFilename() { 

  return Unicode::convert(_filename.str()); 
  
}

void Root::setDirty(Core &core, bool state) {

  _filename.set(_filename.str(), state ? Colours::red : Colours::black);
  _filename.build(core);

}

RectList Root::calcLayout() {

  RectList layout;
  
  auto s = _filename.size();
  auto o = Point();
  layout.push_back(Rect(o, s));
  o.y += s.h + Sizes::listgap;
  auto os = _obj->size();
  layout.push_back(Rect(o, os));
  o.y += os.h;
  if (os.w > s.w) {
    s.w = os.w;
  }
  Layout::addSize(&layout, Size(s.w, o.y));
  
  return layout;
  
}

void Root::layout() {

  _layout = calcLayout();
  _size = Layout::size(_layout);
  
}

void Root::build(Core &core) {

  _filename.build(core);

}

void Root::render(Core &core, const Point &origin) {

//  core.renderLayout(origin, _layout);
  
  auto i = _layout.begin();
  _filename.render(core, origin + (*i).origin);
  i++;
  _obj->render(core, origin + (*i).origin);
  
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

  if (!_obj->visit(f)) {
    return false;
  }
  
  return f(this);
  
}

void Root::processKey(Core &core, SDL_Keycode code) {

  Commandable::cast(_obj.get())->processKey(core, code);
  
}

std::wstring Root::getString() {

  return Editable::cast(_obj.get())->getString();
  
}

void Root::setString(Core &core, const wstring &s) {

  Editable::cast(_obj.get())->setString(core, s);
  
}

void Root::initHUD(HUD *hud) {

  Commandable::cast(_obj.get())->initHUD(hud);

}

void Root::setMode(Core &core, HUD *hud) {

  Commandable::cast(_obj.get())->setMode(core, hud);
  
}
