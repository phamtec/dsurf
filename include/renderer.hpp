/*
  renderer.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 4-Aug-2025
    
  Renderer class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_renderer
#define H_renderer

class SDL_Window;
class SDL_Renderer;
class TTF_TextEngine;

class Renderer {

public:
  Renderer();
  ~Renderer();
  
  bool init();
  void prepare();
  void present();
  bool processEvents();

private:
  friend class Text;
  
  int _width, _height;
  SDL_Window *_window;
  TTF_TextEngine *_engine;
  SDL_Renderer *_renderer;
  
};

#endif // H_renderer
