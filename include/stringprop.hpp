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
#include "parentable.hpp"
#include "sizeable.hpp"

class StringProp: public Box, public Parentable, public Sizeable {

  typedef Box super;
  
public:
  StringProp(Box *parent, int index, const std::string &name, const std::string &value):
    _parent(parent), _index(index),
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

  // Parentable
  virtual Box *getParent() { return _parent; }
  virtual int getIndex() { return _index; }
  
  // Sizeable
  virtual Size getSize() { return _size; }

protected:
  
  Box *_parent;
  int _index;
  Size _size;
  Text _name;
  Text _value;
};

#endif // H_stringprop
