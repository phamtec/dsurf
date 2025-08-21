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

class StringProp: public Box {

  typedef Box super;
  
public:
  StringProp(Box *parent, int index, const std::string &name, const std::string &value):
    super(parent, index),
    _name(name, Colours::blue),
    _value(value, Colours::green) 
      {}

  // Box
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual std::string getName() { return _name.str(); }
  virtual rfl::Generic getGeneric();
  virtual void edit(TextEditor *editor);

protected:
  
  Text _name;
  Text _value;
};

#endif // H_stringprop
