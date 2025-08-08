/*
  dict.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Dictionary class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_dict
#define H_dict

#include "list.hpp"

class Dict: public List {

public:
  Dict(const std::string &name): List(name) {}

};

#endif // H_dict
