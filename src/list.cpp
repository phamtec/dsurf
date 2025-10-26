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
#include "string.hpp"
#include "long.hpp"
#include "bool.hpp"
#include "newelement.hpp"
#include "root.hpp"
#include "property.hpp"

#include <iostream>
#include <SDL3/SDL.h>
#include <ranges>

#define FIXED_HEIGHT  200
#define FIXED_WIDTH   600

using namespace std;

bool List::isParentRoot() {
  
  return dynamic_cast<Root *>(_parent) != nullptr;
  
}

List *List::cast(Element *obj) {

  return ThrowErrable<List>::cast(obj);

}

string List::describe() {

  string rootname = _dict ? "Dict" : "List";
  
  if (isParentRoot()) {
    if (_elements.size() == 0) {
      return "empty root " + rootname;
    }
    return "root " + rootname;
  }
  if (_elements.size() == 0) {
    return "empty " + rootname;
  }
  return rootname;
  
}

RectList List::calcLayout() {

  RectList layout;
  Point o = Point(Sizes::group_indent, Sizes::listgap);
  float w = Sizes::bottomlinelength;
  for (auto& i: _elements) {
    auto s = i->size();
    layout.push_back(Rect(o, s));
    o.y += s.h + Sizes::listgap;
    if ((o.x + s.w) > w) {
      w = o.x + s.w;
    }
  }
  if (_elements.size() == 0) {
    o.y += (Sizes::leftlinelength * 2) - (Sizes::listgap * 2);
  }
  w += Sizes::listgap;
  Layout::addSize(&layout, Size(w, o.y));
  
  return layout;
  
}

void List::layout() {

  _layout = calcLayout();
  _size = Layout::size(_layout);
  
}

void List::destroy(Renderer &renderer) {

  endEdit(renderer);

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
    for (auto&& i: std::ranges::views::zip(_elements, _layout)) {
      get<0>(i)->render(renderer, origin + get<1>(i).origin);
    }
 }
  
//  renderer.renderRect(_r);

}

void List::endEdit(Renderer &renderer) {

  if (!_dict) {
    for_each(_elements.begin(), _elements.end(), [&renderer](auto& e) {
      ListElem *le = dynamic_cast<ListElem *>(e.get());
      if (!le) {
        cerr << typeid(e.get()).name() << " not a list element" << endl;
      }
      le->setEdit(renderer, false);
    });
  }

}

void List::startEdit(Renderer &renderer) {

  if (!_dict) {
    for_each(_elements.begin(), _elements.end(), [&renderer](auto& e) {
      ListElem *le = dynamic_cast<ListElem *>(e.get());
      if (!le) {
        cerr << typeid(e.get()).name() << " not a list element" << endl;
      }
      le->setEdit(renderer, true);
    });
  }
  
}

rfl::Generic List::getGeneric() { 

  return _dict ? getGenericObject() : getGenericVector(); 
  
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

  for (auto&& i: std::ranges::views::zip(_elements, _layout)) {
    Element *hit = get<0>(i)->hitTest(origin + get<1>(i).origin, p);
    if (hit) {
      return hit;
    }
  }

  return super::hitTest(origin, p);
  
}

Element *List::otherElementHit(const Point &origin, const Point &p) {

  for (auto&& i: std::ranges::views::zip(_elements, _layout)) {
    auto obj = get<0>(i).get();
    if (obj == _moving) {
//      cout << "ignoreing " << index << endl;
    }
    else {
//      cout << "trying " << index<< endl;
      if ((get<1>(i) + origin).contains(p)) {
//        cout << "hit" << endl;
        return obj; 
      }
    }
  }

  return nullptr;
}

void List::setMoving(Element *elem) {
  _moving = elem;
  _moveover = nullptr;
}

Point List::localOrigin(Element *elem) {

  for (auto&& i: std::ranges::views::zip(_elements, _layout)) {
    if (get<0>(i).get() == elem) {
      return get<1>(i).origin;
    }
  }

  return Point(0, 0);
  
}

