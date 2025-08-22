/*
  bool.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Aug-2025
    
  Boolean class.
  
  true
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_bool
#define H_bool

#include "box.hpp"
#include "text.hpp"
#include "colours.hpp"
#include "parentable.hpp"
#include "sizeable.hpp"
#include "writeable.hpp"

class Bool: public Box, public Parentable, public Sizeable, public Writeable {

  typedef Box super;

public:
  Bool(Box *parent, int index, bool value);
  
  // Box
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual void edit(TextEditor *editor);

  // Writeable
  virtual rfl::Generic getGeneric() { return _value.str() == "true"; }

  // Parentable
  virtual void setParent(Box *parent) { _parent = parent; }
  virtual Box *getParent() { return _parent; }
  virtual int getIndex() { return _index; }
  
  // Sizeable
  virtual Size getSize() { return _size; }

private:

  Box *_parent;
  int _index;
  Size _size;
  Text _value;

};

#endif // H_bool
