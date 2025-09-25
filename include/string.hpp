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
#include "hudable.hpp"
#include "keyable.hpp"

class String: public Element,  public Writeable, public Editable, public HUDable, public Keyable {

  typedef Element super;
  
public:
  String(const std::wstring &value);

  // Element
  virtual void setParent(Element *parent) { _parent = parent; }
  virtual Element *getParent() { return _parent; }
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual Size size() { return _size; }

  // Writeable
  virtual rfl::Generic getGeneric();

  // Editable
  virtual std::wstring getString() { return _value.str(); }
  virtual void setString(Renderer &renderer, const std::wstring &s);

  // HUDable
  virtual void initHUD(HUD *hud) {}
  virtual void setMode(Renderer &renderer, HUD *hud);

  // Keyable
  virtual void processKey(Renderer &renderer, SDL_Keycode code);

protected:

  Element *_parent;
  Size _size;
  Text _value;
  
};

#endif // H_string
