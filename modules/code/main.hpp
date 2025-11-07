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

class Core;
class HUD;

class Code: public Module {

public:

  virtual bool isA(const DictG &obj);
  virtual Element *load(const DictG &obj, const std::string &filename);
  
  static void registerHUDModes(Core &core, HUD *hud);
  
  static Element *build(const DictG &g);

};

#endif // H_code_main
