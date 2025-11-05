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
#include "fileobj.hpp"
#include "hud.hpp"
#include "hudmode.hpp"
#include "shortcut.hpp"
#include "core.hpp"
#include "unkobj.hpp"
#include "zmqobj.hpp"
#include "generic.hpp"

#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;
using flo::Generic;

bool Project::isA(const rfl::Generic &obj) {

  // we need a Mondoid here :-)
  auto name = Generic::getString(Generic::getObject(Generic::getObject(obj), "project"), "name");
  if (name) {
    cout << "Project " << *name << endl;
    return true;
  }

  return false;
  
}

Element *Project::load(const rfl::Generic &obj, const string &filename) {

  auto project = Generic::getObject(Generic::getObject(obj), "project");
  if (!project) {
    cerr << "no project!" << endl;
    return nullptr;
  }
  auto name = Generic::getString(project, "name");
  if (!name) {
    cerr << "no name!" << endl;
    return nullptr;
  }
  auto objects = Generic::getVector(project, "objects");
  if (!objects) {
    cerr << "no objects!" << endl;
    return nullptr;
  }
  std::vector<Element *> objs;
  transform(objects->begin(), objects->end(), back_inserter(objs), [filename](auto e) -> Element * {
    auto obj = Generic::getObject(e);
    if (!obj) {
      cerr << "obj is not an object!" << endl;
      return nullptr;
    }
    auto name = Generic::getString(*obj, "name");
    if (!name) {
      cerr << "no name in obj!" << endl;
      return nullptr;
    }
    auto file = Generic::getString(*obj, "file");    
    if (file) {
      fs::path p = filename;
      p = p.parent_path();
      p /= *file;
      return new ProjectFileObj(*name, p, *file);
    }
    else {
      auto zmq = Generic::getObject(*obj, "zmq");    
      if (zmq) {
        return new ProjectZMQObj(*name, *zmq);
      }
      else {
        cerr << "unknown type of object" << Generic::toString(*obj) << endl;
        return new ProjectUnknownObj(*name, *obj);
      }
    }
  });
  
  return new ProjectRoot(*name, filename, objs);
  
}

void Project::registerHUDModes(Core &core, HUD *hud) {

  {
    auto mode = new HUDMode(false);
    Core::registerRootHUDMode(mode);
    mode->add(new Shortcut(L"E", L"dit"));
    hud->registerMode("projectroot", mode);
  }

  {
    auto mode = new HUDMode(false);
    core.registerGlobalHUDMode(mode);
    mode->add(new Shortcut(L"L", L"oad"));
    hud->registerMode("projectfileobj", mode);
  }

  {
    auto mode = new HUDMode(false);
    core.registerGlobalHUDMode(mode);
    mode->add(new Shortcut(L"C", L"onnect"));
    mode->add(new Shortcut(L"E", L"edit", canEdit));
    hud->registerMode("projectzmqobj", mode);
  }

  {
    auto mode = new HUDMode(false);
    core.registerGlobalHUDMode(mode);
    mode->add(new Shortcut(L"", L"???"));
    hud->registerMode("projectunkobj", mode);
  }

  {
    auto mode = new HUDMode(false);
    core.registerGlobalHUDMode(mode);
    mode->add(new Shortcut(L"E", L"xecute", canRun));
    hud->registerMode("projectcode", mode);
  }

}
