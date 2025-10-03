/*
  dict.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "dict.hpp"

#include "renderer.hpp"
#include "colours.hpp"
#include "sizes.hpp"
#include "list.hpp"
#include "property.hpp"
#include "string.hpp"
#include "long.hpp"
#include "bool.hpp"
#include "newelement.hpp"
#include "objable.hpp"
#include "root.hpp"

#include <iostream>

using namespace std;

bool Dict::isParentRoot() {
  
  return dynamic_cast<Root *>(_parent) != nullptr;
  
}

string Dict::describe() {

  if (isParentRoot()) {
    if (_elements.size() == 0) {
      return "empty root Dict";
    }
    return "root Dict";
  }
  if (_elements.size() == 0) {
    return "empty Dict";
  }
  return "Dict";
  
}

Size Dict::layout() {

  _size = List::layoutVector(Size(0, Sizes::listgap), _elements);
  _size.h += _elements.size() == 0 ? Sizes::listgap: 0;
  _size.w += _elements.size() == 0 ? Sizes::bottomlinelength : 0;
  
  return _size;
  
}

void Dict::build(Renderer &renderer) {

  List::buildVector(renderer, _elements);
  
}

void Dict::destroy(Renderer &renderer) {

  List::destroyVector(_elements, renderer);
  
}

void Dict::render(Renderer &renderer, const Point &origin) {

  drawBorder(renderer, origin, _size, false);

  List::renderVector(renderer, origin + Point(Sizes::group_indent, Sizes::listgap), _elements);

//  renderer.renderRect(Rect(origin, _size));

}

rfl::Generic Dict::getGeneric() { 

  return getGenericVector(_elements);
  
}

Element *Dict::hitTest(const Point &origin, const Point &p) { 

  Element *hit = List::hitTestVector(origin + Point(Sizes::group_indent, Sizes::listgap), p, _elements);
  if (hit) {
    return hit;
  }

  return super::hitTest(origin, p);
  
};

Point Dict::localOrigin(Element *elem) {

  return List::localOriginVector(_elements, elem, false);
  
}

void Dict::registerHUDModes(HUD *hud) {

  {
    auto mode = new HUDMode(false);
    Renderer::registerRootHUDMode(mode);
    mode->add(new Shortcut(L"N", L"ew"));
    hud->registerMode("rootdict", mode);
  }

  {
    auto mode = new HUDMode(false);
    Renderer::registerGlobalHUDMode(mode);
    mode->add(new Shortcut(L"C", L"opy"));
    mode->add(new Shortcut(L"P", L"aste"));
    mode->add(new Shortcut(L"N", L"ew"));
    Renderer::registerTextHUDMode(mode);
    hud->registerMode("dict", mode);
  }

  {
    auto mode = new HUDMode(true);
    mode->add(new Shortcut(L"Esc", L"(finish)"));
    mode->add(new Shortcut(L"D", L"ict"));
    mode->add(new Shortcut(L"L", L"ist"));
    mode->add(new Shortcut(L"S", L"tring"));
    mode->add(new Shortcut(L"N", L"umber"));
    mode->add(new Shortcut(L"B", L"ool"));
    hud->registerMode("adddict", mode);
  }

}

void Dict::initHUD(HUD *hud) {

  _hudrootdict = hud->findMode("rootdict");
  _huddict = hud->findMode("dict");
  _hudadddict = hud->findMode("adddict");

  // and walk the list.
  List::initHUDVector(_elements, hud);
  
}

void Dict::setMode(Renderer &renderer, HUD *hud) {

  if (renderer.textTooSmall()) {
    hud->setMode(0);
    return;
  }
  if (_adding) {
    hud->setMode(_hudadddict);
    return;
  }
  if (isParentRoot()) {
    hud->setMode(_hudrootdict);
    return;
  }
  hud->setMode(_huddict);
  
}

void Dict::processKey(Renderer &renderer, SDL_Keycode code) {

  if (isParentRoot()) {  
    if (renderer.processRootKey(this, code)) {
      return;
    }
  }
  switch (code) {      
    case SDLK_C:
      renderer.copy(this);
      break;

    case SDLK_P:
      {
        auto elem = renderer.getClipboard();
        if (elem) {
          auto dict = dynamic_cast<Dict *>(elem);
          if (dict) {
            Objable::cast(getParent())->setObj(renderer, elem);
          }
          else {
            renderer.setError("Not a dictionary");
          }
        }
        else {
          renderer.setError("Invalid Dict");
        }
      }
      break;

    case SDLK_ESCAPE:
      _adding = false;
      break;

    case SDLK_D:
      if (_adding) {
        add(renderer, L"dict", new Dict(), true);
        return;
      }
      if (!isParentRoot()) {
        renderer.processDeleteKey(getParent());
      }
      break;

    case SDLK_L:
      if (!_adding) {
        return;
      }
      add(renderer, L"list", new List(), true);
      break;

    case SDLK_S:
      if (!_adding) {
        return;
      }
      add(renderer, L"string", new String(L"value"), false);
      break;

    case SDLK_N:
      if (!_adding) {
        // also "New"
        _adding = true;
        return;
      }
      add(renderer, L"long", new Long(0), false);
      break;

    case SDLK_B:
      if (!_adding) {
        return;
      }
      add(renderer, L"bool", new Bool(false), false);
      break;
  }
  
}

void Dict::add(Renderer &renderer, const std::wstring &name, Element *element, bool container) {

  _adding = false;
  
  // wrap in a property
  auto p = new Property(name, element, container); 
  renderer.initElement(this, p);

  renderer.exec(this, new NewElement(this, p));
  
}

void Dict::drawBorder(Renderer &renderer, const Point &origin, const Size &size, bool prop) {

  // top left corner
  renderer.resources.topleft.render(renderer, origin);
     
  renderer.renderFilledRect(Rect(origin + Size(Sizes::thickness, 0), Size(Sizes::toplinelength, Sizes::thickness)), Colours::dictE);
  renderer.renderFilledRect(Rect(origin + Size(0, Sizes::thickness), Size(Sizes::thickness, Sizes::leftlinelength + (prop ? 20 : 0) - Sizes::thickness)), Colours::dictE);

  // bottom left corner
   renderer.resources.bottomleft.render(renderer, origin + Size(0, size.h - Sizes::thickness));
  
  renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::leftlinelength), Size(Sizes::thickness, Sizes::leftlinelength - Sizes::thickness)), Colours::dictE);
  renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::thickness) + Size(Sizes::thickness, 0), Size(Sizes::bottomlinelength - Sizes::thickness, Sizes::thickness)), Colours::dictE);

}

rfl::Generic Dict::getGenericVector(std::vector<std::unique_ptr<Element> > &list) { 

  rfl::Object<rfl::Generic> obj = rfl::Object<rfl::Generic>();

  for (auto&& i: list) {
    auto *wx = dynamic_cast<Writeable *>(i.get());
    if (wx) {
      string name = wx->getName();
      obj[name] = wx->getGeneric();
    }
    else {
      obj[typeid(i.get()).name()] = "not Writeable";
    }
  }
  
  return obj; 
  
}
