/*
  listable.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 15-Sep-2025
    
  Listable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "listable.hpp"

#include "element.hpp"
#include "err.hpp"

#include <sstream>

using namespace std;

class ListableErr: public Listable {

public:

  virtual void push(Element *element) {
    Err::typeError(typeid(Listable));
  }
  virtual void remove(Renderer &renderer, Element *element) {
    Err::typeError(typeid(Listable));
  }
  virtual int count() {
    Err::typeError(typeid(Listable));
    return 0;
  }
  virtual Element *at(int index) {
    Err::typeError(typeid(Listable));
    return nullptr;
  }

};

Listable *Listable::_err = nullptr;

Listable *Listable::cast(Element *obj) {

  return Errable<Listable, ListableErr>::cast(obj);
  
}

Element *Listable::getByIndex(Element *elem, const string &str) {

//  cout << "getByIndex " << elem->describe() << " " << str << endl;
  
  auto slash = str.find("/");
  if (slash != string::npos) {
    string i = str.substr(0, slash);
    auto sub = getByIndex(elem, i);
    if (!sub) {
//      cerr << "element " << elem->describe() << " has nothing at " << i << endl;
      return nullptr;
    }    
    return getByIndex(sub, str.substr(slash+1));
  }
  
//  cout << "found " << str << endl;
  stringstream ss(str);
  int index;
  ss >> index;
  if (cast(elem)->count() <= index) {
//     cerr << "element " << ss.str() << " doesn't exist" << endl;
//     cerr << "element " << elem->describe() << " has " << cast(elem)->count() << endl;
    return nullptr;
  }
  
  return cast(elem)->at(index);

}

Element *Listable::getByPath(Element *root, const string &path) {

  if (path.size() > 1) {
    return getByIndex(root, path.substr(1));
  }
  
  return root;

}
