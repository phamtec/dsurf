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
#include "bool.hpp"
#include "long.hpp"
#include "property.hpp"

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

Box *Builder::castGeneric(const rfl::Generic &g)  {

  Box *obj = nullptr;
  std::visit([&obj](const auto &field) {
  
    using Type = std::decay_t<decltype(field)>;
    if constexpr (std::is_same<Type, vector<rfl::Generic> >()) {
      auto *l = new List();
      walk(l, field, l);
      obj = l;
    }
    else if constexpr (std::is_same<Type, rfl::Object<rfl::Generic> >()) {
      auto *d = new Dict();
      walk(d, field, d);
      obj = d;
    }
    else if constexpr (std::is_same<Type, string>()) {
      obj = new String(field);
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

Box *Builder::walk(Box *parent, int index, const rfl::Generic &g) {

  Box *obj = castGeneric(g);

  Parentable *px = dynamic_cast<Parentable *>(obj);
  if (px) {
    px->setParent(parent, index);
  }
  else {
    cerr << "obj not parentable!" << endl;
  }
    
  return obj;
    
}

Box *Builder::walk(Box *parent, int index, const rfl::Generic &g, const string &name) {

  Box *obj = castGeneric(g);

  if (obj) {
    obj = new Property(name, obj, dynamic_cast<Pushable *>(obj) != 0);
  }
  
  Parentable *px = dynamic_cast<Parentable *>(obj);
  if (px) {
    px->setParent(parent, index);
  }
  else {
    cerr << "obj not parentable!" << endl;
  }
    
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

  auto *wx = dynamic_cast<Writeable *>(box);
  if (wx) {
    return rfl::json::write(wx->getGeneric(), rfl::json::pretty); 
  }
  return "not writeable";
  
}