bool List::visit(std::function<bool (Element *)> f) {

  for (auto& e: _elements) {
    if (!e->visit(f)) {
      return false;
    }
  }
  return f(this);
  
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
    Renderer::registerRootHUDMode(mode);
    mode->add(new Shortcut(L"N", L"ew"));
    hud->registerMode("rootdict", mode);
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

void List::initHUD(HUD *hud) {

  _hudrootlist = hud->findMode(_dict ? "rootdict" : "rootlist");
  _hudlist = hud->findMode(_dict ? "dict" : "list");
  _hudlistedit = hud->findMode("listedit");
  _hudlistmove = hud->findMode("listmove");
  _hudaddlist = hud->findMode(_dict ? "adddict" : "addlist");
  
  // and walk the list. order isn't important.
  for_each(_elements.begin(), _elements.end(), [hud](auto& e) { 
    auto *cx = dynamic_cast<Commandable *>(e.get());
    if (cx) {
      cx->initHUD(hud);
    }
    else {
      cerr << typeid(e.get()).name() << " not Commandable" << endl;
    }
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
  if (isParentRoot()) {
    hud->setMode(_hudrootlist);
    return;
  }
  
  hud->setMode(_hudlist);
  
}

void List::mergeIntoUs(Renderer &renderer, List *other) {

  if (other->isDict() != isDict()) {
    cerr << "can't merge. We are not the same." << endl;
    return;
  }

  // move the elements onto us.
  transform(other->_elements.begin(), other->_elements.end(), back_inserter(_elements), [this, &renderer](auto &e) {
    auto elem = std::move(e);
    elem->setParent(this);
    renderer.build(elem.get());
    return elem;
  });
  
  renderer.layout(root());
  
}

void List::processKey(Renderer &renderer, SDL_Keycode code) {

  if (isParentRoot()) {  
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

    case SDLK_P:
      if (!_dict) {
        break;
      }
      {
        auto elem = renderer.getClipboard();
        if (elem) {
          auto list = dynamic_cast<List *>(elem);
          if (list) {
            if (list->isDict() == isDict()) {
              mergeIntoUs(renderer, list);
            }
            else {
              Objable::cast(getParent())->setObj(renderer, elem);
            }
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

    case SDLK_E:
      if (_dict) {
        break;
      }
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
        add(renderer, L"dict", new List(true), true);
        break;
      }
      if (_moving) {
        reorder();
        _moving = nullptr;
        _moveover = nullptr;
        root()->layout();
        break;
      }
      if (!isParentRoot()) {
        renderer.processDeleteKey(getParent());
      }
      break;

    case SDLK_L:
      if (!_adding) {
        return;
      }
      add(renderer, L"list", new List(false), true);
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

void List::add(Renderer &renderer, const std::wstring &name, Element *element, bool container) {

  _adding = false;
  
  Element *e;
  if (_dict) {
    // wrap in a property
    e = new Property(name, element, container); 
    renderer.initElement(this, e);

  }
  else {
    // wrap in a list element.
    e = new ListElem(element);
    renderer.initElement(this, e);
  }

  renderer.exec(this, new NewElement(this, e));
  
}

void List::reorder() {

  if (!_moving || !_moveover) {
    cerr << "nothing to move." << endl;
    return;
  }
  
  // and move them.
  Move::moveObj(&_elements, _moving, _moveover);

}

rfl::Generic List::getGenericObject() { 

  auto obj = rfl::Object<rfl::Generic>();

  for (auto&& i: _elements) {
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

rfl::Generic List::getGenericVector() { 

  auto obj = vector<rfl::Generic>();

  transform(_elements.begin(), _elements.end(), back_inserter(obj), [](auto& e) {
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

void List::drawBorder(Renderer &renderer, const Point &origin, const Size &size, bool prop) {

  if (_dict) {
    // top left corner
    renderer.resources.topleft.render(renderer, origin);
       
    renderer.renderFilledRect(Rect(origin + Size(Sizes::thickness, 0), Size(Sizes::toplinelength, Sizes::thickness)), Colours::dictE);
    renderer.renderFilledRect(Rect(origin + Size(0, Sizes::thickness), Size(Sizes::thickness, Sizes::leftlinelength + (prop ? 20 : 0) - Sizes::thickness)), Colours::dictE);
  
    // bottom left corner
    renderer.resources.bottomleft.render(renderer, origin + Size(0, size.h - Sizes::thickness));
    
    renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::leftlinelength), Size(Sizes::thickness, Sizes::leftlinelength - Sizes::thickness)), Colours::dictE);
    renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::thickness) + Size(Sizes::thickness, 0), Size(Sizes::bottomlinelength - Sizes::thickness, Sizes::thickness)), Colours::dictE);
  }
  else {
    renderer.renderFilledRect(Rect(origin + Size(Sizes::thickness, 0), Size(Sizes::toplinelength, Sizes::thickness)), Colours::listE);
    renderer.renderFilledRect(Rect(origin, Size(Sizes::thickness, Sizes::leftlinelength + (prop ? 40 : 0))), Colours::listE);
    renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::leftlinelength), Size(Sizes::thickness, Sizes::leftlinelength)), Colours::listE);
    renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::thickness), Size(Sizes::bottomlinelength, Sizes::thickness)), Colours::listE);
  }

}
