/*
  list.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dead
*/

#include "list.hpp"

void List::render(Renderer &renderer, Font &font, float x, float *y) {

  _name.render(renderer, font, x, y);
  for (auto&& i: _objs) {
    if (i) {
      i->render(renderer, font, x, y);
    }
  }

}
