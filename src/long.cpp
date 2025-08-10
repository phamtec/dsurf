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

Size Long::layout(Resources &res) {

  _size = _value.size();
  return _size;
  
}

void Long::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  _value.build(renderer, font);

}

void Long::render(Renderer &renderer, Resources &res, const Point &origin) {

  super::render(renderer, res, origin);
  
  _value.render(renderer, origin);
  
//  renderer.renderRect(_r);
  
}
