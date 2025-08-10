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

  typedef Box super;
  
public:
  String(const std::string &value): 
    _value(value, Colours::green) 
      {}

  // Box
  virtual void build(Renderer &renderer, Font &font);
  virtual Size layout(Resources &res, const Point &origin);
  virtual void render(Renderer &renderer, Resources &pool);

private:
  Text _value;
};

#endif // H_string
