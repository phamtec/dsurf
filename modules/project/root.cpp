/*
  root.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 2-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "root.hpp"

#include "renderer.hpp"
#include "unicode.hpp"
#include "sizes.hpp"

using namespace std;

ProjectRoot::ProjectRoot(const string &name, const string &filename, vector<Element *> &objs): _parent(0), _filename(filename) {

  _name.set(Unicode::convert(name), Colours::black);
  
  transform(objs.begin(), objs.end(), back_inserter(_objs), [this](auto e) {
    e->setParent(this);
    return unique_ptr<Element>(e);
  });
}

Size ProjectRoot::layout() {

  _size = _name.size();
  for_each(_objs.begin(), _objs.end(), [this](auto& e) {
  
    Size s = e->layout();
    _size.h += s.h + Sizes::text_padding;
    if (_size.w < s.w) {
      _size.w = s.w;
    }
  });
  
  return _size;
  
}

void ProjectRoot::build(Renderer &renderer) {

  _name.build(renderer);
  for_each(_objs.begin(), _objs.end(), [&renderer](auto& e) {
    e->build(renderer);
  });
}

void ProjectRoot::render(Renderer &renderer, const Point &origin) {

  _name.render(renderer, origin);

  float y = origin.y + _name.size().h + Sizes::text_padding;
  for (auto& i: _objs) {
    i->render(renderer, origin + Size(Sizes::group_indent, y));
    y += i->size().h + Sizes::text_padding;
  }
  
//  renderer.renderRect(_r);

}

Element *ProjectRoot::hitTest(const Point &origin, const Point &p) { 

  Point o = origin + Size(Sizes::group_indent, _name.size().h);
  for (auto& i: _objs) {
    Element *hit = i->hitTest(o, p);
    if (hit) {
      return hit;
    }
    o.y += i->size().h + Sizes::text_padding;
  }

  return super::hitTest(origin, p);
  
}

Point ProjectRoot::localOrigin(Element *elem) {

  int j=0;
  int y=0;
  for (auto& i: _objs) {
    if (i.get() == elem) {
      return Point(Sizes::group_indent, y);
    }
    y += i->size().h + Sizes::text_padding;
    j++;
  }

  return Point(0, 0);
  
}

void ProjectRoot::initHUD(HUD *hud) {

  _hudroot = hud->findMode("projectroot");

  for_each(_objs.begin(), _objs.end(), [hud](auto& e) { 
    Commandable::cast(e.get())->initHUD(hud);
  });

}

void ProjectRoot::setMode(Renderer &renderer, HUD *hud) {

  hud->setMode(_hudroot);
  
}

void ProjectRoot::processKey(Renderer &renderer, SDL_Keycode code) {

  switch (code) {      
    case SDLK_E:
      renderer.addFile(_filename, true);
      break;
  }

}

