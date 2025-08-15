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

#include <memory>
#include <vector>

class List: public Box, public Pushable {

  typedef Box super;

public:
  List() {}
  
  // Box
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual std::string getName() { return "????"; };
  virtual rfl::Generic getGeneric();

  // Pushable
  virtual void push(Box *box) {
    _objs.push_back(std::unique_ptr<Box>(box));
  }
  
  // helpers for things that look like a list.
  static void buildVector(Renderer &renderer, std::vector<std::unique_ptr<Box> > &list);
  static Size layoutVector(const Size &size, std::vector<std::unique_ptr<Box> > &list);
  static void renderVector(Renderer &renderer, const Point &origin, std::vector<std::unique_ptr<Box> > &list);
  static void drawBorder(Renderer &renderer, const Point &origin, const Size &size, bool prop);
  
private:

  std::vector<std::unique_ptr<Box> > _objs;
  
};

#endif // H_list
