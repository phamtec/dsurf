/*
  writeable.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 21-Aug-2025
    
  Writeable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_writeable
#define H_writeable

#include <rfl.hpp>

class Writeable {

public:
  virtual ~Writeable() {};

  virtual std::string getName() { return "????"; }
  virtual rfl::Generic getGeneric() = 0;
    // return the name and the object for serialization.
        
};

#endif // H_writeable
