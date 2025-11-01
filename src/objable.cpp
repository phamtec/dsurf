/*
  objable.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 29-Sep-2025
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "objable.hpp"

#include "element.hpp"
#include "err.hpp"

using namespace std;

class ObjableErr: public Objable {

public:
  ObjableErr(Element *elem): _elem(elem) {}

  virtual void setObj(Core &core, Element *obj) {
    Err::typeError(_elem, typeid(Objable));
  }
  virtual Element *getObj() {
    Err::typeError(_elem, typeid(Objable));
    return nullptr;
  }

private:
  Element *_elem;

};

Objable *Objable::_err = nullptr;

Objable *Objable::cast(Element *obj) {

  return Errable<Objable, ObjableErr>::cast(obj);

}
