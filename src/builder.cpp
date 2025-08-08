/*
  builder.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 7-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "builder.hpp"

#include "dict.hpp"
#include "list.hpp"
#include "stringprop.hpp"
#include "boolprop.hpp"

using namespace std;

Box *Builder::walk(const rfl::Generic &g, const string &name) {

  Box *obj = nullptr;
  std::visit([&obj, name](const auto &field) {
  
    using Type = std::decay_t<decltype(field)>;
    if constexpr (std::is_same<Type, vector<rfl::Generic >>()) {
      List *l = new List(name);
      walk(field, l);
      obj = l;
    }
    else if constexpr (std::is_same<Type, rfl::Object<rfl::Generic> >()) {
      Dict *d = new Dict(name);
      walk(field, d);
      obj = d;
    }
    else if constexpr (std::is_same<Type, string>()) {
      obj = new StringProp(name, field);
    }
    else if constexpr (std::is_same<Type, bool>()) {
      obj = new BoolProp(name, field);
    }
    else {
      cout << "unknown type " << typeid(field).name() << endl;
    }

  }, g.variant());

  return obj;
    
}

void Builder::walk(const rfl::Object<rfl::Generic> &obj, List *list) {

  for (const auto& [k, v]: obj) {
    
    using V = remove_cvref_t<decltype(v)>;
    if (is_same<rfl::Generic, V>::value) {
      auto obj = walk(v, k);
      if (obj) {
        list->push(obj);
      }
    }
    else {
      cout << "unknown type " << typeid(v).name();
    }
    
  }

}

void Builder::walk(const std::vector<rfl::Generic > &v, List *list) {

  for (auto i: v) {
    auto obj = walk(i, "");
    if (obj) {
      list->push(obj);
    }
  }

}

