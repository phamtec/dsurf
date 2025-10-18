/*
  filledbox.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Aug-2025
    
  A filled box.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_filledbox
#define H_filledbox

#include "element.hpp"
#include "text.hpp"
#include "colours.hpp"

class FilledBox: public Element {

  typedef Element super;
  
public:
  FilledBox(const Size &size): _size(size) {}

  // Element
  virtual void layout() {};
  virtual void render(Renderer &renderer, const Point &origin);

private:

  Size _size;
  
};

#endif // H_filledbox
