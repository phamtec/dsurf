/*
  keyable.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 15-Sep-2025
    
  Keyable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "keyable.hpp"

#include "element.hpp"
#include "err.hpp"

using namespace std;

class KeyableErr: public Keyable {

public:

  virtual void processKey(Renderer &renderer, SDL_Keycode code) {
    Err::typeError(typeid(Keyable));
  }

};

Keyable *Keyable::_err = nullptr;

Keyable *Keyable::cast(Element *obj) {

  return Errable<Keyable, KeyableErr>::cast(obj);
  
}
