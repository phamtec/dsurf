/*
  dict.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Dictionary class.
  
  {
    ... props
  }
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_dict
#define H_dict

#include "element.hpp"
#include "pushable.hpp"
#include "parentable.hpp"
#include "sizeable.hpp"
#include "writeable.hpp"

#include <memory>
#include <vector>

class Dict: public Element, public Pushable, public Parentable, public Sizeable, public Writeable {

  typedef Element super;

public:
  Dict(): _parent(0), _index(0) {}
  
  // Element
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual Element *hitTest(const Point &origin, const Point &p);
  virtual Point localOrigin(int index);
  virtual void setState(HUD *hud);
  
  // Writeable
  virtual rfl::Generic getGeneric();

  // Pushable
  virtual void push(Element *element) {
    _elements.push_back(std::unique_ptr<Element>(element));
  }
  
  // Parentable
  virtual void setParent(Element *parent, int index) { _parent = parent; _index = index; }
  virtual Element *getParent() { return _parent; }
  virtual int getIndex() { return _index; }
  
  // Sizeable
  virtual Size getSize() { return _size; }

  static void drawBorder(Renderer &renderer, const Point &origin, const Size &size, bool prop);
  static rfl::Generic getGenericVector(std::vector<std::unique_ptr<Element> > &list);

private:

  Element *_parent;
  int _index;
  Size _size;
  std::vector<std::unique_ptr<Element> > _elements;

};

#endif // H_dict
