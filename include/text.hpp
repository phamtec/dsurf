/*
  text.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 4-Aug-2025
    
  Text class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_text
#define H_text

#include "colours.hpp"
#include "size.hpp"

#include <string>
#include <SDL3/SDL_pixels.h>

class SDL_Texture;

class Renderer;
class Font;
class Point;

class Text {

public:
  Text(): _str(L"??"), _fgcolor(Colours::black), _texture(0) {}
  Text(const std::wstring &str, const SDL_Color &fgcolor, const SDL_Color &bgcolor=Colours::white): 
    _str(str), _fgcolor(fgcolor), _bgcolor(bgcolor), _texture(0) {}
  ~Text();

  void set(const std::wstring &str, const SDL_Color &fgcolor) {
    _str = str;
    _fgcolor = fgcolor;
  }
  
  void build(Renderer &renderer);
    // build the texture ready to render.
    
  void render(Renderer &renderer, const Point &origin, bool offs=true);
    // render the text at x, y
    
  Size size() { return _size; }
    // return the size of the text object.
      
  std::wstring str() { return _str; }
    
private:

  std::wstring _str;
  SDL_Color _fgcolor;
  SDL_Color _bgcolor;
  Size _size;
  SDL_Texture *_texture;
  
};

#endif // H_text
