/*
  handlers.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 1-Nov-2025
    
  Handler type definitions.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_handler
#define H_handler

class Core;
class Element;

typedef std::function<void ()> msgHandler;
typedef std::function<void (Element *element)> elementMsgHandler;
typedef std::function<void (Core &core)> coreMsgHandler;

#endif // H_handler
