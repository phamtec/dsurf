/*
  dict.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "functions/dict.hpp"
#include "generic.hpp"
#include "state.hpp"

#include <boost/log/trivial.hpp>

using namespace flo;

optional<rfl::Generic> Dict::exec(Transform &transform, State *state, rfl::Generic &closure) {
  
//  BOOST_LOG_TRIVIAL(trace) << "Dict exec " << Generic::toString(closure);
  
  auto obj = Generic::getObject(closure);
  if (obj) {
    if (obj->size() == 0) {
      BOOST_LOG_TRIVIAL(trace) << "empty closure";
      if (state->hasColl()) {
        BOOST_LOG_TRIVIAL(trace) << "state is a vector";
        // we convert name value pairs in the vector to dictionary entries.
        auto v = state->getColl();
        rfl::Object<rfl::Generic> obj;
        for (auto i: v) {
          auto v2 = Generic::getVector(i);
          if (!v2 || v2->size() != 2) {
            BOOST_LOG_TRIVIAL(error) << "need vector of vectors (pairs)";
            return nullopt;
          }
          auto name = Generic::getString(*v2->begin());
          if (!name) {
            BOOST_LOG_TRIVIAL(error) << "first in pair must be name";
            return nullopt;
          }
          BOOST_LOG_TRIVIAL(trace) << *name << "=";
          obj[*name] = *(v2->begin() + 1);
        }
        return obj;
      }
      return nullopt;
    }
     
    return *obj;
  }
  
  // closure is not a dictionary.
  return nullopt;
  
}

shared_ptr<Function> Dict::create() {

  return shared_ptr<Function>(new Dict());
  
}
