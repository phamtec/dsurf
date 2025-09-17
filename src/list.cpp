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
#include "listelem.hpp"
#include "err.hpp"
#include "move.hpp"

#include <iostream>
#include <SDL3/SDL.h>

#define FIXED_HEIGHT  200
#define FIXED_WIDTH   600

using namespace std;

List *List::cast(Element *obj) {

  return ThrowErrable<List>::cast(obj);

}

Size List::layout() {

  _size = List::layoutVector(Size(0, Sizes::listgap), _elements);
  _size.h += _elements.size() == 0 ? Sizes::listgap - 10 : Sizes::listgap;

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

  if (_moveindex >= 0) {
  
    // Collect raw pointers.
    vector<Element *> list;
    transform(_elements.begin(), _elements.end(), back_inserter(list), [](auto& e) { return e.get(); });
  
    // Sort by index.
    sort(list.begin(), list.end(), [](Element *a, Element *b) {
      return Indexable::cast(a)->getIndex() < Indexable::cast(b)->getIndex() ;
    });
    
    // all the non moving objects.
    {
      Point o = origin + Point(Sizes::group_indent, Sizes::listgap);
      for (auto i: list) {
        Size s = Sizeable::cast(i)->getSize();
        int index = Indexable::cast(i)->getIndex();
        if (index != _moveindex) {
          i->render(renderer, o);
        }
        o.y += s.h + Sizes::listgap;
      }
    }
    
    // and the one we are moving.
    {
      Point o = origin + Point(Sizes::group_indent, Sizes::listgap);
      for (auto i: list) {
        Size s = Sizeable::cast(i)->getSize();
        int index = Indexable::cast(i)->getIndex();
        if (index == _moveindex) {
        
          Point ro = o;
          Point p = renderer.noOffset(_mouse) - _moveoffs;
          p.y += (index * (s. h + Sizes::listgap)) + Sizes::listgap;
          
          // constrain on y.
          ro.y = p.y;
          
          // constrain to the list.
          Point lo = this->origin();
          Size ls = getSize();
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
    if (_moveindex >= 0) {
      // what are we over?
      auto hit = otherElementHit(origin, p);
      if (hit) {
        _moveover = Indexable::cast(hit)->getIndex();
      }
      else {
        _moveover = -1;
      }
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
    Size s = Sizeable::cast(obj)->getSize();
    int index = Indexable::cast(obj)->getIndex();
    if (index == _moveindex) {
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

void List::setMoving(int index) {
  _moveindex = index;
  _moveover = -1;
}

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
    mode->add(new Shortcut(L"C", L"opy"));
    mode->add(new Shortcut(L"E", L"dit"));
    hud->registerMode("list", mode);
  }
  
  {
    auto mode = new HUDMode(false);
    mode->add(new Shortcut(L"Esc", L"(finish)"));
    hud->registerMode("listedit", mode);
  }

  {
    auto mode = new HUDMode(false);
    mode->add(new Shortcut(L"Esc", L"(finish)"));
    mode->add(new Shortcut(L"D", L"(rop)"));
    hud->registerMode("listmove", mode);
  }

}

void List::initHUD(HUD *hud) {

  _hudrootlist = hud->findMode("rootlist");
  _hudlist = hud->findMode("list");
  _hudlistedit = hud->findMode("listedit");
  _hudlistmove = hud->findMode("listmove");
  
  // and walk the list.
  initHUDVector(_elements, hud);

}

void List::destroyVector(std::vector<std::unique_ptr<Element> > &list, Renderer &renderer) {

  for_each(list.begin(), list.end(), [&renderer](auto& e) { 
    e->destroy(renderer);
  });

}

void List::setMode(Renderer &renderer, HUD *hud) {

  if (_editing) {
    if (_moveindex >= 0) {
      hud->setMode(_hudlistmove);
    }
    else {
      hud->setMode(_hudlistedit);
    }
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
      if (!_editing) {
        break;
      }
      if (_moveindex >= 0) {
        _moveindex = -1;
        _moveover = -1;
      }
      else {
        _editing = false;
        endEdit(renderer);
      }
      root()->layout();
      break;

    case SDLK_D:
      if (_moveindex < 0) {
        break;
      }
      reorder();
      _moveindex = -1;
      _moveover = -1;
      root()->layout();
      break;

  }
  
}

Indexable *List::findIndex(int index) {

  auto it = std::find_if(_elements.begin(), _elements.end(), [index](auto & e) {
    return Indexable::cast(e.get())->getIndex() == index;
  });
  if (it != _elements.end()) {
    return Indexable::cast(it->get());
  }
  return nullptr;
}

void List::reorder() {

  if (_moveindex < 0 || _moveover < 0) {
    cerr << "nothing to move." << endl;
    return;
  }
  
  // get all the indexable objects.
  vector<Indexable *> objs;
  transform(_elements.begin(), _elements.end(), back_inserter(objs), [](auto& e) { return Indexable::cast(e.get()); });
  
  // and move them.
  Move::moveObj(objs, _moveindex, _moveover);

}

rfl::Generic List::getGenericVector(std::vector<std::unique_ptr<Element> > &list) { 

  vector<Element *> l2;
  orderElements(list, &l2);

  vector<rfl::Generic> obj = vector<rfl::Generic>();

  transform(l2.begin(), l2.end(), back_inserter(obj), [](auto& e) {
    auto *wx = dynamic_cast<Writeable *>(e);
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

void List::orderElements(vector<unique_ptr<Element> > &list, vector<Element *> *l2) {

  // Collect raw pointers.
  transform(list.begin(), list.end(), back_inserter(*l2), [](auto& e) { return e.get(); });

  // Sort by index.
  sort(l2->begin(), l2->end(), [](Element *a, Element *b) {
    return Indexable::cast(a)->getIndex() < Indexable::cast(b)->getIndex() ;
  });

}

Element* List::hitTestVector(const Point &origin, const Point &p, std::vector<std::unique_ptr<Element> > &list) {

  vector<Element *> l2;
  orderElements(list, &l2);
    
  Point o = origin;
  for (auto i: l2) {
    Element *hit = i->hitTest(o, p);
    if (hit) {
      return hit;
    }
    o.y += Sizeable::cast(i)->getSize().h + Sizes::listgap;
  }
  return nullptr;
  
}

Size List::layoutVector(const Size &size, std::vector<std::unique_ptr<Element> > &list) {

  vector<Element *> l2;
  orderElements(list, &l2);
    
  Size newsize = size;
  for (auto i: l2) {
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

  vector<Element *> l2;
  orderElements(list, &l2);
    
  // And render, We use a for loop because... side effect :-)
  Point o = origin;
  for (auto i: l2) {
    i->render(renderer, o);
    o.y += Sizeable::cast(i)->getSize().h + Sizes::listgap;
  }

}

Point List::localOriginVector(std::vector<std::unique_ptr<Element> > &list, int index, bool prop) {

  vector<Element *> l2;
  orderElements(list, &l2);
    
  int j=0;
  int y=Sizes::listgap + (prop ? 40 : 0);
  for (auto i: l2) {
    if (j == index) {
      return Point(Sizes::group_indent, y);
    }
    y += Sizeable::cast(i)->getSize().h + Sizes::listgap;
    j++;
  }

  return Point(0, 0);
  
}

void List::initHUDVector(std::vector<std::unique_ptr<Element> > &list, HUD *hud) {

  // order isn't important.
  for_each(list.begin(), list.end(), [hud](auto& e) { 
    auto *hx = dynamic_cast<HUDable *>(e.get());
    if (hx) {
      hx->initHUD(hud);
    }
    else {
      cerr << typeid(e.get()).name() << " not HUDable" << endl;
    }
  });
  
}

void List::drawBorder(Renderer &renderer, const Point &origin, const Size &size, bool prop) {

  renderer.renderFilledRect(Rect(origin + Size(Sizes::thickness, 0), Size(Sizes::toplinelength, Sizes::thickness)), Colours::orange);
  renderer.renderFilledRect(Rect(origin, Size(Sizes::thickness, Sizes::leftlinelength + (prop ? 40 : 0))), Colours::orange);
  renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::leftlinelength), Size(Sizes::thickness, Sizes::leftlinelength)), Colours::orange);
  renderer.renderFilledRect(Rect(origin + Size(0, size.h - Sizes::thickness), Size(Sizes::bottomlinelength, Sizes::thickness)), Colours::orange);

}
