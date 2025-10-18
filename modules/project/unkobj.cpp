/*
  unkobj.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "unkobj.hpp"

#include "renderer.hpp"
#include "unicode.hpp"

using namespace std;

ProjectUnknownObj::ProjectUnknownObj(const std::string &name): 
  _parent(0), _hudobj(-1) {

  _name.set(Unicode::convert(name), Colours::white);
  
}

void ProjectUnknownObj::layout() {

  _size = _name.size();
  
}

void ProjectUnknownObj::build(Renderer &renderer) {

  _name.build(renderer);

}

void ProjectUnknownObj::render(Renderer &renderer, const Point &origin) {

  _name.render(renderer, origin);

//  renderer.renderRect(_r);

}

void ProjectUnknownObj::initHUD(HUD *hud) {

  _hudobj = hud->findMode("projectunkobj");
  
}

void ProjectUnknownObj::setMode(Renderer &renderer, HUD *hud) {

  hud->setMode(_hudobj);
  
}

void ProjectUnknownObj::processKey(Renderer &renderer, SDL_Keycode code) {

}
