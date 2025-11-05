/*
  listelem.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 15-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "listelem.hpp"

#include "sizes.hpp"
#include "texteditor.hpp"
#include "hud.hpp"
#include "core.hpp"
#include "list.hpp"
#include "setobj.hpp"

#include <iostream>

using namespace std;

#define FIXED_HEIGHT  200
#define FIXED_WIDTH   600

ListElem::ListElem(Element *obj): _obj(obj)
{
  obj->setParent(this);      
}

void ListElem::setEdit(Core &core, bool state) {

  _editing = state;
  
}

void ListElem::layout() {

  if (_editing) {
    _size = Size(FIXED_WIDTH, FIXED_HEIGHT);
  }
  else {
    _size = _obj->size();
  }
  
}

void ListElem::render(Core &core, const Point &origin) {

  _obj->render(core, origin);

}

std::string ListElem::getName() {

  return Writeable::cast(_obj.get())->getName();
  
}

rfl::Generic ListElem::getGeneric() { 

  return Writeable::cast(_obj.get())->getGeneric();
  
}

Element *ListElem::hitTest(const Point &origin, const Point &p) { 

  if (!_editing) {
    return _obj->hitTest(origin, p);
  }
  
  return super::hitTest(origin, p);

}

Point ListElem::localOrigin(Element *elem) {

  return _obj->localOrigin(elem);
  
}

bool ListElem::visit(std::function<bool (Element *)> f) {

  if (!_obj->visit(f)) {
    return false;
  }
  
  return f(this);
  
}

void ListElem::processKey(Core &core, SDL_Keycode code) {

  if (_editing) {
    switch (code) {

      case SDLK_C:
        core.copy(_obj.get());
        break;

      case SDLK_ESCAPE:
        List::cast(getParent())->processKey(core, code);
        break;

      case SDLK_M:
        List::cast(getParent())->setMoving(this);
        break;
    }
    return;
  }
  
  Commandable::cast(_obj.get())->processKey(core, code);
  
}

std::wstring ListElem::getString() {

  return Editable::cast(_obj.get())->getString();
  
}

void ListElem::setString(Core &core, const wstring &s) {

  Editable::cast(_obj.get())->setString(core, s);
  
}

void ListElem::registerHUDModes(HUD *hud) {

  {
    auto mode = new HUDMode(false);
    mode->add(new Shortcut(L"Esc", L"(finish)"));
    mode->add(new Shortcut(L"C", L"opy"));
    mode->add(new Shortcut(L"M", L"ove"));
    hud->registerMode("listelem", mode);
  }

}

void ListElem::initHUD(HUD *hud) {

  _hudlistelem = hud->findMode("listelem");

  Commandable::cast(_obj.get())->initHUD(hud);

}

void ListElem::setMode(Core &core, HUD *hud) {

  if (_editing) {
    hud->setMode(_hudlistelem);
  }
  else {
    Commandable::cast(_obj.get())->setMode(core, hud);
  }
  
}

void ListElem::setObj(Core &core, Element *obj) {
  
  core.initElement(this, obj);
  core.exec(this, new SetObj(&_obj, obj));
  
}


