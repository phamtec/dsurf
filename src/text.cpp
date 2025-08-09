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

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

using namespace std;

Text::~Text() {
  if (_surface) {
    SDL_DestroySurface(_surface);
  }
  if (_texture) {
    SDL_DestroyTexture(_texture);
  }
}

void Text::build(Renderer &renderer, Font &font) {

  SDL_Color white = { 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE };

  _surface = TTF_RenderText_Shaded(font._font, _str.c_str(), 0, _fgcolor, white);
  if (!_surface) {
    SDL_Log("could not create surface");
    return;
  }
  
  _texture = SDL_CreateTextureFromSurface(renderer._renderer, _surface);
  if (!_texture) {
    SDL_Log("Couldn't render text: %s", SDL_GetError());
    return;
  }

}

void Text::render(Renderer &renderer, float x, float y) {

  if (!_surface) {
    SDL_Log("need to build first!");
    return;
  }
  
  SDL_FRect messageRect;
  messageRect.x = x;
  messageRect.y = y;
  messageRect.w = _surface->w;
  messageRect.h = _surface->h;

  SDL_RenderTexture(renderer._renderer, _texture, NULL, &messageRect);
  
}

float Text::width() {

  if (!_surface) {
    SDL_Log("need to build first!");
    return 0;
  }
  return _surface->w;
  
}

float Text::height() {

  if (!_surface) {
    SDL_Log("need to build first!");
    return 0;
  }
  return _surface->h;

}
