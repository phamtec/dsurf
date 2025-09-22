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

  auto ix = Indexable::cast(_elem)->getIndex();
  
  auto elements = _list->getElements();
  
//  cout << "removing " << ix << endl;
  auto it = find_if(elements->begin(), elements->end(), [ix](auto& e) { 
    return Indexable::cast(e.get())->getIndex() == ix;
  });
  if (it != elements->end()) {
  
//    cout << "found" << endl;

    // save away the old element in the list.
    _oldelem = std::move(*it);
    
    // remove the element from the list (we have it)
    elements->erase(it);
    
    // collect all the indexables.
    vector<Indexable *> objs;
    transform(elements->begin(), elements->end(), back_inserter(objs), [](auto& e) { return Indexable::cast(e.get()); });

    // make sure all the indexe match up.
    Move::shuffleDownFrom(&objs, Indexable::cast(_oldelem.get())->getIndex());
  }

}

void RemoveFromList::undo(Renderer &renderer) {

  auto elements = _list->getElements();

  // collect all the indexables.
  vector<Indexable *> objs;
  transform(elements->begin(), elements->end(), back_inserter(objs), [](auto& e) { return Indexable::cast(e.get()); });

  // make sure all the indexes will match up.
  Move::shuffleUpFrom(&objs, Indexable::cast(_oldelem.get())->getIndex());

  // give it back to the list.
  elements->push_back(std::move(_oldelem));

}
