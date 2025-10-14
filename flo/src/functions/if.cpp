/*
  if.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "functions/if.hpp"

#include "transform.hpp"
#include "generic.hpp"

#include <boost/log/trivial.hpp>

using namespace flo;

optional<rfl::Generic> If::exec(Transform &transform, State *state, rfl::Generic &closure) {
  
//  BOOST_LOG_TRIVIAL(trace) << "if " << Generic::toString(closure);

  auto obj = Generic::getObject(closure);
  if (!obj) {
    BOOST_LOG_TRIVIAL(trace) << "closure not object";
    return nullopt;
  }
  auto p = Generic::getGeneric(obj, "p");
  if (!p) {
    BOOST_LOG_TRIVIAL(error) << "no p";
    return nullopt;
  }
  auto result = transform.exec(*p, state);
  if (!result) {
    return nullopt;
  }
  auto b = Generic::getBool(*result);
  if (b && *b) {
    auto then = Generic::getGeneric(obj, "then");
    if (!then) {
      BOOST_LOG_TRIVIAL(error) << "then not object";
      return nullopt;
    }
    return transform.exec(*then, state);
  }
  
  return nullopt;
  
}

shared_ptr<Function> If::create() {

  return shared_ptr<Function>(new If());
  
}
