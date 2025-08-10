/*
  long.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "long.hpp"
#include "sizes.hpp"
#include "spatial.hpp"
#include "renderer.hpp"

#include <sstream>

using namespace std;

Long::Long(long value) {

  stringstream ss;
  ss << value;
  _value.set(ss.str(), Colours::green);
  
}

Size Long::layout(Resources &res, const Point &origin) {

  _r.origin = origin;
  _r.size = _value.size();
  return _r.size;
  
}

void Long::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  _value.build(renderer, font);

}

void Long::render(Renderer &renderer, Resources &res) {

  super::render(renderer, res);
  
  _value.render(renderer, _r.origin);
  
//  renderer.renderRect(_r);
  
}
