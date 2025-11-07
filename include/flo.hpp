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

#include "dict.hpp"

using vops::DictG;
using vops::DictO;

class Flo {

public:
  Flo(const DictG &library): _library(library) {}
  Flo() {}
  
  std::optional<DictO> evalObj(const DictG &obj, const DictO &transform);
    // evaulate, expecting an object to be returned.
    
  std::optional<DictG> eval(const DictG &obj, const DictO &transform);
    // evaluate getting anything back.
    
private:
  
  DictG _library;
  
};

#endif // H_flo
