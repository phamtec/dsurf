/*
  hud.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 5-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "hud.hpp"

#include "core.hpp"
#include "colours.hpp"

using namespace std;

HUD::HUD() {

  // nothing in first mode.
  _modes.push_back(0);

}

void HUD::setMode(int mode) { 
  _mode = mode;
  _hint = nullptr;
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

void HUD::build(Core &core) {
  
  for (auto &&i: _modes) {
    if (i) {
      i->build(core);
    }
  }
  
}

void HUD::render(Core &core, const Point &mouse) {

  Point origin = mouse + Size(40, -40);
//  cout << "origin " << origin << endl;
  
  if (_mode == 0) {
    return;
  }
  
  if (_mode == -1) {
    if (_hint) {
      double scale = 0.3;
      core.setScale(scale, scale);
      
      double kiscale = 1 / scale;
      Point p = (mouse + Size(10, -10)) * kiscale;
      
      _hint->render(core, p, false);
  
      core.setScale(1.0, 1.0);
    }
    return;
  }
  
  if (_mode >= _modes.size()) {
    cerr << "state doesn't exist " << _mode << endl;
    return;
  }
  
  Point l = _modes[_mode]->location(origin, _loc);
  Size s = _modes[_mode]->size();

  core.setDrawColor(Colours::white);
  core.fillRect(Rect(l + Size(-4, -4), s + Size(8, 8)));
  core.setDrawColor(Colours::grey);
  core.drawRect(Rect(l + Size(-4, -4), s + Size(8, 8)));
  
  _modes[_mode]->render(core, origin, _loc);
  
}

void HUD::setFlag(Core &core, HUDFlags flag, bool state) {

//  cout << "setFlag " << flag << ": " << state << endl;
  
  for (auto &&i: _modes) {
    if (i) {
      i->setFlag(core, flag, state);
    }
  }

}

void HUD::setHint(Core &core, Text *hint) {

  _mode = -1;
  _hint = hint;
  
}
