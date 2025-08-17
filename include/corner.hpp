/*
  corner.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 17-Aug-2025
    
  Corner class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_corner
#define H_corner

class Renderer;
class Point;
class SDL_Texture;

class Corner {

public:
  Corner(bool topleft): _topleft(topleft) {}
  ~Corner();
  
  virtual void build(Renderer &renderer);
    // build the texture ready to render.
    
  virtual void render(Renderer &renderer, const Point &origin);
    // render the corner at x, y
    
private:
  bool _topleft;
  SDL_Texture *_texture;
  
};

#endif // H_corner
