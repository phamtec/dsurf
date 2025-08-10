/*
  bool.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Aug-2025
    
  Boolean class.
  
  true
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_bool
#define H_bool

#include "string.hpp"

class Bool: public String {

public:
  Bool(bool value): String(value ? "true" : "false") {}
  
};

#endif // H_bool
