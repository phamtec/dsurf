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
#include "string.hpp"
#include "stringprop.hpp"
#include "bool.hpp"
#include "boolprop.hpp"
#include "dictprop.hpp"
#include "listprop.hpp"
#include "long.hpp"
#include "longprop.hpp"

using namespace std;

Box *Builder::loadFile(const string &fn) {

  auto result = rfl::json::load<rfl::Generic>(fn);
  return walk(*result);

}

Box *Builder::loadText(const char *text) {

  auto result =rfl::json::read<rfl::Generic>(text);
  return walk(*result);

}

Box *Builder::walk(const rfl::Generic &g) {

  Box *obj = nullptr;
  std::visit([&obj](const auto &field) {
  
    using Type = std::decay_t<decltype(field)>;
    if constexpr (std::is_same<Type, vector<rfl::Generic> >()) {
      Pushable *l = new List();
      walk(field, l);
      obj = dynamic_cast<Box *>(l);
    }
    else if constexpr (std::is_same<Type, rfl::Object<rfl::Generic> >()) {
      Pushable *d = new Dict();
      walk(field, d);
      obj = dynamic_cast<Box *>(d);
    }
    else if constexpr (std::is_same<Type, string>()) {
      stringstream ss;
      ss << "\"" << field << "\"";
      obj = new String(ss.str());
    }
    else if constexpr (std::is_same<Type, bool>()) {
      obj = new Bool(field);
    }
    else if constexpr (std::is_same<Type, long long>() || std::is_same<Type, long>()) {
      obj = new Long(field);
    }
    else {
      cout << "unknown type in generic " << typeid(field).name() << endl;
    }

  }, g.variant());

  return obj;
    
}

Box *Builder::walk(const rfl::Generic &g, const string &name) {

  Box *obj = nullptr;
  std::visit([&obj, name](const auto &field) {
  
    using Type = std::decay_t<decltype(field)>;
    if constexpr (std::is_same<Type, vector<rfl::Generic> >()) {
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
      stringstream ss;
      ss << "\"" << field << "\"";
      obj = new StringProp(name, ss.str());
    }
    else if constexpr (std::is_same<Type, bool>()) {
      obj = new BoolProp(name, field);
    }
    else if constexpr (std::is_same<Type, long long>() || std::is_same<Type, long>()) {
      obj = new LongProp(name, field);
    }
    else {
      cout << "unknown type in generic " << name << ": " << typeid(field).name() << endl;
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
      cout << "unknown type in object " << typeid(v).name();
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

std::string Builder::getJson(Box *box) { 

  return rfl::json::write(box->getGeneric(), rfl::json::pretty); 
  
}

