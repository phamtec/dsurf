/*
  editable.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 15-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "editable.hpp"

#include "element.hpp"
#include "err.hpp"

using namespace std;

class EditableErr: public Editable {

public:
  EditableErr(Element *elem): _elem(elem) {}
  
  virtual std::wstring getString() {
    Err::typeError(_elem, typeid(Editable));
    return L"???";
  }
  virtual void setString(Renderer &renderer, const std::wstring &s) {
    Err::typeError(_elem, typeid(Editable));
  }

private:
  Element *_elem;

};

Editable *Editable::_err = nullptr;

Editable *Editable::cast(Element *obj) {

  return Errable<Editable, EditableErr>::cast(obj);

}
