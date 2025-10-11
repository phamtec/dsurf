/*
  flo.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 11-Oct-2025
    
  Simple Dictiomnary based evaluator.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_flo
#define H_flo

#include <rfl.hpp>

class Flo {

public:
  Flo(const std::vector<rfl::Generic> &library): _library(library) {}
  Flo() {}
  
  std::optional<std::string> evalStringMember(std::optional<rfl::Object<rfl::Generic> > obj, const std::string &name);
  std::optional<long> evalNumMember(std::optional<rfl::Object<rfl::Generic> > obj, const std::string &name);
  std::optional<rfl::Object<rfl::Generic> > evalObjMember(std::optional<rfl::Object<rfl::Generic> > obj, const std::string &name);

private:
  
  std::vector<rfl::Generic> _library;

};

#endif // H_flo
