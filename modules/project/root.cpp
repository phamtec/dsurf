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

ProjectRoot::ProjectRoot(const string &name, const string &filename, vector<Element *> &objs) {

  _filename.set(Unicode::convert(filename), Colours::black);
  _name.set(Unicode::convert(name), Colours::white);
  
  transform(objs.begin(), objs.end(), back_inserter(_objs), [this](auto e) {
    e->setParent(this);
    return unique_ptr<Element>(e);
  });
}

void ProjectRoot::setParent(Element *parent) {
  cerr << "setParent on ProjectRoot!" << endl;
}

optional<string> ProjectRoot::getFilename() { 

  return Unicode::convert(_filename.str()); 
  
}

void ProjectRoot::setDirty(Renderer &renderer, bool state) {

  _filename.set(_filename.str(), state ? Colours::red : Colours::black);
  _filename.build(renderer);

}

RectList ProjectRoot::calcLayout() {

  RectList layout;
  
  auto o = Point();
  auto fs = _filename.size();
  layout.push_back(Rect(o, fs));
  o.y += fs.h + Sizes::text_padding;

  auto s = _name.size();
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

  _filename.build(renderer);
  _name.build(renderer);

}

void ProjectRoot::render(Renderer &renderer, const Point &origin) {

//  renderer.renderLayout(origin, _layout);

  auto i = _layout.begin();
  _filename.render(renderer, origin + (*i).origin);
  i++;
  
  renderer.renderFilledRect(Rect(origin + (*i).origin, _size - Size(0, _filename.size().h)), Colours::racingGreen);

  _name.render(renderer, origin + (*i).origin);
  i++;

  for_each(_objs.begin(), _objs.end(), [&renderer, origin, &i](auto& e) {
    e->render(renderer, origin + i->origin);
    i++;
  });

}

Element *ProjectRoot::hitTest(const Point &origin, const Point &p) { 

  auto i = _layout.begin();
  // skip filename
  i++;

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
  // skip filename
  i++;

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
      renderer.addFile(Unicode::convert(_filename.str()), true);
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

std::string ProjectRoot::getName() {

  return Unicode::convert(_name.str());
  
}

rfl::Generic ProjectRoot::getGeneric() { 

  rfl::Object<rfl::Generic> obj;
  
  // build a code object from the parts
  rfl::Object<rfl::Generic> project;
  project["name"] = Unicode::convert(_name.str());
  vector<rfl::Generic> objects;
  transform(_objs.begin(), _objs.end(), back_inserter(objects), [](auto& e) {
    return Writeable::cast(e.get())->getGeneric();
  });
  project["objects"] = objects;
  obj["project"] = project;
  
  return obj;
  
}
