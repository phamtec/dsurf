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

void SetObj::destroy(Core &core) {
  if (_oldobj) {
    _oldobj->destroy(core);
  }
}

void SetObj::exec(Core &core) {

//   cout << "exec" << endl;
//   cout << "old null " << (_oldobj.get() == nullptr) << endl;

  _oldobj = std::move(*_obj);
  _obj->reset(_newobj);
  
//   cout << "new " << (*_obj)->describe() << endl;
//   cout << "old " << _oldobj->describe() << endl;
  
}

void SetObj::undo(Core &core) {

//   cout << "undo" << endl;
//   cout << "old " << (*_obj)->describe() << endl;
//   cout << "new " << _oldobj->describe() << endl;
  
  _obj->release();
  *_obj = std::move(_oldobj);
  
//   cout << "moved " << (*_obj)->describe() << endl;
//   cout << "old null " << (_oldobj.get() == nullptr) << endl;
  
}
