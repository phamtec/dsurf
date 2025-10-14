/*
  flo.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 11-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "flo.hpp"

#include "generic.hpp"

#include "../flo/include/processor.hpp"
#include "../flo/include/functions.hpp"

#include <rfl.hpp>
#include <rfl/json.hpp>

using namespace std;
using namespace flo;

std::optional<std::string> Flo::evalStringMember(std::optional<rfl::Object<rfl::Generic> > obj, const std::string &name) {

  if (!obj) {
    return nullopt;
  }
  
//  cout << Generic::toString(*obj) << endl;

  auto transform = obj->get(name);
  if (!transform) {
    return nullopt;
  }

//  cout << Generic::toString(*transform) << endl;
  
  Functions f(_library);
  Processor p(f);

  auto result = p.transform(*transform);
  if (!result) {
    return nullopt;
  }
  return Generic::getString(*result);
  
}

std::optional<long> Flo::evalNumMember(std::optional<rfl::Object<rfl::Generic> > obj, const std::string &name) {

  if (!obj) {
    return nullopt;
  }
  
  auto transform = obj->get(name);
  if (!transform) {
    return nullopt;
  }

//  cout << Generic::toString(*transform) << endl;
  
  Functions f(_library);
  Processor p(f);

  auto result = p.transform(*transform);
  if (!result) {
    return nullopt;
  }

  return Generic::getNum(*result);
  
}

std::optional<rfl::Object<rfl::Generic> > Flo::evalObjMember(std::optional<rfl::Object<rfl::Generic> > obj, const std::string &name) {

  if (!obj) {
    return nullopt;
  }
  
  auto transform = obj->get(name);
  if (!transform) {
    return nullopt;
  }

//  cout << Generic::toString(*transform) << endl;
  
  Functions f(_library);
  Processor p(f);

  auto result = p.transform(*transform);
  if (!result) {
    return nullopt;
  }

  return Generic::getObject(*result);
  
}
