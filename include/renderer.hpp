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
#include "box.hpp"
#include "font.hpp"
#include "resources.hpp"
#include "texteditor.hpp"

#include <SDL3/SDL_pixels.h>

class SDL_Window;
class SDL_Renderer;
class TTF_TextEngine;
class SDL_Texture;
class SDL_Surface;
class Rect;
class SDL_Cursor;

class Renderer {

public:
  Renderer(const Size &wsize, float scalemult, float scale, const Size &offset, bool editing): 
    _size(wsize), _scalemult(scalemult), _scale(scale), _offs(offset),
    _mousedown(false), _lastclick(0),
    _window(0), _renderer(0), _engine(0), _editing(editing),
    _pointercursor(0), _editcursor(0)
      {};
  ~Renderer();
  
  static Size displaySize();
  
  void setRoot(Box *box);
    // switch out the root we will use to render.
    
  bool init(const char *fontpath);
    // initialise with th epath to the font file.
    
  void loop();
    // the main render loop.
    
  // functions used to create and render.
  SDL_Texture *createTexture(int width, int height);
  SDL_Surface *renderText(const char *str, const SDL_Color &color);
  void setTarget(SDL_Texture *texture);
  SDL_Texture *createTexture(SDL_Surface *surface);
  void renderTexture(SDL_Texture *texture, const Rect &rect);
  void renderRect(const Rect &rect);
  void renderFilledRect(const Rect &rect, const SDL_Color &color);
  void renderFilledPie(const Point &origin, int radius, int start, int end, const SDL_Color &color);

  bool textTooSmall(const Rect &rect);
    // if the text is too small for the rectangle, return true.
  
  Resources resources;
  
private:
  friend class TextEditor;
  
  Size _size;
  SDL_Window *_window;
  TTF_TextEngine *_engine;
  SDL_Renderer *_renderer;
  float _scale;
  float _scalemult;
  bool _mousedown;
  Point _last;
  Size _offs;
  Point _mouse;
  Size _osize;
  unsigned int _lastclick;
  std::unique_ptr<Box> _root;
  std::unique_ptr<Font> _font;
  std::unique_ptr<TextEditor> _editor;
  bool _editing;
  SDL_Cursor *_pointercursor;
  SDL_Cursor *_editcursor;
  Point _renderorigin;
  
  bool processEvents();
  bool isDoubleClick();

  void debugOffs();
  void debugScale();
  void debugMouse(const Point &p);
  void debugSize();
  
};

#endif // H_renderer
