/*
  boolprop.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Boolean property class.
  
  "name": true
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_boolprop
#define H_boolprop

#include "stringprop.hpp"

class BoolProp: public StringProp {

public:
  BoolProp(const std::string &name, bool value): StringProp(name, value ? "true" : "false") {}
  
  // Box
  virtual rfl::Generic getGeneric() { return _value.str() == "true"; }

};

#endif // H_boolprop
