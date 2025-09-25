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

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>
#include <sstream>

using namespace std;

#define DOUBLE_CLICK_TIME 300

Renderer::~Renderer() {

  // allow objects to cleanup.
  for_each(_roots.begin(), _roots.end(), [this](auto& e) { e->destroy(*this); });

  // make sure alkl the chnages objects get destroyed too.
  for_each(_changes.begin(), _changes.end(), [this](auto&& e) { 
    e->destroy(*this); 
  });
  
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

bool Renderer::init(const char *path) {

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
  if (!_font->init(path)) {
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
  
  // initialisation for types after everything has been created.
  initTypes();
  
  // build the hud with all modes
  _hud->build(*this);
  
//   _pointercursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
//   _editcursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT);
  
  return true;
   
}

void Renderer::addRoot(Element *element, const string &name) {

  // if there is only 1 element and it is <new> with an empty dict
  // then we replace it.
  if (_rootnames.size() == 1 && _rootnames[0]->str() == L"<new>") {
    auto dict = dynamic_cast<Dict *>(_roots[0].get());
    if (dict && Listable::cast(dict)->count() == 0) {
      for_each(_roots.begin(), _roots.end(), [this](auto& e) { e->destroy(*this); });
      _roots.clear();
      _rootnames.clear();
    }
  }
  // setup the name to draw.
  auto rootname = new Text();
  rootname->set(Unicode::convert(name), Colours::black);
  rootname->build(*this);
  _rootnames.push_back(unique_ptr<Text>(rootname));
  
  // setup the HUD in the object.
  HUDable *hx = dynamic_cast<HUDable *>(element);
  if (hx) {
    hx->initHUD(_hud.get());
  }

  // build all objects with this renderer.
  element->build(*this);
  
  // lay it out.
  element->layout();
  
  // remember it.
  _roots.push_back(unique_ptr<Element>(element));
  
  // calculate the total size of all the objects.
  _osize = Size();
  for (int i=0; i<_roots.size(); i++) {
    Size s = _roots[i]->size();
    _osize.w += s.w;
    if (_osize.w > 0) {
      _osize.w += Sizes::group_indent;
    }
    if (s.h > _osize.h) {
      _osize.h = s.h;
    }
  }
  if (_rootnames.size() > 0) {
    _osize.h += _rootnames[0]->size().h;
  }
  
  _offs = Spatial::center(_size, _osize, _scale);
//  cout << _offs << endl;

}

void Renderer::initElement(Element *parent, int index, Element *element) {

  element->setParent(parent);
  element->build(*this);
  HUDable::cast(element)->initHUD(_hud.get());
  
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

    auto x = 0.0;
    for (int i=0; i<_roots.size(); i++) {
      _rootnames[i]->render(*this, Point(x, 0.0));
      _roots[i]->render(*this, Point(x, _rootnames[i]->size().h+Sizes::listgap));
      x += _roots[i]->size().w+Sizes::group_indent;
    }
    _editor->render(*this, Point(0.0, 0.0));

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

void Renderer::setHUD() {

  for (auto& i: _roots) {
    Element *hit = i->hitTest(Point(_offs), _mouse * (1 / _scale));
    if (hit) {
      HUDable *hx = dynamic_cast<HUDable *>(hit);
      if (hx) {
        _hud->setEditingLoc(localToGlobal(hit->origin()));
        hx->setMode(*this, _hud.get());
      }
      return;
    }
  }
  _hud->setMode(0);
}

void Renderer::endEdit() {

  setHUD();

}

void Renderer::copy(Element *element) {
  SDL_SetClipboardText(Builder::getJson(element).c_str());
}

void Renderer::registerRootHUDMode(HUDMode *mode) {

  mode->add(new Shortcut(L"C", L"opy"));
  mode->add(new Shortcut(L"P", L"aste"));
  mode->add(new Shortcut(L"U", L"ndo"));
  mode->add(new Shortcut(L"R", L"edo"));
  
}

bool Renderer::processRootKey(Element *element, SDL_Keycode code) {

  switch (code) {
    case SDLK_P:
      {
        char *text = SDL_GetClipboardText();
        auto json = Builder::loadText(text);
        SDL_free(text);
        if (json) {
          addRoot(json, "<clipboard>");
        }
        else {
          cerr << "invalid json" << endl;
        }
      }
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
  }
  
  return false;
  
}

void Renderer::setTextState() {

  _editor->setHUD(_hud.get());

}

void Renderer::registerTextHUDMode(HUDMode *mode) {

  mode->add(new Shortcut(L"D", L"elete"));
  
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

void Renderer::processTextKey(Element *element, const Point &origin, const Size &size, SDL_Keycode code) {

  switch (code) {
    case SDLK_D:
      processDeleteKey(element);
      return;
  }
  
  // pass to the editor.
  _editor->processTextKey(*this, Editable::cast(element), origin, size, code, _hud.get());

}

Point Renderer::localToGlobal(const Point &p) {

  return (p + _offs) * _scale;
  
}

Point Renderer::noOffset(const Point &p) {

  return p - _offs;
  
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
        if (!_editor->capture()) {
          for (auto& i: _roots) {
            Element *hit = i->hitTest(Point(_offs), _mouse * (1 / _scale));
            if (hit) {
              Keyable *kx = dynamic_cast<Keyable *>(hit);
              if (kx) {
                kx->processKey(*this, event.key.key);
              }
              // we search again since processKet might invalidate
              // the hit object.
              hit = i->hitTest(Point(_offs), _mouse * (1 / _scale));
              HUDable *hx = dynamic_cast<HUDable *>(hit);
              if (hx) {
                _hud->setEditingLoc(localToGlobal(hit->origin()));
                hx->setMode(*this, _hud.get());
              }
              break;
            }
          }
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

bool Renderer::textTooSmall(const Rect &rect) {

  return (rect.size.h * _scale) < 9;
  
}

SDL_Surface *Renderer::renderText(const std::wstring &str, const SDL_Color &color) {

//  wcout << "w " << str << endl;
  char* u8str = SDL_iconv_wchar_utf8(str.c_str());
  SDL_Surface *surface = TTF_RenderText_Shaded(_font->_font, u8str, 0, color, Colours::white);
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

void Renderer::exec(Element *element, Change *change) {

  // throw away all the changes after the undo ptr.
  if (_changes.size() > 0) {
    auto ii = _undoptr;
    ii++;
    for (std::vector<std::unique_ptr<Change> >::iterator i=ii; i != _changes.end(); i++) {
      _changes.erase(i);
    }
  }
  
  // execute this change
  change->exec(*this);
  
  // layout the root.
  element->root()->layout();
  
  // remember it.
  _changes.push_back(unique_ptr<Change>(change));
  
  // point at the last element.
  _undoptr = _changes.end();
  _undoptr--;
  
}

void Renderer::undo(Element *element) {

  if (_changes.size() == 0) {
    cout << "no changes" << endl;
    return;
  }
  
  (*_undoptr)->undo(*this);
  element->root()->layout();
  
  if (_undoptr == _changes.begin()) {
    _undoptr = _changes.end();
  }
  else {
    _undoptr--;
  }
}

void Renderer::redo(Element *element) {

  if (_changes.size() == 0) {
    cout << "no changes" << endl;
    return;
  }
  
  if (_undoptr == _changes.end()) {
    _undoptr = _changes.begin();
  }
  else {
    _undoptr++;
  }
  (*_undoptr)->exec(*this);
  element->root()->layout();
  
}


