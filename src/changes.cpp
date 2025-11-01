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
#include "core.hpp"
#include "generic.hpp"

#include <algorithm>

using namespace std;
using flo::Generic;

void Changes::destroy(Core &core) {

  // make sure alkl the chnages objects get destroyed too.
  for_each(_changes.begin(), _changes.end(), [&core](auto& e) { 
    e->destroy(core); 
  });

}

void Changes::setUndoFlags(Core &core, HUD *hud) {

  if (_changes.size() == 0) {
    hud->setFlag(core, canRedo, false);
    hud->setFlag(core, canUndo, false);
    return;
  }

  hud->setFlag(core, canRedo, _undoptr != (_changes.end() - 1));
  hud->setFlag(core, canUndo, _undoptr != _changes.end());

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

void Changes::exec(Core &core, HUD *hud, Element *element, Change *change) {

  // throw away all the changes after the undo ptr.
  if (_changes.size() > 0 && _undoptr != _changes.end()) {
  
//     cout << "before trim" << endl;
//     dump();

    _changes.erase(_undoptr+1, _changes.end());
    
//     cout << "trimmed" << endl;
//     dump();
  }

  // execute this change
  change->exec(core);
  
  // layout the root.
  if (element) {
    auto root = element->root();
    if (root) {
      core.layout(root);
    }
  }
    
  // remember it.
  _changes.push_back(unique_ptr<Change>(change));
  
  // make sure the undoptr is at the end.
  _undoptr = _changes.end() - 1;
  
  setUndoFlags(core, hud);
  
}

void Changes::undo(Core &core, HUD *hud, Element *element) {

  if (_changes.size() == 0) {
    cout << "no changes" << endl;
    return;
  }
  
  if (_undoptr == _changes.end()) {
//    cout << "nothing to undo" << endl;
    return;
  }
  
  (*_undoptr)->undo(core);
  
  // layout the root.
  if (element) {
    auto root = element->root();
    if (root) {
      core.layout(root);
    }
  }
  
  if (_undoptr == _changes.begin()) {
    _undoptr = _changes.end();
  }
  else {
    _undoptr--;
  }
  
  setUndoFlags(core, hud);
  
}

void Changes::redo(Core &core, HUD *hud, Element *element) {

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
    (*_undoptr)->exec(core);
  }

  // layout the root.
  if (element) {
    auto root = element->root();
    if (root) {
      core.layout(root);
    }
  }
  
  
  setUndoFlags(core, hud);

}
