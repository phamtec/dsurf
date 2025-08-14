/*
  dictprop.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "dictprop.hpp"
#include "renderer.hpp"
#include "resources.hpp"
#include "sizes.hpp"
#include "list.hpp"
#include "spatial.hpp"

#include <iostream>

using namespace std;

Size DictProp::layout(Resources &res) {

  _size = List::layoutVector(res, _name.size() + Size(Sizes::text_padding, 0), _objs);
  _size.h += _objs.size() == 0 ? Sizes::listgap - 10 : 0;
  return _size;

}

void DictProp::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  List::buildVector(renderer, font, _objs);

}

void DictProp::render(Renderer &renderer, Resources &res, const Point &origin) {

  renderer.renderFilledRect(Rect(origin + Size(Sizes::group_indent / 2, 0), Size(Sizes::toplinelength, Sizes::thickness)), Colours::plum);
  renderer.renderFilledRect(Rect(origin + Size(Sizes::group_indent / 2, 0), Size(Sizes::thickness, _size.h)), Colours::plum);
  renderer.renderFilledRect(Rect(origin + Size(Sizes::group_indent / 2, 0) + Size(0, _size.h - Sizes::thickness), Size(Sizes::bottomlinelength, Sizes::thickness)), Colours::plum);

  super::render(renderer, res, origin);
  
  List::renderVector(renderer, res, origin + Point(Sizes::group_indent, 60), _objs);

//  renderer.renderRect(_r);
  
}
