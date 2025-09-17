/*
  pushable.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Aug-2025
    
  Pushable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_pushable
#define H_pushable

class Element;

class Pushable {

public:
  virtual ~Pushable() {};

  virtual void push(Element *element) = 0;
    // push the object onto the parent.
    
  static Pushable *cast(Element *obj);
  static Pushable *_err;
  
};

#endif // H_pushable
