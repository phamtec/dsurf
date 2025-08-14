/*
  resources.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Resources class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_resources
#define H_resources

#include "colours.hpp"

class Renderer;
class Font;

class Resources {

public:
  Resources() {}
  
  void build(Renderer &renderer, Font &font);
        
};

#endif // H_resources
