/*
  list.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "list.hpp"

#include "core.hpp"
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
#include "generic.hpp"
#include "../modules/code/main.hpp"

#include <iostream>
#include <SDL3/SDL.h>
#include <ranges>

using namespace std;
using flo::Generic;

List::List(bool dict): _dict(dict), _parent(0), _state(none), _moving(0), _moveover(0) {

  registerListKeyHandlers();
  
}

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

void List::destroy(Core &core) {

  endEdit(core);

}

void List::render(Core &core, const Point &origin) {

  drawBorder(core, origin, _size, false);

  auto zipped = std::ranges::views::zip(_elements, _layout);
  
  if (_moving) {
 
    // all the non moving objects.
    for (auto&& i: zipped) {
      if (get<0>(i).get() != _moving) {
        core.renderFilledRect(Rect(origin + get<1>(i).origin, get<0>(i)->size()), Colours::lightGrey);
        get<0>(i)->render(core, origin + get<1>(i).origin);
      }
    }
    
    // and the one we are moving.
    Point o = origin + get<1>(*zipped.begin()).origin;
    int index = 0;
    for (auto&& i: zipped) {
      Size s = get<0>(i)->size();
      if (get<0>(i).get() == _moving) {
      
        Point ro = o;
        Point p = core.noOffset(_mouse) - _moveoffs;
        p.y += (index * (s.h + Sizes::listgap)) + Sizes::listgap;
        
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
        core.renderFilledRect(Rect(ro, s), Colours::lime);
        get<0>(i)->render(core, ro);
      }
      o.y += s.h + Sizes::listgap;
      index++;
    }
  }
  else {
    // just render like normal.
    for (auto&& i: zipped) {
      if (_state == reorder) {
        core.renderFilledRect(Rect(origin + get<1>(i).origin, get<0>(i)->size()), Colours::lightGrey);
      }
      get<0>(i)->render(core, origin + get<1>(i).origin);
    }
 }
  
//  core.renderRect(_r);

}

void List::endEdit(Core &core) {

  if (!_dict) {
    for_each(_elements.begin(), _elements.end(), [&core](auto& e) {
      ListElem *le = dynamic_cast<ListElem *>(e.get());
      if (!le) {
        cerr << typeid(e.get()).name() << " not a list element" << endl;
      }
      le->setEdit(core, false);
    });
  }

}

void List::startEdit(Core &core) {

  if (!_dict) {
    for_each(_elements.begin(), _elements.end(), [&core](auto& e) {
      ListElem *le = dynamic_cast<ListElem *>(e.get());
      if (!le) {
        cerr << typeid(e.get()).name() << " not a list element" << endl;
      }
      le->setEdit(core, true);
    });
  }
  
}

rfl::Generic List::getGeneric() { 

  return _dict ? getGenericObject() : getGenericVector(); 
  
}

Element *List::hitTest(const Point &origin, const Point &p) { 

  if (_state == reorder) {
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
    Core::registerRootHUDMode(mode);
    mode->add(new Shortcut(L"O", L"rder"));
    mode->add(new Shortcut(L"N", L"ew"));
    mode->add(new Shortcut(L"T", L"ransform"));
    hud->registerMode("rootlist", mode);
  }

  {
    auto mode = new HUDMode(false);
    Core::registerRootHUDMode(mode);
    mode->add(new Shortcut(L"N", L"ew"));
    mode->add(new Shortcut(L"T", L"ransform"));
    hud->registerMode("rootdict", mode);
  }

  {
    auto mode = new HUDMode(false);
    Core::registerGlobalHUDMode(mode);
    mode->add(new Shortcut(L"C", L"opy"));
    mode->add(new Shortcut(L"P", L"aste"));
    mode->add(new Shortcut(L"O", L"rder"));
    mode->add(new Shortcut(L"N", L"ew"));
    Core::registerTextHUDMode(mode);
    hud->registerMode("list", mode);
  }
  
  {
    auto mode = new HUDMode(false);
    Core::registerGlobalHUDMode(mode);
    mode->add(new Shortcut(L"C", L"opy"));
    mode->add(new Shortcut(L"P", L"aste"));
    mode->add(new Shortcut(L"N", L"ew"));
    Core::registerTextHUDMode(mode);
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

void List::registerListKeyHandlers() {

  _listHandlers[SDLK_O] =  [&](Core &core) { 
    if (_dict) {
      return;
    }
    if (_state == reorder) {
      return;
    }
    _state = reorder;
    startEdit(core);
    core.layout(root());
  };
  _listHandlers[SDLK_N] =  [&](Core &core) {
    if (_state == none) {
      // also "New"
      _state = adding;
      return;
    }
    add(core, L"long", new Long(0), false);
  };
  _listHandlers[SDLK_T] =  [&](Core &core) {
    transformCode(core);
  };
  
  _listHandlers[SDLK_C] =  [&](Core &core) { 
    if (_state != none) {
      return;
    }
    core.copy(this);
  };
  _listHandlers[SDLK_P] =  [&](Core &core) { 
    auto elem = core.getClipboard();
    if (elem) {
      auto l = dynamic_cast<List *>(elem);
      if (l) {
        if (l->isDict() == isDict()) {
          mergeIntoUs(core, l);
        }
        else {
          add(core, L"list", l, true);
        }
      }
      else {
        add(core, L"elem", elem, false);
      }
    }
    else {
      core.setError("Invalid Dict");
    }
  };
  
  _listHandlers[SDLK_ESCAPE] =  [&](Core &core) { 
    if (_state == adding) {
      _state = none;
      return;
    }
    if (_state != reorder) {
      return;
    }
    if (_moving) {
      _moving = nullptr;
      _moveover = nullptr;
    }
    else {
      _state = none;
      endEdit(core);
    }
    core.layout(root());
  };
  
  _listHandlers[SDLK_D] =  [&](Core &core) {
    if (_state == adding) {
      add(core, L"dict", new List(true), true);
      return;
    }
    if (_state == reorder && _moving) {
      reorderList();
      _state = none;
      endEdit(core);
      _moving = nullptr;
      _moveover = nullptr;
      core.layout(root());
      core.setDirty(this);
      return;
    }
    if (!isParentRoot()) {
      core.processDeleteKey(getParent());
    }
  };
  _listHandlers[SDLK_L] =  [&](Core &core) {
    if (_state != adding) {
      return;
    }
    add(core, L"list", new List(false), true);
  };
  _listHandlers[SDLK_S] =  [&](Core &core) {
    if (_state != adding) {
      return;
    }
    add(core, L"string", new String(L"value"), false);
  };
  _listHandlers[SDLK_B] =  [&](Core &core) {
    if (_state != adding) {
      return;
    }
    add(core, L"bool", new Bool(false), false);
  };
}

void List::initHUD(HUD *hud) {

  _hudrootlist = hud->findMode(_dict ? "rootdict" : "rootlist");
  _hudlist = hud->findMode(_dict ? "dict" : "list");
  _hudlistreorder = hud->findMode("listreorder");
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

void List::setMode(Core &core, HUD *hud) {

//  cout << "List::setMode" << endl;
  
  if (core.textTooSmall()) {
    hud->setMode(0);
    return;
  }

  switch (_state) {
    case adding:
      hud->setMode(_hudaddlist);
      return;
      
    case reorder:
      hud->setMode(_moving ? _hudlistmove : _hudlistreorder);
      return;

    default:
      break;
  }
  
  if (isParentRoot()) {
    hud->setMode(_hudrootlist);
    return;
  }
  
  hud->setMode(_hudlist);
  
}

void List::mergeIntoUs(Core &core, List *other) {

  if (isDict()) {
    if (!other->isDict()) {
      cerr << "can't merge. We are not the same." << endl;
      return;
    }
  
    // move the elements onto us.
    for (auto& e: other->_elements) {
      auto elem = std::move(e.get());
      core.initElement(this, elem);
      core.exec(this, new NewElement(this, elem));
    }
    return;
  }
  
  
  auto e = new ListElem(other);
  core.initElement(this, e);
  core.exec(this, new NewElement(this, e));
  
}

void List::processKey(Core &core, SDL_Keycode code) {

  if (isParentRoot() && _state != adding) {
    // root keys as "S" so we don't do it while we are adding.
    if (core.processRootKey(this, code)) {
      return;
    }
  }
  else {
    if (core.processGlobalKey(code)) {
      return;
    }
  }
  core.processKeyHandler(_listHandlers, code);

}

void List::add(Core &core, const std::wstring &name, Element *element, bool container) {

  _state = none;
  
  Element *e;
  if (_dict) {
    // wrap in a property
    e = new Property(name, element, container); 
    core.initElement(this, e);

  }
  else {
    // wrap in a list element.
    e = new ListElem(element);
    core.initElement(this, e);
  }

  core.exec(this, new NewElement(this, e));
  
}

void List::reorderList() {

  if (!_moving || !_moveover) {
    cerr << "nothing to move." << endl;
    return;
  }
  
  // and move them. Not undoable :-(
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

void List::drawBorder(Core &core, const Point &origin, const Size &size, bool prop) {

  if (_dict) {
    // top left corner
    core.resources.topleft.render(core, origin);
       
    core.renderFilledRect(Rect(origin + Size(Sizes::thickness, 0), Size(Sizes::toplinelength, Sizes::thickness)), Colours::dictE);
    core.renderFilledRect(Rect(origin + Size(0, Sizes::thickness), Size(Sizes::thickness, Sizes::leftlinelength + (prop ? 20 : 0) - Sizes::thickness)), Colours::dictE);
  
    // bottom left corner
    core.resources.bottomleft.render(core, origin + Size(0, size.h - Sizes::thickness));
    
    core.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::leftlinelength), Size(Sizes::thickness, Sizes::leftlinelength - Sizes::thickness)), Colours::dictE);
    core.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::thickness) + Size(Sizes::thickness, 0), Size(Sizes::bottomlinelength - Sizes::thickness, Sizes::thickness)), Colours::dictE);
  }
  else {
    core.renderFilledRect(Rect(origin + Size(Sizes::thickness, 0), Size(Sizes::toplinelength, Sizes::thickness)), Colours::listE);
    core.renderFilledRect(Rect(origin, Size(Sizes::thickness, Sizes::leftlinelength + (prop ? 40 : 0))), Colours::listE);
    core.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::leftlinelength), Size(Sizes::thickness, Sizes::leftlinelength)), Colours::listE);
    core.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::thickness), Size(Sizes::bottomlinelength, Sizes::thickness)), Colours::listE);
  }

}

void List::transformCode(Core &core) {

  auto code = Code::build(getGeneric());
  if (!code) {
    cerr << "didn't create code correctly" << endl;
    return;
  }
  
  core.removeRoot(root());
  core.addRoot(code);

}

