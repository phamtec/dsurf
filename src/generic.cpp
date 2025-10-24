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
#include <rfl/yaml.hpp>

using namespace std;
namespace fs = std::filesystem;

optional<string> Generic::getString(optional<rfl::Object<rfl::Generic> > obj, const string &name) {

  if (!obj) {
    return nullopt;
  }
  
  auto prop = obj->get(name);
  if (!prop) {
    return nullopt;
  }
  
  return getString(*prop);
  
}

optional<long> Generic::getNum(optional<rfl::Object<rfl::Generic> > obj, const string &name) {

  if (!obj) {
    return nullopt;
  }
  
  auto prop = obj->get(name);
  if (!prop) {
    return nullopt;
  }
  
  return getNum(*prop);
  
}

optional<bool> Generic::getBool(optional<rfl::Object<rfl::Generic> > obj, const string &name) {

  if (!obj) {
    return nullopt;
  }
  
  auto prop = obj->get(name);
  if (!prop) {
    return nullopt;
  }
  
  return getBool(*prop);
  
}

optional<rfl::Object<rfl::Generic> > Generic::getObject(optional<rfl::Object<rfl::Generic> > obj, const string &name) {

  if (!obj) {
    return nullopt;
  }
  
  auto prop = obj->get(name);
  if (!prop) {
    return nullopt;
  }
  
  return getObject(*prop);
  
}

optional<rfl::Generic> Generic::getGeneric(optional<rfl::Object<rfl::Generic> > obj, const string &name) {

  if (!obj) {
    return nullopt;
  }
  
  auto prop = obj->get(name);
  if (!prop) {
    return nullopt;
  }
  
  return *prop;
  
}

optional<vector<rfl::Generic> > Generic::getVector(optional<rfl::Object<rfl::Generic> > obj, const string &name) {

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

string Generic::toString(const rfl::Generic &g, const std::string &format) {

  if (format == ".json") {
    return rfl::json::write(g, rfl::json::pretty);
  }
  else if (format == ".yml") {
    return rfl::yaml::write(g);
  }
  else {
    cerr << "invalid format " << format << endl;
    return "???";
  }

}

optional<rfl::Generic> Generic::parseStream(istream &s, const string &format) {

  if (format == ".json") {
    auto g = rfl::json::read<rfl::Generic>(s);
    if (g) {
      return *g;
    }
  }
  else if (format == ".yml") {
    auto g = rfl::yaml::read<rfl::Generic>(s);
    if (g) {
      return *g;
    }
  }
  else {
    cerr << "invalid format " << format << endl;
    return nullopt;
  }
  
  cerr << "could not parse stream to " << format << endl;
  return nullopt;
  
}

optional<rfl::Generic> Generic::parseFile(const string &fn) {

  fs::path p = fn;

  if (p.extension() == ".json") {
    auto g = rfl::json::load<rfl::Generic>(fn);
    if (g) {
      return *g;
    }
  }
  else if (p.extension() == ".yml") {
    auto g = rfl::yaml::load<rfl::Generic>(fn);
    if (g) {
      return *g;
    }
  }
  else {
    cerr << "invalid format " << p.extension() << endl;
    return nullopt;
  }
  
  cerr << "could not parse stream to " << p.extension() << endl;
  return nullopt;
  
}
