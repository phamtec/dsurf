/*
  corner.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 17-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "corner.hpp"
#include "sizes.hpp"
#include "core.hpp"
#include "colours.hpp"

#include <SDL3/SDL.h>

Corner::~Corner() {
  if (_texture) {
    SDL_DestroyTexture(_texture);
  }
}

void Corner::build(Core &core) {

  // over sample it 4 times.
  int size = Sizes::thickness * 4;
  
  // create texture and set it as the render target.
  _texture = core.createTexture(size, size);
  core.setTarget(_texture);
  
  // draw...
  if (_topleft) {
    core.renderFilledPie(Point(size, size), size, 180, 270, Colours::dictE);
  }
  else {
    core.renderFilledPie(Point(size, 0), size, 90, 180, Colours::dictE);
  }
  
  core.setTarget(0);
  
}
    
void Corner::render(Core &core, const Point &origin) {

  // just copy in our texture.
  core.renderTexture(_texture, Rect(origin, Size(Sizes::thickness, Sizes::thickness)));
  
}
