/*
  shortcut.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 5-Sep-2025
    
  A shortcut for the heads up display.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_shortcut
#define H_shortcut

#include "text.hpp"
#include "point.hpp"

class Renderer;

class Shortcut {

public:
  Shortcut(const std::wstring &key, const std::wstring &text);
  
  void build(Renderer &renderer);
  void render(Renderer &renderer, const Point &origin);
  
  Size size();
  
private:

  Text _key;
  Text _text;

};

#endif // H_shortcut
