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
  
  When a Dict:
  
  {
    ... props
  }
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_list
#define H_list

#include "element.hpp"
#include "writeable.hpp"
#include "commandable.hpp"

#include <memory>
#include <vector>

class List: public Element,  public Writeable, public Commandable  {

  typedef Element super;

public:
  List(bool dict): _dict(dict), _parent(0), _editing(false), _moving(0), _moveover(0), _adding(false) {}
  
  void setMoving(Element *elem);
    // this object is currently moving.
    
  bool isDict() { return _dict; }
    // is this really a dictionary?
    
  int count() { return _elements.size(); }
  Element *at(int index) { return _elements[index].get(); }
  std::vector<std::unique_ptr<Element> > *getElements() { return &_elements; }
  
  // Element
  virtual void setParent(Element *parent) override { _parent = parent; }
  virtual Element *getParent() override { return _parent; }
  virtual std::string describe() override;
  virtual void destroy(Renderer &renderer) override;
  virtual void layout() override;
  virtual void render(Renderer &renderer, const Point &origin) override;
  virtual Element *hitTest(const Point &origin, const Point &p) override;
  virtual Point localOrigin(Element *elem) override;
  virtual Size size() override { return _size; }
  virtual RectList calcLayout() override;
  virtual bool visit(std::function<bool (Element *)> f) override;
  
  // Writeable
  virtual rfl::Generic getGeneric() override;

  // Commandable
  virtual void initHUD(HUD *hud) override;
  virtual void setMode(Renderer &renderer, HUD *hud) override;
  virtual void processKey(Renderer &renderer, SDL_Keycode code) override;
  static void registerHUDModes(HUD *hud);

  static List *cast(Element *obj);

private:
  friend class TestList;
  
  bool _dict;
  Element *_parent;
  Size _size;
  std::vector<std::unique_ptr<Element> > _elements;
  RectList _layout;
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
  
  void drawBorder(Renderer &renderer, const Point &origin, const Size &size, bool prop);
  rfl::Generic getGenericVector();
  rfl::Generic getGenericObject();
  void startEdit(Renderer &renderer);
  void endEdit(Renderer &renderer);
  Element *otherElementHit(const Point &origin, const Point &p);
  void reorder();
  void add(Renderer &renderer, const std::wstring &name, Element *element, bool container);
  bool isParentRoot();
  void mergeIntoUs(Renderer &renderer, List *other);
    
};

#endif // H_list
