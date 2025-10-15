/*
  cur.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 15-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "functions/cur.hpp"

#include "transform.hpp"
#include "state.hpp"

#include <boost/log/trivial.hpp>

using namespace flo;

optional<rfl::Generic> Cur::exec(Transform &transform, State *state, rfl::Generic &closure) {
  
  if (state->hasElem()) {
    return state->getElem();
  }
  if (state->hasColl()) {
    return state->getColl();
  }

  return nullopt;
    
}

shared_ptr<Function> Cur::create() {

  return shared_ptr<Function>(new Cur());
  
}
