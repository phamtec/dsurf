/*
  setobj.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 29-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "setobj.hpp"

#include "objable.hpp"

#include <iostream>

using namespace std;

SetObj::SetObj(std::unique_ptr<Element> *obj, Element *newobj) {

  _obj = obj;
  _newobj = newobj;
  
}

void SetObj::destroy(Renderer &renderer) {
}

void SetObj::exec(Renderer &renderer) {

  (*_obj)->destroy(renderer);
  _obj->reset(_newobj);
}

void SetObj::undo(Renderer &renderer) {
}
