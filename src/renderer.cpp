/*
  renderer.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 4-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "renderer.hpp"
#include "spatial.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>

using namespace std;

Renderer::Renderer(): _width(0.0l), _height(0.0l), 
  _scale(0.3l), _offs(0, 0),
  _mousedown(false), 
  _window(0), _renderer(0), _engine(0) {

  if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL Video: %s", SDL_GetError());
    return;
  }

  int n;
  SDL_DisplayID *displays = SDL_GetDisplays(&n);
  if (n < 1) {
    SDL_Log("Not enough displays");
    return;
  }

  const SDL_DisplayMode *dm = SDL_GetCurrentDisplayMode(displays[0]);
  if (!dm) {
    SDL_Log("Couldn't get display mode: %s", SDL_GetError());    
    return;
  }
  
  _width = dm->w - 20;
  _height = dm->h - 120;
  
}

Renderer::~Renderer() {

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

bool Renderer::init() {

   /* Initialize the TTF library */
  if (!TTF_Init()) {
    SDL_Log("Couldn't initialize TTF: %s",SDL_GetError());
    return false;
  }

  /* Create a window */
  _window = SDL_CreateWindow("dsurf", _width, _height, 0);
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

  return true;
   
}

Point Renderer::rootPoint(const Size &size) {
//   cout << _width << endl;
//   cout << size << endl;
//   cout << _scale << endl;
  return { .x = 0, .y = 0 };
}

void Renderer::prepare() {

  SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(_renderer);
  SDL_SetRenderScale(_renderer, _scale, _scale);

}

void Renderer::present() {

  SDL_RenderPresent(_renderer);

}

bool Renderer::processEvents() {

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
  
    SDL_ConvertEventToRenderCoordinates(_renderer, &event);

    switch (event.type) {

      case SDL_EVENT_MOUSE_BUTTON_DOWN:
//        cout << "click: " << event.button.x << ", " << event.button.y << endl;
        _mousedown = true;
        _last = Point(event.button.x, event.button.y);
        break;

      case SDL_EVENT_MOUSE_BUTTON_UP:
//        cout << "up: " << event.button.x << ", " << event.button.y << endl;
        _mousedown = false;
        break;

      case SDL_EVENT_MOUSE_MOTION:
//        cout << "move: " << event.motion.x << ", " << event.motion.y << endl;
        if (_mousedown) {
          Spatial::calcPan(Point(event.motion.x, event.motion.y), &_last, &_offs);
//          cout << "drag: " << dx << ", " << dy << endl;
        }
        break;

      case SDL_EVENT_MOUSE_WHEEL:
        _scale += event.wheel.y * 0.06l;
//        cout << "mouse wheel: " << event.wheel.x << ", " << event.wheel.y << endl;
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

SDL_Texture *Renderer::createTexture(SDL_Surface *surface) {
  
  SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);
  if (!texture) {
    SDL_Log("Couldn't render text: %s", SDL_GetError());
    return 0;
  }
  return texture;
  
}

void Renderer::renderTexture(SDL_Texture *texture, const Rect &rect) {

  Rect r = rect;
  r -= _offs;

  SDL_FRect sr = r.srect();
  SDL_RenderTexture(_renderer, texture, NULL, &sr);

}

void Renderer::renderRect(const Rect &rect) {

  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);

  Rect r = rect;
  r -= _offs;
  r -= 1; // inset by 1
  
  SDL_FRect sr = r.srect();
  SDL_RenderRect(_renderer, &sr);

}
