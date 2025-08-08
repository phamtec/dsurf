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
#include "dictprop.hpp"
#include "listprop.hpp"

using namespace std;

Box *Builder::walk(const rfl::Generic &g) {

  Box *obj = nullptr;
  std::visit([&obj](const auto &field) {
  
    using Type = std::decay_t<decltype(field)>;
    if constexpr (std::is_same<Type, vector<rfl::Generic >>()) {
      Pushable *l = new List();
      walk(field, l);
      obj = dynamic_cast<Box *>(l);
    }
    else if constexpr (std::is_same<Type, rfl::Object<rfl::Generic> >()) {
      Pushable *d = new Dict();
      walk(field, d);
      obj = dynamic_cast<Box *>(d);
    }
    else {
      cout << "unknown type " << typeid(field).name() << endl;
    }

  }, g.variant());

  return obj;
    
}

Box *Builder::walk(const rfl::Generic &g, const string &name) {

  Box *obj = nullptr;
  std::visit([&obj, name](const auto &field) {
  
    using Type = std::decay_t<decltype(field)>;
    if constexpr (std::is_same<Type, vector<rfl::Generic >>()) {
      ListProp *l = new ListProp(name);
      walk(field, l);
      obj = l;
    }
    else if constexpr (std::is_same<Type, rfl::Object<rfl::Generic> >()) {
      DictProp *d = new DictProp(name);
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

void Builder::walk(const rfl::Object<rfl::Generic> &obj, Pushable *list) {

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

void Builder::walk(const std::vector<rfl::Generic > &v, Pushable *list) {

  for (auto i: v) {
    auto obj = walk(i);
    if (obj) {
      list->push(obj);
    }
  }

}

