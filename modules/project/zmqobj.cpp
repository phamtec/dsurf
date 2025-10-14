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

using namespace std;

ProjectZMQObj::ProjectZMQObj(const std::string &name, const rfl::Object<rfl::Generic> &obj): 
  _parent(0), _hudobj(-1) {

  _name.set(Unicode::convert(name), Colours::white);
  
  auto library = Generic::getVector(obj, "library");    
  if (library) {
    _flo.reset(new Flo(*library));
  }
  else {
    _flo.reset(new Flo());
  }
  auto remote = Generic::getObject(obj, "remote");
  if (remote) {
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
  auto local = Generic::getObject(obj, "local");    
  if (local) {
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
  auto connect = Generic::getObject(obj, "connect");    
  if (connect) {
    auto o = _flo->evalObjMember(connect, "send");
    if (o) {
      _send = *o;
    }
    o =  _flo->evalObjMember(connect, "next");
    if (o) {
      _next = *o;
    }
  }
}

Size ProjectZMQObj::layout() {

  _size = _name.size();
  return _size;
  
}

void ProjectZMQObj::build(Renderer &renderer) {

  _name.build(renderer);

}

void ProjectZMQObj::render(Renderer &renderer, const Point &origin) {

  _name.render(renderer, origin);

//  renderer.renderRect(_r);

}

void ProjectZMQObj::initHUD(HUD *hud) {

  _hudobj = hud->findMode("projectzmqobj");
  
}

void ProjectZMQObj::setMode(Renderer &renderer, HUD *hud) {

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
  }

}

void ProjectZMQObj::load(Renderer &renderer) {

  if (_send.size() == 0 || _next.size() == 0) {
    cerr << "no send or next" << endl;
    return;
  }

  cout << "connecting to " << _remoteAddress << ":" << _remotePort << endl;
  
  if (renderer.setupRemote(_remoteAddress, _remotePort, _remotePubKey, _privateKey, _publicKey)) {
    renderer.sendRemote(_send, _next);
  }
  
}
