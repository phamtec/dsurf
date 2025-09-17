/*
  indexable.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 16-Sep-2025
    
  Indexable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_indexable
#define H_indexable

#include <vector>

class Element;

class Indexable {

public:
  virtual ~Indexable() {};

  virtual void setIndex(int index) = 0;
    // set the index of this object in the parent.
    
  virtual int getIndex() = 0;
    // get the index of this object in the parent.
    
  static Indexable *cast(Element *obj);
  static Indexable *_err;
  
};

#endif // H_indexable
