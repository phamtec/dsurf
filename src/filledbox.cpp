/*
  filledbox.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "filledbox.hpp"
#include "colours.hpp"
#include "core.hpp"

void FilledBox::render(Core &core, const Point &origin) {

  core.renderFilledRect(Rect(origin, _size), Colours::blue);
  
}
