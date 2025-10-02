/*
  root.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 2-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "root.hpp"

#include "renderer.hpp"
#include "unicode.hpp"
#include "sizes.hpp"

using namespace std;

ProjectRoot::ProjectRoot(const std::string &name, std::vector<Element *> &objs): _parent(0) {

  _name.set(Unicode::convert(name), Colours::black);
  
  transform(objs.begin(), objs.end(), back_inserter(_objs), [this](auto e) {
    e->setParent(this);
    return unique_ptr<Element>(e);
  });
}

Size ProjectRoot::layout() {

  _size = _name.size();
  for_each(_objs.begin(), _objs.end(), [this](auto& e) {
    _size.h += e->layout().h + Sizes::text_padding;
  });
  return _size;
  
}

void ProjectRoot::build(Renderer &renderer) {

  _name.build(renderer);
  for_each(_objs.begin(), _objs.end(), [&renderer](auto& e) {
    e->build(renderer);
  });
}

void ProjectRoot::render(Renderer &renderer, const Point &origin) {

  _name.render(renderer, origin);

  float y = origin.y;
  for (auto& i: _objs) {
    i->render(renderer, origin + Size(Sizes::group_indent, y));
    y += i->size().h + Sizes::text_padding;
  }
  
//  renderer.renderRect(_r);

}
