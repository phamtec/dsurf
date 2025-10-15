/*
  state.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "state.hpp"

#include "generic.hpp"

#include <boost/log/trivial.hpp>

using namespace flo;

State::State() {
}

State::State(const State &state) {
  _elem = state._elem;
  _coll = state._coll;
}

void State::setElem(const rfl::Generic &elem) {

//  BOOST_LOG_TRIVIAL(trace) << "setElem " << Generic::toString(elem);

  _elem = elem;
}

void State::clearElem() {

//  BOOST_LOG_TRIVIAL(trace) << "clearElem";
  
  _elem = nullopt;
}

bool State::hasElem() const {
  return _elem != nullopt;
}

const rfl::Generic State::getElem() const {
  return *_elem;
}

void State::setColl(const std::vector<rfl::Generic> &coll) {
  _coll = coll;
}

void State::clearColl() {
  _coll = nullopt;
}

bool State::hasColl() const {
  return _coll != nullopt;
}

const vector<rfl::Generic> State::getColl() const {
  return *_coll;
}
