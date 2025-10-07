/*
  locatable.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 7-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "locatable.hpp"

#include "err.hpp"

#include <sstream>

using namespace std;

class LocatableErr: public Locatable {

public:
  LocatableErr(Element *elem): _elem(elem) {}

  virtual Point getLocation() {
    Err::typeError(_elem, typeid(Locatable));
    return Point();
  }
      
  virtual void setLocation(const Point &loc) {
    Err::typeError(_elem, typeid(Locatable));
  }

private:
  Element *_elem;
  
};

Locatable *Locatable::_err = nullptr;

Locatable *Locatable::cast(Element *obj) {

  return Errable<Locatable, LocatableErr>::cast(obj);
  
}
