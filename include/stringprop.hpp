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

#include "prop.hpp"

class StringProp: public Prop {

  typedef Prop super;
  
public:
  StringProp(const std::string &name, const std::string &value): 
    Prop(name), 
    _value(value, Colours::green) 
      {}

  // Box
  virtual void build(Renderer &renderer, Font &font);
  virtual float layout(Resources &res, const Point &origin);
  virtual void render(Renderer &renderer, Resources &pool);

private:

  Text _value;
};

#endif // H_stringprop
