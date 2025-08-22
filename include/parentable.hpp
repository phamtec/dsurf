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

class Parentable {

public:
  virtual ~Parentable() {};

  virtual void setParent(Box *parent, int index) = 0;
    // set the parent.
    
  virtual Box *getParent() = 0;
    // get the parent of this object.
    
  virtual int getIndex() = 0;
    // get the index of this object in the parent.
    
};

#endif // H_parentable
