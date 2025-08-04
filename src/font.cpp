/*
  font.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 4-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dead
*/

#include "font.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#define DEFAULT_PTSIZE  18.0f

Font::~Font() {

  if (_font) {
    TTF_CloseFont(_font);
  }

}

bool Font::init(const char *path) {

  _font = TTF_OpenFont(path, DEFAULT_PTSIZE);
  if (!_font) {
    SDL_Log("Couldn't load %g pt font from %s: %s", DEFAULT_PTSIZE, path, SDL_GetError());
    return false;
  }
  
  TTF_SetFontStyle(_font, TTF_STYLE_NORMAL);
  TTF_SetFontOutline(_font, 0);
  TTF_SetFontKerning(_font, 1);
  TTF_SetFontWrapAlignment(_font, TTF_HORIZONTAL_ALIGN_LEFT);
 
  return true;
   
}
