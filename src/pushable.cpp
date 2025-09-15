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

#include <iostream>

using namespace std;

class PushableErr: public Pushable {

public:

  virtual void push(Element *element) {
    cerr << "obj not pushable!" << endl;
  }

};

Pushable *Pushable::_err = nullptr;

Pushable *Pushable::cast(Element *obj) {

  Pushable *px = dynamic_cast<Pushable *>(obj);
  if (!px) {
    if (!_err) {
     _err = new PushableErr();
    }
    px = _err;
  }
  return px;
  
}
