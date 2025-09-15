/*
  sizeable.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 21-Aug-2025
    
  Sizeable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_sizeable
#define H_sizeable

#include "size.hpp"

class Element;

class Sizeable {

public:
  virtual ~Sizeable() {};

  virtual Size getSize() = 0;
    // return the size of this object.
    
  static Sizeable *cast(Element *obj);
  
private:

  static Sizeable *_err;
  
};

#endif // H_sizeable
