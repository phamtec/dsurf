/*
  indexable.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 15-Sep-2025
    
  Indexable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "indexable.hpp"

#include "element.hpp"
#include "err.hpp"

using namespace std;

class IndexableErr: public Indexable {

public:

  virtual void setIndex(int index) {
    Err::typeError(typeid(Indexable));
  }

  virtual int getIndex() {
    Err::typeError(typeid(Indexable));
    return 0;
  }

};

Indexable *Indexable::_err = nullptr;

Indexable *Indexable::cast(Element *obj) {

  return Errable<Indexable, IndexableErr>::cast(obj);
  
}
