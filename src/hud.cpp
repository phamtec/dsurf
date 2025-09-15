/*
  hud.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 5-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "hud.hpp"

#include "renderer.hpp"
#include "colours.hpp"

using namespace std;

HUD::HUD() {

  // nothing in first mode.
  _modes.push_back(0);

}

int HUD::registerMode(const std::string &name, HUDMode *mode) {

  _modes.push_back(unique_ptr<HUDMode>(mode));
  int i = _modes.size() - 1;
//  cout << name << ": " << i << endl;
  _keys[name] = i;
  return i;
  
}

int HUD::findMode(const std::string &name) {

  return _keys[name];
  
}

void HUD::build(Renderer &renderer) {
  
  for (auto &&i: _modes) {
    if (i) {
      i->build(renderer);
    }
  }
  
}

void HUD::render(Renderer &renderer, const Point &mouse) {

  Point origin = mouse + Size(40, -40);
//  cout << "origin " << origin << endl;
  
  if (_mode == 0) {
    return;
  }
  
  if (_mode >= _modes.size()) {
    cerr << "state doesn't exist " << _mode << endl;
    return;
  }
  
  Point l = _modes[_mode]->location(origin, _loc);
  Size s = _modes[_mode]->size();

  renderer.setDrawColor(Colours::white);
  renderer.fillRect(Rect(l + Size(-4, -4), s + Size(8, 8)));
  renderer.setDrawColor(Colours::grey);
  renderer.drawRect(Rect(l + Size(-4, -4), s + Size(8, 8)));
  
  _modes[_mode]->render(renderer, origin, _loc);
  
}