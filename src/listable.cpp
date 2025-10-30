/*
  listable.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 15-Sep-2025
    
  Listable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "listable.hpp"

#include "list.hpp"

#include <sstream>

using namespace std;

Element *Listable::getByIndex(List *list, const string &str) {

//  cout << "getByIndex " << list->describe() << " " << str << endl;
  
  auto slash = str.find("/");
  if (slash != string::npos) {
    auto i = str.substr(0, slash);
    auto sub = getByIndex(list, i);
    if (!sub) {
      cerr << "element " << list->describe() << " has nothing at " << i << endl;
      return nullptr;
    }    
    auto list = dynamic_cast<List *>(sub);
    if (!list) {
      cerr << "element " << list->describe() << " is not a list" << endl;
      return nullptr;
    }    
    return getByIndex(list, str.substr(slash+1));
  }
  
//  cout << "found " << str << endl;
  stringstream ss(str);
  int index;
  ss >> index;
  if (list->count() <= index) {
//     cout << "element " << ss.str() << " doesn't exist" << endl;
//     cout << "element " << elem->describe() << " has " << cast(elem)->count() << endl;
    return nullptr;
  }
  
  return list->at(index);

}

Element *Listable::getByPath(List *root, const string &path) {

  if (path.size() > 1) {
    return getByIndex(root, path.substr(1));
  }
  
  return root;

}
