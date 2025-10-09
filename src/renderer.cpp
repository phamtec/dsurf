/*
  renderer.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 4-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "renderer.hpp"

#include "spatial.hpp"
#include "builder.hpp"
#include "colours.hpp"
#include "dict.hpp"
#include "gfx.hpp"
#include "editable.hpp"
#include "property.hpp"
#include "removefromlist.hpp"
#include "unicode.hpp"
#include "sizes.hpp"
#include "root.hpp"
#include "list.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>
#include <sstream>

using namespace std;

#define DOUBLE_CLICK_TIME 300

Renderer::~Renderer() {

  // allow objects to cleanup.
  destroyRoots();

  _changes.destroy(*this);
  
  // font first.
  _font.reset(0);
//   if (_pointercursor) {
//     SDL_DestroyCursor(_pointercursor);
//   }
//   if (_editcursor) {
//     SDL_DestroyCursor(_editcursor);
//   }

  if (_engine) {
    TTF_DestroyRendererTextEngine(_engine);
  }
  if (_renderer) {
    SDL_DestroyRenderer(_renderer);
  }
  if (_window) {
    SDL_DestroyWindow(_window);
  }
  TTF_Quit();
  SDL_Quit();

}

void Renderer::destroyRoots() {

  for_each(_roots.begin(), _roots.end(), [this](auto& e) { e->destroy(*this); });

}

Size Renderer::displaySize() {

  if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL Video: %s", SDL_GetError());
    return Size(0, 0);
  }

  int n;
  SDL_DisplayID *displays = SDL_GetDisplays(&n);
  if (n < 1) {
    SDL_Log("Not enough displays");
    return Size(0, 0);
  }

  const SDL_DisplayMode *dm = SDL_GetCurrentDisplayMode(displays[0]);
  if (!dm) {
    SDL_Log("Couldn't get display mode: %s", SDL_GetError());    
    return Size(0, 0);
  }
  
  return Size(dm->w, dm->h);

}

bool Renderer::init(const string &path) {

   /* Initialize the TTF library */
  if (!TTF_Init()) {
    SDL_Log("Couldn't initialize TTF: %s",SDL_GetError());
    return false;
  }

  /* Create a window */
  _window = SDL_CreateWindow("dsurf", _size.w, _size.h, SDL_WINDOW_RESIZABLE);
  if (!_window) {
    SDL_Log("SDL_CreateWindow() failed: %s", SDL_GetError());
    return false;
  }
  
  _renderer = SDL_CreateRenderer(_window, NULL);
  if (!_renderer) {
    SDL_Log("SDL_CreateRenderer() failed: %s", SDL_GetError());
    return false;
  }

  SDL_SetRenderVSync(_renderer, 1);

  _engine = TTF_CreateRendererTextEngine(_renderer);
  if (!_engine) {
    SDL_Log("Couldn't create renderer text engine: %s", SDL_GetError());
    return false;
  }

  _font = unique_ptr<Font>(new Font());
  if (!_font->init(path.c_str())) {
    SDL_Log("Couldn't iniitalise font");
    return false;
  }
  
  // allow shared resources to build.
  resources.build(*this);
  
  // build the HUD
  _hud.reset(new HUD());

  // build our editor.
  _editor.reset(new TextEditor(_startedit));
  _editor->build(*this);
  _editor->layout();
  
  // add all the register HUD modes.
  _editor->registerHUD(_hud.get());
  
  {
    auto mode = new HUDMode(false);
    mode->add(new Shortcut(L"Esc", L"(finish)"));
    mode->add(new Shortcut(L"D", L"(rop)"));
    _hudmoving = _hud->registerMode("moving", mode);
  }
  
  {
    auto mode = new HUDMode(false);    
    mode->add(new Shortcut(L"N", L"ew"));
    mode->add(new Shortcut(L"P", L"aste"));
    _hudnone = _hud->registerMode("moving", mode);
  }
  
  {
    auto mode = new HUDMode(false);
    mode->add(new Shortcut(L"Esc", L"(finish)"));
    mode->add(new Shortcut(L"D", L"ict"));
    mode->add(new Shortcut(L"L", L"ist"));
    _hudadding = _hud->registerMode("adding", mode);
  }
  
  // initialisation for types after everything has been created.
  initTypes();
  
  // build the hud with all modes
  _hud->build(*this);

  // make sure the hud flags are set correctly.
  _changes.setUndoFlags(*this, _hud.get());
  
  // other flags we have.
  _hud->setFlag(*this, canWrite, false);
  
//   _pointercursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
//   _editcursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT);
  
  return true;
   
}

