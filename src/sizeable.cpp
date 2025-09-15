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

#include <iostream>

using namespace std;

class SizeableErr: public Sizeable {

public:

  virtual Size getSize() {
    cerr << "obj not sizeable!" << endl;
    return Size();
  }

};

Sizeable *Sizeable::_err = nullptr;

Sizeable *Sizeable::cast(Element *obj) {

  Sizeable *px = dynamic_cast<Sizeable *>(obj);
  if (!px) {
    if (!_err) {
     _err = new SizeableErr();
    }
    px = _err;
  }
  return px;
  
}
