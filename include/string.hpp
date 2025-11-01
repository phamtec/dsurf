/*
  string.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  String class.
  
  "string"
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_string
#define H_string

#include "element.hpp"
#include "text.hpp"
#include "colours.hpp"
#include "writeable.hpp"
#include "editable.hpp"
#include "commandable.hpp"

class String: public Element,  public Writeable, public Editable, public Commandable {

  typedef Element super;
  
public:
  String(const std::wstring &value);

  // Element
  virtual std::string describe();
  virtual void setParent(Element *parent) { _parent = parent; }
  virtual Element *getParent() { return _parent; }
  virtual void build(Core &core);
  virtual void layout();
  virtual void render(Core &core, const Point &origin);
  virtual Size size() { return _size; }

  // Writeable
  virtual rfl::Generic getGeneric();

  // Editable
  virtual std::wstring getString() { return _value.str(); }
  virtual void setString(Core &core, const std::wstring &s);

  // Commandable
  virtual void initHUD(HUD *hud) {}
  virtual void setMode(Core &core, HUD *hud);
  virtual void processKey(Core &core, SDL_Keycode code);

protected:

  Element *_parent;
  Size _size;
  Text _value;
  
};

#endif // H_string
