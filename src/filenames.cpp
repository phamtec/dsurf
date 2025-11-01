/*
  filenames.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 1-Nov-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "filenames.hpp"

#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

optional<string> Filenames::isInvalid(const string &filename) {

  fs::path p = filename;
  if (p.extension() != ".json" && p.extension() != ".yml") {
    return "Filename must have .json or .yml extension.";
  }
  
  auto par = p.parent_path();
  if (!par.empty() && !fs::exists(par)) {
    return "Path to file does not exist.";
  }
  
  // good to go.
  return nullopt;
  
}

string Filenames::addPath(const string &filename) {
  
  fs::path p = filename;
  auto par = p.parent_path();
  if (par.empty()) {
#ifdef __APPLE__
    return "~/Documents/" + filename;
#else
    return "~/" + filename;
#endif
  }
  
  return filename;
  
}
