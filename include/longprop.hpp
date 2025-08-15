/*
  longprop.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Long property class.
  
  name: 123
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_longprop
#define H_longprop

#include "prop.hpp"

class LongProp: public Prop {

  typedef Prop super;
  
public:
  LongProp(const std::string &name, long value);

  // Box
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);

private:
  Text _value;
};

#endif // H_longprop
