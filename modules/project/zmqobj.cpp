/*
  zmqobj.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "zmqobj.hpp"

#include "renderer.hpp"
#include "unicode.hpp"
#include "builder.hpp"
#include "flo.hpp"
#include "generic.hpp"
#include "sizes.hpp"

#include <ranges>
#include <string_view>

using namespace std;

ProjectZMQObj::ProjectZMQObj(const std::string &name, const rfl::Object<rfl::Generic> &obj): 
  _parent(0), _hudobj(-1) {

  _name.set(Unicode::convert(name), Colours::white);

  rfl::Object<rfl::Generic> empty;

  _subheadings.push_back(make_unique<Text>(L"Scenarios", Colours::white));
  auto v =  Generic::getVector(obj, "scenarios");
  if (v) {
    _source.push_back(*v);
  }
  else {
    _source.push_back(empty);
  }

  auto library = Generic::getVector(obj, "library");    
  if (library) {
    _flo.reset(new Flo(*library));
  }
  else {
    _flo.reset(new Flo());
  }
  
  _subheadings.push_back(make_unique<Text>(L"Remote", Colours::white));
  auto remote = Generic::getObject(obj, "remote");
  if (remote) {
    _source.push_back(*remote);
    auto s = _flo->evalStringMember(remote, "address");
    if (s) {
      _remoteAddress = *s;
    }
    s = _flo->evalStringMember(remote, "public");
    if (s) {
      _remotePubKey = *s;
    }
    auto i = _flo->evalNumMember(remote, "port");
    if (i) {
      _remotePort = *i;
    }
  }
  else {
    _source.push_back(empty);
  }
  
  _subheadings.push_back(make_unique<Text>(L"Local", Colours::white));
  auto local = Generic::getObject(obj, "local");    
  if (local) {
    _source.push_back(*local);
    auto s = _flo->evalStringMember(local, "uuid");
    if (s) {
      _uuid = *s;
    }
    s = _flo->evalStringMember(local, "private");
    if (s) {
      _privateKey = *s;
    }
    s = _flo->evalStringMember(local, "public");
    if (s) {
      _publicKey = *s;
    }
  }
  else {
    _source.push_back(empty);
  }
  
  _subheadings.push_back(make_unique<Text>(L"Send", Colours::white));
  auto o = Generic::getObject(obj, "send");
  if (o) {
    _source.push_back(*o);
    _send = *o;
  }
  else {
    _source.push_back(empty);
  }
  
  _subheadings.push_back(make_unique<Text>(L"Next", Colours::white));
  o =  Generic::getObject(obj, "next");
  if (o) {
    _source.push_back(*o);
    _next = *o;
  }
  else {
    _source.push_back(empty);
  }

  _subheadings.push_back(make_unique<Text>(L"Library", Colours::white));
  if (library) {
    _source.push_back(*library);
  }
  else {
    _source.push_back(empty);
  }
  
}

RectList ProjectZMQObj::calcLayout() {

  // at least just the name.
  auto s = _name.size();
  RectList layout;
  layout.push_back(Rect(Point(), s));
  
  if (_editing) {
  
    // when editing, all the headings and subobjects.
    auto o = Point(Sizes::group_indent, s.h + Sizes::text_padding);
    auto z = std::ranges::views::zip(_subheadings, _code);
    float w = s.w;
    for_each(z.begin(), z.end(), [&layout, &o, &w](auto e) {
      auto s = get<0>(e)->size();
      layout.push_back(Rect(o, s));
      o.y += s.h + Sizes::text_padding;
      s = get<1>(e)->size();
      layout.push_back(Rect(o, s));
      o.y += s.h + Sizes::text_padding;
      if ((o.x + s.w) > w) {
        w = o.x + s.w;
      }
    });
    Layout::addSize(&layout, Size(w, o.y));
    
  }
  else {
    Layout::addSize(&layout, s);
  }

  return layout;
  
}

void ProjectZMQObj::layout() {

  // make sure the objects are layed out.
  if (_editing) {
    for_each(_code.begin(), _code.end(), [](auto& e) {
      e->layout();
    });
  }
  
  // calculate the layout.
  _layout = calcLayout();
  _size = Layout::size(_layout);
  
}

void ProjectZMQObj::build(Renderer &renderer) {

  _name.build(renderer);
  for_each(_subheadings.begin(), _subheadings.end(), [&renderer](auto& e) {
    e->build(renderer);
  });

}

void ProjectZMQObj::destroy(Renderer &renderer) {

  for_each(_code.begin(), _code.end(), [&renderer](auto& e) {
    e->destroy(renderer);
  });

}

void ProjectZMQObj::render(Renderer &renderer, const Point &origin) {

//  renderer.renderLayout(origin, _layout);

  auto i = _layout.begin();
  _name.render(renderer, origin + (*i).origin);
  i++;
  
  if (_editing) {
    for (auto elem: std::ranges::views::zip(_subheadings, _code)) {
      // render subheading.
      get<0>(elem)->render(renderer, origin + i->origin);
      i++;
      // render code.
      renderer.renderFilledRect(*i + origin, Colours::white);
      get<1>(elem)->render(renderer, origin + i->origin);
      i++;
    }
  }

}

void ProjectZMQObj::initHUD(HUD *hud) {

  _hudobj = hud->findMode("projectzmqobj");
  
}

void ProjectZMQObj::setMode(Renderer &renderer, HUD *hud) {

  hud->setMode(_hudobj);
  
}

void ProjectZMQObj::createObjects(Renderer &renderer) {

  // build all the source into elements.
  transform(_source.begin(), _source.end(), back_inserter(_code), [this](auto e) {
    return unique_ptr<Element>(Builder::walk(this, e));
  });
  
  // build all the code we just made.
  for_each(_code.begin(), _code.end(), [&renderer](auto& e) {
    e->build(renderer);
  });
  
  // layout the screen.
  root()->layout();

}

void ProjectZMQObj::processKey(Renderer &renderer, SDL_Keycode code) {

  if (renderer.processGlobalKey(code)) {
    return;
  }

  switch (code) {      
    case SDLK_C:
      load(renderer);
      break;

    case SDLK_E:
      _editing = true;
      createObjects(renderer);
      break;
  }

}

void ProjectZMQObj::load(Renderer &renderer) {

  if (_send.size() == 0 || _next.size() == 0) {
    cerr << "no send or next" << endl;
    return;
  }

  cout << "connecting to " << _remoteAddress << ":" << _remotePort << endl;
  
  if (renderer.setupRemote(_remoteAddress, _remotePort, _remotePubKey, _privateKey, _publicKey)) {
    
    // we hand off our flo here.
    renderer.startRemote(_flo, _send, _next);
    
  }
  
}
