/*
  listable.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Aug-2025
    
  Listable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_listable
#define H_listable

#include <string>
#include <vector>

class Element;
class Renderer;

class Listable {

public:
  virtual ~Listable() {};

  virtual void push(Element *element) = 0;
    // push the element.
    
  virtual int count() = 0;
      // the count of the elements
      
  virtual Element *at(int index) = 0;
      // return the element at that index
      
  virtual std::vector<std::unique_ptr<Element> > *getElements() { return nullptr; }
    // direct access to the elements is necesary for the undo (command) system.
    
  static Element *getByPath(Element *root, const std::string &path);
    // given a root, get the element by the path given.
    
  static Listable *cast(Element *obj);
  static Listable *_err;
  
private:
  static Element *getByIndex(Element *elem, const std::string &str);

};

#endif // H_listable
