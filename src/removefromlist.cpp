/*
  removefromlist.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 22-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "removefromlist.hpp"

#include "list.hpp"
#include "move.hpp"

#include <iostream>

using namespace std;

RemoveFromList::RemoveFromList(Listable *list, Element *elem) {

  _list = list;
  _elem = elem;
  
}

void RemoveFromList::destroy(Renderer &renderer) {
  if (_oldelem) {
    _oldelem->destroy(renderer);
  }
}

void RemoveFromList::exec(Renderer &renderer) {

  auto elements = _list->getElements();
  if (!elements) {
    cerr << "list doesnt provide access to elements" << endl;
    return;
  }
  
  //  cout << "removing " << ix << endl;
  auto it = find_if(elements->begin(), elements->end(), [this](auto& e) { 
    return e.get() == _elem;
  });
  if (it == elements->end()) {
    cerr << "elem " << _elem->describe() << " not found to remove!" << endl;
    return;
  }
  
//   cout << "found" << endl;

  // save away the old element in the list.
  _oldelem = std::move(*it);
  cerr << "elem saved " << _oldelem->describe() << endl;
  
  // remove the element from the list (we have it)
  elements->erase(it);

}

void RemoveFromList::undo(Renderer &renderer) {

  auto elements = _list->getElements();
  if (!elements) {
    cerr << "list doesnt provide access to elements" << endl;
    return;
  }
  
  if (!_oldelem) {
    cerr << "cant undo, old elem is empty!" << endl;
    return;
  }
  
  // give it back to the list.
  elements->push_back(std::move(_oldelem));

}
