/*
  prop.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "prop.hpp"
#include "spatial.hpp"

void Prop::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  _name.build(renderer, font);

}

void Prop::render(Renderer &renderer, Resources &res) {

  super::render(renderer, res);
  
  _name.render(renderer, Spatial::origin(_r));
  
}
