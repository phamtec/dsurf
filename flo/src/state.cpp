/*
  state.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "state.hpp"

State::State() {
}

State::State(const State &state) {
  _elem = state._elem;
}

void State::setElem(rfl::Object<rfl::Generic> &elem) {
  _elem = elem;
}

bool State::hasElem() const {
  return _elem != nullopt;
}

const rfl::Object<rfl::Generic> State::getElem() const {
  return *_elem;
}
