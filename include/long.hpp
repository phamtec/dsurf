/*
  long.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Long class.
  
  123
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_long
#define H_long

#include "element.hpp"
#include "text.hpp"
#include "colours.hpp"
#include "writeable.hpp"
#include "editable.hpp"
#include "hudable.hpp"
#include "keyable.hpp"

class Long: public Element,  public Writeable, public Editable, public HUDable, public Keyable {

  typedef Element super;
  
public:
  Long(long value);

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

private:

  Element *_parent;
  Size _size;
  Text _value;
};

#endif // H_long
