/*
  fileobj.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 2-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "fileobj.hpp"

#include "renderer.hpp"
#include "unicode.hpp"

using namespace std;

ProjectFileObj::ProjectFileObj(const std::string &name, const std::string &fullfilename, const std::string &filename): 
  _parent(0), _fullfilename(fullfilename), _filename(filename), _hudobj(-1) {

  _name.set(Unicode::convert(name), Colours::white);
  
}

void ProjectFileObj::layout() {

  _size = _name.size();
  
}

void ProjectFileObj::build(Renderer &renderer) {

  _name.build(renderer);

}

void ProjectFileObj::render(Renderer &renderer, const Point &origin) {

  _name.render(renderer, origin);

//  renderer.renderRect(_r);

}

void ProjectFileObj::initHUD(HUD *hud) {

  _hudobj = hud->findMode("projectfileobj");
  
}

void ProjectFileObj::setMode(Renderer &renderer, HUD *hud) {

  hud->setMode(_hudobj);
  
}

void ProjectFileObj::processKey(Renderer &renderer, SDL_Keycode code) {

  if (renderer.processGlobalKey(code)) {
    return;
  }
  
  switch (code) {      
    case SDLK_L:
      renderer.addFile(_fullfilename, false);
      break;
  }

}

rfl::Generic ProjectFileObj::getGeneric() {

  rfl::Object<rfl::Generic> obj;

  // build a code object from the parts
  obj["name"] = Unicode::convert(_name.str());
  obj["file"] = _filename;
  
  return obj;

}

