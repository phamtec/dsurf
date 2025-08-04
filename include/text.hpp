/*
  text.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 4-Aug-2025
    
  Text class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dead
*/

#ifndef H_text
#define H_text

class SDL_Surface;
class SDL_Texture;

class Renderer;
class Font;

class Text {

public:
  Text(int width, int height): _width(width), _height(height), _surface(0), _texture(0) {}
  ~Text();
  
  bool init(Renderer &renderer, Font &font, const char *message);
  void render(Renderer &renderer, int x, int y);

private:

  int _width, _height;
  SDL_Surface *_surface;
  SDL_Texture *_texture;
  
};

#endif // H_text
