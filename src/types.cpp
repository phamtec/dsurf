/*
  types.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 11-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "renderer.hpp"

#include "list.hpp"
#include "listelem.hpp"
#include "dict.hpp"

void Renderer::initTypes() {

  // register for types that deal with their own HUD
  List::registerHUDModes(_hud.get());
  ListElem::registerHUDModes(_hud.get());
  Dict::registerHUDModes(_hud.get());

}
