/*
  parentable.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 15-Sep-2025
    
  Parentable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "parentable.hpp"

#include "element.hpp"
#include "err.hpp"

using namespace std;

class ParentableErr: public Parentable {

public:

  virtual void setParent(Element *parent) {
    Err::typeError(typeid(Parentable));
  }
    
  virtual Element *getParent() {
    Err::typeError(typeid(Parentable));
    return 0;
  }
    
};

Parentable *Parentable::_err = nullptr;

Parentable *Parentable::cast(Element *obj) {

  return Errable<Parentable, ParentableErr>::cast(obj);
  
}
