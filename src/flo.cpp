/*
  flo.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 11-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "flo.hpp"

#include "builder.hpp"

std::optional<std::string> Flo::evalStringMember(std::optional<rfl::Object<rfl::Generic> > obj, const std::string &name) {

  return Builder::getString(obj, name);
  
}

std::optional<long> Flo::evalNumMember(std::optional<rfl::Object<rfl::Generic> > obj, const std::string &name) {

  return Builder::getNum(obj, name);
  
}

std::optional<rfl::Object<rfl::Generic> > Flo::evalObjMember(std::optional<rfl::Object<rfl::Generic> > obj, const std::string &name) {

  return Builder::getObject(obj, name);
  
}

