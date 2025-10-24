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

ProjectRoot::ProjectRoot(const string &name, const string &filename, vector<Element *> &objs): _filename(filename) {

  _name.set(Unicode::convert(name), Colours::white);
  
  transform(objs.begin(), objs.end(), back_inserter(_objs), [this](auto e) {
    e->setParent(this);
    return unique_ptr<Element>(e);
  });
}

void ProjectRoot::setParent(Element *parent) {
  cerr << "setParent on ProjectRoot!" << endl;
}

RectList ProjectRoot::calcLayout() {

  RectList layout;
  
  auto s = _name.size();
  auto o = Point();
  layout.push_back(Rect(o, s));
  o.y += s.h + Sizes::text_padding;
  o.x += Sizes::group_indent;
  float w = s.w;
  for_each(_objs.begin(), _objs.end(), [&o, &layout, &w](auto& e) {
    auto s = e->size();
    s.w += Sizes::text_padding;
    layout.push_back(Rect(o, s));
    o.y += s.h + Sizes::text_padding;
    if ((o.x + s.w) > w) {
      w = o.x + s.w;
    }
  });
  Layout::addSize(&layout, Size(w, o.y));
  
  return layout;
  
}

void ProjectRoot::layout() {

  _layout = calcLayout();
  _size = Layout::size(_layout);
  
}

void ProjectRoot::build(Renderer &renderer) {

  _name.build(renderer);

}

void ProjectRoot::render(Renderer &renderer, const Point &origin) {

//  renderer.renderLayout(origin, _layout);

  renderer.renderFilledRect(Rect(origin, _size), Colours::racingGreen);

  auto i = _layout.begin();
  _name.render(renderer, origin + (*i).origin);
  i++;
  
  for_each(_objs.begin(), _objs.end(), [&renderer, origin, &i](auto& e) {
    e->render(renderer, origin + i->origin);
    i++;
  });

}

Element *ProjectRoot::hitTest(const Point &origin, const Point &p) { 

  auto i = _layout.begin();
  // skip name
  i++;
  
  for (auto& j: _objs) {
    Element *hit = j->hitTest(origin + i->origin, p);
    if (hit) {
      return hit;
    }
    i++;
  }
  
  return super::hitTest(origin, p);
  
}

Point ProjectRoot::localOrigin(Element *elem) {

  auto i = _layout.begin();
  // skip name
  i++;
  
  for (auto& j: _objs) {
    if (j.get() == elem) {
//      cout << "ProjectRoot " << i->origin << endl;
      return i->origin;
    }
    i++;
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

  if (renderer.processRootKey(this, code)) {
    return;
  }

  switch (code) {      
    case SDLK_E:
      renderer.addFile(_filename, true);
      break;
  }

}

bool ProjectRoot::visit(function<bool (Element *)> f) {

  for (auto& e: _objs) {
    if (!e->visit(f)) {
      return false;
    }
  };
  return f(this);
  
}
