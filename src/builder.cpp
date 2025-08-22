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

Box *Builder::walk(Box *parent, int index, const rfl::Generic &g) {

  Box *obj = nullptr;
  std::visit([parent, index, &obj](const auto &field) {
  
    Parentable *pobj = nullptr;
    using Type = std::decay_t<decltype(field)>;
    if constexpr (std::is_same<Type, vector<rfl::Generic> >()) {
      auto *l = new List();
      walk(l, field, l);
      pobj = l;
      obj = l;
    }
    else if constexpr (std::is_same<Type, rfl::Object<rfl::Generic> >()) {
      auto *d = new Dict();
      walk(d, field, d);
      pobj = d;
      obj = d;
    }
    else if constexpr (std::is_same<Type, string>()) {
      auto s = new String(field);
      pobj = s;
      obj = s;
    }
    else if constexpr (std::is_same<Type, bool>()) {
      auto b = new Bool(field);
      pobj = b;
      obj = b;
    }
    else if constexpr (std::is_same<Type, long long>() || std::is_same<Type, long>()) {
      auto l = new Long(field);
      pobj = l;
      obj = l;
    }
    else {
      cout << "unknown type in generic " << typeid(field).name() << endl;
    }

    if (pobj) {
      pobj->setParent(parent, index);
    }
    
  }, g.variant());

  return obj;
    
}

Box *Builder::walk(Box *parent, int index, const rfl::Generic &g, const string &name) {

  Box *obj = nullptr;
  std::visit([parent, index, &obj, name](const auto &field) {
  
    Parentable *pobj = nullptr;
    using Type = std::decay_t<decltype(field)>;
    if constexpr (std::is_same<Type, vector<rfl::Generic> >()) {
      auto *l = new List();
      walk(l, field, l);
      auto p = new Property(name, l, true);
      pobj = p;
//      p->setParent(parent, index);
      l->setParent(p, 0);
      obj = p;
      pobj = p;
    }
    else if constexpr (std::is_same<Type, rfl::Object<rfl::Generic> >()) {
      auto *d = new Dict();
      walk(d, field, d);
      auto p = new Property(name, d, true);
//      p->setParent(parent, index);
      d->setParent(p, 0);
      obj = p;
      pobj = p;
    }
    else if constexpr (std::is_same<Type, string>()) {
      auto s = new String(field);
      auto p = new Property(name, s, false);
//      p->setParent(parent, index);
      s->setParent(p, 0);
      obj = p;
    }
    else if constexpr (std::is_same<Type, bool>()) {
      auto s = new Bool(field);
      auto p = new Property(name, s, false);
//      p->setParent(parent, index);
      s->setParent(p, 0);
      obj = p;
    }
    else if constexpr (std::is_same<Type, long long>() || std::is_same<Type, long>()) {
      auto s = new Long(field);
      auto p = new Property(name, s, false);
//      p->setParent(parent, index);
      s->setParent(p, 0);
      obj = p;
    }
    else {
      cout << "unknown type in generic " << name << ": " << typeid(field).name() << endl;
    }

    if (pobj) {
      pobj->setParent(parent, index);
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

  auto *wx = dynamic_cast<Writeable *>(box);
  if (wx) {
    return rfl::json::write(wx->getGeneric(), rfl::json::pretty); 
  }
  return "not writeable";
  
}

