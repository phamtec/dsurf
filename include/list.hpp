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
#include "writeable.hpp"
#include "hudable.hpp"
#include "keyable.hpp"

#include <memory>
#include <vector>

class List: public Element, public Listable, public Parentable, public Writeable, public HUDable, public Keyable  {

  typedef Element super;

public:
  List(): _parent(0), _editing(false), _moving(0), _moveover(0), _adding(false) {}
  
  void setMoving(Element *elem);
    // this object is currently moving.
    
  // Element
  virtual std::string describe();
  virtual void build(Renderer &renderer);
  virtual void destroy(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual Element *hitTest(const Point &origin, const Point &p);
  virtual Point localOrigin(Element *elem);
  virtual Size size() { return _size; }
  
  // Writeable
  virtual rfl::Generic getGeneric();

  // Listable
  virtual int count() { return _elements.size(); }
  virtual Element *at(int index) { return _elements[index].get(); }
  virtual std::vector<std::unique_ptr<Element> > *getElements() { return &_elements; }
  
  // Parentable
  virtual void setParent(Element *parent) { _parent = parent; }
  virtual Element *getParent() { return _parent; }

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
  static Point localOriginVector(std::vector<std::unique_ptr<Element> > &list, Element *elem, bool prop);
  static rfl::Generic getGenericVector(std::vector<std::unique_ptr<Element> > &list);
  static void initHUDVector(std::vector<std::unique_ptr<Element> > &list, HUD *hud);
  static void destroyVector(std::vector<std::unique_ptr<Element> > &list, Renderer &renderer);
 
  static List *cast(Element *obj);

private:
  
  Element *_parent;
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
  Element *_moving;
  Element *_moveover;
  Point _moveoffs;
  
  void startEdit(Renderer &renderer);
  void endEdit(Renderer &renderer);
  Element *otherElementHit(const Point &origin, const Point &p);
  void reorder();
  void add(Renderer &renderer, Element *element);
    
};

#endif // H_list
