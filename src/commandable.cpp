/*
  commandable.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 25-Sep-2025
    
  Commandable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "commandable.hpp"

#include "element.hpp"
#include "err.hpp"

class CommandableErr: public Commandable {

public:
  CommandableErr(Element *elem): _elem(elem) {}

  virtual void initHUD(HUD *hud) {
    Err::typeError(_elem, typeid(Commandable));
  }
  virtual void setMode(Core &core, HUD *hud) {
    Err::typeError(_elem, typeid(Commandable));
  }
  virtual void processKey(Core &core, SDL_Keycode code) {
    Err::typeError(_elem, typeid(Commandable));
  }

private:
  Element *_elem;

};

Commandable *Commandable::_err = nullptr;

Commandable *Commandable::cast(Element *obj) {

  return Errable<Commandable, CommandableErr>::cast(obj);
  
}
