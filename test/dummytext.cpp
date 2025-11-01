/*
  dummytext.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 16-Oct-2025
    
  We may need to make a simple non-drawing text so that we can test with.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/


#include "text.hpp"

Text::Text(const std::wstring &str, const SDL_Color &fgcolor, std::optional<SDL_Color> bgcolor) {
  set(str, fgcolor, bgcolor);
}
void Text::set(const std::wstring &str, const SDL_Color &fgcolor, std::optional<SDL_Color> bgcolor) {
  _str = str;
}
Text::~Text() {
}
void Text::build(Core &core) {
}
void Text::render(Core &core, const Point &origin, bool offs) {
}
