/*
  list.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Aug-2025
    
  List class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_list
#define H_list

#include "box.hpp"
#include "text.hpp"
#include <memory>
#include <vector>

class List: public Box {

public:
  List(const std::string &name): _name(name) {}
  
  // Box
  virtual void render(Renderer &renderer, Font &font, float x, float *y);

  void push(Box *box) {
    _objs.push_back(std::unique_ptr<Box>(box));
  }
  
private:
  Text _name;
  
  std::vector<std::unique_ptr<Box> > _objs;
  
};

#endif // H_list
