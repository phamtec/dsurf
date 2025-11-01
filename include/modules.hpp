/*
  modules.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 2-Oct-2025
    
  Implement modules which can identify and load custom elements
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_modules
#define H_modules

#include <rfl.hpp>

class Element;
class Core;
class HUD;

class Modules {

public:
  
  static Element *load(const rfl::Generic &obj, const std::string &filename);
    // Determine and load the module.
    
  static void registerHUDModes(Core &core, HUD *hud);

};

#endif // H_modules
