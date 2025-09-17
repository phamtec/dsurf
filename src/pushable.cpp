/*
  pushable.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 15-Sep-2025
    
  Pushable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "pushable.hpp"

#include "element.hpp"
#include "err.hpp"

using namespace std;

class PushableErr: public Pushable {

public:

  virtual void push(Element *element) {
    Err::typeError(typeid(Pushable));
  }

};

Pushable *Pushable::_err = nullptr;

Pushable *Pushable::cast(Element *obj) {

  return Errable<Pushable, PushableErr>::cast(obj);
  
}
