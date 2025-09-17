/*
  writeable.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 15-Sep-2025
    
  Parentable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "writeable.hpp"

#include "element.hpp"
#include "err.hpp"

using namespace std;

class WriteableErr: public Writeable {

public:

  virtual std::string getName() {
    Err::typeError(typeid(Writeable));
    return "???";
  }
  virtual rfl::Generic getGeneric() {
    Err::typeError(typeid(Writeable));
    return 0;
  }

};

Writeable *Writeable::_err = nullptr;

Writeable *Writeable::cast(Element *obj) {

  return Errable<Writeable, WriteableErr>::cast(obj);
  
}
