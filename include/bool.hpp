/*
  bool.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Aug-2025
    
  Boolean class.
  
  true
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_bool
#define H_bool

#include "element.hpp"
#include "text.hpp"
#include "colours.hpp"
#include "writeable.hpp"
#include "editable.hpp"
#include "commandable.hpp"

class Bool: public Element,  public Writeable, public Editable, public Commandable {

  typedef Element super;

public:
  Bool(bool value);
  
  // Element
  virtual void setParent(Element *parent) { _parent = parent; }
  virtual Element *getParent() { return _parent; }
  virtual void build(Core &core);
  virtual void layout();
  virtual void render(Core &core, const Point &origin);
  virtual Size size() { return _size; }

  // Writeable
  virtual rfl::Generic getGeneric() { return _value.str() == L"true"; }

  // Editable
  virtual std::wstring getString() { return _value.str(); }
  virtual void setString(Core &core, const std::wstring &s);

  // Commandable
  virtual void initHUD(HUD *hud) {}
  virtual void setMode(Core &core, HUD *hud);
  virtual void processKey(Core &core, SDL_Keycode code);

private:

  Element *_parent;
  Size _size;
  Text _value;

};

#endif // H_bool
