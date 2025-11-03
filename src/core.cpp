/*
  core.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 4-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "core.hpp"

#include "spatial.hpp"
#include "builder.hpp"
#include "colours.hpp"
#include "gfx.hpp"
#include "editable.hpp"
#include "property.hpp"
#include "listelem.hpp"
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

Core::~Core() {

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

void Core::destroyRoots() {

  for_each(_roots.begin(), _roots.end(), [this](auto& e) { 
    destroy(e.get()); 
  });

}

Size Core::displaySize() {

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

bool Core::init(const string &path) {

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
    registerGlobalHUDMode(mode);
    mode->add(new Shortcut(L"N", L"ew"));
    mode->add(new Shortcut(L"P", L"aste"));
    mode->add(new Shortcut(L"O", L"pen"));
    _hudnone = _hud->registerMode("none", mode);
  }
  
  {
    auto mode = new HUDMode(false);
    mode->add(new Shortcut(L"Esc", L"(finish)"));
    mode->add(new Shortcut(L"D", L"ict"));
    mode->add(new Shortcut(L"L", L"ist"));
    _hudadding = _hud->registerMode("adding", mode);
  }
  
  // register all of the key handlers.
  registerGlobalKeyHandlers();
  registerRootKeyHandlers();
  registerTextKeyHandlers();
  registerCoreKeyHandlers();
  
  // initialisation for types after everything has been created.
  initTypes();
  
  // build the hud with all modes
  _hud->build(*this);

  // make sure the hud flags are set correctly.
  _changes.setUndoFlags(*this, _hud.get());
  
  // other flags we have.
  _hud->setFlag(*this, canWrite, false);
  _hud->setFlag(*this, canRun, false);
  _hud->setFlag(*this, canEdit, false);
  _hud->setFlag(*this, canLoad, false);
  
//   _pointercursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
//   _editcursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT);
  
  return true;
   
}

void Core::addFile(const string &filename, bool raw) {

  auto obj = Builder::loadFile(filename, raw);
  if (!obj) {
    cerr << "file '" << filename << "' not found." << endl;
    return;
  }
  
  addRoot(obj);
  
}

void Core::addRoot(const std::string &name, const rfl::Generic &g) {
  
  addRoot(new Root(name, Builder::walk(0, g)));
  
}

void Core::addRoot(Element *element, bool useloc) {

  // if there is only 1 element and it is <new> with an empty dict
  // then we replace it.
  if (_roots.size() == 1) {
    auto root = dynamic_cast<Root *>(_roots[0].get());
    if (root) {
      if (root->getFilename() == "<new>") {
        auto list = dynamic_cast<List *>(root->getObj());
        if (list && list->isDict() && list->count() == 0) {
          destroyRoots();
          _roots.clear();
        }
      }
    }
  }

  // setup the HUD in the object.
  Commandable::cast(element)->initHUD(_hud.get());

  // build all objects with this core.
  build(element);
  
  // lay it out.
  layout(element);
  
  if (!useloc) {
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
  }

  // remember it.
  _roots.push_back(unique_ptr<Element>(element));
  
  // calculate the total size of all the objects.
  recenter();

}

void Core::removeRoot(Element *element) {

  auto r = find_if(_roots.begin(), _roots.end(), [element](auto& e) {
    return e.get() == element;
  });
  if (r == _roots.end()) {
    cerr << "couldnt find root." << endl;
    return;
  }

  destroy(r->get());
  _roots.erase(r);
  recenter();
  
}

void Core::build(Element *elem) {

  if (!elem) {
    cerr << "trying to build a null elem" << endl;
    return;
  }
  
  elem->visit([this](auto e) {
//    cout << "building " << e->describe() << endl;
    e->build(*this);
    return true;
  });

}

void Core::destroy(Element *elem) {

  if (!elem) {
    cerr << "trying to destroy a null elem" << endl;
    return;
  }
  
  elem->visit([this](auto e) {
//    cout << "destroying " << e->describe() << endl;
    e->destroy(*this);
    return true;
  });

}

void Core::layout(Element *elem) {

  if (!elem) {
    cerr << "trying to visit a null elem" << endl;
    return;
  }
  
  elem->visit([this](auto e) {
//    cout << "layout " << e->describe() << endl;
    e->layout();
    return true;
  });
  
}

void Core::layoutAll() {

  for_each(_roots.begin(), _roots.end(), [this](auto& e) {
    layout(e.get());
  });
  
}

void Core::recenter() {

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

void Core::initElement(Element *parent, Element *element) {

  element->setParent(parent);
  build(element);
  Commandable::cast(element)->initHUD(_hud.get());
  
}

void Core::loop(int rep) {

  setupTest(rep);

  bool done = false;
  while (!done) {
  
    // handle ZMQ stuff.
    processMsg();

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

void Core::setDrawColor(const SDL_Color &color) {

  SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
  
}

void Core::drawRect(const Rect &r) {

  SDL_FRect sr = r.srect();
  SDL_RenderRect(_renderer, &sr);
  
}

void Core::setScale(double x, double y) {

  SDL_SetRenderScale(_renderer, x, y);

}

void Core::fillRect(const Rect &r) {

  SDL_FRect sr = r.srect();
  SDL_RenderFillRect(_renderer, &sr);
  
}

void Core::debugOffs() {

  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);
  stringstream ss;
  ss << "offs: " << _offs << endl;
  SDL_RenderDebugText(_renderer, _size.w - 300, 10, ss.str().c_str());

}

void Core::debugScale() {

  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);
  stringstream ss;
  ss << "scale: " << _scale << endl;
  SDL_RenderDebugText(_renderer, _size.w - 300, 30, ss.str().c_str());

}

void Core::debugMouse(const Point &p) {

  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);
  stringstream ss;
  ss << "mouse: " << p.x << ", " << p.y << endl;
  SDL_RenderDebugText(_renderer, _size.w - 300, 50, ss.str().c_str());

}

void Core::debugSize() {

  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);
  stringstream ss;
  ss << "size: " << _size << endl;
  SDL_RenderDebugText(_renderer, _size.w - 300, 70, ss.str().c_str());

}

bool Core::isDoubleClick() {

  auto ticks = SDL_GetTicks();
  bool dbl = (ticks - _lastclick) < DOUBLE_CLICK_TIME;
  _lastclick = ticks;
  return dbl;
  
}

optional<Core::getHitReturnType> Core::getHit() {

//  cout << "getHit" << endl;
  
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

void Core::setHUD() {

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

void Core::changed(Element *elem) {

//  cout << "changed " << elem->describe() << endl;
  
  // tell all the objects about this change!
  for (auto& r: _roots) {
    bool result = r->visit([this, elem](auto e) {
      e->changed(*this, elem);
      return true;
    });
    if (!result) {
      break;
    }
  }

}

void Core::endEdit(Editable *obj) {

  setHUD();

  Element *el = dynamic_cast<Element *>(obj);
  if (!el) {
    cerr << "thing that changed wasn't an element" << endl;
    return;
  }
  
  changed(el);
    
}

void Core::registerGlobalHUDMode(HUDMode *mode) {

  mode->add(new Shortcut(L"[1-9]", L"Zoom"));
  mode->add(new Shortcut(L"U", L"ndo", canUndo));
  mode->add(new Shortcut(L"R", L"edo", canRedo));
  
}

void Core::registerGlobalKeyHandlers() {

  _globalHandlers[SDLK_U] =  [&]() { 
    undo(0); 
  };
  _globalHandlers[SDLK_R] =  [&]() { 
    redo(0); 
  };

}

void Core::registerRootHUDMode(HUDMode *mode) {

  registerGlobalHUDMode(mode);
  mode->add(new Shortcut(L"M", L"ove"));
  mode->add(new Shortcut(L"K", L"ill"));
  mode->add(new Shortcut(L"W", L"rite", canWrite));
  mode->add(new Shortcut(L"S", L"ave as"));
  mode->add(new Shortcut(L"C", L"opy"));
  mode->add(new Shortcut(L"P", L"aste"));
  
}

void Core::registerRootKeyHandlers() {

  // all the handlers related to "root" objects.
  _rootHandlers[SDLK_M] =  [&](Element *element) { 
    _moving = element->root();
    _movoffs = _mouse;
  };
  _rootHandlers[SDLK_K] =  [&](Element *element) { 
    removeRoot(element->root());
  };
  _rootHandlers[SDLK_W] =  [&](Element *element) { 
    write(element); 
  };
  _rootHandlers[SDLK_S] =  [&](Element *element) {
    saveFile(element);
  };
  _rootHandlers[SDLK_C] =  [&](Element *element) { 
    copy(element); 
  };
  _rootHandlers[SDLK_P] =  [&](Element *element) { 
    paste(); 
  };

  // and overide the root undor and redo to take an element.
  _rootHandlers[SDLK_U] =  [&](Element *element) { 
    undo(element); 
  };
  _rootHandlers[SDLK_R] =  [&](Element *element) { 
    redo(element); 
  };

}

void Core::registerTextHUDMode(HUDMode *mode) {

  mode->add(new Shortcut(L"D", L"elete"));
  
}

void Core::registerTextKeyHandlers() {

  _textHandlers[SDLK_D] =  [&](Element *element) { 
    processDeleteKey(element);
  };

  // and overide the root undo and redo to take an element.
  _textHandlers[SDLK_U] =  [&](Element *element) { 
    undo(element); 
  };
  _textHandlers[SDLK_R] =  [&](Element *element) { 
    redo(element); 
  };
  
}

void Core::registerCoreKeyHandlers() {

  // all the core handlers. pasting, movind and dropping, new items on the desktop.
  _coreHandlers[SDLK_P] =  [&]() { 
    paste(); 
  };
  _coreHandlers[SDLK_ESCAPE] =  [&]() { 
    if (_moving) {
      _moving = nullptr;
      return;
    }
    _adding = false;
  };
  _coreHandlers[SDLK_N] =  [&]() { 
    _adding = true;
  };
  _coreHandlers[SDLK_O] =  [&]() {
    openFile();
  };
  _coreHandlers[SDLK_D] =  [&]() { 
    if (_moving) {
        // it's dropped.
        auto loc = Locatable::cast(_moving)->getLocation();
        Locatable::cast(_moving)->setLocation(((_mouse - _movoffs) / _scale) + loc);
        _moving = nullptr;
        return;
    }
    if (_adding) {
      addRoot(new Root("<new>", new List(true)));
      _adding = false;
    }
  };
  _coreHandlers[SDLK_L] =  [&]() { 
    if (_adding) {
      addRoot(new Root("<new>", new List(false)));
      _adding = false;
    }
  };

}

Element *Core::getClipboard() {

  char *text = SDL_GetClipboardText();
  auto elem = Builder::loadText(text);
  SDL_free(text);
  return elem;
  
}

void Core::write(Element *element) {

  auto root = element->root();
  
  auto writeable = dynamic_cast<Writeable *>(root);
  if (!writeable) {
    cerr << "no root or root isn't Writeable!" << endl;
    return;
  }
  auto filename = writeable->getFilename();
  if (!filename) {
    cerr << "Not a file based object" << endl;
    return;
  }
  if (filename->find('<') == 0 && filename->find('>') == (filename->size() - 1)) {
    saveFile(root);
  }
  else {
    Builder::write(writeable->getGeneric(), *filename);
  }
  
  setDirty(root, false);
  
}

void Core::exec(Element *element, Change *change) {
  _changes.exec(*this, _hud.get(), element, change);
  if (element) {
    setDirty(element);
  }
  else {
    layoutAll();
  }
}

void Core::undo(Element *element) {
  _changes.undo(*this, _hud.get(), element);
  if (element) {
    setDirty(element);
  }
  else {
    layoutAll();
  }
}

void Core::redo(Element *element) {
  _changes.redo(*this, _hud.get(), element);
  if (element) {
    setDirty(element);
  }
  else {
    layoutAll();
  }
}

bool Core::processKeyHandler(map<SDL_Keycode, msgHandler> &handlers, SDL_Keycode code) {

  auto handler = handlers.find(code);
  if (handler == handlers.end()) {
//    cout << "ignoring key" << code << endl;
    return false;
  }
  handler->second();
  return true;

}

bool Core::processKeyHandler(std::map<SDL_Keycode, elementMsgHandler> &handlers, Element *element, SDL_Keycode code) {

  auto handler = handlers.find(code);
  if (handler == handlers.end()) {
//    cout << "ignoring key" << code << endl;
    return false;
  }
  handler->second(element);
  return true;
}

bool Core::processKeyHandler(std::map<SDL_Keycode, coreMsgHandler> &handlers, SDL_Keycode code) {

  auto handler = handlers.find(code);
  if (handler == handlers.end()) {
//    cout << "ignoring key" << code << endl;
    return false;
  }
  handler->second(*this);
  return true;
}

bool Core::processGlobalKey(SDL_Keycode code) {

  // explictly test for zoom.
  if (code >= SDLK_1 && code <= SDLK_9) {
    zoom((SDLK_9 - code) + 1);
    return true;
  }
  
  // otherwise try a handler.
  return processKeyHandler(_globalHandlers, code);

}

bool Core::processRootKey(Element *element, SDL_Keycode code) {

  if (processKeyHandler(_rootHandlers, element, code)) {
    return true;
  }

  return processGlobalKey(code);
  
}

void Core::setTextState() {

  if (_editor->capture()) {
    _editor->setHUD(_hud.get());
  }

}

bool Core::removeFromList(Element *p, Element *element) {

  auto list = dynamic_cast<List *>(p);
  if (list) {
    exec(element, new RemoveFromList(list, element));
    return true;
  }
  return false;
  
}

void Core::processDeleteKey(Element *element) {

  auto p = element->getParent();
  if (!removeFromList(p, element)) {
    auto prop = dynamic_cast<Property *>(p);
    if (prop) {
      p = prop->getParent();
      if (!removeFromList(p, element)) {
        cerr << "Parent is not list" << endl;
      }
    }
    else {
      auto elem = dynamic_cast<ListElem *>(p);
      if (elem) {
        p = elem->getParent();
        if (!removeFromList(p, element)) {
          cerr << "Parent is not list" << endl;
        }
      }
      else {
        cerr << "parent is not list or listelem or property" << endl;
      }
    }
  }
  
}

Point Core::addRootOrigin(Element *element, const Point &origin) {

//  cout << "addRootOrigin" << element->describe() << endl;
  
  auto root = element->root();
//  cout << "root" << root->describe() << endl;
  
  return origin + Locatable::cast(root)->getLocation();

}

void Core::processTextKey(Element *element, const Point &origin, const Size &size, SDL_Keycode code) {

  if (processKeyHandler(_textHandlers, element, code)) {
    return;
  }

  if (processGlobalKey(code)) {
    return;
  }
  
  // find where the element is/
  Point o = addRootOrigin(element, origin);

  // pass to the editor.
  _editor->processTextKey(*this, Editable::cast(element), o, size, code, _hud.get());

}

Point Core::localToGlobal(const Point &p) {

  return (p + _offs) * _scale;
  
}

Point Core::noOffset(const Point &p) {

  return p - _offs;
  
}

void Core::copy(Element *element) {

//  cout << "copying " << element->describe() << endl;
  SDL_SetClipboardText(Builder::getJson(element).c_str());
  
}

void Core::paste() {

  auto elem = getClipboard();
  if (elem) {
    if (elem->isRoot()) {
      addRoot(elem);
    }
    else {
      addRoot(new Root("<clipboard>", elem));
    }
  }
  else {
    setError("Invalid JSON");
  }
  
}

bool Core::processEvents() {

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
        if (!_editor->capture()) {
          auto hit = getHit();
          if (hit) {
            // try the actual object.
            // it is up to an object to try the global handlers first if they 
            // aren't in some special mode.
            get<0>(*hit)->processKey(*this, event.key.key);
          }
          else {
            // no hit, try a core key otherwise a global key.
            if (!processGlobalKey(event.key.key)) {
              processKeyHandler(_coreHandlers, event.key.key);
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

void Core::zoom(int key) {

  float newscale = Spatial::calcScale(key);
  if (newscale != 0) {
    Spatial::scaleAndCenter(_size, _osize, _mouse, newscale - _scale, 1.0, &_scale, &_offs);
  }

}

void Core::clearScale() {
  _oldscale = _scale;
  _oldoffs = _offs;
  _scale = 1.0;
  _offs = Size(0, 0);
}

void Core::restoreScale() {
  _scale = _oldscale;
  _offs = _oldoffs;
}

void Core::setTarget(SDL_Texture *texture) {

  if (!SDL_SetRenderTarget(_renderer, texture)) {
    SDL_Log("Couldn't set render target: %s", SDL_GetError());
  }

}

SDL_Texture *Core::createTexture(int width, int height) {

  SDL_Texture *texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
  if (!texture) {
    SDL_Log("Couldn't create texture: %s", SDL_GetError());
    return 0;
  }
  return texture;

}

SDL_Texture *Core::createTexture(SDL_Surface *surface) {
  
  SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);
  if (!texture) {
    SDL_Log("Couldn't create texture from surface: %s", SDL_GetError());
    return 0;
  }
  return texture;
  
}

void Core::destroyTexture(SDL_Texture *texture) {
  SDL_DestroyTexture(texture);
}

void Core::renderTexture(SDL_Texture *texture, const Rect &rect, bool offs) {

  Rect r = rect + (offs ? _offs : Size(0, 0));

  SDL_FRect sr = r.srect();
  SDL_RenderTexture(_renderer, texture, NULL, &sr);

}

void Core::renderRect(const Rect &rect) {

  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);

  Rect r = rect + _offs;
  r -= 1; // inset by 1
  
  SDL_FRect sr = r.srect();
  SDL_RenderRect(_renderer, &sr);

}

void Core::renderFilledRect(const Rect &rect, const SDL_Color &color) {

  SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
  
  Rect r = rect + _offs;

  SDL_FRect sr = r.srect();
  SDL_RenderFillRect(_renderer, &sr);


}

bool Core::textTooSmall() {

  return _scale < 0.14;
  
}

SDL_Surface *Core::renderText(const std::wstring &str, const SDL_Color &fgcolor, const SDL_Color &bgcolor) {

  if (str.size() == 0) {
    return nullptr;
  }
  
//  wcout << "w " << str << endl;
  char* u8str = SDL_iconv_wchar_utf8(str.c_str());
  SDL_Surface *surface = TTF_RenderText_Shaded(_font->_font, u8str, 0, fgcolor, bgcolor);
  SDL_free(u8str);
  if (!surface) {
    wcerr << str << endl;
    SDL_Log("could not create surface");
    return 0;
  }

  return surface;
  
}

void Core::renderFilledPie(const Point &origin, int radius, int start, int end, const SDL_Color &color) {

  Point o = origin + _offs;
  Gfx::aaFilledPieRGBA(_renderer, 
    o.x, o.y, radius, radius,
		start, end, 0, 
		color.r, color.g, color.b, color.a);

}

void Core::setError(const string &str) {

  cerr << str << endl;

  auto error = new Text();
  error->set(Unicode::convert(str), Colours::red);
  error->build(*this);
  _error.reset(error);
  
}

void Core::setDirty(Element *elem, bool state) {

  auto elemroot = elem->root();
  if (!elemroot) {
    cerr << "element " << elem->describe() << " has no root" << endl;
    return;
  }

  auto r = find_if(_roots.begin(), _roots.end(), [elemroot](auto& e) {
    return e.get() == elemroot;
  });
  if (r == _roots.end()) {
    cerr << "couldnt find elements root." << endl;
    return;
  }
    
  Writeable::cast(elemroot)->setDirty(*this, state);
  
  _hud->setFlag(*this, canWrite, state);

}

void Core::renderLayout(const Point &origin, const RectList &layout) {

  if (layout.size() == 0) {
    return;
  }
  
  auto last = layout.back();
  Rect r(Point(), last.origin + last.size);
  renderFilledRect(r + origin, Colours::lightGrey);
  
  for (auto i: layout) {
    renderFilledRect(i + origin, Colours::blue);
  }
  
}

void Core::openFile() {

  const SDL_DialogFileFilter filters[] = {
    { "JSON",  "json" },
    { "YAML", "yaml" }
  };
  SDL_ShowOpenFileDialog([](auto userdata, auto list, auto filter) {
    if (list && *list) {
      Core *core = (Core *)userdata;
      core->addRoot(Builder::loadFile(list[0], false));
    }
  }, this, _window, filters, 2, nullptr, false);
  
}

void Core::saveFile(Element *element) {

  const SDL_DialogFileFilter filters[] = {
    { "JSON",  "json" },
    { "YAML", "yaml" }
  };
  typedef struct {
    Core *core;
    Element *element;
  } UserData;
  auto ud = new UserData{this, element->root()};
  SDL_ShowSaveFileDialog([](auto userdata, auto list, auto filter) {
    if (list && *list) {
      auto ud = (UserData *)userdata;
      auto ex = dynamic_cast<Editable *>(ud->element);
      auto wx = dynamic_cast<Writeable *>(ud->element);
      if (ex && wx) {
        Builder::write(wx->getGeneric(), list[0]);
        ex->setString(*(ud->core), Unicode::convert(list[0]));
      }
      else {
        cerr << "can't write and edit." << endl;
      }
      delete ud;
    }
  }, ud, _window, filters, 2, nullptr);
}
