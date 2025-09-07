/*
  hud.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 5-Sep-2025
    
  A heads up display.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_hud
#define H_hud

#include "point.hpp"
#include "hudmode.hpp"

class Renderer;

class HUD {

public:
  HUD();

  int registerMode(HUDMode *mode);
    // register a mode for the HUD. it returns the mode index used in setMode
    // register them ALL before calling build.
    
  void build(Renderer &renderer);
  void render(Renderer &renderer, const Point &mouse);
  
  void setMode(int mode) { _mode = mode; }
    // set the mode to draw in the hub.
    
  void setEditingLoc(const Point &loc) { _loc = loc; }
    // set what we are editing in case a HUD want's to draw at that fixed location.
    
private:
  
  int _mode;
  Point _loc;
  std::vector<std::unique_ptr<HUDMode> > _modes;
  
};

#endif // H_hud
