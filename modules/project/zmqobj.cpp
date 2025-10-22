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
#include "code.hpp"
#include "list.hpp"

#include <ranges>
#include <string_view>

using namespace std;

ProjectZMQObj::ProjectZMQObj(const string &name, const rfl::Object<rfl::Generic> &obj): 
  _parent(0), _hudobj(-1), _editing(true) {

  _name.set(Unicode::convert(name), Colours::white);

  rfl::Object<rfl::Generic> empty;

  auto library = Generic::getVector(obj, "library");    
  if (library) {
    _flo.reset(new Flo(*library));
  }
  else {
    _flo.reset(new Flo());
  }
  
  auto scenarios =  Generic::getVector(obj, "scenarios");

  auto remote = Generic::getObject(obj, "remote");
  if (remote) {
    _code.push_back(unique_ptr<Element>(new ProjectCode("Remote", *remote, library, findScenario(scenarios, "/remote"))));
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
    _code.push_back(unique_ptr<Element>(new ProjectCode("Remote", empty, library, empty)));
 }
  
  auto local = Generic::getObject(obj, "local");    
  if (local) {
    _code.push_back(unique_ptr<Element>(new ProjectCode("Local", *local, library, findScenario(scenarios, "/local"))));
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
    _code.push_back(unique_ptr<Element>(new ProjectCode("Local", empty, library, empty)));
 }
  
  auto o = Generic::getObject(obj, "send");
  if (o) {
    _code.push_back(unique_ptr<Element>(new ProjectCode("Send", *o, library, findScenario(scenarios, "/send"))));
    _send = *o;
  }
  else {
    _code.push_back(unique_ptr<Element>(new ProjectCode("Send", empty, library, empty)));
 }
  
  auto next =  Generic::getObject(obj, "next");
  if (next) {
    _code.push_back(unique_ptr<Element>(new ProjectCode("Next", *next, library, findScenario(scenarios, "/next"))));
    _next = *next;
  }
  else {
    _code.push_back(unique_ptr<Element>(new ProjectCode("Next", empty, library, empty)));
  }

  _libindex = _code.size();
  if (library) {
    _code.push_back(unique_ptr<Element>(Builder::walk(this, *library)));
  }
  else {
    _code.push_back(unique_ptr<Element>(new List(false)));
 }
  
  if (scenarios) {
    _code.push_back(unique_ptr<Element>(Builder::walk(this, *scenarios)));
  }
  else {
    _code.push_back(unique_ptr<Element>(new List(false)));
 }

  // make sure all the codes have their parent set.
  for_each(_code.begin(), _code.end(), [this](auto& e) {
    e->setParent(this);
  });
}

optional<rfl::Object<rfl::Generic> > ProjectZMQObj::findScenario(optional<vector<rfl::Generic> > scenarios, const string &path) {

  if (scenarios) {
    for (auto i: *scenarios) {
      auto obj = Generic::getObject(i);
      auto p = Generic::getString(obj, "path");
      if (p && *p == path) {
        return obj;
      }
    }
  }
  return nullopt;
  
}

RectList ProjectZMQObj::calcLayout() {

  // at least just the name.
  auto s = _name.size();
  RectList layout;
  layout.push_back(Rect(Point(), s));
  
  if (_editing) {
  
    // when editing, add in the size of the code.
    auto o = Point(Sizes::group_indent, s.h + Sizes::text_padding);
    float w = s.w;
    for_each(_code.begin(), _code.end(), [&layout, &o, &w](auto& e) {
      auto s = e->size();
      layout.push_back(Rect(o, s));
      o.y += s.h + Sizes::text_padding;
      if ((o.x + s.w) > w) {
        w = o.x + s.w;
      }
    });
    w += Sizes::listgap;
    Layout::addSize(&layout, Size(w, o.y));
    
  }
  else {
    Layout::addSize(&layout, s + Size(Sizes::listgap, 0));
  }

  return layout;
  
}

void ProjectZMQObj::layout() {

  // make sure the code is layed out.
  for_each(_code.begin(), _code.end(), [](auto& e) {
    e->layout();
  });
  
  // calculate the layout.
  _layout = calcLayout();
  _size = Layout::size(_layout);
  
}

void ProjectZMQObj::build(Renderer &renderer) {

  _name.build(renderer);

}

void ProjectZMQObj::render(Renderer &renderer, const Point &origin) {

//  renderer.renderLayout(origin, _layout);

  auto i = _layout.begin();
  _name.render(renderer, origin + (*i).origin);
  i++;
  
  if (_editing) {
    for_each(_code.begin(), _code.end(), [&renderer, &i, origin](auto& e) {
      renderer.renderFilledRect(*i + origin, Colours::white);
      e->render(renderer, origin + i->origin);
      i++;
    });
  }

}

Element *ProjectZMQObj::hitTest(const Point &origin, const Point &p) { 

  auto i = _layout.begin();
  // skip name
  i++;
  
  if (_editing) {
    for (auto& j: _code) {
      Element *hit = j->hitTest(origin + i->origin, p);
      if (hit) {
        return hit;
      }
      i++;
    }
  }
  
  return super::hitTest(origin, p);
  
}

Point ProjectZMQObj::localOrigin(Element *elem) {

  auto i = _layout.begin();
  // skip name
  i++;
  
  if (_editing) {
    for (auto& j: _code) {
      if (j.get() == elem) {
//        cout << "ProjectZMQObj " << i->origin << endl;
        return i->origin;
      }
      i++;
    }
  }
  
  return Point(0, 0);
  
}

void ProjectZMQObj::initHUD(HUD *hud) {

  _hudobj = hud->findMode("projectzmqobj");
  
  // and walk the list. order isn't important.
  for_each(_code.begin(), _code.end(), [hud](auto& e) { 
    Commandable::cast(e.get())->initHUD(hud);
  });

}

void ProjectZMQObj::setMode(Renderer &renderer, HUD *hud) {

  hud->setFlag(renderer, canEdit, !_editing);
  
  hud->setMode(_hudobj);
  
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
      root()->layout();
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

bool ProjectZMQObj::visit(function<bool (Element *)> f) {

  if (!f(this)) {
    return false;
  }
  if (_editing) {
    for (auto& e: _code) {
      if (!e->visit(f)) {
        return false;
      }
    }
  }
  return true;
  
}

void ProjectZMQObj::changed(Renderer &renderer, Element *obj) {

  // test the library.
  auto lib = _code[_libindex].get();
  if (!lib->visit([this, &renderer, obj, lib](auto e) {
    if (e == obj) {
      auto v = Generic::getVector(Writeable::cast(lib)->getGeneric());
      if (!v) {
        cerr << "lib is not a vector!" << endl;
        return false;
      }
      for_each(_code.begin(), _code.end(), [&renderer, v](auto& e) {
        auto code = dynamic_cast<ProjectCode *>(e.get());
        if (code) {
          code->libChanged(renderer, *v);
//          code->run(renderer);
        }
      });
      return false;
    }
    return true;
  })) {
    return;
  }
  
}

