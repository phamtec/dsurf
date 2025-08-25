/*
  property.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 22-Aug-2025
    
  List property class.
  
  "name": something

  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_property
#define H_property

#include "element.hpp"
#include "text.hpp"
#include "parentable.hpp"
#include "sizeable.hpp"
#include "writeable.hpp"
#include "editable.hpp"

#include <memory>
#include <vector>

class Property: public Element, public Parentable, public Sizeable, public Writeable, public Editable  {

  typedef Element super;

public:
  Property(const std::string &name, Element *obj, bool container);
  
  // Element
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual Element *hitTest(const Point &origin, const Point &p);
  virtual Point localOrigin(int index);
  virtual void edit(TextEditor *editor);

  // Writeable
  virtual std::string getName() { return _name.str(); }
  virtual rfl::Generic getGeneric();

  // Parentable
  virtual void setParent(Element *parent, int index) { _parent = parent; _index = index; }
  virtual Element *getParent() { return _parent; }
  virtual int getIndex() { return _index; }
  
  // Sizeable
  virtual Size getSize() { return _size; }

  // Editable
  virtual std::string getString() { return _name.str(); }
  virtual void setString(Renderer &renderer, const std::string &s);

private:

  Element *_parent;
  int _index;
  Size _size;
  Text _name;
  bool _container;
  std::unique_ptr<Element> _obj;
  
};

#endif // H_property
