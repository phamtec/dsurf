/*
  flo.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 11-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "flo.hpp"

#include "generic.hpp"

#include <rfl.hpp>
#include <rfl/json.hpp>

using namespace std;

std::optional<std::string> Flo::evalStringMember(std::optional<rfl::Object<rfl::Generic> > obj, const std::string &name) {

  if (!obj) {
    return nullopt;
  }
  
  auto prop = obj->get(name);
  if (!prop) {
    return nullopt;
  }
  
  return Generic::getString(*prop);
  
}

std::optional<long> Flo::evalNumMember(std::optional<rfl::Object<rfl::Generic> > obj, const std::string &name) {

  if (!obj) {
    return nullopt;
  }
  
  auto prop = obj->get(name);
  if (!prop) {
    return nullopt;
  }
  
  return Generic::getNum(*prop);
  
}

std::optional<rfl::Object<rfl::Generic> > Flo::evalObjMember(std::optional<rfl::Object<rfl::Generic> > obj, const std::string &name) {

  if (!obj) {
    return nullopt;
  }
  
  auto prop = obj->get(name);
  if (!prop) {
    return nullopt;
  }
  
  return Generic::getObject(*prop);
  
}
