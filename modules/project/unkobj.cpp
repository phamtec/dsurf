/*
  unkobj.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "unkobj.hpp"

#include "core.hpp"
#include "unicode.hpp"

using namespace std;

ProjectUnknownObj::ProjectUnknownObj(const std::string &name, rfl::Generic g): 
  _parent(0), _hudobj(-1), _g(g) {

  _name.set(Unicode::convert(name), Colours::white);
  
}

void ProjectUnknownObj::layout() {

  _size = _name.size();
  
}

void ProjectUnknownObj::build(Core &core) {

  _name.build(core);

}

void ProjectUnknownObj::render(Core &core, const Point &origin) {

  _name.render(core, origin);

//  core.renderRect(_r);

}

void ProjectUnknownObj::initHUD(HUD *hud) {

  _hudobj = hud->findMode("projectunkobj");
  
}

void ProjectUnknownObj::setMode(Core &core, HUD *hud) {

  hud->setMode(_hudobj);
  
}

void ProjectUnknownObj::processKey(Core &core, SDL_Keycode code) {

}

rfl::Generic ProjectUnknownObj::getGeneric() {

  return _g;

}

