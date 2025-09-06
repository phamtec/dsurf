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

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>
#include <sstream>

using namespace std;

#define DOUBLE_CLICK_TIME 300

Renderer::~Renderer() {

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
  
  // always just a new dictiionary.
  setRoot(new Dict());

  // build our editor.
  _editor.reset(new TextEditor(_startedit));
  _editor->build(*this);
  _editor->layout();
  
  _hud.reset(new HUD());
  _hud->build(*this);
  
//   _pointercursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
//   _editcursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT);
  
  return true;
   
}

void Renderer::setRoot(Element *root) {

  _root.reset(root);
  
  // build all objects with this renderer.
  _root->build(*this);
  
  // lay it out.
  _osize = _root->layout();
  
  _offs = Spatial::center(_size, _osize, _scale);
//  cout << _offs << endl;

}

void Renderer::loop() {

  bool done = false;
  while (!done) {
  
    // handle all the events.
    if (processEvents()) {
      done = true;
    }

    SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(_renderer);
    
    // set the scale ready to do our calculations.
    SDL_SetRenderScale(_renderer, _scale, _scale);

    _root->render(*this, Point(0.0, 0.0));
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

void Renderer::endEdit() {
  _hud->setState(Text);
}

bool Renderer::processEvents() {

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
  
    SDL_ConvertEventToRenderCoordinates(_renderer, &event);

    _editor->processEvent(event);

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
          // set the hud.
          Element *hit = _root->hitTest(Point(_offs), _mouse * (1 / _scale));
          if (hit) {
            HUDable *hx = dynamic_cast<HUDable *>(hit);
            if (hx) {
              hx->setState(_hud.get());
            }
          }
          else {
            _hud->setState(None);
          }
        }
        break;

      case SDL_EVENT_MOUSE_WHEEL:
        Spatial::scaleAndCenter(_size, _osize, _mouse, event.wheel.y, _scalemult, &_scale, &_offs);
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
        {
          Element *hit = _root->hitTest(Point(_offs), _mouse * (1 / _scale));
          if (hit) {
            Keyable *kx = dynamic_cast<Keyable *>(hit);
            if (kx) {
              kx->processKey(*this, event.key.key);
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

void Renderer::copy(Element *element) {

  SDL_SetClipboardText(Builder::getJson(element).c_str());
  
}

void Renderer::paste() {

  char *text = SDL_GetClipboardText();
  auto json = Builder::loadText(text);
  SDL_free(text);
  if (json) {
    setRoot(json);
  }
  
}

void Renderer::editText(Element *element, const Point &origin, const Size &size) {

  Editable *ex = dynamic_cast< Editable *>(element);
  if (!ex) {
    cerr << "object not text editable." << endl;
    return;
  }
  
  // focus the editor on the object
  _editor->focus(origin, size, ex);
  
  // locate the hud on the edited object.
  _hud->setEditingLoc((element->origin() + _offs) * _scale);  
  _hud->setState(Editing);
  
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

SDL_Surface *Renderer::renderText(const char *str, const SDL_Color &color) {

  SDL_Surface *surface = TTF_RenderText_Shaded(_font->_font, str, 0, color, Colours::white);
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
