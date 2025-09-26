/*
  changes.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 26-Sep-2025
    
  The changes to the system (for undo)
  
  changes looks like:
  
  As they are added.
  change1
  change2
  change3 
  <-undo ptr (can't redo since ptr is at end)
  
  
  Undo:
  ...
  change3 <-undo ptr (undo and redo)
  
  Undo:
  ...
  change2 <-undo ptr (undo and redo)
  ...
  
  Undo:
  ...
  change1 <-undo ptr (can't undo since ptr is at beginning)
  ...
  
  Redo
  ...
  change2 <-undo ptr
  ...
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_changes
#define H_changes

#include "change.hpp"
#include <memory>
#include <vector>

class Element;
class HUD;

class Changes {

public:
  Changes(): _undoptr(_changes.end()) {}
  
  void destroy(Renderer &renderer);
  void undo(Renderer &renderer, HUD *hud, Element *element);
  void redo(Renderer &renderer, HUD *hud, Element *element);
  void exec(Renderer &renderer, HUD *hud, Element *element, Change *change);
  void setUndoFlags(Renderer &renderer, HUD *hud);
  int size() { return _changes.size(); }
  
private:

  std::vector<std::unique_ptr<Change> > _changes;
  std::vector<std::unique_ptr<Change> >::iterator _undoptr;

  void dump();

};

#endif // H_changes
