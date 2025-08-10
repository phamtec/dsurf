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

class SDL_Surface;
class SDL_Texture;

class Renderer;
class Font;
class Point;

class Text {

public:
  Text(): _str("??"), _fgcolor(Colours::black), _surface(0), _texture(0) {}
  Text(const std::string &str, const SDL_Color &fgcolor): _str(str), _fgcolor(fgcolor), _surface(0), _texture(0) {}
  ~Text();

  void set(const std::string &str, const SDL_Color &fgcolor) {
    _str = str;
    _fgcolor = fgcolor;
  }
  
  virtual void build(Renderer &renderer, Font &font);
    // build the texture and surface ready to render.
    
  virtual void render(Renderer &renderer, const Point &origin);
    // render the text after first initialising it at x, y
    
  Size size();
    // return the size of the text object.
      
private:

  std::string _str;
  SDL_Color _fgcolor;
  SDL_Surface *_surface;
  SDL_Texture *_texture;
  
};

#endif // H_text
