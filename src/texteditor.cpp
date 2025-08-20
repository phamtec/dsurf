/*
  texteditor.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 18-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "texteditor.hpp"
#include "colours.hpp"
#include "renderer.hpp"

void TextEditor::render(Renderer &renderer, const Point &origin) {

  renderer.renderFilledRect(Rect(_origin, _size), Colours::blue);
  
}
