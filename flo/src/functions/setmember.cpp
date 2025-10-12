/*
  setmember.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "functions/setmember.hpp"

#include "transform.hpp"
#include "state.hpp"
#include "generic.hpp"

#include <boost/log/trivial.hpp>

optional<rfl::Generic> SetMember::exec(Transform &transform, State *state, rfl::Generic &closure) {

  BOOST_LOG_TRIVIAL(trace) << "setmember " << *Generic::getString(closure);

  auto obj = Generic::getObject(closure);
  if (!obj) {
    BOOST_LOG_TRIVIAL(error) << "closure not object";
    return nullopt;
  }
	if (!state->hasElem()) {
    BOOST_LOG_TRIVIAL(error) << "state has no elem";
    return nullopt;
	}

  auto name = Generic::getString(obj, "name");
  if (!name) {
    BOOST_LOG_TRIVIAL(error) << "missing name";
    return nullopt;
  }
  auto v = Generic::getGeneric(obj, "value");
  if (!v) {
    BOOST_LOG_TRIVIAL(error) << "missing value";
    return nullopt;
  }
	auto elem = state->getElem();
  auto value = transform.exec(*v, state);
  if (value) {
    elem[*name] = *value;
  }
  else {
    cerr << "not sure how to erase an element in Object" << endl;
  }
	
  return elem;
    
}

shared_ptr<Function> SetMember::create() {

  return shared_ptr<Function>(new SetMember());
  
}
