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
using flo::Generic;

ProjectZMQObj::ProjectZMQObj(const string &name, const rfl::Object<rfl::Generic> &obj): 
  _parent(0), _hudobj(-1), _editing(false) {

  _name.set(Unicode::convert(name), Colours::white);

  rfl::Object<rfl::Generic> empty;

  auto library = Generic::getVector(obj, "library");    

  _flo.reset(new Flo(obj));
  
  auto scenarios =  Generic::getVector(obj, "scenarios");

  ProjectCode *code = nullptr;
  auto remote = Generic::getObject(obj, "remote");
  if (remote) {
    auto obj = _flo->evalObj(empty, *remote);
    if (obj) {
      code = new ProjectCode("remote", *remote, library, findScenario(scenarios, "/remote"));
      auto s = Generic::getString(obj, "address");
      if (s) {
        _remoteAddress = *s;
      }
      s = Generic::getString(obj, "public");
      if (s) {
        _remotePubKey = *s;
      }
      auto i = Generic::getNum(obj, "port");
      if (i) {
        _remotePort = *i;
      }
    }
  }
  if (!code) {
    code = new ProjectCode("remote", empty, library, empty);
  }
  _code.push_back(unique_ptr<Element>(code));
  
  code = nullptr;
  auto local = Generic::getObject(obj, "local");    
  if (local) {
    auto obj = _flo->evalObj(empty, *local);
    if (obj) {
      code = new ProjectCode("local", *local, library, findScenario(scenarios, "/local"));
      auto s = Generic::getString(obj, "uuid");
      if (s) {
        _uuid = *s;
      }
      s = Generic::getString(obj, "private");
      if (s) {
        _privateKey = *s;
      }
      s = Generic::getString(obj, "public");
      if (s) {
        _publicKey = *s;
      }
    }
  }
  if (!code) {
    code = new ProjectCode("local", empty, library, empty);
  }
  _code.push_back(unique_ptr<Element>(code));
  
  // send and next are late binding so we just have the code for them.
  auto o = Generic::getObject(obj, "send");
  if (o) {
    _code.push_back(unique_ptr<Element>(new ProjectCode("send", *o, library, findScenario(scenarios, "/send"))));
    _send = *o;
  }
  else {
    _code.push_back(unique_ptr<Element>(new ProjectCode("send", empty, library, empty)));
 }
  
  auto next =  Generic::getObject(obj, "next");
  if (next) {
    _code.push_back(unique_ptr<Element>(new ProjectCode("next", *next, library, findScenario(scenarios, "/next"))));
    _next = *next;
  }
  else {
    _code.push_back(unique_ptr<Element>(new ProjectCode("next", empty, library, empty)));
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
      renderer.layout(root());
      break;
  }

}

void ProjectZMQObj::load(Renderer &renderer) {

  if (_send.size() == 0) {
    cerr << "no send" << endl;
    return;
  }

  cout << "connecting to " << _remoteAddress << ":" << _remotePort << endl;
  
  renderer.connectRemote(_remoteAddress, _remotePort, _remotePubKey, _privateKey, _publicKey, _flo, _send, _next);
  
}

bool ProjectZMQObj::visit(function<bool (Element *)> f) {

  for (auto& e: _code) {
    if (!e->visit(f)) {
      return false;
    }
  }
  return f(this);
  
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

rfl::Generic ProjectZMQObj::getGeneric() {

  rfl::Object<rfl::Generic> obj;

  // build a code object from the parts
  obj["name"] = Unicode::convert(_name.str());
  
  // collect all the writeables.
  rfl::Generic library;
  rfl::Generic scenarios;
  vector<Writeable *> code;
  for (int i=0; i<_code.size(); i++) {
    auto e = _code[i].get();
    auto writeable = dynamic_cast<Writeable *>(e);
    if (!writeable) {
      cerr << "something not writeable!" << endl;
      continue;
    }
    if (i < _libindex) {
      code.push_back(writeable);
    }
    else if (i == _libindex) {
      library = writeable->getGeneric();  
    }
    else {
      scenarios = writeable->getGeneric();  
    }
  }
  
  rfl::Object<rfl::Generic> zmq;
  zmq["library"] = library;
  for (auto i: code) {
    zmq[i->getName()] = i->getGeneric();
  }
  zmq["scenarios"] = scenarios;  
  obj["zmq"] = zmq;
  
  return obj;
}

