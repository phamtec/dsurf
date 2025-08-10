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

#include <sstream>

using namespace std;

Long::Long(long value) {

  stringstream ss;
  ss << value;
  _value.set(ss.str(), Colours::green);
  
}

float Long::layout(Resources &res, const Point &origin) {

  _r.origin = origin;
  _r.size = Size(100, _value.size().h);
  return _r.size.h;
  
}

void Long::build(Renderer &renderer, Font &font) {

  super::build(renderer, font);
  
  _value.build(renderer, font);

}

void Long::render(Renderer &renderer, Resources &res) {

  super::render(renderer, res);
  
  _value.render(renderer, _r.origin);
  
}
