/*
  modules.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 2-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "modules.hpp"

#include "../modules/project/main.hpp"

using namespace std;

Element *Modules::load(const rfl::Generic &obj) {

  // all the modules might register and be used here.
  Project p;
  if (p.isA(obj)) {
    cout << "found a project module" << endl;
    return p.load(obj);
  }
    
  return nullptr;
  
}
