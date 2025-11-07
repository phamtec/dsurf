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
#include "handlers.hpp"

#include <memory>
#include <vector>

class List: public Element,  public Writeable, public Commandable  {

  typedef Element super;

public:
  List(bool dict);
  
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
  virtual void destroy(Core &core) override;
  virtual void layout() override;
  virtual void render(Core &core, const Point &origin) override;
  virtual Element *hitTest(const Point &origin, const Point &p) override;
  virtual Point localOrigin(Element *elem) override;
  virtual Size size() override { return _size; }
  virtual RectList calcLayout() override;
  virtual bool visit(std::function<bool (Element *)> f) override;
  
  // Writeable
  virtual DictG getGeneric() override;

  // Commandable
  virtual void initHUD(HUD *hud) override;
  virtual void setMode(Core &core, HUD *hud) override;
  virtual void processKey(Core &core, SDL_Keycode code) override;
  static void registerHUDModes(HUD *hud);

  static List *cast(Element *obj);

private:
  friend class TestList;
  
  typedef enum {
    
    none,
    adding,
    reorder
  
  } State;

  bool _dict;
  Element *_parent;
  Size _size;
  std::vector<std::unique_ptr<Element> > _elements;
  RectList _layout;
  int _hudrootlist;
  int _hudlist;
  int _hudlistreorder;
  int _hudlistmove;
  int _hudaddlist;
  State _state;
  Point _mouse;
  Element *_moving;
  Element *_moveover;
  Point _moveoffs;
  std::map<SDL_Keycode, coreMsgHandler> _listHandlers;
  
  void drawBorder(Core &core, const Point &origin, const Size &size, bool prop);
  DictG getGenericVector();
  DictG getGenericObject();
  void startEdit(Core &core);
  void endEdit(Core &core);
  Element *otherElementHit(const Point &origin, const Point &p);
  void reorderList();
  void add(Core &core, const std::wstring &name, Element *element, bool container);
  bool isParentRoot();
  void mergeIntoUs(Core &core, List *other);
  void transformCode(Core &core);
  void registerListKeyHandlers();
    
};

#endif // H_list
