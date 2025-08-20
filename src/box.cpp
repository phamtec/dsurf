/*
  box.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "box.hpp"

#include <iostream>

using namespace std;

Box *Box::hitTest(const Point &origin, const Point &p) { 

//  cout << "testing " << typeid(*this).name() << endl;
  if (Rect(origin, _size).contains(p)) {
//    cout << "hit" << endl;
    return this; 
  }
  
  return nullptr;
  
};