void Renderer::addFile(const string &filename, bool raw) {

  auto obj = Builder::loadFile(filename, raw);
  if (!obj) {
    cerr << "file '" << filename << "' not found." << endl;
    return;
  }
  
  addRoot(obj);
  
}

void Renderer::addRoot(Element *element) {

  // if there is only 1 element and it is <new> with an empty dict
  // then we replace it.
  if (_roots.size() == 1) {
    auto root = dynamic_cast<Root *>(_roots[0].get());
    if (root) {
      if (root->getFilename() == "<new>") {
        auto dict = dynamic_cast<Dict *>(root->getObj());
        if (dict && Listable::cast(dict)->count() == 0) {
          destroyRoots();
          _roots.clear();
        }
      }
    }
  }

  // setup the HUD in the object.
  Commandable *cx = dynamic_cast<Commandable *>(element);
  if (cx) {
    cx->initHUD(_hud.get());
  }

  // build all objects with this renderer.
  element->build(*this);
  
  // lay it out.
  element->layout();
  
  // find the right most object.
  Point rightmost;
  for_each(_roots.begin(), _roots.end(), [&rightmost](auto& e) {
    auto l = Locatable::cast(e.get())->getLocation() + e->size();
    if (l.x > rightmost.x) {
      rightmost.x = l.x;
    }
  });
  
  // and put this to the right of that.
  Locatable::cast(element)->setLocation(rightmost + Size(Sizes::group_indent, 0));

  // remember it.
  _roots.push_back(unique_ptr<Element>(element));
  
  
  // calculate the total size of all the objects.
  recenter();

}

void Renderer::recenter() {

  // find the widest and tallest.
  _osize = Size();
  for_each(_roots.begin(), _roots.end(), [this](auto& e) {
    Point o = Locatable::cast(e.get())->getLocation() + e->size();
    if (o.x > _osize.w) {
      _osize.w = o.x;
    }
    if (o.y > _osize.h) {
      _osize.h = o.y;
    }
  });

  _offs = Spatial::center(_size, _osize, _scale);
//  cout << _offs << endl;

}

void Renderer::removeRoot(Element *element) {

  auto r = find_if(_roots.begin(), _roots.end(), [element](auto& e) {
    return e.get() == element;
  });
  if (r == _roots.end()) {
    cerr << "couldnt find root." << endl;
    return;
  }

  (*r)->destroy(*this);
  _roots.erase(r);
  recenter();
  
}


void Renderer::initElement(Element *parent, Element *element) {

  element->setParent(parent);
  element->build(*this);
  Commandable::cast(element)->initHUD(_hud.get());
  
}

void Renderer::loop(int rep) {

  setupTest(rep);

  bool done = false;
  while (!done) {
  
    processTestMsg();

    // handle all the events.
    if (processEvents()) {
      done = true;
    }

    SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(_renderer);
    
    // set the scale ready to do our calculations.
    SDL_SetRenderScale(_renderer, _scale, _scale);

    // space out all the roots.
    for_each(_roots.begin(), _roots.end(), [this](auto& e) {
      auto loc = Locatable::cast(e.get())->getLocation();
      if (e.get() == _moving) {
        loc = loc + ((_mouse - _movoffs) / _scale);
      }
      e->render(*this, loc);
    });

    _editor->render(*this, Point(0.0, 0.0));

    if (_error) {
      _error->render(*this, Point(0.0, 0.0), false);
    }
  
    // set the scale back to 1.0 so that our draw will work.
    SDL_SetRenderScale(_renderer, 1.0, 1.0);
  
//     debugOffs();
//     debugScale();
//     debugMouse(_mouse);
//     debugSize();
    
    _hud->render(*this, _mouse);
    
    SDL_RenderPresent(_renderer);

  }
  
}

void Renderer::setDrawColor(const SDL_Color &color) {

  SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
  
}

void Renderer::drawRect(const Rect &r) {

  SDL_FRect sr = r.srect();
  SDL_RenderRect(_renderer, &sr);
  
}

void Renderer::setScale(double x, double y) {

  SDL_SetRenderScale(_renderer, x, y);

}

void Renderer::fillRect(const Rect &r) {

  SDL_FRect sr = r.srect();
  SDL_RenderFillRect(_renderer, &sr);
  
}

void Renderer::debugOffs() {

  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);
  stringstream ss;
  ss << "offs: " << _offs << endl;
  SDL_RenderDebugText(_renderer, _size.w - 300, 10, ss.str().c_str());

}

