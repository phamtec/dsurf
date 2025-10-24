/*
  flo.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 11-Oct-2025
    
  Simple Dictiomnary based evaluator.
  
  For now we need to support:
  
  - libraries
  - string
  - apply
  - dict
  - if
  - equal
  - getmember
  - cur
  - send
  - close
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_flo
#define H_flo

#include <rfl.hpp>

class Flo {

public:
  Flo(const rfl::Generic &library): _library(library) {}
  Flo() {}
  
  std::optional<rfl::Object<rfl::Generic> > evalObj(const rfl::Generic &obj, const rfl::Object<rfl::Generic> &transform);

private:
  
  rfl::Generic _library;
  
};

#endif // H_flo
