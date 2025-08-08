/*
  box.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Aug-2025
    
  Box class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_box
#define H_box

class Renderer;
class Font;

class Box {

public:
  virtual ~Box()  {};
  
  virtual void render(Renderer &renderer, Font &font, float x, float *y) = 0;

};

#endif // H_box
