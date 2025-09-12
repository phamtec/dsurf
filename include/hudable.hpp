/*
  hudable.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Sep-2025
    
  Interface class for dealing with the HUD on objects..
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_hudable
#define H_hudable

class HUD;

class HUDable {

public:
  virtual ~HUDable() {};

  virtual void initHUD(HUD *hud) = 0;
    // for those objects that deal with the HUD direcrly, initialise them.
    
  virtual void setMode(Renderer &renderer, HUD *hud) = 0;
    // set the mode in the heads up display.
    
};

#endif // H_hudable
