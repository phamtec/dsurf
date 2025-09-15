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

#include <iostream>

using namespace std;

class WriteableErr: public Writeable {

public:

  virtual std::string getName() {
    cerr << "obj not writeable!" << endl;
    return "???";
  }
  virtual rfl::Generic getGeneric() {
    cerr << "obj not writeable!" << endl;
    return 0;
  }

};

Writeable *Writeable::_err = nullptr;

Writeable *Writeable::cast(Element *obj) {

  Writeable *px = dynamic_cast<Writeable *>(obj);
  if (!px) {
    if (!_err) {
     _err = new WriteableErr();
    }
    px = _err;
  }
  return px;
  
}
