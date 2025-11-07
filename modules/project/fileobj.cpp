/*
  fileobj.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 2-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "fileobj.hpp"

#include "core.hpp"
#include "unicode.hpp"

using namespace std;

ProjectFileObj::ProjectFileObj(const std::string &name, const std::string &fullfilename, const std::string &filename): 
  _parent(0), _fullfilename(fullfilename), _filename(filename), _hudobj(-1) {

  _name.set(Unicode::convert(name), Colours::white);
  
}

void ProjectFileObj::layout() {

  _size = _name.size();
  
}

void ProjectFileObj::build(Core &core) {

  _name.build(core);

}

void ProjectFileObj::render(Core &core, const Point &origin) {

  _name.render(core, origin);

//  core.renderRect(_r);

}

void ProjectFileObj::initHUD(HUD *hud) {

  _hudobj = hud->findMode("projectfileobj");
  
}

void ProjectFileObj::setMode(Core &core, HUD *hud) {

  hud->setMode(_hudobj);
  
}

void ProjectFileObj::processKey(Core &core, SDL_Keycode code) {

  if (core.processGlobalKey(code)) {
    return;
  }
  
  switch (code) {      
    case SDLK_L:
      core.addFile(_fullfilename, false);
      break;
  }

}

DictG ProjectFileObj::getGeneric() {

  DictO obj;

  // build a code object from the parts
  obj["name"] = Unicode::convert(_name.str());
  obj["file"] = _filename;
  
  return obj;

}

