/*
  parentable.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 21-Aug-2025
    
  Parentable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_parentable
#define H_parentable

class Element;

class Parentable {

public:
  virtual ~Parentable() {};

  virtual void setParent(Element *parent, int index) = 0;
    // set the parent.
    
  virtual Element *getParent() = 0;
    // get the parent of this object.
    
  virtual int getIndex() = 0;
    // get the index of this object in the parent.
    
  static Parentable *cast(Element *obj);
  
private:

  static Parentable *_err;
  
};

#endif // H_parentable
