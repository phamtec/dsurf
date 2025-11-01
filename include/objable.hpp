/*
  objable.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 29-Sep-2025
    
  Objable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_objable
#define H_objable

#include <string>

class Core;
class Element;

class Objable {

public:
  virtual ~Objable() {};

  virtual void setObj(Core &core, Element *obj) = 0;
    // set the object
    
  virtual Element *getObj() = 0;
    // get the object.

  static Objable *cast(Element *obj);
  static Objable *_err;
    
};

#endif // H_objable
