/*
  err.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 16-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "err.hpp"

#include <cxxabi.h>
#include <iostream>

using namespace std;

string Err::demangle(const type_info &type) {
  int status = -1;
  char *demangled_name = abi::__cxa_demangle(type.name(), nullptr, nullptr, &status);
  string result(demangled_name ? demangled_name : type.name());
  free(demangled_name);
  return result;
}


void Err::typeError(Element *elem, const std::type_info &type) {

    cerr << "obj " << (elem ? elem->describe() : "empty") << " not " << demangle(type) << "!" << endl;

}
