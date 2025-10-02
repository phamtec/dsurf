/*
  main.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 2-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "main.hpp"
#include "builder.hpp"
#include "root.hpp"
#include "obj.hpp"

#include <iostream>

using namespace std;

bool Project::isA(const rfl::Generic &obj) {

  // we need a Mondoid here :-)
  auto name = Builder::getString(Builder::getObject(Builder::getObject(obj), "project"), "name");
  if (name) {
    cout << "Project " << *name << endl;
    return true;
  }

  return false;
  
}

Element *Project::load(const rfl::Generic &obj) {

  auto project = Builder::getObject(Builder::getObject(obj), "project");
  if (!project) {
    cerr << "no project!" << endl;
    return nullptr;
  }
  auto name = Builder::getString(project, "name");
  if (!name) {
    cerr << "no name!" << endl;
    return nullptr;
  }
  auto objects = Builder::getVector(project, "objects");
  if (!objects) {
    cerr << "no objects!" << endl;
    return nullptr;
  }
  std::vector<Element *> objs;
  transform(objects->begin(), objects->end(), back_inserter(objs), [](auto e) -> Element * {
    auto obj = Builder::getObject(e);
    if (!obj) {
      cerr << "obj is not an object!" << endl;
      return nullptr;
    }
    auto name = Builder::getString(*obj, "name");
    if (!name) {
      cerr << "no name in obj!" << endl;
      return nullptr;
    }
    auto file = Builder::getString(*obj, "file");    
    if (!file) {
      cerr << "no file in obj!" << endl;
      return nullptr;
    }
    return new ProjectObj(*name, *file);
  });
  
  return new ProjectRoot(*name, objs);
  
}
