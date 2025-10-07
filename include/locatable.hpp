/*
  locatable.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 7-Oct-2025
    
  Locatable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_locatable
#define H_locatable

#include "point.hpp"

class Element;

class Locatable {

public:
  virtual ~Locatable() {};

  virtual Point getLocation() = 0;
      // Get the location of something
      
  virtual void setLocation(const Point &loc) = 0;
      // Set the location of something.
      
  static Locatable *cast(Element *obj);
  static Locatable *_err;
  
};

#endif // H_locatable
