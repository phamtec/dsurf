/*
  transform.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "transform.hpp"

#include "functions.hpp"
#include "generic.hpp"

#include <boost/log/trivial.hpp>
#include <rfl/json.hpp>

using namespace flo;

Transform::Transform(Functions &functions) : _functions(functions) {
}

optional<rfl::Generic> Transform::exec(const rfl::Generic &closure, State *state) {

  BOOST_LOG_TRIVIAL(trace) << "exec " << *Generic::getString(closure);

  auto obj = Generic::getObject(closure);
  if (obj) {
    if (obj->size() == 0) {
      BOOST_LOG_TRIVIAL(trace) << "empty closure is terminal";
      return nullopt; // treat like a terminal.
    }
    
    // get first nvp in closure
    auto first = obj->begin();
    
    string name = get<0>(*first);
    BOOST_LOG_TRIVIAL(trace) << "first " << name;
    
    if (_functions.hasNative(name)) {
    
      // get function representing name.
      auto f = _functions.getNative(name);
      
      // call down with the closure.
      return f->exec(*this, state, get<1>(*first));
      
    }
    else if (_functions.hasLibrary(name)) {

      // get transform representing name.
      auto t = _functions.getLibrary(name);
      
      // recursively call down
      return exec(t, state);
      
    }
    else {
      return error("function " + name + " not found");
    }
    
  }
  return nullopt;
  
}

rfl::Generic Transform::error(const string &msg) const {

  BOOST_LOG_TRIVIAL(error) << msg;
  return *rfl::json::read<rfl::Generic>("{\"error\":\"" + msg + "\"}");
  
}
