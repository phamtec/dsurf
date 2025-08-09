/*
  string.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  String class.
  
  "string"
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_string
#define H_string

#include "box.hpp"
#include "text.hpp"
#include "colours.hpp"

class String: public Box {

public:
  String(const std::string &value): 
    _value(value, Colours::green) 
      {}

  typedef Box super;
  
  // Box
  virtual void build(Renderer &renderer, Font &font);
  virtual float layout(Resources &pool, float x, float y);
  virtual void render(Renderer &renderer, Resources &pool);

private:
  Text _value;
};

#endif // H_string
