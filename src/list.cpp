/*
  list.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "list.hpp"

#include "renderer.hpp"
#include "colours.hpp"
#include "sizes.hpp"
#include "sizeable.hpp"

#include <iostream>
#include <SDL3/SDL.h>

#define FIXED_HEIGHT  200
#define FIXED_WIDTH   600

using namespace std;

Size List::layout() {

  if (_editing) {
    _size.h = (_elements.size() * (Sizes::listgap + FIXED_HEIGHT)) + Sizes::listgap;
    _size.w = FIXED_WIDTH;
  }
  else  {
    _size = List::layoutVector(Size(0, Sizes::listgap), _elements);
    _size.h += _elements.size() == 0 ? Sizes::listgap - 10 : Sizes::listgap;
  }
  return _size;
  
}

void List::build(Renderer &renderer) {

  buildVector(renderer, _elements);
  
}

void List::destroy(Renderer &renderer) {

  killTextures(renderer);

  // and walk the list.
  destroyVector(_elements, renderer);
}

void List::render(Renderer &renderer, const Point &origin) {

  drawBorder(renderer, origin, _size, false);

  if (_editing) {
  
    // when editing a list we render fixed size elements with previews in them.
    Point o = origin + Point(Sizes::group_indent, Sizes::listgap);
    for (int i=0; i<_textures.size(); i++) {
    
      renderer.renderFilledRect(Rect(o, Size(FIXED_WIDTH, FIXED_HEIGHT)), Colours::lightGrey);

      Size s = _texturesizes[i];
      if (s.h > FIXED_HEIGHT) {
        float scale = FIXED_HEIGHT / s.h;
        s.h = FIXED_HEIGHT;
        s.w *= scale;
      }
      else {
        s.h *= 2;
        s.w *= 2;
      }
      Rect r(o, s);
      r -= 10;
      renderer.renderTexture(_textures[i], r);
      
      o.y += FIXED_HEIGHT + Sizes::listgap;
    }
    
  }
  else {
    renderVector(renderer, origin + Point(Sizes::group_indent, Sizes::listgap), _elements);
  }
  
//  renderer.renderRect(_r);

}

void List::killTextures(Renderer &renderer) {
  for (auto i: _textures) {
    renderer.destroyTexture(i);
  }
  _textures.clear();
  _texturesizes.clear();
}

void List::renderTextures(Renderer &renderer) {

  for (auto& i: _elements) {
    Sizeable *sz = dynamic_cast<Sizeable *>(i.get());
    if (sz) {
    
      Size s = sz->getSize();
      if (s.h >= FIXED_HEIGHT) {
        s.h = FIXED_HEIGHT * 2;
      }
      Rect r(Point(0, 0), s);
      
      auto texture = renderer.createTexture(s.w, s.h);
      renderer.setTarget(texture);
      renderer.setDrawColor(Colours::lightGrey);
      renderer.fillRect(r);
      renderer.clearScale();
      i->render(renderer, Point(0, 0));
      renderer.restoreScale();
      renderer.setTarget(0);
      _textures.push_back(texture);
      _texturesizes.push_back(s);
      
    }
  }
  
}

rfl::Generic List::getGeneric() { 

  return getGenericVector(_elements); 
  
}

Element *List::hitTest(const Point &origin, const Point &p) { 

  if (!_editing) {
    Element *hit = hitTestVector(origin + Size(Sizes::group_indent, Sizes::listgap), p, _elements);
    if (hit) {
      return hit;
    }
  }

  return super::hitTest(origin, p);
  
};

Point List::localOrigin(int index) {

  return localOriginVector(_elements, index, false);
  
}

void List::registerHUDModes(HUD *hud) {

  {
    auto mode = new HUDMode(false);
    mode->add(new Shortcut(L"C", L"opy"));
    mode->add(new Shortcut(L"P", L"aste"));
    mode->add(new Shortcut(L"E", L"dit"));
    hud->registerMode("rootlist", mode);
  }

  {
    auto mode = new HUDMode(false);
    mode->add(new Shortcut(L"E", L"dit"));
    hud->registerMode("list", mode);
  }
  
  {
    auto mode = new HUDMode(false);
    mode->add(new Shortcut(L"Esc", L"(finish)"));
    hud->registerMode("listedit", mode);
  }

}

void List::initHUD(HUD *hud) {

  _hudrootlist = hud->findMode("rootlist");
  _hudlist = hud->findMode("list");
  _hudlistedit = hud->findMode("listedit");
  
  // and walk the list.
  initHUDVector(_elements, hud);

}

void List::destroyVector(std::vector<std::unique_ptr<Element> > &list, Renderer &renderer) {

  for (auto &i: list) {
    i->destroy(renderer);
  }
}

void List::setMode(Renderer &renderer, HUD *hud) {

  if (_editing) {
    hud->setMode(_hudlistedit);
  }
  else if (getParent() == 0) {
    hud->setMode(_hudrootlist);
  }
  else {
    hud->setMode(_hudlist);
  }
  
}

void List::processKey(Renderer &renderer, SDL_Keycode code) {

  if (getParent() == 0) {  
    if (renderer.processRootKey(this, code)) {
      return;
    }
  }
  switch (code) {
    case SDLK_E:
      if (_editing) {
        break;
      }
      _editing = true;
      _oldsize = _size;
//      _size.h = (_elements.size() * (Sizes::listgap + FIXED_HEIGHT)) + Sizes::listgap;
      renderTextures(renderer);
      root()->layout();
      break;
      
    case SDLK_ESCAPE:
      if (!_editing) {
        break;
      }
      _editing = false;
      _size = _oldsize;
      killTextures(renderer);
      root()->layout();
      break;
  }
  
}

rfl::Generic List::getGenericVector(std::vector<std::unique_ptr<Element> > &list) { 

  vector<rfl::Generic> obj = vector<rfl::Generic>();

  for (auto& i: list) {
    auto *wx = dynamic_cast<Writeable *>(i.get());
    if (wx) {
      obj.push_back(wx->getGeneric());
    }
    else {
      stringstream ss;
      ss << typeid(i.get()).name() << " not Writeable";
      obj.push_back(ss.str());
    }
  }
  
  return obj; 
  
}

Element* List::hitTestVector(const Point &origin, const Point &p, std::vector<std::unique_ptr<Element> > &list) {

  Point o = origin;
  for (auto& i: list) {
    Element *hit = i->hitTest(o, p);
    if (hit) {
      return hit;
    }
    Sizeable *sz = dynamic_cast<Sizeable *>(i.get());
    if (sz) {
      o.y += sz->getSize().h;
    }
    else {
    }
    o.y += Sizes::listgap;
  }
  return nullptr;
  
}

Size List::layoutVector(const Size &size, std::vector<std::unique_ptr<Element> > &list) {

  Size newsize = size;
  for (auto& i: list) {
    Size s = i->layout();
    newsize.h += s.h + Sizes::listgap;
    if ((Sizes::group_indent + s.w) > newsize.w) {
      newsize.w = Sizes::group_indent + s.w;
    }
  }
  return newsize;
  
}

void List::buildVector(Renderer &renderer, std::vector<std::unique_ptr<Element> > &list) {

  for (auto& i: list) {
    i->build(renderer);
  }

}

void List::renderVector(Renderer &renderer, const Point &origin, std::vector<std::unique_ptr<Element> > &list) {

  Point o = origin;
  for (auto& i: list) {
    i->render(renderer, o);
    Sizeable *sz = dynamic_cast<Sizeable *>(i.get());
    if (sz) {
      o.y += sz->getSize().h;
    }
    o.y += Sizes::listgap;
  }

}

Point List::localOriginVector(std::vector<std::unique_ptr<Element> > &list, int index, bool prop) {

  int j=0;
  int y=Sizes::listgap + (prop ? 40 : 0);
  for (auto& i: list) {
    if (j == index) {
      return Point(Sizes::group_indent, y);
    }
    Sizeable *sz = dynamic_cast<Sizeable *>(i.get());
    if (sz) {
      y += sz->getSize().h;
    }
    y += Sizes::listgap;
    j++;
  }

  return Point(0, 0);
  
}

void List::initHUDVector(std::vector<std::unique_ptr<Element> > &list, HUD *hud) {

  for (auto& i: list) {
    auto *hx = dynamic_cast<HUDable *>(i.get());
    if (hx) {
      hx->initHUD(hud);
    }
    else {
      cerr << typeid(i.get()).name() << " not HUDable" << endl;
    }
  }
  
}

void List::drawBorder(Renderer &renderer, const Point &origin, const Size &size, bool prop) {

  renderer.renderFilledRect(Rect(origin + Size(Sizes::thickness, 0), Size(Sizes::toplinelength, Sizes::thickness)), Colours::orange);
  renderer.renderFilledRect(Rect(origin, Size(Sizes::thickness, Sizes::leftlinelength + (prop ? 40 : 0))), Colours::orange);
  renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::leftlinelength), Size(Sizes::thickness, Sizes::leftlinelength)), Colours::orange);
  renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::thickness), Size(Sizes::bottomlinelength, Sizes::thickness)), Colours::orange);

}
