/*
  main.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 2-Oct-2025
    
  The main file for a Project
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_project_main
#define H_project_main

#include "module.hpp"

class Project: public Module {

public:

  virtual bool isA(const rfl::Generic &obj);
  virtual Element *load(const rfl::Generic &obj);
  
private:

};

#endif // H_project_main
