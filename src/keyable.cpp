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

#include <iostream>

using namespace std;

class KeyableErr: public Keyable {

public:

  virtual void processKey(Renderer &renderer, SDL_Keycode code) {
    cerr << "obj not keyable!" << endl;
  }

};

Keyable *Keyable::_err = nullptr;

Keyable *Keyable::cast(Element *obj) {

  Keyable *px = dynamic_cast<Keyable *>(obj);
  if (!px) {
    if (!_err) {
     _err = new KeyableErr();
    }
    px = _err;
  }
  return px;
  
}
