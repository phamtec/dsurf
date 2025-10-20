/*
  code.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 19-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "code.hpp"

#include "builder.hpp"
#include "sizes.hpp"
#include "unicode.hpp"
#include "renderer.hpp"
#include "generic.hpp"

using namespace std;

ProjectCode::ProjectCode(const string &name, rfl::Generic source, optional<rfl::Object<rfl::Generic> > scenario): 
  _source(source), _hudobj(-1) {

  if (scenario) {
    _context = Generic::getGeneric(*scenario, "context");
  }
  _obj = unique_ptr<Element>(Builder::walk(this, _source));
  _name.set(Unicode::convert(name), Colours::black);
  
}

void ProjectCode::build(Renderer &renderer) {

  _name.build(renderer);
  _obj->build(renderer);
  
}

void ProjectCode::destroy(Renderer &renderer) {

  _obj->destroy(renderer);
  
}

RectList ProjectCode::calcLayout() {

  // at least just the name.
  auto s = _name.size();
  RectList layout;
  auto o = Point();
  layout.push_back(Rect(o, s));
  
  o.y += s.h + Sizes::text_padding;
  s = _obj->size();
  s.h += Sizes::text_padding * 2;
  s.w += Sizes::text_padding * 2;
  layout.push_back(Rect(o, s));
  o.y += s.h;
  Layout::addSize(&layout, Size(s.w, o.y));

  return layout;
  
}

void ProjectCode::layout() {

  _obj->layout();
  
  // calculate the layout.
  _layout = calcLayout();
  _size = Layout::size(_layout);
  
}

void ProjectCode::render(Renderer &renderer, const Point &origin) {

//  cout << origin << endl;

  auto i = _layout.begin();
  _name.render(renderer, origin + (*i).origin);
  i++;
  renderer.renderFilledRect(*i + origin, Colours::straw);
  _obj->render(renderer, origin + (*i).origin + Size(Sizes::text_padding, Sizes::text_padding));
  
}

Point ProjectCode::localOrigin(Element *elem) {

  auto i = _layout.begin();
  // skip name
  i++;
  
  if (elem == _obj.get()) {
//    cout << "ProjectCode " << i->origin << endl;
    return i->origin;
  }
  
  return Point(0, 0);
  
}

void ProjectCode::initHUD(HUD *hud) {

  _hudobj = hud->findMode("projectcode");
  
}

void ProjectCode::setMode(Renderer &renderer, HUD *hud) {

  // we can run if we have an input context.
  hud->setFlag(renderer, canRun, _context.has_value());
  
  hud->setMode(_hudobj);
  
}

void ProjectCode::processKey(Renderer &renderer, SDL_Keycode code) {

  if (renderer.processGlobalKey(code)) {
    return;
  }

  switch (code) {      
    case SDLK_R:
      {
        auto obj = Builder::walk(0, *_context);
        obj->layout();
//        cout << "R " << origin() << endl;
        renderer.addRoot("<context>", obj, origin() - Size(500, 0));
      }
      break;

  }

}


