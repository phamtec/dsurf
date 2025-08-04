/*
  text.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 4-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dead
*/

#include "text.hpp"
#include "font.hpp"
#include "renderer.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

Text::~Text() {
  if (_surface) {
    SDL_DestroySurface(_surface);
  }
  if (_texture) {
    SDL_DestroyTexture(_texture);
  }
}

bool Text::init(Renderer &renderer, Font &font, const char *message) {

  SDL_Color white = { 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE };
  SDL_Color black = { 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE };

  _surface = TTF_RenderText_Shaded(font._font, message, 0, black, white);
  if (!_surface) {
    return false;
  }
  
  _texture = SDL_CreateTextureFromSurface(renderer._renderer, _surface);
  if (!_texture) {
    SDL_Log("Couldn't render text: %s", SDL_GetError());
    return false;
  }

  return true;
  
}

void Text::render(Renderer &renderer, int x, int y) {

  SDL_FRect messageRect;
  messageRect.x = (float)x;
  messageRect.y = (float)y;
  messageRect.w = (float)_surface->w;
  messageRect.h = (float)_surface->h;

  SDL_RenderTexture(renderer._renderer, _texture, NULL, &messageRect);
  
}