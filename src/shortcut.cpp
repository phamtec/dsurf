/*
  shortcut.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 5-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "hud.hpp"

#include "renderer.hpp"
#include "colours.hpp"

using namespace std;

void Shortcut::set(const std::string &key, const std::string &text) {

  _key.set(key, Colours::black);
  _text.set(text, Colours::grey);
  
}

void Shortcut::build(Renderer &renderer) {

  _key.build(renderer);
  _text.build(renderer);
  
}

void Shortcut::render(Renderer &renderer, const Point &origin) {

  renderer.setScale(0.3, 0.3);
  
  double scale = 1 / 0.3;
  Point p = origin * scale;
  
//  cout << "key " << p << endl;
  _key.render(renderer, p, false);
  
  renderer.setScale(0.25, 0.25);
  
  scale = 1 / 0.25;
  p = ((origin + Size(0, 3)) * scale) + Size(_key.size().w + 10, 0);
  
//  cout << "text "  << p << endl;
  _text.render(renderer, p, false);

  renderer.setScale(1.0, 1.0);

}

Size Shortcut::size() {

  return Size(_key.size().w + _text.size().w, _key.size().h) * 0.3;
  
}
