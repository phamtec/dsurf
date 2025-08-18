/*
  texteditor.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 18-Aug-2025
    
  Text editor class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_texteditor
#define H_texteditor

#include "box.hpp"
#include "text.hpp"
#include "colours.hpp"

class TextEditor: public Box {

  typedef Box super;
  
public:
  TextEditor(): _size(100, 100) {}

  // Box
  virtual Size layout() { return _size; };
  virtual void render(Renderer &renderer, const Point &origin);
  virtual std::string getName() { return "????"; }
  virtual rfl::Generic getGeneric() { return 0; }

private:

  Size _size;
  
};

#endif // H_texteditor
