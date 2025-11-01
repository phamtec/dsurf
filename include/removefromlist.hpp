/*
  removefromlist.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 22-Sep-2025
    
  A change that removes an element from a list.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_removefromlist
#define H_removefromlist

#include "change.hpp"

#include <memory>

class Element;
class List;

class RemoveFromList: public Change {

public:
  RemoveFromList(List *list, Element *elem);
  
  virtual void exec(Core &core);
  virtual void undo(Core &core);
  virtual void destroy(Core &core);
    
private:
  List *_list;
  Element *_elem;
  std::unique_ptr<Element> _oldelem;
  
};

#endif // H_removefromlist
