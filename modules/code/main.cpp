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
#include "renderer.hpp"
#include "generic.hpp"
#include "root.hpp"

#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;
using flo::Generic;

bool Code::isA(const rfl::Generic &obj) {

  auto code = Generic::getObject(obj);
  auto transform = Generic::getObject(code, "transform");
  if (transform) {
    cout << "Code"  << endl;
    return true;
  }

  return false;
  
}

Element *Code::load(const rfl::Generic &obj, const string &filename) {

  return new CodeRoot(filename, obj);
  
}

void Code::registerHUDModes(Renderer &renderer, HUD *hud) {

  {
    auto mode = new HUDMode(false);
    Renderer::registerRootHUDMode(mode);
    hud->registerMode("code", mode);
  }

  {
    auto mode = new HUDMode(false);
    renderer.registerGlobalHUDMode(mode);
    mode->add(new Shortcut(L"L", L"oad", canLoad));
    hud->registerMode("scenario", mode);
  }

}
