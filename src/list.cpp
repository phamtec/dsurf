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
#include "listelem.hpp"
#include "err.hpp"
#include "move.hpp"
#include "dict.hpp"
#include "string.hpp"
#include "long.hpp"
#include "bool.hpp"
#include "newelement.hpp"

#include <iostream>
#include <SDL3/SDL.h>

#define FIXED_HEIGHT  200
#define FIXED_WIDTH   600

using namespace std;

List *List::cast(Element *obj) {

  return ThrowErrable<List>::cast(obj);

}

string List::describe() {

  if (!getParent()) {
    if (_elements.size() == 0) {
      return "empty root List";
    }
    return "root List";
  }
  if (_elements.size() == 0) {
    return "empty List";
  }
  return "List";
  
}

Size List::layout() {

  _size = List::layoutVector(Size(0, Sizes::listgap), _elements);
  _size.h += _elements.size() == 0 ? Sizes::listgap: 0;
  _size.w += _elements.size() == 0 ? Sizes::bottomlinelength : 0;

  return _size;
  
}

void List::build(Renderer &renderer) {

  buildVector(renderer, _elements);
  
}

void List::destroy(Renderer &renderer) {

  endEdit(renderer);

  // and walk the list.
  destroyVector(_elements, renderer);
}

void List::render(Renderer &renderer, const Point &origin) {

  drawBorder(renderer, origin, _size, false);

  if (_moving) {
 
    // all the non moving objects.
    {
      Point o = origin + Point(Sizes::group_indent, Sizes::listgap);
      for (auto& i: _elements) {
        Size s = i->size();
        if (i.get() != _moving) {
          i->render(renderer, o);
        }
        o.y += s.h + Sizes::listgap;
      }
    }
    
    // and the one we are moving.
    {
      Point o = origin + Point(Sizes::group_indent, Sizes::listgap);
      int index = 0;
      for (auto& i: _elements) {
        Size s = i->size();
        if (i.get() == _moving) {
        
          Point ro = o;
          Point p = renderer.noOffset(_mouse) - _moveoffs;
          p.y += (index * (s. h + Sizes::listgap)) + Sizes::listgap;
          
          // constrain on y.
          ro.y = p.y;
          
          // constrain to the list.
          Point lo = this->origin();
          Size ls = size();
          float top = lo.y + Sizes::listgap;
          float bottom = lo.y + ls.h - s.h - (Sizes::listgap * 2);
          if (ro.y < top) {
            ro.y = top;
          }
          if (ro.y > bottom) {
            ro.y = bottom;
          }
          i->render(renderer, ro);
        }
        o.y += s.h + Sizes::listgap;
        index++;
      }
    }
  }
  else {
    // just render like normal.
    renderVector(renderer, origin + Point(Sizes::group_indent, Sizes::listgap), _elements);
 }
  
//  renderer.renderRect(_r);

}

void List::endEdit(Renderer &renderer) {

  for_each(_elements.begin(), _elements.end(), [&renderer](auto& e) {
    ListElem *le = dynamic_cast<ListElem *>(e.get());
    if (!le) {
      cerr << typeid(e.get()).name() << " not a list element" << endl;
    }
    le->setEdit(renderer, false);
  });

}

void List::startEdit(Renderer &renderer) {

  for_each(_elements.begin(), _elements.end(), [&renderer](auto& e) {
    ListElem *le = dynamic_cast<ListElem *>(e.get());
    if (!le) {
      cerr << typeid(e.get()).name() << " not a list element" << endl;
    }
    le->setEdit(renderer, true);
  });

}

rfl::Generic List::getGeneric() { 

  return getGenericVector(_elements); 
  
}

Element *List::hitTest(const Point &origin, const Point &p) { 

  if (_editing) {
    _mouse = p;
    if (_moving) {
      // what are we over?
      _moveover = otherElementHit(origin, p);
      // and hit this.
      return super::hitTest(origin, p);
    }
    else {
      _moveoffs = p - origin;
    }
  }

  Element *hit = hitTestVector(origin + Size(Sizes::group_indent, Sizes::listgap), p, _elements);
  if (hit) {
    return hit;
  }

  return super::hitTest(origin, p);
  
}

