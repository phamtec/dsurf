/*
  dictprop.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Dictionary property class.
  
  "name": {
    ... props
  }
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_dictprop
#define H_dictprop

#include "box.hpp"
#include "text.hpp"
#include "pushable.hpp"

#include <memory>
#include <vector>

class DictProp: public Box, public Pushable {

  typedef Box super;

public:
  DictProp(const std::string &name): _name(name, Colours::blue) {}

  // Box
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual std::string getName() { return _name.str(); }
  virtual rfl::Generic getGeneric();

  // Pushable
  virtual void push(Box *box) {
    _objs.push_back(std::unique_ptr<Box>(box));
  }
  
private:
  Text _name;
  std::vector<std::unique_ptr<Box> > _objs;
  
};

#endif // H_dictprop
