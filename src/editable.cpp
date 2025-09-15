/*
  editable.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 15-Sep-2025
    
  Editable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "editable.hpp"
#include "element.hpp"

#include <iostream>

using namespace std;

class EditableErr: public Editable {

public:

  virtual std::wstring getString() {
    cerr << "obj not editable!" << endl;
    return L"???";
  }
  virtual void setString(Renderer &renderer, const std::wstring &s) {
    cerr << "obj not editable!" << endl;
  }

};

Editable *Editable::_err = nullptr;

Editable *Editable::cast(Element *obj) {

  Editable *px = dynamic_cast<Editable *>(obj);
  if (!px) {
    if (!_err) {
     _err = new EditableErr();
    }
    px = _err;
  }
  return px;
  
}
