/*
  list.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Aug-2025
    
  List class.
  
  [
    {
      ... props
    }
  ]
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_list
#define H_list

#include "element.hpp"
#include "pushable.hpp"
#include "parentable.hpp"
#include "sizeable.hpp"
#include "writeable.hpp"
#include "hudable.hpp"
#include "keyable.hpp"

#include <memory>
#include <vector>

class List: public Element, public Pushable, public Parentable, public Sizeable, public Writeable, public HUDable, public Keyable  {

  typedef Element super;

public:
  List(): _parent(0), _index(0) {}
  
  // Element
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual Element *hitTest(const Point &origin, const Point &p);
  virtual Point localOrigin(int index);
  
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

  // HUDable
  virtual void setState(HUD *hud);

  // Keyable
  virtual void processKey(Renderer &renderer, SDL_Keycode code);

  // helpers for things that look like a list.
  static void buildVector(Renderer &renderer, std::vector<std::unique_ptr<Element> > &list);
  static Size layoutVector(const Size &size, std::vector<std::unique_ptr<Element> > &list);
  static void renderVector(Renderer &renderer, const Point &origin, std::vector<std::unique_ptr<Element> > &list);
  static void drawBorder(Renderer &renderer, const Point &origin, const Size &size, bool prop);
  static Element* hitTestVector(const Point &origin, const Point &p, std::vector<std::unique_ptr<Element> > &list);
  static Point localOriginVector(std::vector<std::unique_ptr<Element> > &list, int index, bool prop);
  static rfl::Generic getGenericVector(std::vector<std::unique_ptr<Element> > &list);
 
private:

  Element *_parent;
  int _index;
  Size _size;
  std::vector<std::unique_ptr<Element> > _elements;
  
};

#endif // H_list
