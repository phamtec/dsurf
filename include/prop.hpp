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

  typedef Box super;
  
public:
  Prop(const std::string &name): 
    _name(name, Colours::blue)
      {}

  // Box
  virtual void build(Renderer &renderer);
  virtual void render(Renderer &renderer, const Point &origin);

protected:
  Text _name;

};

#endif // H_prop
