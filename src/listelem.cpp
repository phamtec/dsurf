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
#include "renderer.hpp"

#include <iostream>

#define FIXED_HEIGHT  200
#define FIXED_WIDTH   600

using namespace std;

ListElem::ListElem(Element *obj): _obj(obj)
{
  Parentable::cast(obj)->setParent(this, 0);      
}

void ListElem::setEdit(Renderer &renderer, bool state) {

  _editing = state;
  
  if (_editing) {
    _textsize = getSize();
    if (_textsize.h >= FIXED_HEIGHT) {
      _textsize.h = FIXED_HEIGHT * 2;
    }
    Rect r(Point(0, 0), _textsize);
    
    _texture = renderer.createTexture(_textsize.w, _textsize.h);
    renderer.setTarget(_texture);
    renderer.setDrawColor(Colours::lightGrey);
    renderer.fillRect(r);
    renderer.clearScale();
    _obj->render(renderer, Point(0, 0));
    renderer.restoreScale();
    renderer.setTarget(0);
  }
  else {
    renderer.destroyTexture(_texture);
    _texture = 0;
  }
  
}

Size ListElem::layout() {

  if (_editing) {
    _size.h = Sizes::listgap + FIXED_HEIGHT;
    _size.w = FIXED_WIDTH;
  }
  else  {
    _size = _obj->layout();
  }
  
  return _size;
  
}

void ListElem::build(Renderer &renderer) {

  _obj->build(renderer);

}

void ListElem::destroy(Renderer &renderer) {

  _obj->destroy(renderer);
  
}

void ListElem::render(Renderer &renderer, const Point &origin) {

  if (_editing) {
    renderer.renderFilledRect(Rect(origin, Size(FIXED_WIDTH, FIXED_HEIGHT)), Colours::lightGrey);

    Size s = _textsize;
    if (_textsize.h > FIXED_HEIGHT) {
      float scale = FIXED_HEIGHT / s.h;
      s.h = FIXED_HEIGHT;
      s.w *= scale;
    }
    else {
      s.h *= 2;
      s.w *= 2;
    }
    Rect r(origin, s);
    r -= 10;
    renderer.renderTexture(_texture, r);
  }
  else {
    _obj->render(renderer, origin);
  }

}

std::string ListElem::getName() {

  return Writeable::cast(_obj.get())->getName();
  
}

rfl::Generic ListElem::getGeneric() { 

  return Writeable::cast(_obj.get())->getGeneric();
  
}

Element *ListElem::hitTest(const Point &origin, const Point &p) { 

  return _obj->hitTest(origin, p);
  
}

Point ListElem::localOrigin(int index) {

  return _obj->localOrigin(index);
  
}

void ListElem::processKey(Renderer &renderer, SDL_Keycode code) {

  Keyable::cast(_obj.get())->processKey(renderer, code);
  
}

std::wstring ListElem::getString() {

  return Editable::cast(_obj.get())->getString();
  
}

void ListElem::setString(Renderer &renderer, const wstring &s) {

  Editable::cast(_obj.get())->setString(renderer, s);
  
}

void ListElem::initHUD(HUD *hud) {

  HUDable::cast(_obj.get())->initHUD(hud);

}

void ListElem::setMode(Renderer &renderer, HUD *hud) {

  HUDable::cast(_obj.get())->setMode(renderer, hud);
  
}

