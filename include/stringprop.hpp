/*
  stringprop.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  String property class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dead
*/

#ifndef H_stringprop
#define H_stringprop

#include "text.hpp"

class StringProp: public Box {

public:
  StringProp(const std::string &name, const std::string &value): _name(name), _value(value) {}
  
  // Box
  virtual void render(Renderer &renderer, Font &font, float x, float *y);

private:
  Text _name;
  Text _value;
};

#endif // H_stringprop
