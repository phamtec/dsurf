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

#include <string>

class Core;

class Change {

public:
  virtual ~Change() {}
  
  virtual std::string describe() { return "?"; }
    // describe the change.
    
  virtual void exec(Core &core) = 0;
    // execute (do) the change.
    
  virtual void undo(Core &core) = 0;
    // undo the change.
    
  virtual void destroy(Core &core) {};
    // opposite of build, but called with the core.
    
};

#endif // H_change
