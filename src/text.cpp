/*
  text.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 4-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "text.hpp"
#include "font.hpp"
#include "renderer.hpp"
#include "spatial.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

using namespace std;

Text::~Text() {
  if (_texture) {
    SDL_DestroyTexture(_texture);
  }
}

void Text::build(Renderer &renderer) {

  if (_texture) {
    SDL_DestroyTexture(_texture);
    _texture = 0;
  }

  // create a surface from the text. We don't have
  // to keep it around after making a texture from it.
  unique_ptr<SDL_Surface> surface(renderer.renderText(_str, _fgcolor));
  if (!surface) {
    return;
  }
  // remember the size.
  _size = Size(surface->w, surface->h);
  
  // create a texture.
  _texture = renderer.createTexture(surface.get());
}

void Text::render(Renderer &renderer, const Point &origin, bool offs) {

  if (!_texture) {
    SDL_Log("need to build first!");
    return;
  }
  
  Rect r(origin, _size);
  if (!offs && renderer.textTooSmall()) {
    r -= 4;
    renderer.renderFilledRect(r, _fgcolor);
  }
  else {
    renderer.renderTexture(_texture, r, offs);
  }
  
}
