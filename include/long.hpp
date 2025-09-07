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
#include "parentable.hpp"
#include "sizeable.hpp"
#include "writeable.hpp"
#include "editable.hpp"
#include "hudable.hpp"
#include "keyable.hpp"

class Long: public Element, public Parentable, public Sizeable, public Writeable, public Editable, public HUDable, public Keyable {

  typedef Element super;
  
public:
  Long(long value);

  // Element
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);

  // Writeable
  virtual rfl::Generic getGeneric();

  // Parentable
  virtual void setParent(Element *parent, int index) { _parent = parent; _index = index; }
  virtual Element *getParent() { return _parent; }
  virtual int getIndex() { return _index; }
  
  // Sizeable
  virtual Size getSize() { return _size; }

  // Editable
  virtual std::wstring getString() { return _value.str(); }
  virtual void setString(Renderer &renderer, const std::wstring &s);

  // HUDable
  virtual void setMode(Renderer &renderer, HUD *hud);

  // Keyable
  virtual void processKey(Renderer &renderer, SDL_Keycode code);

private:

  Element *_parent;
  int _index;
  Size _size;
  Text _value;
};

#endif // H_long
