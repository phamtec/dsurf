/*
  main.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 23-Oct-2025
    
  The main file for Code
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_code_main
#define H_code_main

#include "module.hpp"

class Renderer;
class HUD;

class Code: public Module {

public:

  virtual bool isA(const rfl::Generic &obj);
  virtual Element *load(const rfl::Generic &obj, const std::string &filename);
  
  static void registerHUDModes(Renderer &renderer, HUD *hud);
  
private:

};

#endif // H_code_main
