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

class Pushable {

public:

  virtual void push(Box *box) = 0;
  
private:
  
};

#endif // H_pushable
