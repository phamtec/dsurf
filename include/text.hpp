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

#include "box.hpp"

#include <string>

class SDL_Surface;
class SDL_Texture;

class Text: public Box {

public:
  Text(const std::string &str): _str(str), _surface(0), _texture(0) {}
  ~Text();
  
  // Box
  virtual void render(Renderer &renderer, Font &font, float x, float *y);
  
private:

  std::string _str;
  SDL_Surface *_surface;
  SDL_Texture *_texture;
  
};

#endif // H_text
