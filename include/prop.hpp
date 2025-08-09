/*
  prop.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Property class.
  
  "name": something
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_prop
#define H_prop

#include "box.hpp"
#include "text.hpp"
#include "colours.hpp"

class Prop: public Box {

public:
  Prop(const std::string &name): 
    _name(name, Colours::blue)
      {}

  typedef Box super;
  
  // Box
  virtual void build(Renderer &renderer, Font &font);
  virtual void render(Renderer &renderer, Resources &pool);

protected:
  Text _name;

};

#endif // H_prop
