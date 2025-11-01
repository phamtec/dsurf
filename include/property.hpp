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
  virtual void setParent(Element *parent) override { _parent = parent; }
  virtual Element *getParent() override { return _parent; }
  virtual std::string describe() override;
  virtual void build(Core &core) override;
  virtual void layout() override;
  virtual void render(Core &core, const Point &origin) override;
  virtual Element *hitTest(const Point &origin, const Point &p) override;
  virtual Point localOrigin(Element *elem) override;
  virtual Size size() override { return _size; }
  virtual RectList calcLayout() override;
  virtual bool visit(std::function<bool (Element *)> f) override;

  // Writeable
  virtual std::string getName() override;
  virtual std::optional<std::string> getPropName() override { return getName(); }
  virtual rfl::Generic getGeneric() override;

  // Editable
  virtual std::wstring getString() override { return _name.str(); }
  virtual void setString(Core &core, const std::wstring &s) override;

  // Commandable
  virtual void initHUD(HUD *hud) override;
  virtual void setMode(Core &core, HUD *hud) override;
  virtual void processKey(Core &core, SDL_Keycode code) override;

  // Objable
  virtual void setObj(Core &core, Element *obj) override;
  virtual Element *getObj() override { return _obj.get(); }
    
private:

  Element *_parent;
  Size _size;
  Text _name;
  bool _container;
  std::unique_ptr<Element> _obj;
  RectList _layout;
  
};

#endif // H_property
