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
  virtual ~Change() {}
  
  virtual void exec(Renderer &renderer) = 0;
    // execute (do) the change.
    
  virtual void undo(Renderer &renderer) = 0;
    // undo the change.
    
  virtual void destroy(Renderer &renderer) {};
    // opposite of build, but called with the renderer.
    
    
};

#endif // H_change
