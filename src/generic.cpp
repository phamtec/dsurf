/*
  generic.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "generic.hpp"

#include <rfl.hpp>
#include <rfl/json.hpp>

using namespace std;

std::optional<std::string> Generic::getString(std::optional<rfl::Object<rfl::Generic> > obj, const std::string &name) {

  if (!obj) {
    return nullopt;
  }
  
  auto prop = obj->get(name);
  if (!prop) {
    return nullopt;
  }
  
  return getString(*prop);
  
}

std::optional<rfl::Object<rfl::Generic> > Generic::getObject(std::optional<rfl::Object<rfl::Generic> > obj, const std::string &name) {

  if (!obj) {
    return nullopt;
  }
  
  auto prop = obj->get(name);
  if (!prop) {
    return nullopt;
  }
  
  return getObject(*prop);
  
}

std::optional<rfl::Generic> Generic::getGeneric(std::optional<rfl::Object<rfl::Generic> > obj, const std::string &name) {

  if (!obj) {
    return nullopt;
  }
  
  auto prop = obj->get(name);
  if (!prop) {
    return nullopt;
  }
  
  return *prop;
  
}

std::optional<vector<rfl::Generic> > Generic::getVector(std::optional<rfl::Object<rfl::Generic> > obj, const std::string &name) {

  if (!obj) {
    return nullopt;
  }
  
  auto prop = obj->get(name);
  if (!prop) {
    return nullopt;
  }
  
  return getVector(*prop);
  
}

optional<rfl::Object<rfl::Generic> > Generic::getObject(const rfl::Generic &obj) {

  optional<rfl::Object<rfl::Generic> > dict;
  std::visit([&obj, &dict](const auto &field) {
  
    if constexpr (is_same<decay_t<decltype(field)>, rfl::Object<rfl::Generic> >()) {
      dict = field;
    }

  }, obj.variant());

  return dict;
  
}

optional<vector<rfl::Generic> > Generic::getVector(const rfl::Generic &obj) {

  optional<vector<rfl::Generic> > v;
  std::visit([&obj, &v](const auto &field) {
  
    if constexpr (is_same<decay_t<decltype(field)>, vector<rfl::Generic> >()) {
      v = field;
    }

  }, obj.variant());

  return v;
  
}

optional<string> Generic::getString(const rfl::Generic &obj) {

  optional<string> str;
  std::visit([&obj, &str](const auto &field) {
  
    if constexpr (is_same<decay_t<decltype(field)>, string>()) {
      str = field;
    }

  }, obj.variant());

  return str;
  
}

optional<long> Generic::getNum(const rfl::Generic &obj) {

  optional<long> i;
  std::visit([&obj, &i](const auto &field) {
  
    if constexpr (is_same<decay_t<decltype(field)>, long>() || is_same<decay_t<decltype(field)>, long long>()) {
      i = field;
    }
  }, obj.variant());

  return i;
  
}

optional<bool> Generic::getBool(const rfl::Generic &obj) {

  optional<bool> b;
  std::visit([&obj, &b](const auto &field) {
  
    if constexpr (is_same<decay_t<decltype(field)>, bool>()) {
      b = field;
    }
  }, obj.variant());

  return b;
  
}

string Generic::toString(const rfl::Generic &g) {

  return rfl::json::write(g, rfl::json::pretty);
  
}
