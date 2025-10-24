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
#include "list.hpp"

using namespace std;

ProjectCode::ProjectCode(const string &name, rfl::Generic transform, optional<vector<rfl::Generic> > library, optional<rfl::Object<rfl::Generic> > scenario): 
  _hudobj(-1), _running(false) {

  if (library) {
    rfl::Object<rfl::Generic> lib;
    lib["library"] = *library;
    _flo.reset(new Flo(lib));
  }
  else {
    _flo.reset(new Flo());
  }
  
  _name.set(Unicode::convert(name), Colours::black);
  _transform = unique_ptr<Element>(Builder::walk(this, transform));
  if (scenario) {
    auto input = Generic::getGeneric(*scenario, "input");
    if (input) {
      _input = unique_ptr<Element>(Builder::walk(this, *input));
    }
  }
  
  if (!_input) {
    _input = unique_ptr<Element>(new List(false));
    _input->setParent(this);
  }

  _output = unique_ptr<Element>(new List(false));
  _output->setParent(this);
 
}

void ProjectCode::build(Renderer &renderer) {

  _name.build(renderer);

}

bool ProjectCode::visit(std::function<bool (Element *)> f) {

  if (!_transform->visit(f)) {
    return false;
  }
  if (!_input->visit(f)) {
    return false;
  }
  if (!_output->visit(f)) {
    return false;
  }

  return f(this);
  
}

RectList ProjectCode::calcLayout() {

  // at least just the name.
  auto names = _name.size();
  RectList layout;
  auto o = Point();
  layout.push_back(Rect(o, names));
  
  o.y += names.h + Sizes::text_padding;
  auto ts = _transform->size();
  ts.h += Sizes::text_padding * 2;
  ts.w += Sizes::text_padding * 2;
  layout.push_back(Rect(o, ts));
  o.y += ts.h;
  
  if (_running) {
    auto ins = _input->size();
    layout.push_back(Rect(Point(-ins.w - Sizes::group_indent, 0), ins + (Sizes::text_padding * 2)));
    auto outs = _output->size();
    layout.push_back(Rect(Point(ts.w + Sizes::group_indent, 0), outs + (Sizes::text_padding * 2)));
  }
  Layout::addSize(&layout, Size(ts.w, o.y));

  return layout;
  
}

void ProjectCode::layout() {

  _layout = calcLayout();
  _size = Layout::size(_layout);
  
}

void ProjectCode::render(Renderer &renderer, const Point &origin) {

//  cout << origin << endl;

  auto i = _layout.begin();
  _name.render(renderer, origin + (*i).origin);
  i++;
  renderer.renderFilledRect(*i + origin, Colours::straw);
  _transform->render(renderer, origin + (*i).origin + Size(Sizes::text_padding, Sizes::text_padding));
  i++;
  
  if (_running) {
    renderer.renderFilledRect(*i + origin, Colours::lightPlum);
    _input->render(renderer, origin + (*i).origin + Sizes::text_padding);
    i++;
    renderer.renderFilledRect(*i + origin, Colours::teaGreen);
    _output->render(renderer, origin + (*i).origin + Sizes::text_padding);
  }
}

Element *ProjectCode::hitTest(const Point &origin, const Point &p) { 

  auto i = _layout.begin();
  // skip name
  i++;
  
  auto hit = _transform->hitTest(origin + i->origin, p);
  if (hit) {
//    cout << "hit " << hit->describe() << endl;
    return hit;
  }
  i++;
  
  if (_running) {
    hit = _input->hitTest(origin + i->origin, p);
    if (hit) {
      return hit;
    }
    i++;
  }
  
  return super::hitTest(origin, p);
  
}


Point ProjectCode::localOrigin(Element *elem) {

  auto i = _layout.begin();
  // skip name
  i++;
  
  if (elem == _transform.get()) {
//    cout << "ProjectCode " << i->origin << endl;
    return i->origin + Sizes::text_padding;
  }
  i++;
  
  if (_running) {
    if (elem == _input.get()) {
      return i->origin + Sizes::text_padding;
    }
    i++;
    if (elem == _output.get()) {
      return i->origin + Sizes::text_padding;
    }
  }
  
  return Point(0, 0);
  
}

void ProjectCode::initHUD(HUD *hud) {

  _hudobj = hud->findMode("projectcode");
  
  Commandable::cast(_transform.get())->initHUD(hud);
  Commandable::cast(_input.get())->initHUD(hud);
  Commandable::cast(_output.get())->initHUD(hud);
  
}

void ProjectCode::setMode(Renderer &renderer, HUD *hud) {

  // we can run if we have an input context.
  hud->setFlag(renderer, canRun, !_running);
  
  hud->setMode(_hudobj);
  
}

void ProjectCode::processKey(Renderer &renderer, SDL_Keycode code) {

  if (renderer.processGlobalKey(code)) {
    return;
  }

  switch (code) {      
    case SDLK_R:
      _running = true;
      run(renderer);
      renderer.layout(root());
      break;

  }

}

void ProjectCode::run(Renderer &renderer) {

  auto in = Writeable::cast(_input.get())->getGeneric();
  auto t = Writeable::cast(_transform.get())->getGeneric();
  auto to = Generic::getObject(t);
  auto out = _flo->evalObj(in, *to);
  
  // clear out the output
  _output->destroy(renderer);
  
  // rebuild it.
  if (out) {
    _output = unique_ptr<Element>(Builder::walk(this, *out));
  }
  else {
    _output = unique_ptr<Element>(new List(false));
    _output->setParent(this);
  }
  renderer.build(_output.get());
  layout();
  
}

void ProjectCode::libChanged(Renderer &renderer, const std::vector<rfl::Generic> &library) {

  rfl::Object<rfl::Generic> lib;
  lib["library"] = library;
  _flo.reset(new Flo(lib));
  run(renderer);
}

void ProjectCode::changed(Renderer &renderer, Element *obj) {

  // test the transform.
  if (!_transform->visit([this, &renderer, obj](auto e) {
    if (e == obj) {
      run(renderer);
      return false;
    }
    return true;
  })) {
    return;
  }

  // test the input.
  _input->visit([this, &renderer, obj](auto e) {
    if (e == obj) {
      run(renderer);
      return false;
    }
    return true;
  });
  
}
