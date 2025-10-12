/*
  string.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "functions/string.hpp"

#include "transform.hpp"
#include "generic.hpp"

#include <boost/log/trivial.hpp>

optional<rfl::Generic> String::exec(Transform &transform, State *state, rfl::Generic &closure) {

  BOOST_LOG_TRIVIAL(trace) << "string " << *Generic::getString(closure);

  return closure;
    
}

shared_ptr<Function> String::create() {

  return shared_ptr<Function>(new String());
  
}
