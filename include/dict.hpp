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
#include "listable.hpp"
#include "parentable.hpp"
#include "writeable.hpp"
#include "hudable.hpp"
#include "keyable.hpp"

#include <memory>
#include <vector>

class Dict: public Element, public Listable, public Parentable, public Writeable, public HUDable, public Keyable {

  typedef Element super;

public:
  Dict(): _parent(0), _adding(false) {}
  
  // Element
  virtual std::string describe();
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual Element *hitTest(const Point &origin, const Point &p);
  virtual Point localOrigin(Element *elem);
  virtual void destroy(Renderer &renderer);
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
  
  static void drawBorder(Renderer &renderer, const Point &origin, const Size &size, bool prop);
  static rfl::Generic getGenericVector(std::vector<std::unique_ptr<Element> > &list);

private:

  Element *_parent;
  Size _size;
  std::vector<std::unique_ptr<Element> > _elements;
  int _hudrootdict;
  int _huddict;
  int _hudadddict;
  bool _adding;
  
  void add(Renderer &renderer, const std::wstring &name, Element *element, bool container);
  
};

#endif // H_dict