void Renderer::debugScale() {

  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);
  stringstream ss;
  ss << "scale: " << _scale << endl;
  SDL_RenderDebugText(_renderer, _size.w - 300, 30, ss.str().c_str());

}

void Renderer::debugMouse(const Point &p) {

  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);
  stringstream ss;
  ss << "mouse: " << p.x << ", " << p.y << endl;
  SDL_RenderDebugText(_renderer, _size.w - 300, 50, ss.str().c_str());

}

void Renderer::debugSize() {

  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);
  stringstream ss;
  ss << "size: " << _size << endl;
  SDL_RenderDebugText(_renderer, _size.w - 300, 70, ss.str().c_str());

}

bool Renderer::isDoubleClick() {

  auto ticks = SDL_GetTicks();
  bool dbl = (ticks - _lastclick) < DOUBLE_CLICK_TIME;
  _lastclick = ticks;
  return dbl;
  
}

optional<Renderer::getHitReturnType> Renderer::getHit() {

  for (auto& i: _roots) {

    auto loc = Locatable::cast(i.get())->getLocation();
    
    Element *hit = i->hitTest(Point(_offs) + loc, _mouse * (1 / _scale));
    if (hit) {
      Commandable *cx = dynamic_cast<Commandable *>(hit);
      if (cx) {
        auto root = dynamic_cast<Root *>(i.get());
        if (root) {
          return getHitReturnType(cx, hit, i.get() == hit, root->getFilenameObj());
        }
        return getHitReturnType(cx, hit, i.get() == hit, nullptr);
      }
    }
  }
  return nullopt;
}

void Renderer::setHUD() {

  if (_moving) {
    _hud->setMode(_hudmoving);
    return;
  }
  
  if (_adding) {
    _hud->setMode(_hudadding);
    return;
  }

  auto hit = getHit();
  if (hit) {
  
    // if we are very small and the elem hit IS the root then use the name 
    if (textTooSmall() && get<2>(*hit)) {
      _hud->setHint(*this, get<3>(*hit));
      return;
    }
    
    _hud->setEditingLoc(localToGlobal(addRootOrigin(get<1>(*hit), get<1>(*hit)->origin())));

    get<0>(*hit)->setMode(*this, _hud.get());
    
    return;
  }
  
  _hud->setMode(_hudnone);
}

void Renderer::endEdit() {

  setHUD();

}

void Renderer::copy(Element *element) {
  SDL_SetClipboardText(Builder::getJson(element).c_str());
}

void Renderer::registerGlobalHUDMode(HUDMode *mode) {

  mode->add(new Shortcut(L"[1-9]", L"Zoom"));
  
}

void Renderer::registerRootHUDMode(HUDMode *mode) {

  registerGlobalHUDMode(mode);
  mode->add(new Shortcut(L"U", L"ndo", canUndo));
  mode->add(new Shortcut(L"R", L"edo", canRedo));
  mode->add(new Shortcut(L"M", L"ove"));
  mode->add(new Shortcut(L"K", L"ill"));
  mode->add(new Shortcut(L"W", L"rite", canWrite));
  mode->add(new Shortcut(L"C", L"opy"));
  mode->add(new Shortcut(L"P", L"aste"));
  
}

void Renderer::registerTextHUDMode(HUDMode *mode) {

  mode->add(new Shortcut(L"D", L"elete"));
  
}

Element *Renderer::getClipboard() {

  char *text = SDL_GetClipboardText();
  auto elem = Builder::loadText(text);
  SDL_free(text);
  return elem;
  
}

void Renderer::write(Element *element) {

  auto root = dynamic_cast<Root *>(element->root());
  if (!root) {
    cerr << "no root or root isn't a Root!" << endl;
    return;
  }
  
  auto r = find_if(_roots.begin(), _roots.end(), [root](auto& e) {
    return e.get() == root;
  });
  if (r == _roots.end()) {
    cerr << "couldnt find elements root." << endl;
    return;
  }

  Builder::write(root->getObj(), root->getFilename());
  
  setDirty(root, false);
  
}

void Renderer::exec(Element *element, Change *change) {
  _changes.exec(*this, _hud.get(), element, change);
  setDirty(element); 
}

void Renderer::undo(Element *element) {
  _changes.undo(*this, _hud.get(), element);
  setDirty(element, true);  
}

void Renderer::redo(Element *element) {
  _changes.redo(*this, _hud.get(), element);
  setDirty(element, true);  
}

bool Renderer::processGlobalKey(SDL_Keycode code) {

  if (code >= SDLK_1 && code <= SDLK_9) {
    zoom((SDLK_9 - code) + 1);
    return true;
  }
  
  return false;
}

