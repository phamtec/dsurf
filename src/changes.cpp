/*
  changes.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 26-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "changes.hpp"

#include "hud.hpp"
#include "element.hpp"
#include "renderer.hpp"

#include <algorithm>

using namespace std;

void Changes::destroy(Renderer &renderer) {

  // make sure alkl the chnages objects get destroyed too.
  for_each(_changes.begin(), _changes.end(), [&renderer](auto& e) { 
    e->destroy(renderer); 
  });

}

void Changes::setUndoFlags(Renderer &renderer, HUD *hud) {

  if (_changes.size() == 0) {
    hud->setFlag(renderer, canRedo, false);
    hud->setFlag(renderer, canUndo, false);
    return;
  }

  hud->setFlag(renderer, canRedo, _undoptr != (_changes.end() - 1));
  hud->setFlag(renderer, canUndo, _undoptr != _changes.end());

}

void Changes::dump() {
  if (_undoptr != _changes.end()) {
    cout << "undo at " << (*_undoptr)->describe() << endl;
  }
  cout << "list" << endl;
  for_each(_changes.begin(), _changes.end(), [](auto& e) {
    cout << e->describe() << endl;
  });
}

void Changes::exec(Renderer &renderer, HUD *hud, Element *element, Change *change) {

  // throw away all the changes after the undo ptr.
  if (_changes.size() > 0 && _undoptr != _changes.end()) {
  
//     cout << "before trim" << endl;
//     dump();

    _changes.erase(_undoptr+1, _changes.end());
    
//     cout << "trimmed" << endl;
//     dump();
  }

  // execute this change
  change->exec(renderer);
  
  // layout the root.
  auto root = element->root();
  if (root) {
    renderer.layout(root);
  }
  
  // remember it.
  _changes.push_back(unique_ptr<Change>(change));
  
  // make sure the undoptr is at the end.
  _undoptr = _changes.end() - 1;
  
  setUndoFlags(renderer, hud);
  
}

void Changes::undo(Renderer &renderer, HUD *hud, Element *element) {

  if (_changes.size() == 0) {
    cout << "no changes" << endl;
    return;
  }
  
  if (_undoptr == _changes.end()) {
//    cout << "nothing to undo" << endl;
    return;
  }
  
  (*_undoptr)->undo(renderer);
  
  // layout the root.
  auto root = element->root();
  if (root) {
    renderer.layout(root);
  }
  
  if (_undoptr == _changes.begin()) {
    _undoptr = _changes.end();
  }
  else {
    _undoptr--;
  }
  
  setUndoFlags(renderer, hud);
  
}

void Changes::redo(Renderer &renderer, HUD *hud, Element *element) {

  if (_changes.size() == 0) {
    cout << "no changes" << endl;
    return;
  }
  
  if (_undoptr == (_changes.end() - 1)) {
//    cout << "nothing to redo" << endl;
    return;
  }
  
  if (_undoptr == _changes.end()) {
    _undoptr = _changes.begin();
  }
  else {
    _undoptr++;
  }
  
  if (_undoptr != _changes.end()) {
    (*_undoptr)->exec(renderer);
  }

  // layout the root.
  auto root = element->root();
  if (root) {
    renderer.layout(root);
  }
  
  setUndoFlags(renderer, hud);

}
