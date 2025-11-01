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
#include "hudflags.hpp"

#include <vector>

class Core;

class HUDMode {

public:
  HUDMode(bool fixed): _fixed(fixed) {}
  
  void add(Shortcut *shortcut);
    // add a shortcut to the mode.
    
  void build(Core &core);
  void render(Core &core, const Point &origin, const Point &loc);
  
  Point location(const Point &origin, const Point &loc);
  Size size();

  void setFlag(Core &core, HUDFlags flag, bool state);
  
private:

  bool _fixed;
  std::vector<std::unique_ptr<Shortcut> > _shortcuts;
  
};


#endif // H_hudmode
