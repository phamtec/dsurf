/*
  prop.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "prop.hpp"
#include "spatial.hpp"

void Prop::build(Renderer &renderer) {

  super::build(renderer);
  
  _name.build(renderer);

}

void Prop::render(Renderer &renderer, const Point &origin) {

  super::render(renderer, origin);
  
  _name.render(renderer, origin);
  
}
