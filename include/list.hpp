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
#include "listable.hpp"
#include "parentable.hpp"
#include "indexable.hpp"
#include "indexable.hpp"
#include "sizeable.hpp"
#include "writeable.hpp"
#include "hudable.hpp"
#include "keyable.hpp"

#include <memory>
#include <vector>

class List: public Element, public Listable, public Parentable, public Indexable, public Sizeable, public Writeable, public HUDable, public Keyable  {

  typedef Element super;

public:
  List(): _parent(0), _index(0), _editing(false), _moveindex(-1), _moveover(-1), _adding(false) {}
  
  void setMoving(int index);
    // this object is currently moving.
    
  // Element
  virtual std::string describe();
  virtual void build(Renderer &renderer);
  virtual void destroy(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual Element *hitTest(const Point &origin, const Point &p);
  virtual Point localOrigin(int index);
  
  // Writeable
  virtual rfl::Generic getGeneric();

  // Listable
  virtual void push(Element *element) {
    _elements.push_back(std::unique_ptr<Element>(element));
  }
  virtual int count() { return _elements.size(); }
  virtual Element *at(int index) { return _elements[index].get(); }
  virtual std::vector<std::unique_ptr<Element> > *getElements() { return &_elements; }
  
  // Parentable
  virtual void setParent(Element *parent) { _parent = parent; }
  virtual Element *getParent() { return _parent; }
  
  // Indexable
  virtual void setIndex(int index) { _index = index; }
  virtual int getIndex() { return _index; }
  
  // Sizeable
  virtual Size getSize() { return _size; }

  // HUDable
  virtual void initHUD(HUD *hud);
  virtual void setMode(Renderer &renderer, HUD *hud);
  static void registerHUDModes(HUD *hud);

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
  static void initHUDVector(std::vector<std::unique_ptr<Element> > &list, HUD *hud);
  static void destroyVector(std::vector<std::unique_ptr<Element> > &list, Renderer &renderer);
 
  static List *cast(Element *obj);

private:
  
  Element *_parent;
  int _index;
  Size _size;
  std::vector<std::unique_ptr<Element> > _elements;
  int _hudrootlist;
  int _hudlist;
  int _hudlistedit;
  int _hudlistmove;
  int _hudaddlist;
  bool _adding;
  bool _editing;
  Point _mouse;
  int _moveindex;
  int _moveover;
  Point _moveoffs;
  
  void startEdit(Renderer &renderer);
  void endEdit(Renderer &renderer);
  Element *otherElementHit(const Point &origin, const Point &p);
  void reorder();
  Indexable *findIndex(int index);
  static void orderElements(std::vector<std::unique_ptr<Element> > &list, std::vector<Element *> *l2);
  void add(Renderer &renderer, Element *element);
    
};

#endif // H_list
