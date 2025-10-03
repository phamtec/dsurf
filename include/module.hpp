/*
  module.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 2-Oct-2025
    
  Base class for a module.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_module
#define H_module

#include <rfl.hpp>

class Element;

class Module {

public:
  
  virtual bool isA(const rfl::Generic &obj) = 0;
    // return true if the object is the module..
    
  virtual Element *load(const rfl::Generic &obj, const std::string &filename) = 0;
    // create the module element from the generic object.
    // we pass the filename so that modules can learn things like the relative path
    // etc.
    
};

#endif // H_module
