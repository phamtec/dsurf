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

#include "shortcut.hpp"
#include "point.hpp"

class Renderer;

enum HUDState {
  None      = 0,
  Text      = 1,
  All       = 2,
  Editing   = 3
};

class HUD {

public:
  HUD();

  void setState(HUDState state) { _state = state; }
  void setEditingLoc(const Point &loc) { _loc = loc; }
  void build(Renderer &renderer);
  void render(Renderer &renderer, const Point &mouse);
  
private:
  
  HUDState _state;
  Point _loc;
  Shortcut _Append;
  Shortcut _Copy;
  Shortcut _Paste;
  Shortcut _Escape;

};

#endif // H_hud
