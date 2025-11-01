/*
  text.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 4-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "text.hpp"
#include "font.hpp"
#include "core.hpp"
#include "spatial.hpp"
#include "unicode.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

using namespace std;

Text::Text(const std::wstring &str, const SDL_Color &fgcolor, std::optional<SDL_Color> bgcolor) {
  
  set(str, fgcolor, bgcolor);

}

void Text::set(const std::wstring &str, const SDL_Color &fgcolor, std::optional<SDL_Color> bgcolor) {
  _str = str;
  _fgcolor = fgcolor;
  if (bgcolor) {
    _bgcolor = *bgcolor;
  }
}

Text::~Text() {
  if (_texture) {
    SDL_DestroyTexture(_texture);
  }
}

void Text::build(Core &core) {

  if (_texture) {
    SDL_DestroyTexture(_texture);
    _texture = 0;
  }

  // create a surface from the text. We don't have
  // to keep it around after making a texture from it.
  unique_ptr<SDL_Surface> surface(core.renderText(_str, _fgcolor, _bgcolor));
  if (!surface) {
    return;
  }
  // remember the size.
  _size = Size(surface->w, surface->h);
  
  // create a texture.
  _texture = core.createTexture(surface.get());
}

void Text::render(Core &core, const Point &origin, bool offs) {

  if (!_texture) {
    SDL_Log("need to build %s first!", Unicode::convert(_str).c_str());
    return;
  }
  
  Rect r(origin, _size);
  if (offs && core.textTooSmall()) {
    r -= 4;
    core.renderFilledRect(r, _fgcolor);
  }
  else {
    core.renderTexture(_texture, r, offs);
  }
  
}
