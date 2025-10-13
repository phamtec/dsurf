/*
  equal.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "functions/equal.hpp"

#include "transform.hpp"
#include "generic.hpp"

#include <boost/log/trivial.hpp>

using namespace flo;

optional<rfl::Generic> Equal::exec(Transform &transform, State *state, rfl::Generic &closure) {

  BOOST_LOG_TRIVIAL(trace) << "exec " << *Generic::getString(closure);

  auto a = Generic::getVector(closure);
  if (!a) {
    BOOST_LOG_TRIVIAL(error) << "closure not array";
    return nullopt;
  }
  
	auto i = a->begin();
	auto head = *i;
	auto lvalue = transform.exec(head, state);
	i++;
	while (i != a->end()) {
		auto rvalue = transform.exec(*i, state);
		if (!lvalue || !rvalue || (Generic::getString(*lvalue) != Generic::getString(*rvalue))) {
			return false;
		}
		i++;
	}
  return true;
    
}

shared_ptr<Function> Equal::create() {

  return shared_ptr<Function>(new Equal());
  
}
