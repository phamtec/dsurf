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

#include "element.hpp"
#include "text.hpp"
#include "colours.hpp"
#include "parentable.hpp"
#include "sizeable.hpp"
#include "writeable.hpp"

class Bool: public Element, public Parentable, public Sizeable, public Writeable {

  typedef Element super;

public:
  Bool(bool value);
  
  // Element
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual void edit(TextEditor *editor);

  // Writeable
  virtual rfl::Generic getGeneric() { return _value.str() == "true"; }

  // Parentable
  virtual void setParent(Element *parent, int index) { _parent = parent; _index = index; }
  virtual Element *getParent() { return _parent; }
  virtual int getIndex() { return _index; }
  
  // Sizeable
  virtual Size getSize() { return _size; }

private:

  Element *_parent;
  int _index;
  Size _size;
  Text _value;

};

#endif // H_bool
