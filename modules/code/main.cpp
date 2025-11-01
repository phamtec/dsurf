/*
  main.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 23-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "main.hpp"
#include "builder.hpp"
#include "hud.hpp"
#include "hudmode.hpp"
#include "shortcut.hpp"
#include "core.hpp"
#include "generic.hpp"
#include "root.hpp"

#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;
using flo::Generic;

bool Code::isA(const rfl::Generic &obj) {

  auto transform = Generic::getObject(Generic::getObject(obj), "transform");
  if (transform) {
//    cout << "Code"  << endl;
    return true;
  }

  return false;
  
}

Element *Code::load(const rfl::Generic &obj, const string &filename) {

  return new CodeRoot(filename, obj);
  
}

void Code::registerHUDModes(Core &core, HUD *hud) {

  {
    auto mode = new HUDMode(false);
    Core::registerRootHUDMode(mode);
    hud->registerMode("code", mode);
  }

  {
    auto mode = new HUDMode(false);
    core.registerGlobalHUDMode(mode);
    mode->add(new Shortcut(L"L", L"oad", canLoad));
    hud->registerMode("scenario", mode);
  }

}

 Element *Code::build(const rfl::Generic &g) {

  vector<rfl::Generic> scenarios;
  rfl::Object<rfl::Generic> simple;
  simple["name"] = "Simple";
  simple["input"] = g;
  scenarios.push_back(simple);
  
  vector<rfl::Generic> apply;
  rfl::Object<rfl::Generic> cur;
  rfl::Object<rfl::Generic> empty;
  cur["cur"] = empty;
  apply.push_back(cur);
  rfl::Object<rfl::Generic> transform;
  transform["apply"] = apply;
  
  vector<rfl::Generic> library;
  
  rfl::Object<rfl::Generic> code;
  code["scenarios"] = scenarios;
  code["transform"] = transform;
  code["library"] = library;
  
  Code c;
  if (!c.isA(code)) {
    return nullptr;
  }
  
  return c.load(code, "<code>");
  
}