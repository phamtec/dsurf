/*
  flo.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 11-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "flo.hpp"

#include "generic.hpp"

#include "processor.hpp"
#include "functions.hpp"

#include <rfl.hpp>
#include <rfl/json.hpp>

using namespace std;
using namespace flo;

optional<rfl::Object<rfl::Generic> > Flo::evalObj(const rfl::Generic &obj, const rfl::Object<rfl::Generic> &transform) {

//   cout << "obj " << Generic::toString(obj) << endl;
//   cout << "transform " << Generic::toString(transform) << endl;
  
  Functions f(_library);
  Processor p(f);

  rfl::Object<rfl::Generic> t;
  t["transform"] = transform;
  auto result = p.transform(t, obj);
  if (!result) {
    return nullopt;
  }

//  cout << "result " << Generic::toString(*result) << endl;

  return Generic::getObject(*result);
  
}
