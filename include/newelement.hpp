/*
  newelement.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 22-Sep-2025
    
  A change that adds a new element to a list.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_newelement
#define H_newelement

#include "change.hpp"

#include <memory>

class Element;
class List;

class NewElement: public Change {

public:
  NewElement(List *list, Element *elem);
  
  virtual void exec(Core &core);
  virtual void undo(Core &core);
  virtual void destroy(Core &core);
    
private:
  List *_list;
  Element *_elem;
  std::unique_ptr<Element> _newelem;
  
};

#endif // H_newelement
