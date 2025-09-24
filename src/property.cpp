/*
  property.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 22-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "property.hpp"

#include "sizes.hpp"
#include "texteditor.hpp"
#include "hud.hpp"
#include "renderer.hpp"
#include "unicode.hpp"

#include <iostream>

using namespace std;

Property::Property(const std::wstring &name, Element *obj, bool container):
    _parent(0), _name(name, Colours::propertyE), 
    _obj(obj), _container(container)
{
  Parentable::cast(obj)->setParent(this);
}

string Property::describe() {

  return "Property: " + Unicode::convert(_name.str());
  
}

Size Property::layout() {

  _size = _obj->layout();
  
  Size nsize = _name.size();
  _size.w += nsize.w + Sizes::name_var_padding;
  if (_container) {
    _size.h += nsize.h + Sizes::thickness;
  }
  return _size;

}

void Property::build(Renderer &renderer) {

  _name.build(renderer);
  _obj->build(renderer);

}

void Property::destroy(Renderer &renderer) {

  _obj->destroy(renderer);
  
}

void Property::render(Renderer &renderer, const Point &origin) {

  if (_container) {
    _name.render(renderer, origin + Point(0, -Sizes::thickness));
    _obj->render(renderer, origin + Point(0, _name.size().h));
  }
  else {
    _name.render(renderer, origin);
    _obj->render(renderer, origin + Size(_name.size().w + Sizes::name_var_padding, 0));
  }
  
//  renderer.renderRect(_r);

}

std::string Property::getName() {

  wstring s = _name.str();
  return string(s.begin(), s.end());
  
}

rfl::Generic Property::getGeneric() { 

  auto wx = dynamic_cast<Writeable *>(_obj.get());
  if (wx) {
    return wx->getGeneric();
  }
  return "not writeable";
  
}

Element *Property::hitTest(const Point &origin, const Point &p) { 

  Size nsize = _name.size();
  Point o = origin + (_container ? Size(0, nsize.h) : Size(nsize.w + Sizes::name_var_padding, 0));

  Element *hit = _obj->hitTest(o, p);
  if (hit) {
    return hit;
  }

  return super::hitTest(origin, p);
  
}

Point Property::localOrigin(Element *elem) {

  Size nsize = _name.size();
  if (_container) {
    return Point(0, nsize.h);
  }
  
  return Point(nsize.w + Sizes::name_var_padding, 0);
  
}

void Property::processKey(Renderer &renderer, SDL_Keycode code) {

  renderer.processTextKey(this, origin() + Size(0, _container ? -Sizes::thickness : 0), _name.size(), code);
  
}

void Property::setString(Renderer &renderer, const wstring &s) {

  _name.set(s, Colours::red);
  _name.build(renderer);
  
}

void Property::initHUD(HUD *hud) {

  auto *hx = dynamic_cast<HUDable *>(_obj.get());
  if (hx) {
    hx->initHUD(hud);
  }
  else {
    cerr << typeid(_obj.get()).name() << " not HUDable" << endl;
  }

}

void Property::setMode(Renderer &renderer, HUD *hud) {

  renderer.setTextState();
  
}

