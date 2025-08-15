/*
  listprop.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Aug-2025
    
  List property class.
  
  "name": [
    {
      ... props
    }
  ]

  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_listprop
#define H_listprop

#include "prop.hpp"
#include "pushable.hpp"

#include <memory>
#include <vector>

class ListProp: public Prop, public Pushable {

  typedef Prop super;

public:
  ListProp(const std::string &name): Prop(name) 
      {}
  
  // Box
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);

  // Pushable
  virtual void push(Box *box) {
    _objs.push_back(std::unique_ptr<Box>(box));
  }
  
private:
  
  std::vector<std::unique_ptr<Box> > _objs;
  
};

#endif // H_listprop
