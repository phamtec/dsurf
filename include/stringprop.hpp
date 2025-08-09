/*
  stringprop.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  String property class.
  
  "name": "string"
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_stringprop
#define H_stringprop

#include "box.hpp"
#include "text.hpp"
#include "colours.hpp"

class StringProp: public Box {

public:
  StringProp(const std::string &name, const std::string &value): 
    _name(name, Colours::blue), 
    _value(value, Colours::green) 
      {}
  
  // Box
  virtual void build(Renderer &renderer, Font &font);
  virtual float layout(Resources &pool, float x, float y);
  virtual void render(Renderer &renderer, Resources &pool);

private:
  Text _name;
  Text _value;
};

#endif // H_stringprop
