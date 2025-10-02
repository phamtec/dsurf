/*
  obj.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 2-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "obj.hpp"

#include "renderer.hpp"
#include "unicode.hpp"

using namespace std;

ProjectObj::ProjectObj(const std::string &name, const std::string &filename): _parent(0), _filename(filename) {

  _name.set(Unicode::convert(name), Colours::black);
  
}

Size ProjectObj::layout() {

  _size = _name.size();
  return _size;
  
}

void ProjectObj::build(Renderer &renderer) {

  _name.build(renderer);

}

void ProjectObj::render(Renderer &renderer, const Point &origin) {

  _name.render(renderer, origin);

//  renderer.renderRect(_r);

}
