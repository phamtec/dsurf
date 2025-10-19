/*
  code.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 19-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "code.hpp"

#include "builder.hpp"
#include "sizes.hpp"
#include "unicode.hpp"
#include "renderer.hpp"

using namespace std;

ProjectCode::ProjectCode(const string &name, rfl::Generic source) {

  _source = source;
  _obj = unique_ptr<Element>(Builder::walk(this, _source));
  _name.set(Unicode::convert(name), Colours::white);
  
}

void ProjectCode::build(Renderer &renderer) {

  _name.build(renderer);
  _obj->build(renderer);
  
}

void ProjectCode::destroy(Renderer &renderer) {

  _obj->destroy(renderer);
  
}

RectList ProjectCode::calcLayout() {

  // at least just the name.
  auto s = _name.size();
  RectList layout;
  auto o = Point();
  layout.push_back(Rect(o, s));
  
  o.y += s.h + Sizes::text_padding;
  s = _obj->size();
  layout.push_back(Rect(o, s));
  o.y += s.h;
  Layout::addSize(&layout, Size(s.w, o.y));

  return layout;
  
}

void ProjectCode::layout() {

  _obj->layout();
  
  // calculate the layout.
  _layout = calcLayout();
  _size = Layout::size(_layout);
  
}

void ProjectCode::render(Renderer &renderer, const Point &origin) {

  auto i = _layout.begin();
  _name.render(renderer, origin + (*i).origin);
  i++;
  renderer.renderFilledRect(*i + origin, Colours::white);
  _obj->render(renderer, origin + (*i).origin);
  
}
