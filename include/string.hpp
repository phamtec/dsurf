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

#include "element.hpp"
#include "text.hpp"
#include "colours.hpp"
#include "parentable.hpp"
#include "sizeable.hpp"
#include "writeable.hpp"
#include "editable.hpp"

class String: public Element, public Parentable, public Sizeable, public Writeable, public Editable {

  typedef Element super;
  
public:
  String(const std::string &value);

  // Element
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual void edit(TextEditor *editor);

  // Writeable
  virtual rfl::Generic getGeneric();

  // Parentable
  virtual void setParent(Element *parent, int index) { _parent = parent; _index = index; }
  virtual Element *getParent() { return _parent; }
  virtual int getIndex() { return _index; }
  
  // Sizeable
  virtual Size getSize() { return _size; }

  // Editable
  virtual std::string getString();
  virtual void setString(Renderer &renderer, const std::string &s);

protected:
  Element *_parent;
  int _index;
  Size _size;
  Text _value;
};

#endif // H_string
