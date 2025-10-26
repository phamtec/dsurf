/*
  builder.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 7-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "builder.hpp"

#include "list.hpp"
#include "listelem.hpp"
#include "string.hpp"
#include "bool.hpp"
#include "long.hpp"
#include "property.hpp"
#include "unicode.hpp"
#include "modules.hpp"
#include "root.hpp"
#include "generic.hpp"

#include <rfl/json.hpp>
#include <rfl/yaml.hpp>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;
using flo::Generic;

Element *Builder::loadFile(const string &fn, bool raw) {

  auto obj = Generic::parseFile(fn);
  if (!obj) {
    return nullptr;
  }

  if (!raw) {
    auto elem = Modules::load(*obj, fn);
    if (elem) {
      return elem;
    }
  }

  // parse out the object.
  auto elem = walk(0, *obj);
  
  // interdict the root element.
  return new Root(fn, elem);
  
}

Element *Builder::loadText(const char *text) {

  auto result = rfl::json::read<rfl::Generic>(text);
  if (!result) {
    return nullptr;
  }
  
  auto elem = Modules::load(*result, "x.json");
  if (elem) {
    return elem;
  }

  return walk(0, *result);

}

void Builder::write(const rfl::Generic &g, const string &fn) {

  fs::path p = fn;
  if (p.extension() == ".json") {
    rfl::json::save(fn, g, rfl::json::pretty);
  }
  else  if (p.extension() == ".yaml") {
    rfl::yaml::save(fn, g);
  }
  else {
    cerr << "unknown file type " << fn << endl;
  }
}

Element *Builder::castGeneric(const rfl::Generic &g)  {

  Element *obj = nullptr;
  std::visit([&obj](const auto &field) {
  
    using Type = std::decay_t<decltype(field)>;
    if constexpr (std::is_same<Type, vector<rfl::Generic> >()) {
      auto *l = new List(false);
      walk(l, field, l);
      obj = l;
    }
    else if constexpr (std::is_same<Type, rfl::Object<rfl::Generic> >()) {
      auto *d = new List(true);
//      auto *d = new Dict();
      walk(d, field, d);
      obj = d;
    }
    else if constexpr (std::is_same<Type, string>()) {
      obj = new String(Unicode::convert(field));
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

Element *Builder::walk(Element *parent, const rfl::Generic &g) {

  Element *obj = castGeneric(g);

  obj->setParent(parent);
    
  return obj;
    
}

Element *Builder::walk(Element *parent, const rfl::Generic &g, const string &name) {

  Element *obj = castGeneric(g);

  if (obj) {
    obj = new Property(Unicode::convert(name), obj, dynamic_cast<Listable *>(obj) != 0);
  }
  
  obj->setParent(parent);
    
  return obj;
    
}

void Builder::walk(Element *parent, const rfl::Object<rfl::Generic> &obj, Element *list) {

//  cout << "walk object " << list->describe() << endl;
  
  for (const auto& [k, v]: obj) {
    
    using V = remove_cvref_t<decltype(v)>;
    if (is_same<rfl::Generic, V>::value) {
      auto obj = walk(list, v, k);
      if (obj) {
        auto elements = Listable::cast(list)->getElements();
        if (elements) {
          elements->push_back(unique_ptr<Element>(obj));
        }
        else {
          cerr << "list doesnt provide access to elements" << endl;
        }
      }
      else {
        cerr << "null obj returned for " << list->describe() << endl;
      }
    }
    else {
      cout << "unknown type in object " << typeid(v).name();
    }
  }
}

void Builder::walk(Element *parent, const std::vector<rfl::Generic > &v, Element *list) {

//  cout << "walk vector " << list->describe() << endl;
  
  for (auto i: v) {
    auto *l = dynamic_cast<List *>(list);
    if (l) {
      auto obj = walk(l, i);
      if (obj) {
        // interdict the list element.
        auto le = new ListElem(obj);
        le->setParent(parent);
        auto elements = Listable::cast(list)->getElements();
        if (elements) {
          elements->push_back(unique_ptr<Element>(le));
        }
        else {
          cerr << "list doesnt provide access to elements" << endl;
        }
      }
      else {
        cerr << "null obj returned for " << l->describe() << endl;
      }
    }
    else {
      cerr << "Not List ??" << endl;
    }
  }

}

std::string Builder::getJson(Element *element) { 

  auto *wx = dynamic_cast<Writeable *>(element);
  if (wx) {
    return rfl::json::write(wx->getGeneric(), rfl::json::pretty); 
  }
  return "not Writeable";
  
}

std::string Builder::getYaml(Element *element) { 

  auto *wx = dynamic_cast<Writeable *>(element);
  if (wx) {
    return rfl::yaml::write(wx->getGeneric()); 
  }
  return "not Writeable";
  
}
