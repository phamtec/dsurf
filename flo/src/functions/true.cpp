/*
  true.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "functions/true.hpp"

#include "generic.hpp"

#include <boost/log/trivial.hpp>

using namespace flo;

optional<rfl::Generic> True::exec(Transform &transform, State *state, rfl::Generic &closure) {
  
//  BOOST_LOG_TRIVIAL(trace) << "true " << Generic::toString(closure);

  return true;
  
}

shared_ptr<Function> True::create() {

  return shared_ptr<Function>(new True());
  
}
