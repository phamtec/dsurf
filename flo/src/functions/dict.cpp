/*
  dict.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "functions/dict.hpp"

using namespace flo;

optional<rfl::Generic> Dict::exec(Transform &transform, State *state, rfl::Generic &closure) {
  
  // the closure is a dictionary.
  return closure;
  
}

shared_ptr<Function> Dict::create() {

  return shared_ptr<Function>(new Dict());
  
}
