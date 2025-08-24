/*
  box.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "box.hpp"

#include "parentable.hpp"
#include "sizeable.hpp"

#include <iostream>

using namespace std;

Box *Box::hitTest(const Point &origin, const Point &p) { 

//  cout << "testing " << typeid(*this).name() << endl;
  Sizeable *sz = dynamic_cast<Sizeable *>(this);
  if (sz) {
    if (Rect(origin, sz->getSize()).contains(p)) {
  //    cout << "hit" << endl;
      return this; 
    }
  }
  
  return nullptr;
  
}

Point Box::origin() {

  // see if the object can have a parent.
  Parentable *p = dynamic_cast<Parentable *>(this);
  if (!p) {
    cerr << "not parentable!" << endl;
    return Point();
  }
  
  // get the parent.
  Box *parent = p->getParent();
  if (!parent) {
    return Point();
  }
  
  Point o = parent->origin();
  Point lo = parent->localOrigin(p->getIndex());
//   cout << "origin parent " << typeid(*parent).name() << endl;
//   cout << "o: " << o << endl;
//   cout << "lo: " << lo << endl;
  
  // recursively calculate the origin.
  return o + lo;
  
}
