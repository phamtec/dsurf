/*
  setobj.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 22-Sep-2025
    
  A change that set's an object into another.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_setobj
#define H_setobj

#include "change.hpp"
#include "element.hpp"

#include <memory>

class Objable;

class SetObj: public Change {

public:
  SetObj(std::unique_ptr<Element> *obj, Element *newobj);
  
  virtual void exec(Renderer &renderer);
  virtual void undo(Renderer &renderer);
  virtual void destroy(Renderer &renderer);
    
private:
  std::unique_ptr<Element> *_obj;
  Element *_newobj;
//  std::unique_ptr<Element> _oldelem;
  
};

#endif // H_setobj
