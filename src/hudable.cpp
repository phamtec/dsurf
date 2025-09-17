/*
  hudable.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 15-Sep-2025
    
  Hudable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "hudable.hpp"

#include "element.hpp"
#include "err.hpp"

class HUDableErr: public HUDable {

public:

  virtual void initHUD(HUD *hud) {
    Err::typeError(typeid(HUDable));
  }
  virtual void setMode(Renderer &renderer, HUD *hud) {
    Err::typeError(typeid(HUDable));
  }

};

HUDable *HUDable::_err = nullptr;

HUDable *HUDable::cast(Element *obj) {

  return Errable<HUDable, HUDableErr>::cast(obj);
  
}
