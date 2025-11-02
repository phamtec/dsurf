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
#include <sstream>

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
    auto home = getenv("HOME");
    stringstream ss;
    ss << home << "/" << "Documents" << "/" << filename;
    return ss.str();
  }
  
  return filename;
  
}
