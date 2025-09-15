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

#include <iostream>

using namespace std;

class HUDableErr: public HUDable {

public:

  virtual void initHUD(HUD *hud) {
    cerr << "obj not hudable!" << endl;
  }
  virtual void setMode(Renderer &renderer, HUD *hud) {
    cerr << "obj not hudable!" << endl;
  }

};

HUDable *HUDable::_err = nullptr;

HUDable *HUDable::cast(Element *obj) {

  HUDable *px = dynamic_cast<HUDable *>(obj);
  if (!px) {
    if (!_err) {
     _err = new HUDableErr();
    }
    px = _err;
  }
  return px;
  
}
