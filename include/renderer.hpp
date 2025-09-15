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
#include "element.hpp"
#include "font.hpp"
#include "resources.hpp"
#include "texteditor.hpp"
#include "hud.hpp"

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_events.h>
#include <memory>

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
    _window(0), _renderer(0), _engine(0), _startedit(editing)/*,
    _pointercursor(0), _editcursor(0)*/
      {};
  ~Renderer();
  
  static Size displaySize();
  
  void setRoot(Element *element);
    // switch out the root we will use to render.
    
  bool init(const char *fontpath);
    // initialise with th epath to the font file.
    
  void initTypes();
    // specifically initialise types.
    
  void loop();
    // the main render loop.
    
  bool processRootKey(Element *element, SDL_Keycode code);
    // process keys and HUD for the root object.
   
  void setTextState();
  void processTextKey(Editable *editable, const Point &origin, const Size &size, SDL_Keycode code);
    // set the state for the hud when we are over text
    
  void copy(Element *element);
    // coy the element onto the clipboard

  // dealing with textures.
  SDL_Texture *createTexture(int width, int height);
  void setTarget(SDL_Texture *texture);
  SDL_Texture *createTexture(SDL_Surface *surface);
  void destroyTexture(SDL_Texture *texture);
  
  // functions used to create and render.  
  // these all take into account the offset and scqle.
  SDL_Surface *renderText(const std::wstring &str, const SDL_Color &color);
  void renderTexture(SDL_Texture *texture, const Rect &rect, bool offs=true);
  void renderRect(const Rect &rect);
  void renderFilledRect(const Rect &rect, const SDL_Color &color);
  void renderFilledPie(const Point &origin, int radius, int start, int end, const SDL_Color &color);

  bool textTooSmall(const Rect &rect);
    // if the text is too small for the rectangle, return true.
  
  Point localToGlobal(const Point &p);
    // convert the point to global coors (apply offset and scale)
  
  // these are for raw drawing. No offset and scqle.
  void setDrawColor(const SDL_Color &color);
  void setScale(double x, double y);
  void drawRect(const Rect &r);
  void fillRect(const Rect &r);

  void clearScale();
  void restoreScale();
  
  Resources resources;
  
private:
  friend class TextEditor;

  Size _size;
  float _scalemult;
  float _scale;
  Size _offs;
  bool _startedit;
  
  float _oldscale;
  Size _oldoffs;
  
  SDL_Window *_window;
  TTF_TextEngine *_engine;
  SDL_Renderer *_renderer;
  bool _mousedown;
  Point _last;
  Point _mouse;
  Size _osize;
  unsigned int _lastclick;
  std::unique_ptr<Element> _root;
  std::unique_ptr<Font> _font;
  std::unique_ptr<TextEditor> _editor;
  std::unique_ptr<HUD> _hud;
//   SDL_Cursor *_pointercursor;
//   SDL_Cursor *_editcursor;
  Point _renderorigin;
  
  bool processEvents();
  bool isDoubleClick();
  void endEdit();
  void setHUD();

  void debugOffs();
  void debugScale();
  void debugMouse(const Point &p);
  void debugSize();
      
};

#endif // H_renderer
