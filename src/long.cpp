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

Size Long::layout() {

  _size = _value.size();
  return _size;
  
}

void Long::build(Renderer &renderer) {

  super::build(renderer);
  
  _value.build(renderer);

}

void Long::render(Renderer &renderer, const Point &origin) {

  super::render(renderer, origin);
  
  _value.render(renderer, origin);
  
//  renderer.renderRect(_r);
  
}

rfl::Generic Long::getGeneric() { 
  
  stringstream ss(_value.str());
  long l;
  ss >> l;

  return l;

}

