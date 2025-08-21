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
#include "parentable.hpp"
#include "sizeable.hpp"

class String: public Box, public Parentable, public Sizeable {

  typedef Box super;
  
public:
  String(Box *parent, int index, const std::string &value): 
    _parent(parent), _index(index), _value(value, Colours::green) 
      {}

  // Box
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual rfl::Generic getGeneric();

  // Parentable
  virtual Box *getParent() { return _parent; }
  virtual int getIndex() { return _index; }
  
  // Sizeable
  virtual Size getSize() { return _size; }

protected:
  Box *_parent;
  int _index;
  Size _size;
  Text _value;
};

#endif // H_string
