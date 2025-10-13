/*
  select.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "functions/select.hpp"

#include "transform.hpp"
#include "state.hpp"
#include "generic.hpp"

#include <boost/log/trivial.hpp>

using namespace flo;

optional<rfl::Generic> Select::exec(Transform &transform, State *state, rfl::Generic &closure) {
  
  BOOST_LOG_TRIVIAL(trace) << "select " << *Generic::getString(closure);

  auto a = Generic::getVector(closure);
  if (!a) {
    BOOST_LOG_TRIVIAL(error) << "closure not array";
    return nullopt;
  }
  
  // select the first function that returns something.
	for (auto i: *a) {
	  auto val = transform.exec(i, state);
	  if (val) {
	    return *val;
	  }
	}

  return nullopt;
    
}

shared_ptr<Function> Select::create() {

  return shared_ptr<Function>(new Select());
  
}
