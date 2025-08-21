/*
  list.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Aug-2025
    
  List class.
  
  [
    {
      ... props
    }
  ]
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_list
#define H_list

#include "box.hpp"
#include "pushable.hpp"
#include "parentable.hpp"
#include "sizeable.hpp"
#include "writeable.hpp"

#include <memory>
#include <vector>

class List: public Box, public Pushable, public Parentable, public Sizeable, public Writeable  {

  typedef Box super;

public:
  List(Box *parent, int index): _parent(parent), _index(index) {}
  
  // Box
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual Box *hitTest(const Point &origin, const Point &p);
  virtual Point localOrigin(int index);
  
  // Writeable
  virtual rfl::Generic getGeneric();

  // Pushable
  virtual void push(Box *box) {
    _objs.push_back(std::unique_ptr<Box>(box));
  }
  
  // Parentable
  virtual Box *getParent() { return _parent; }
  virtual int getIndex() { return _index; }
  
  // Sizeable
  virtual Size getSize() { return _size; }

  // helpers for things that look like a list.
  static void buildVector(Renderer &renderer, std::vector<std::unique_ptr<Box> > &list);
  static Size layoutVector(const Size &size, std::vector<std::unique_ptr<Box> > &list);
  static void renderVector(Renderer &renderer, const Point &origin, std::vector<std::unique_ptr<Box> > &list);
  static void drawBorder(Renderer &renderer, const Point &origin, const Size &size, bool prop);
  static Box* hitTestVector(const Point &origin, const Point &p, std::vector<std::unique_ptr<Box> > &list);
  static Point localOriginVector(std::vector<std::unique_ptr<Box> > &list, int index, bool prop);
  static rfl::Generic getGenericVector(std::vector<std::unique_ptr<Box> > &list);
 
private:

  Box *_parent;
  int _index;
  Size _size;
  std::vector<std::unique_ptr<Box> > _objs;
  
};

#endif // H_list
