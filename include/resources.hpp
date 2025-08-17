/*
  resources.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 17-Aug-2025
    
  Common resources class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_resources
#define H_resources

#include "corner.hpp"

class Resources {

public:
  Resources(): topleft(true), bottomleft(false) {}

  virtual void build(Renderer &renderer);
    // build all resources ready to render
    
  Corner topleft;
  Corner bottomleft;

};

#endif // H_resources
