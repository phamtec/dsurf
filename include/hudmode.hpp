/*
  hudmode.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 7-Sep-2025
    
  A heads up display mode.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_hudmode
#define H_hudmode

#include "shortcut.hpp"
#include "point.hpp"
#include "size.hpp"

class Renderer;

class HUDMode {

public:
  HUDMode(bool fixed): _fixed(fixed) {}
  
  void add(Shortcut *shortcut);
    // add a shortcut to the mode.
    
  void build(Renderer &renderer);
  void render(Renderer &renderer, const Point &origin, const Point &loc);
  
  Point location(const Point &origin, const Point &loc);
  Size size();

private:

  bool _fixed;
  std::vector<std::unique_ptr<Shortcut> > _shortcuts;
  
};


#endif // H_hud
