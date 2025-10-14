/*
  getmember.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "functions/getmember.hpp"

#include "transform.hpp"
#include "state.hpp"
#include "generic.hpp"

#include <boost/log/trivial.hpp>

using namespace flo;

optional<rfl::Generic> GetMember::exec(Transform &transform, State *state, rfl::Generic &closure) {

//  BOOST_LOG_TRIVIAL(trace) << "getmember " << Generic::toString(closure);

  auto obj = Generic::getObject(closure);
  if (!obj) {
    BOOST_LOG_TRIVIAL(error) << "closure not object";
    return nullopt;
  }
	if (!state->hasElem()) {
    BOOST_LOG_TRIVIAL(error) << "state has no elem";
    return nullopt;
	}
	
  auto name = Generic::getString(*obj, "name");
  if (!name) {
    BOOST_LOG_TRIVIAL(error) << "missing name";
    return nullopt;
  }
	auto elem = state->getElem();
	auto elemobj = Generic::getObject(elem);
	if (!elemobj) {
    BOOST_LOG_TRIVIAL(error) << "element is not object";
    return nullopt;
	}
	return (*elemobj)[*name];
    
}

shared_ptr<Function> GetMember::create() {

  return shared_ptr<Function>(new GetMember());
  
}
