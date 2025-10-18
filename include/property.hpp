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
#include "writeable.hpp"
#include "editable.hpp"
#include "commandable.hpp"
#include "objable.hpp"

#include <memory>
#include <vector>

class Property: public Element,  public Writeable, public Editable, public Commandable, public Objable  {

  typedef Element super;

public:
  Property(const std::wstring &name, Element *obj, bool container);
  
  // Element
  virtual void setParent(Element *parent) { _parent = parent; }
  virtual Element *getParent() { return _parent; }
  virtual std::string describe();
  virtual void build(Renderer &renderer);
  virtual void layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual Element *hitTest(const Point &origin, const Point &p);
  virtual Point localOrigin(Element *elem);
  virtual void destroy(Renderer &renderer);
  virtual Size size() { return _size; }
  virtual RectList calcLayout();

  // Writeable
  virtual std::string getName();
  virtual rfl::Generic getGeneric();

  // Editable
  virtual std::wstring getString() { return _name.str(); }
  virtual void setString(Renderer &renderer, const std::wstring &s);

  // Commandable
  virtual void initHUD(HUD *hud);
  virtual void setMode(Renderer &renderer, HUD *hud);
  virtual void processKey(Renderer &renderer, SDL_Keycode code);

  // Objable
  virtual void setObj(Renderer &renderer, Element *obj);
  virtual Element *getObj() { return _obj.get(); }
    
private:

  Element *_parent;
  Size _size;
  Text _name;
  bool _container;
  std::unique_ptr<Element> _obj;
  RectList _layout;
  
};

#endif // H_property
