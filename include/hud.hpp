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
#include "hudflags.hpp"

#include <map>

class Renderer;

class HUD {

public:
  HUD();

  int registerMode(const std::string &name, HUDMode *mode);
    // register a mode for the HUD. it returns the mode index used in setMode
    // register them ALL before calling build.
    
  int findMode(const std::string &name);
    // find the index of a previously registered mode.
    
  void build(Renderer &renderer);
  void render(Renderer &renderer, const Point &mouse);
  
  void setMode(int mode);
    // set the mode to draw in the hub.
    
  void setEditingLoc(const Point &loc) { _loc = loc; }
    // set what we are editing in case a HUD want's to draw at that fixed location.
  
  void setFlag(Renderer &renderer, HUDFlags flag, bool state);
    // set a flag in the HUD used to conditionally show states.
      
  void setHint(Renderer &renderer, const std::wstring &hint);
    // set the hud into HINT mode when it's really small.
    
private:
  
  int _mode;
  Point _loc;
  std::map<std::string, int> _keys;
  std::vector<std::unique_ptr<HUDMode> > _modes;
  std::unique_ptr<Text> _hint;
  
};

#endif // H_hud
