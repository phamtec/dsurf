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

public:
  List() {}
  
  // Box
  virtual void build(Renderer &renderer, Font &font);
  virtual float layout(Resources &pool, float x, float y);
  virtual void render(Renderer &renderer, Resources &pool);

  // Pushable
  virtual void push(Box *box) {
    _objs.push_back(std::unique_ptr<Box>(box));
  }
  
private:

  std::vector<std::unique_ptr<Box> > _objs;
  
};

#endif // H_list
