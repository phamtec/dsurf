/*
  newelement.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 22-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "newelement.hpp"

#include "list.hpp"

#include <iostream>

using namespace std;

NewElement::NewElement(Listable *list, Element *elem) {

  _list = list;
  _elem = elem;
  
}

void NewElement::destroy(Renderer &renderer) {
  if (_newelem) {
    _newelem->destroy(renderer);
  }
}

void NewElement::exec(Renderer &renderer) {

  // the element already has a parent and an index, just add it on.
  _list->getElements()->push_back(unique_ptr<Element>(_elem));

}

void NewElement::undo(Renderer &renderer) {

  auto elements = _list->getElements();
  
  auto last = elements->end();
  last--;
  
  // move out the element.
  _newelem = std::move(*last);
  
  // remove it from the list.
  elements->erase(last);
  
}
