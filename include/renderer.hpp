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

#include "point.hpp"
#include "size.hpp"

#include <SDL3/SDL_pixels.h>

class SDL_Window;
class SDL_Renderer;
class TTF_TextEngine;
class SDL_Texture;
class SDL_Surface;
class Rect;

class Renderer {

public:
  Renderer();
  ~Renderer();
  
  bool init();
  void prepare();
  void present();
  bool processEvents();

  SDL_Texture *createTexture(SDL_Surface *surface);
  void renderTexture(SDL_Texture *texture, const Rect &rect);
  void renderRect(const Rect &rect);
  void renderFilledRect(const Rect &rect, const SDL_Color &color);
  
  void center(const Size &size);
  Point rootPoint();
  bool textTooSmall(const Rect &rect);
  
private:
  
  Size _size;
  SDL_Window *_window;
  TTF_TextEngine *_engine;
  SDL_Renderer *_renderer;
  float _scale;
  bool _mousedown;
  Point _last;
  Size _offs;
  Point _mouse;
  Size _osize;
  
  void debugOffs();
  void debugScale();
  void debugMouse(const Point &p);
  void debugSize();
  
};

#endif // H_renderer
