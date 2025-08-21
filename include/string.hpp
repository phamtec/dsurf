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
  String(Box *parent, int index, const std::string &value): 
    super(parent, index), _value(value, Colours::green) 
      {}

  // Box
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual rfl::Generic getGeneric();

protected:
  Text _value;
};

#endif // H_string
