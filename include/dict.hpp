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

#include <memory>
#include <vector>

class Dict: public Box, public Pushable {

  typedef Box super;

public:
  Dict() {}
  
  // Box
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual rfl::Generic getGeneric();
  virtual std::string getName() { return "????"; };
  virtual Box *hitTest(const Point &origin, const Point &p);
  
  // Pushable
  virtual void push(Box *box) {
    _objs.push_back(std::unique_ptr<Box>(box));
  }
  
  static void drawBorder(Renderer &renderer, const Point &origin, const Size &size, bool prop);

private:

  std::vector<std::unique_ptr<Box> > _objs;

};

#endif // H_dict
