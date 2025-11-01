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
class List;
class Element;

typedef std::function<void (Core &core)> globalMsgHandler;
typedef std::function<void (Core &core, Element *element)> elementMsgHandler;
typedef std::function<void (Core &core, List &list)> listMsgHandler;

#endif // H_handler
