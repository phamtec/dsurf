/*
  hudmode.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 7-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "hudmode.hpp"

#include "renderer.hpp"

using namespace std;

void HUDMode::add(Shortcut *shortcut) {
  _shortcuts.push_back(unique_ptr<Shortcut>(shortcut));
}

void HUDMode::build(Renderer &renderer) {

  for (auto &&i: _shortcuts) {
    i->build(renderer);
  }

}

Point HUDMode::location(const Point &origin, const Point &loc) {

  if (_fixed) {
    auto s = _shortcuts[0]->size();
    s.h *= _shortcuts.size();
    return loc + Size(0, -10 - s.h);
  }
  else {
    return origin;
  }
  
}

Size HUDMode::size() {

  // height same for each shortcut.
  auto s = _shortcuts[0]->size();
  s.h *= _shortcuts.size();
  
  // expand to widest
  for (auto &&i: _shortcuts) {
    auto s2 = i->size();
    if (s2.w > s.w) {
      s.w = s2.w;
    }
  }
  
  return s;
}

void HUDMode::render(Renderer &renderer, const Point &origin, const Point &loc) {

  if (_fixed) {
    auto s = _shortcuts[0]->size();
    for (int i=_shortcuts.size(); i>0; i--) {
      _shortcuts[i-1]->render(renderer, loc + Size(0, -10 - (s.h*i)));
    }
  }
  else {
    auto s = _shortcuts[0]->size();
    for (int i=0; i<_shortcuts.size(); i++) {
      _shortcuts[i]->render(renderer, origin + Size(0, (s.h*i)));
    }
  }

}
