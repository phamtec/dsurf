/*
  flo.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 11-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "flo.hpp"

#include "dict.hpp"

#include "processor.hpp"
#include "functions.hpp"

#include <rfl.hpp>
#include <rfl/json.hpp>

using namespace std;
using namespace vops::flo;
using namespace vops;

optional<DictO> Flo::evalObj(const DictG &obj, const DictO &transform) {

  auto result = eval(obj, transform);
  if (!result) {
    return nullopt;
  }
  
  return Dict::getObject(*result);
  
}

optional<DictG> Flo::eval(const DictG &obj, const DictO &transform) {

//   cout << "obj " << Dict::toString(obj) << endl;
//   cout << "transform " << Dict::toString(transform) << endl;
  
  Functions f(_library);
  Processor p(f);

  DictO t;
  t["transform"] = transform;
  auto result = p.transform(t, obj);
  if (!result) {
    return nullopt;
  }
//  cout << "result " << Dict::toString(*result) << endl;
  return *result;
  
}
