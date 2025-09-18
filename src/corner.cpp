/*
  corner.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 17-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "corner.hpp"
#include "sizes.hpp"
#include "renderer.hpp"
#include "colours.hpp"

#include <SDL3/SDL.h>

Corner::~Corner() {
  if (_texture) {
    SDL_DestroyTexture(_texture);
  }
}

void Corner::build(Renderer &renderer) {

  // over sample it 4 times.
  int size = Sizes::thickness * 4;
  
  // create texture and set it as the render target.
  _texture = renderer.createTexture(size, size);
  renderer.setTarget(_texture);
  
  // draw...
  if (_topleft) {
    renderer.renderFilledPie(Point(size, size), size, 180, 270, Colours::dictE);
  }
  else {
    renderer.renderFilledPie(Point(size, 0), size, 90, 180, Colours::dictE);
  }
  
  renderer.setTarget(0);
  
}
    
void Corner::render(Renderer &renderer, const Point &origin) {

  // just copy in our texture.
  renderer.renderTexture(_texture, Rect(origin, Size(Sizes::thickness, Sizes::thickness)));
  
}
