/*
  element.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "element.hpp"

#include "parentable.hpp"
#include "sizeable.hpp"
#include "err.hpp"

#include <iostream>

using namespace std;

std::string Element::describe() {

  return Err::demangle(typeid(*this));
  
}

Element *Element::hitTest(const Point &origin, const Point &p) { 

  if (Rect(origin, Sizeable::cast(this)->getSize()).contains(p)) {
//    cout << "hit " << describe() << endl;
    return this; 
  }
  
  return nullptr;
  
}

Point Element::origin() {

  // get the parent.
  Element *parent = Parentable::cast(this)->getParent();
  if (!parent) {
    return Point();
  }
  
  Point o = parent->origin();
  Point lo = parent->localOrigin(this);
//   cout << "origin parent " << typeid(*parent).name() << endl;
//   cout << "o: " << o << endl;
//   cout << "lo: " << lo << endl;
  
  // recursively calculate the origin.
  return o + lo;
  
}

Element *Element::root() {

  // walk through Parentable
  Element *p = this;
  Element *root = p;
  while (p) {
    Parentable *px = dynamic_cast<Parentable *>(p);
    if (!px) {
      break;
    }
    p = px->getParent();
    if (p) {
      root = p;
    }
  }

  return root;
  
}
