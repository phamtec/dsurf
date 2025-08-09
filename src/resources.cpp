/*
  resources.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "resources.hpp"

void Resources::build(Renderer &renderer, Font &font) {

  open_brace.build(renderer, font);
  close_brace.build(renderer, font);
  open_bracket.build(renderer, font);
  close_bracket.build(renderer, font);
  
}