bool Renderer::processRootKey(Element *element, SDL_Keycode code) {

  if (processGlobalKey(code)) {
    return true;
  }
  
  switch (code) {
    case SDLK_P:
      paste();
      return true;
      
    case SDLK_C:
      copy(element);
      return true;

    case SDLK_U:
      undo(element);
      return true;

    case SDLK_R:
      redo(element);
      return true;

    case SDLK_W:
      write(element);
      return true;

    case SDLK_M:
      _moving = element->root();
      _movoffs = _mouse;
      return true;

    case SDLK_K:
      removeRoot(element->root());
      return true;
  }
  
  return false;
  
}

void Renderer::setTextState() {

  _editor->setHUD(_hud.get());

}

void Renderer::processDeleteKey(Element *element) {

  auto p = element->getParent();
  auto px = dynamic_cast<Listable *>(p);
  if (px) {
    exec(element, new RemoveFromList(px, element));
  }
  else {
    auto prop = dynamic_cast<Property *>(p);
    if (prop) {
      p = prop->getParent();
      px = dynamic_cast<Listable *>(p);
      if (px) {
        exec(element, new RemoveFromList(px, element));
      }
      else {
        cerr << "Not listable and not in a property" << endl;
      }
    }
  }
  
}

Point Renderer::addRootOrigin(Element *element, const Point &origin) {

  return origin + Locatable::cast(element->root())->getLocation();

}

void Renderer::processTextKey(Element *element, const Point &origin, const Size &size, SDL_Keycode code) {

  if (processGlobalKey(code)) {
    return;
  }
  
  Point o = addRootOrigin(element, origin);

  switch (code) {
    case SDLK_D:
      processDeleteKey(element);
      return;
      
    case SDLK_U:
      undo(element);
      return;

    case SDLK_R:
      redo(element);
      return;
  }

  // pass to the editor.
  _editor->processTextKey(*this, Editable::cast(element), o, size, code, _hud.get());

}

Point Renderer::localToGlobal(const Point &p) {

  return (p + _offs) * _scale;
  
}

Point Renderer::noOffset(const Point &p) {

  return p - _offs;
  
}

void Renderer::paste() {

  auto elem = getClipboard();
  if (elem) {
    addRoot(new Root("<clipboard>", elem));
  }
  else {
    setError("Invalid JSON");
  }
  
}

bool Renderer::processEvents() {

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
  
    SDL_ConvertEventToRenderCoordinates(_renderer, &event);

    if (_editor->processEvent(*this, event)) {
      return false;
    }

    switch (event.type) {

      case SDL_EVENT_MOUSE_BUTTON_DOWN:
        _mousedown = true;
        _last = Point(event.button.x, event.button.y);
        if (isDoubleClick()) {
          Spatial::scaleAndCenter(_size, _osize, _mouse,
            // up or down depending on shift key
            (_scale * ((SDL_GetModState() & (SDL_KMOD_LSHIFT | SDL_KMOD_RSHIFT)) ? 0.5 : 2.0)) - _scale, 
            1.0, &_scale, &_offs);
        }
        break;

      case SDL_EVENT_MOUSE_BUTTON_UP:
        _mousedown = false;
        break;

      case SDL_EVENT_MOUSE_MOTION:
        _mouse = Point(event.motion.x, event.motion.y);
        if (_mousedown) {
          Spatial::calcPan(Point(event.motion.x, event.motion.y), &_last, &_offs, _scale);
        }
        if (!_editor->capture()) {
          setHUD();
        }
        break;

      case SDL_EVENT_MOUSE_WHEEL:
        if (!_editor->capture()) {
          Spatial::scaleAndCenter(_size, _osize, _mouse, event.wheel.y, _scalemult, &_scale, &_offs);
        }
        break;

      case SDL_EVENT_WINDOW_RESIZED:
        {
          int w, h;
          SDL_GetWindowSize(_window, &w, &h);
          Size osize = _size;
          _size = Size(w, h);
          _scale *= _size.w / osize.w;
        }
        break;
               
      case SDL_EVENT_KEY_DOWN:
        _error.reset();
        if (_moving) {
          if (event.key.key == SDLK_ESCAPE) {
            _moving = nullptr;
            break;
          }
          if (event.key.key == SDLK_D) {
            // it's dropped.
            auto loc = Locatable::cast(_moving)->getLocation();
            Locatable::cast(_moving)->setLocation(((_mouse - _movoffs) / _scale) + loc);
            _moving = nullptr;
            break;
          }
        }
        else if (!_editor->capture()) {
          auto hit = getHit();
          if (hit) {
            get<0>(*hit)->processKey(*this, event.key.key);
          }
          else {
            switch (event.key.key) {
              case SDLK_P:
                paste();
                break;
                
              case SDLK_ESCAPE:
                _adding = false;
                break;
                
              case SDLK_N:
                _adding = true;
                break;
                
              case SDLK_D:
                if (!_adding) {
                  break;
                }
                addRoot(new Root("<new>", new Dict()));
                _adding = false;
                break;
          
              case SDLK_L:
                if (!_adding) {
                  break;
                }
                addRoot(new Root("<new>", new List()));
                _adding = false;
                break;
            }
          }
          setHUD();
        }
        break;

     case SDL_EVENT_QUIT:
        return true;
        break;

      default:
//        cout << "got event " << hex << event.type << endl;
        break;
    }
    
  }

  return false;
  
}

