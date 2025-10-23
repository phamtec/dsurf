/*
  modules.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 2-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "modules.hpp"

#include "../modules/project/main.hpp"
#include "../modules/code/main.hpp"

using namespace std;

Element *Modules::load(const rfl::Generic &obj, const string &filename) {

  // all the modules might register and be used here.
  Project p;
  if (p.isA(obj)) {
    cout << "found a project module" << endl;
    return p.load(obj, filename);
  }
  Code c;
  if (c.isA(obj)) {
    cout << "found a code module" << endl;
    return c.load(obj, filename);
  }
    
  return nullptr;
  
}

void Modules::registerHUDModes(Renderer &renderer, HUD *hud) {

  Project::registerHUDModes(renderer, hud);
  Code::registerHUDModes(renderer, hud);
  
}
