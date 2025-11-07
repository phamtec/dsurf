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
#include "core.hpp"
#include "unicode.hpp"
#include "setobj.hpp"

#include <iostream>

using namespace std;

Property::Property(const std::wstring &name, Element *obj, bool container):
    _parent(0), _name(name, Colours::propertyE), 
    _obj(obj), _container(container)
{
  obj->setParent(this);
}

string Property::describe() {

  return "Property: " + Unicode::convert(_name.str());
  
}

RectList Property::calcLayout() {

  RectList layout;
  Size nsize = _name.size();
  layout.push_back(Rect(Point(), nsize));
  auto osize = _obj->size();
  Size s;
  if (_container) {
    // containers draw UNDER the name
    layout.push_back(Rect(Point(0, nsize.h + Sizes::thickness), osize));
    s = Size(0, osize.h + nsize.h);
    if (nsize.w > osize.w) {
      s.w = nsize.w;
    }
    else {
      s.w = osize.w;
    }
  }
  else {
    layout.push_back(Rect(Point(nsize.w + Sizes::name_var_padding, 0), osize));
    s = Size(nsize.w + Sizes::name_var_padding + osize.w, nsize.h);
  }
  s.h += Sizes::thickness;
  Layout::addSize(&layout, s);
  
  return layout;
  
}

void Property::layout() {

  _layout = calcLayout();
  _size = Layout::size(_layout);

}

void Property::build(Core &core) {

  _name.build(core);

}

void Property::render(Core &core, const Point &origin) {

//  core.renderLayout(origin, _layout);
  
  auto i = _layout.begin();
  _name.render(core, origin + i->origin);
  i++;
  _obj->render(core, origin + i->origin);
  
//  core.renderRect(_r);

}

bool Property::visit(std::function<bool (Element *)> f) {

  if (!_obj->visit(f)) {
    return false;
  }
  
  return f(this);
  
}

string Property::getName() {

  wstring s = _name.str();
  return string(s.begin(), s.end());
  
}

DictG Property::getGeneric() { 

  auto wx = dynamic_cast<Writeable *>(_obj.get());
  if (wx) {
    return wx->getGeneric();
  }
  return "not writeable";
  
}

Element *Property::hitTest(const Point &origin, const Point &p) { 

  auto i = _layout.begin();
  // skip the name.
  i++;

  Element *hit = _obj->hitTest(origin + i->origin, p);
  if (hit) {
    return hit;
  }

  return super::hitTest(origin, p);
  
}

Point Property::localOrigin(Element *elem) {

  if (elem != _obj.get()) {
    cerr << "obj not this property!" << endl;
    return Point();
  }

  auto i = _layout.begin();
  // skip the name.
  i++;

  return Point(i->origin);  
  
}

void Property::processKey(Core &core, SDL_Keycode code) {

  // intercept the copy to correctly copy the prooerty
  switch (code) {
    case SDLK_C:
      core.copy(this);
      return;
  }
  
  // let the text editor have it.
  core.processTextKey(this, origin(), _name.size(), code);
  
}

void Property::setString(Core &core, const wstring &s) {

  _name.set(s, Colours::propertyE);
  _name.build(core);
  core.setDirty(this);
  
}

void Property::initHUD(HUD *hud) {

  auto *cx = dynamic_cast<Commandable *>(_obj.get());
  if (cx) {
    cx->initHUD(hud);
  }
  else {
    cerr << typeid(_obj.get()).name() << " not Commandable" << endl;
  }

}

void Property::setMode(Core &core, HUD *hud) {

//  cout << "Property::setMode" << endl;
  
  if (core.textTooSmall()) {
    hud->setHint(core, &_name);
    return;
  }

  core.setTextState();
  
}

void Property::setObj(Core &core, Element *obj) {
  
  core.initElement(this, obj);
  core.exec(this, new SetObj(&_obj, obj));
  
}

