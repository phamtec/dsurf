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

Processor::Processor(istream &infile, Functions &functions):
  _functions(functions) {
  
  auto g = rfl::json::read<rfl::Generic>(infile);
  if (!g) {
	  BOOST_LOG_TRIVIAL(error) << g.error().what();
  }
  _json = *g;
  
}

Processor::Processor(rfl::Generic &json, Functions &functions):
  _json(json), _functions(functions) {
  
}

optional<rfl::Generic> Processor::transform(rfl::Generic &transform) {

  auto tr = Generic::getObject(transform);
  if (!tr) {
	  BOOST_LOG_TRIVIAL(error) << "transform not object";
	  return nullopt;
  }
  if (tr->size() == 0) {
	  BOOST_LOG_TRIVIAL(error) << "object is empty";
	  return nullopt;
  }
  auto obj = Generic::getObject(_json);
  if (!obj) {
	  BOOST_LOG_TRIVIAL(error) << "json is not object";
	  return nullopt;
  }
  
  Transform t(_functions);
  State s;
  s.setElem(*obj);
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
