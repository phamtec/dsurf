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

#include <iostream>

using namespace std;

class ParentableErr: public Parentable {

public:

  virtual void setParent(Element *parent, int index) {
    cerr << "obj not parentable!" << endl;
  }
    
  virtual Element *getParent() {
    cerr << "obj not parentable!" << endl;
    return 0;
  }
    
  virtual int getIndex() {
    cerr << "obj not parentable!" << endl;
    return 0;
  }

};

Parentable *Parentable::_err = nullptr;

Parentable *Parentable::cast(Element *obj) {

  Parentable *px = dynamic_cast<Parentable *>(obj);
  if (!px) {
    if (!_err) {
     _err = new ParentableErr();
    }
    px = _err;
  }
  return px;
  
}
