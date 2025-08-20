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
  if (result) {
    return walk(0, 0, *result);
  }

  return nullptr;
  
}

Box *Builder::loadText(const char *text) {

  auto result = rfl::json::read<rfl::Generic>(text);
  if (result) {
    return walk(0, 0, *result);
  }

  return nullptr;

}

Box *Builder::walk(Box *parent, int index, const rfl::Generic &g) {

  Box *obj = nullptr;
  std::visit([parent, index, &obj](const auto &field) {
  
    using Type = std::decay_t<decltype(field)>;
    if constexpr (std::is_same<Type, vector<rfl::Generic> >()) {
      List *l = new List(parent, index);
//      Pushable *l = new List();
      walk(l, field, l);
//      obj = dynamic_cast<Box *>(l);
      obj = l;
    }
    else if constexpr (std::is_same<Type, rfl::Object<rfl::Generic> >()) {
//      Pushable *d = new Dict();
      Dict *d = new Dict(parent, index);
      walk(d, field, d);
//      obj = dynamic_cast<Box *>(d);
      obj = d;
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

Box *Builder::walk(Box *parent, int index, const rfl::Generic &g, const string &name) {

  Box *obj = nullptr;
  std::visit([parent, index, &obj, name](const auto &field) {
  
    using Type = std::decay_t<decltype(field)>;
    if constexpr (std::is_same<Type, vector<rfl::Generic> >()) {
      ListProp *l = new ListProp(name);
      walk(l, field, l);
      obj = l;
    }
    else if constexpr (std::is_same<Type, rfl::Object<rfl::Generic> >()) {
      DictProp *d = new DictProp(name);
      walk(d, field, d);
      obj = d;
    }
    else if constexpr (std::is_same<Type, string>()) {
      stringstream ss;
      ss << "\"" << field << "\"";
      obj = new StringProp(parent, index, name, ss.str());
    }
    else if constexpr (std::is_same<Type, bool>()) {
      obj = new BoolProp(parent, index, name, field);
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

void Builder::walk(Box *parent, const rfl::Object<rfl::Generic> &obj, Pushable *list) {

  int index = 0;
  for (const auto& [k, v]: obj) {
    
    using V = remove_cvref_t<decltype(v)>;
    if (is_same<rfl::Generic, V>::value) {
      Box *p = dynamic_cast<Box *>(list);
      auto obj = walk(p, index, v, k);
      if (obj) {
        list->push(obj);
      }
    }
    else {
      cout << "unknown type in object " << typeid(v).name();
    }
    index++;
  }
}

void Builder::walk(Box *parent, const std::vector<rfl::Generic > &v, Pushable *list) {

  int index = 0;
  for (auto i: v) {
    Box *p = dynamic_cast<Box *>(list);
    auto obj = walk(p, index, i);
    if (obj) {
      list->push(obj);
    }
    index++;
  }

}

std::string Builder::getJson(Box *box) { 

  return rfl::json::write(box->getGeneric(), rfl::json::pretty); 
  
}

