/*
  long.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Long class.
  
  123
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_long
#define H_long

#include "box.hpp"
#include "text.hpp"
#include "colours.hpp"

class Long: public Box {

  typedef Box super;
  
public:
  Long(long value);

  // Box
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);

private:
  Text _value;
};

#endif // H_long
