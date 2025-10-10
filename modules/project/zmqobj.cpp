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

using namespace std;

ProjectZMQObj::ProjectZMQObj(const std::string &name, const rfl::Object<rfl::Generic> &obj): 
  _parent(0), _hudobj(-1) {

  _name.set(Unicode::convert(name), Colours::white);
  
  auto remote = Builder::getObject(obj, "remote");
  if (remote) {
    auto s = Builder::getString(remote, "address");
    if (s) {
      _remoteAddress = *s;
    }
    s = Builder::getString(remote, "public");
    if (s) {
      _remotePubKey = *s;
    }
    auto i = Builder::getNum(remote, "port");
    if (i) {
      _remotePort = *i;
    }
  }
  auto local = Builder::getObject(obj, "local");    
  if (local) {
    auto s = Builder::getString(local, "uuid");
    if (s) {
      _uuid = *s;
    }
    s = Builder::getString(local, "private");
    if (s) {
      _privateKey = *s;
    }
    s = Builder::getString(local, "public");
    if (s) {
      _publicKey = *s;
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
    case SDLK_L:
      load(renderer);
      break;
  }

}

void ProjectZMQObj::load(Renderer &renderer) {

  cout << "connecting to " << _remoteAddress << ":" << _remotePort << endl;
  
  renderer.setupRemote(_remoteAddress, _remotePort, _remotePubKey, _privateKey, _publicKey);

  // send it an online message.
  Renderer::OnlineMsg msg;
  msg.type = "online";
  msg.src = _uuid;
  msg.build = "32999";
  msg.headerTitle = "DSurfer";
  msg.pubKey = _publicKey;
  msg.synced = true;
  msg.mirror = false;
  renderer.onlineSend(msg);

}