void Renderer::zoom(int key) {

  float newscale = Spatial::calcScale(key);
  if (newscale != 0) {
    Spatial::scaleAndCenter(_size, _osize, _mouse, newscale - _scale, 1.0, &_scale, &_offs);
  }

}

void Renderer::clearScale() {
  _oldscale = _scale;
  _oldoffs = _offs;
  _scale = 1.0;
  _offs = Size(0, 0);
}

void Renderer::restoreScale() {
  _scale = _oldscale;
  _offs = _oldoffs;
}

void Renderer::setTarget(SDL_Texture *texture) {

  if (!SDL_SetRenderTarget(_renderer, texture)) {
    SDL_Log("Couldn't set render target: %s", SDL_GetError());
  }

}


SDL_Texture *Renderer::createTexture(int width, int height) {

  SDL_Texture *texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
  if (!texture) {
    SDL_Log("Couldn't create texture: %s", SDL_GetError());
    return 0;
  }
  return texture;

};


SDL_Texture *Renderer::createTexture(SDL_Surface *surface) {
  
  SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);
  if (!texture) {
    SDL_Log("Couldn't create texture from surface: %s", SDL_GetError());
    return 0;
  }
  return texture;
  
}

void Renderer::destroyTexture(SDL_Texture *texture) {
  SDL_DestroyTexture(texture);
}

void Renderer::renderTexture(SDL_Texture *texture, const Rect &rect, bool offs) {

  Rect r = rect + (offs ? _offs : Size(0, 0));

  SDL_FRect sr = r.srect();
  SDL_RenderTexture(_renderer, texture, NULL, &sr);

}

void Renderer::renderRect(const Rect &rect) {

  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);

  Rect r = rect + _offs;
  r -= 1; // inset by 1
  
  SDL_FRect sr = r.srect();
  SDL_RenderRect(_renderer, &sr);

}

void Renderer::renderFilledRect(const Rect &rect, const SDL_Color &color) {

  SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
  
  Rect r = rect + _offs;

  SDL_FRect sr = r.srect();
  SDL_RenderFillRect(_renderer, &sr);


}

bool Renderer::textTooSmall() {

  return _scale < 0.14;
  
}

SDL_Surface *Renderer::renderText(const std::wstring &str, const SDL_Color &fgcolor, const SDL_Color &bgcolor) {

//  wcout << "w " << str << endl;
  char* u8str = SDL_iconv_wchar_utf8(str.c_str());
  SDL_Surface *surface = TTF_RenderText_Shaded(_font->_font, u8str, 0, fgcolor, bgcolor);
  SDL_free(u8str);
  if (!surface) {
    SDL_Log("could not create surface");
    return 0;
  }

  return surface;
  
}

void Renderer::renderFilledPie(const Point &origin, int radius, int start, int end, const SDL_Color &color) {

  Point o = origin + _offs;
  Gfx::aaFilledPieRGBA(_renderer, 
    o.x, o.y, radius, radius,
		start, end, 0, 
		color.r, color.g, color.b, color.a);

}

void Renderer::setError(const string &str) {

  cerr << str << endl;

  auto error = new Text();
  error->set(Unicode::convert(str), Colours::red);
  error->build(*this);
  _error.reset(error);
  
}

void Renderer::setDirty(Element *elem, bool state) {

  auto root = dynamic_cast<Root *>(elem->root());
  if (!root)  {
    cerr << "element has no root or not a Root" << endl;
    return;
  }
  auto r = find_if(_roots.begin(), _roots.end(), [root](auto& e) {
    return e.get() == root;
  });
  if (r == _roots.end()) {
    cerr << "couldnt find elements root." << endl;
    return;
  }
    
  root->setDirty(*this, state);
  
  _hud->setFlag(*this, canWrite, state);

}


