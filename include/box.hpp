/*
  box.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Aug-2025
    
  Box class.
  
  Box is the baseclass of all drawn objects.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_box
#define H_box

class Renderer;
class Font;
class Resources;

class Box {

public:
  Box(): _x(0.0f), _y(0.0f) {};
  virtual ~Box() {};
  
  float _x, _y;
  float _width, _height;
  
  virtual void build(Renderer &renderer, Font &font) = 0;
    // build the texture and surface ready to render.

  virtual float layout(Resources &pool, float x, float y) = 0;
    // layout the object.

  virtual void render(Renderer &renderer, Resources &pool) = 0;
    // render this object to the screen.
 
};

#endif // H_box