Element *List::otherElementHit(const Point &origin, const Point &p) {

  Point o = origin;
  for (auto& i: _elements) {
    auto obj = i.get();
    Size s = obj->size();
    if (obj == _moving) {
//      cout << "ignoreing " << index << endl;
    }
    else {
//      cout << "trying " << index<< endl;
      if (Rect(o, s).contains(p)) {
//        cout << "hit" << endl;
        return obj; 
      }
    }
    o.y += s.h + Sizes::listgap;
  }

  return nullptr;
}

void List::setMoving(Element *elem) {
  _moving = elem;
  _moveover = nullptr;
}

Point List::localOrigin(Element *elem) {

  return localOriginVector(_elements, elem, false);
  
}

void List::registerHUDModes(HUD *hud) {

  {
    auto mode = new HUDMode(false);
    Renderer::registerRootHUDMode(mode);
    mode->add(new Shortcut(L"E", L"dit"));
    mode->add(new Shortcut(L"N", L"ew"));
    hud->registerMode("rootlist", mode);
  }

  {
    auto mode = new HUDMode(false);
    Renderer::registerGlobalHUDMode(mode);
    mode->add(new Shortcut(L"C", L"opy"));
    mode->add(new Shortcut(L"E", L"dit"));
    mode->add(new Shortcut(L"N", L"ew"));
    Renderer::registerTextHUDMode(mode);
    hud->registerMode("list", mode);
  }
  
  {
    auto mode = new HUDMode(true);
    mode->add(new Shortcut(L"Esc", L"(finish)"));
    hud->registerMode("listedit", mode);
  }

  {
    auto mode = new HUDMode(true);
    mode->add(new Shortcut(L"Esc", L"(finish)"));
    mode->add(new Shortcut(L"D", L"(rop)"));
    hud->registerMode("listmove", mode);
  }

  {
    auto mode = new HUDMode(true);
    mode->add(new Shortcut(L"Esc", L"(finish)"));
    mode->add(new Shortcut(L"D", L"ict"));
    mode->add(new Shortcut(L"L", L"ist"));
    mode->add(new Shortcut(L"S", L"tring"));
    mode->add(new Shortcut(L"N", L"umber"));
    mode->add(new Shortcut(L"B", L"ool"));
    hud->registerMode("addlist", mode);
  }

}

void List::initHUD(HUD *hud) {

  _hudrootlist = hud->findMode("rootlist");
  _hudlist = hud->findMode("list");
  _hudlistedit = hud->findMode("listedit");
  _hudlistmove = hud->findMode("listmove");
  _hudaddlist = hud->findMode("addlist");
  
  // and walk the list.
  initHUDVector(_elements, hud);

}

void List::destroyVector(std::vector<std::unique_ptr<Element> > &list, Renderer &renderer) {

  for_each(list.begin(), list.end(), [&renderer](auto& e) { 
    e->destroy(renderer);
  });

}

void List::setMode(Renderer &renderer, HUD *hud) {

  if (renderer.textTooSmall()) {
    hud->setMode(0);
    return;
  }

  if (_adding) {
    hud->setMode(_hudaddlist);
    return;
  }
  if (_editing) {
    hud->setMode(_moving ? _hudlistmove : _hudlistedit);
    return;
  }
  if (!getParent()) {
    hud->setMode(_hudrootlist);
    return;
  }
  hud->setMode(_hudlist);
  
}

