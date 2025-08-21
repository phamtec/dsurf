/*
  dict.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Dictionary class.
  
  {
    ... props
  }
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_dict
#define H_dict

#include "box.hpp"
#include "pushable.hpp"
#include "parentable.hpp"
#include "sizeable.hpp"

#include <memory>
#include <vector>

class Dict: public Box, public Pushable, public Parentable, public Sizeable {

  typedef Box super;

public:
  Dict(Box *parent, int index): _parent(parent), _index(index) {}
  
  // Box
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual rfl::Generic getGeneric();
  virtual Box *hitTest(const Point &origin, const Point &p);
  virtual Point localOrigin(int index);
  
  // Pushable
  virtual void push(Box *box) {
    _objs.push_back(std::unique_ptr<Box>(box));
  }
  
  // Parentable
  virtual Box *getParent() { return _parent; }
  virtual int getIndex() { return _index; }
  
  // Sizeable
  virtual Size getSize() { return _size; }

  static void drawBorder(Renderer &renderer, const Point &origin, const Size &size, bool prop);

private:

  Box *_parent;
  int _index;
  Size _size;
  std::vector<std::unique_ptr<Box> > _objs;

};

#endif // H_dict
