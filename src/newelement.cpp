/*
  newelement.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 22-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "newelement.hpp"

#include "list.hpp"
#include "core.hpp"

#include <iostream>

using namespace std;

NewElement::NewElement(List *list, Element *elem) {

  _list = list;
  _elem = elem;
  
}

void NewElement::destroy(Core &core) {
  if (_newelem) {
    _newelem->destroy(core);
  }
}

void NewElement::exec(Core &core) {

  auto elements = _list->getElements();
  if (!elements) {
    cerr << "list doesnt provide access to elements" << endl;
    return;
  }

  elements->push_back(unique_ptr<Element>(_elem));

  // tell the core the parent has changed.
  core.changed(_list);
  
}

void NewElement::undo(Core &core) {

  auto elements = _list->getElements();
  if (!elements) {
    cerr << "list doesnt provide access to elements" << endl;
    return;
  }
  auto last = elements->end();
  last--;
  
  // move out the element.
  _newelem = std::move(*last);
  
  // remove it from the list.
  elements->erase(last);
  
  // tell the core the parent has changed.
  core.changed(_list);
  
}
