/*
  processor.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "processor.hpp"

#include "functions.hpp"
#include "state.hpp"
#include "transform.hpp"
#include "generic.hpp"

#include <boost/log/trivial.hpp>
#include <iostream>
#include <rfl/json.hpp>

using namespace flo;

Processor::Processor(istream &infile, Functions &functions):
  _functions(functions) {
  
  auto g = rfl::json::read<rfl::Generic>(infile);
  if (!g) {
	  BOOST_LOG_TRIVIAL(error) << g.error().what();
  }
  _json = *g;
  
}

Processor::Processor(const rfl::Generic &json, Functions &functions):
  _json(json), _functions(functions) {
  
}

Processor::Processor(Functions &functions): _functions(functions) {
}

optional<rfl::Generic> Processor::transform(const rfl::Generic &transform) {

  auto tr = Generic::getObject(transform);
  if (!tr) {
    // if a number, just return that.
    auto num = Generic::getNum(transform);
    if (num) {
      return *num;
    }
    // if a string, just return that.
    auto s = Generic::getString(transform);
    if (s) {
      return *s;
    }
    // if a bool, just return that.
    auto b = Generic::getBool(transform);
    if (b) {
      return *b;
    }
	  BOOST_LOG_TRIVIAL(error) << "transform not object, string, bool or num: " << Generic::toString(transform);
	  return nullopt;
  }
  if (tr->size() == 0) {
	  BOOST_LOG_TRIVIAL(error) << "object is empty";
	  return nullopt;
  }
  
  Transform t(_functions);
  State s;
  auto obj = Generic::getObject(_json);
  if (obj) {
    s.setElem(*obj);
  }
  return t.exec(transform, &s);

}

optional<rfl::Generic> Processor::transform(istream &s) {

  auto g = rfl::json::read<rfl::Generic>(s);
  if (!g) {
	  BOOST_LOG_TRIVIAL(error) << g.error().what();
  }

//	BOOST_LOG_TRIVIAL(trace) << "transforming with " << tj;
  return transform(*g);

}

void Processor::pretty_print( ostream& os, rfl::Generic const& jv) {

  auto s = rfl::json::write(jv, rfl::json::pretty);
  os << s;

}
