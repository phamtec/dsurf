/*
  change.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 20-Sep-2025
    
  A single change to the system.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_change
#define H_change

class Renderer;

class Change {

public:

  virtual void do(Renderer &renderer) = 0;
  virtual void undo(Renderer &renderer) = 0;
    
};

#endif // H_change