void List::processKey(Renderer &renderer, SDL_Keycode code) {

  if (getParent() == 0) {  
    if (renderer.processRootKey(this, code)) {
      return;
    }
  }
  switch (code) {      
    case SDLK_C:
      if (_editing) {
        break;
      }
      renderer.copy(this);
      break;

    case SDLK_E:
      if (_editing) {
        break;
      }
      _editing = true;
      startEdit(renderer);
      root()->layout();
      break;
      
    case SDLK_ESCAPE:
      if (_adding) {
        _adding = false;
        break;
      }
      if (!_editing) {
        break;
      }
      if (_moving) {
        _moving = nullptr;
        _moveover = nullptr;
      }
      else {
        _editing = false;
        endEdit(renderer);
      }
      root()->layout();
      break;

    case SDLK_D:
      if (_adding) {
        add(renderer, new Dict());
        break;
      }
      if (_moving) {
        reorder();
        _moving = nullptr;
        _moveover = nullptr;
        root()->layout();
        break;
      }
      if (getParent()) {
        renderer.processDeleteKey(getParent());
      }
      break;

    case SDLK_L:
      if (!_adding) {
        return;
      }
      add(renderer, new List());
      break;

    case SDLK_S:
      if (!_adding) {
        return;
      }
      add(renderer, new String(L"value"));
      break;

    case SDLK_N:
      if (!_adding) {
        // also "New"
        _adding = true;
        return;
      }
      add(renderer, new Long(0));
      break;

    case SDLK_B:
      if (!_adding) {
        return;
      }
      add(renderer, new Bool(false));
      break;
  }
  
}

void List::add(Renderer &renderer, Element *element) {

  _adding = false;
  
  // wrap in a list element.
  auto le = new ListElem(element);
  renderer.initElement(this, le);

  renderer.exec(this, new NewElement(this, le));
  
}

void List::reorder() {

  if (!_moving || !_moveover) {
    cerr << "nothing to move." << endl;
    return;
  }
  
  // and move them.
  Move::moveObj(&_elements, _moving, _moveover);

}

rfl::Generic List::getGenericVector(std::vector<std::unique_ptr<Element> > &list) { 

  vector<rfl::Generic> obj = vector<rfl::Generic>();

  transform(list.begin(), list.end(), back_inserter(obj), [](auto& e) {
    auto *wx = dynamic_cast<Writeable *>(e.get());
    if (wx) {
      return wx->getGeneric();
    }
    else {
      stringstream ss;
      ss << typeid(e).name() << " not Writeable";
      return (rfl::Generic)ss.str();
    }
  });
  
  return obj; 
  
}

Element* List::hitTestVector(const Point &origin, const Point &p, std::vector<std::unique_ptr<Element> > &list) {

  Point o = origin;
  for (auto& i: list) {
    Element *hit = i->hitTest(o, p);
    if (hit) {
      return hit;
    }
    o.y += i->size().h + Sizes::listgap;
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

  // order doesn't matter.
  for_each(list.begin(), list.end(), [&renderer](auto& e) { 
    e->build(renderer);
  });

}

void List::renderVector(Renderer &renderer, const Point &origin, std::vector<std::unique_ptr<Element> > &list) {

  // And render, We use a for loop because... side effect :-)
  Point o = origin;
  for (auto& i: list) {
    i->render(renderer, o);
    o.y += i->size().h + Sizes::listgap;
  }

}

Point List::localOriginVector(std::vector<std::unique_ptr<Element> > &list, Element *elem, bool prop) {

  int j=0;
  int y=Sizes::listgap + (prop ? 40 : 0);
  for (auto& i: list) {
    if (i.get() == elem) {
      return Point(Sizes::group_indent, y);
    }
    y += i->size().h + Sizes::listgap;
    j++;
  }

  return Point(0, 0);
  
}

void List::initHUDVector(std::vector<std::unique_ptr<Element> > &list, HUD *hud) {

  // order isn't important.
  for_each(list.begin(), list.end(), [hud](auto& e) { 
    auto *cx = dynamic_cast<Commandable *>(e.get());
    if (cx) {
      cx->initHUD(hud);
    }
    else {
      cerr << typeid(e.get()).name() << " not Commandable" << endl;
    }
  });
  
}

void List::drawBorder(Renderer &renderer, const Point &origin, const Size &size, bool prop) {

  renderer.renderFilledRect(Rect(origin + Size(Sizes::thickness, 0), Size(Sizes::toplinelength, Sizes::thickness)), Colours::listE);
  renderer.renderFilledRect(Rect(origin, Size(Sizes::thickness, Sizes::leftlinelength + (prop ? 40 : 0))), Colours::listE);
  renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::leftlinelength), Size(Sizes::thickness, Sizes::leftlinelength)), Colours::listE);
  renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::thickness), Size(Sizes::bottomlinelength, Sizes::thickness)), Colours::listE);

}
