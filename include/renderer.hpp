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
#include "changes.hpp"
#include "flo.hpp"

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_events.h>
#include <memory>
#include <zmq.hpp>
#include <rfl.hpp>

class SDL_Window;
class SDL_Renderer;
class TTF_TextEngine;
class SDL_Texture;
class SDL_Surface;
class Rect;
class SDL_Cursor;
class Commandable;
class Flo;

class Renderer {

public:
  Renderer(const Size &wsize, float scalemult, float scale, const Size &offset, bool editing): 
    _size(wsize), _scalemult(scalemult), _scale(scale), _offs(offset),
    _mousedown(false), _lastclick(0), _moving(0), _hudmoving(-1), _hudnone(-1), 
    _adding(false), _hudadding(-1),
    _window(0), _renderer(0), _engine(0), _startedit(editing)/*,
    _waitingonline(false), _online(false)*/
      {};
  ~Renderer();
  
  static Size displaySize();
  
  void addRoot(Element *element);
    // add a root.
    
  void addFile(const std::string &filename, bool raw);
    // add the file
    // if raw is specified, modules are bypassed.

  bool init(const std::string &fontpath);
    // initialise with th epath to the font file.
    
  void initElement(Element *parent, Element *element);
    // do all the work that would initialise an element.
    
  void initTypes();
    // specifically initialise types.
    
  void loop(int rep);
    // the main render loop.
    
  bool processGlobalKey(SDL_Keycode code);
    // all objects should do this.
    
  bool processRootKey(Element *element, SDL_Keycode code);
  static void registerRootHUDMode(HUDMode *mode);
  static void registerGlobalHUDMode(HUDMode *mode);
    // process keys and HUD for the root object.
   
  void setTextState();
  void processTextKey(Element *editable, const Point &origin, const Size &size, SDL_Keycode code);
  static void registerTextHUDMode(HUDMode *mode);
    // set the state for the hud when we are over text
    
  void processDeleteKey(Element *element);

  void copy(Element *element);
    // copy the element onto the clipboard

  Element *getClipboard();
    // get the contents of the clipboard as an element.
    
  void write(Element *element);
    // write back the contents of the root attached to this element.

  // undo system
  void exec(Element *element, Change *change);
  void undo(Element *element);
  void redo(Element *element);

  // dealing with textures.
  SDL_Texture *createTexture(int width, int height);
  void setTarget(SDL_Texture *texture);
  SDL_Texture *createTexture(SDL_Surface *surface);
  void destroyTexture(SDL_Texture *texture);
  
  // functions used to create and render.  
  // these all take into account the offset and scqle.
  SDL_Surface *renderText(const std::wstring &str, const SDL_Color &color, const SDL_Color &bgcolor=Colours::white);
  void renderTexture(SDL_Texture *texture, const Rect &rect, bool offs=true);
  void renderRect(const Rect &rect);
  void renderFilledRect(const Rect &rect, const SDL_Color &color);
  void renderFilledPie(const Point &origin, int radius, int start, int end, const SDL_Color &color);

  bool textTooSmall();
    // if the text is too small for the rectangle, return true.
  
  Point localToGlobal(const Point &p);
    // convert the point to global coors (apply offset and scale)
  
  Point noOffset(const Point &p);
    // remove the offset from the point.
    
  // these are for raw drawing. No offset and scqle.
  void setDrawColor(const SDL_Color &color);
  void setScale(double x, double y);
  void drawRect(const Rect &r);
  void fillRect(const Rect &r);

  void clearScale();
  void restoreScale();
  
  void setError(const std::string &str);
    // show an error.
    
  void setDirty(Element *elem, bool state=true);
    // set the dirty flag for that element.
    
  Resources resources;
  
  // remote server.
  bool setupRemote(const std::string &server, int req, 
    const std::string &upstreamPubKey, const std::string &privateKey, const std::string &pubKey);
  void startRemote(std::shared_ptr<Flo> &flo, const rfl::Object<rfl::Generic> &msg, const rfl::Object<rfl::Generic> &next);
  void sendRemote(const rfl::Object<rfl::Generic> &msg);
  void evalMsg(const rfl::Generic &msg);
  void msgError(const std::string &err);
  
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
  std::vector<std::unique_ptr<Element> > _roots;
  std::unique_ptr<Font> _font;
  std::unique_ptr<TextEditor> _editor;
  std::unique_ptr<HUD> _hud;
  std::unique_ptr<Text> _error;
//   SDL_Cursor *_pointercursor;
//   SDL_Cursor *_editcursor;
  Point _renderorigin;
  Changes _changes;
  Element *_moving;
  int _hudmoving;
  int _hudnone;
  Point _movoffs;
  bool _adding;
  int _hudadding;
  rfl::Object<rfl::Generic> _next;
  std::shared_ptr<Flo> _flo;
  
  bool processEvents();
  bool isDoubleClick();
  void endEdit();
  void setHUD();
  void destroyRoots();
  Point addRootOrigin(Element *element, const Point &origin);
  void removeRoot(Element *element);
  void recenter();
  void paste();
  void zoom(int scale);
  
  typedef std::tuple<Commandable *, Element *, bool, Text *> getHitReturnType;
    // the commandable interface and the raw element
    // bool is whether it's the root, 
    // and the root text object at the end.
  std::optional<getHitReturnType> getHit();


  void debugOffs();
  void debugScale();
  void debugMouse(const Point &p);
  void debugSize();
        
  // testing via ZMQ
  std::unique_ptr<zmq::context_t> _context;
  std::unique_ptr<zmq::socket_t> _rep;

  struct TestMsg {
    std::string type;
    std::optional<std::string> target;
    std::optional<std::string> payload;
  };

  void setupTest(int rep);
  void processMsg();
  Element *getTestTarget(const std::optional<std::string> &name, bool silent=false);
  void testAck();
  void testErr(const std::string &msg);
  void testSend(const TestMsg &reply);
  void handleTestMsg();
  void handleTestKey(const TestMsg &msg);
  void handleTestCount(const TestMsg &msg);

  // handling remove ZMQ requests to encrypted servers.
  std::unique_ptr<zmq::socket_t> _remotereq;
  
};

#endif // H_renderer
