/*
  sizeable.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 15-Sep-2025
    
  Sizeable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "sizeable.hpp"

#include "element.hpp"
#include "err.hpp"

using namespace std;

class SizeableErr: public Sizeable {

public:

  virtual Size getSize() {
    Err::typeError(typeid(Sizeable));
    return Size();
  }

};

Sizeable *Sizeable::_err = nullptr;

Sizeable *Sizeable::cast(Element *obj) {

  return Errable<Sizeable, SizeableErr>::cast(obj);
  
}
