/*
  resources.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Text pool class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_resources
#define H_resources

#include "text.hpp"
#include "colours.hpp"

class Resources {

public:
  Resources(): 
    open_brace("{", Colours::black), 
    close_brace("}", Colours::black),   
    open_bracket("[", Colours::black), 
    close_bracket("]", Colours::black) 
      {}
  
  void build(Renderer &renderer, Font &font);
  
  Text open_brace;
  Text close_brace;
  Text open_bracket;
  Text close_bracket;
  
};

#endif // H_resources
