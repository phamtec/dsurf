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
class SDL_FRect;
class SDL_Texture;
class SDL_Surface;

class Renderer {

public:
  Renderer();
  ~Renderer();
  
  bool init();
  void prepare();
  void present();
  bool processEvents();

  SDL_Texture *createTexture(SDL_Surface *surface);
  void renderTexture(SDL_Texture *texture, const SDL_FRect &rect);
  void renderRect(const SDL_FRect &rect);
  
private:
  
  int _width, _height;
  SDL_Window *_window;
  TTF_TextEngine *_engine;
  SDL_Renderer *_renderer;
  float _scale;
  bool _mousedown;
  float _lastx;
  float _lasty;
  float _offx;
  float _offy;
};

#endif // H_renderer
