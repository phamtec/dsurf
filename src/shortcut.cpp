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

Shortcut::Shortcut(const wstring &key, const wstring &text, optional<HUDFlags> flags): 
  _flags(flags), _state(true) {
  
  setup(key, text);

}

void Shortcut::build(Renderer &renderer) {

  _key->build(renderer);
  _text->build(renderer);
  
}

void Shortcut::setup(const wstring &key, const wstring &text) {

//  wcout << "Shortcut: " << key << ": " << _state << endl;
  
  auto k = new Text();
  k->set(key, _state ? Colours::black : Colours::grey);
  _key.reset(k);

  auto t = new Text();
  t->set(text, Colours::grey);
  _text.reset(t);

}

bool Shortcut::setFlag(HUDFlags flag, bool state) {
 
  if (!_flags || _state == state || (*_flags & flag) == 0) {
    return false;
  }
  
  _state = state;
  
  setup(_key->str(), _text->str());

  return true;
  
}

void Shortcut::render(Renderer &renderer, const Point &origin) {

  double scale = 0.3;
  renderer.setScale(scale, scale);
  
  double kiscale = 1 / scale;
  Point p = origin * kiscale;
  
//  cout << "key " << p << endl;
  _key->render(renderer, p, false);
  int kwidth = _key->size().w * scale;
  
  scale = 0.22;
  renderer.setScale(scale, scale);
  
  double tiscale = 1 / scale;
  p = (origin + Size(1.8 + kwidth, 3.5)) * tiscale;
  
//  cout << "text "  << p << endl;
  _text->render(renderer, p, false);

  renderer.setScale(1.0, 1.0);

}

Size Shortcut::size() {

  return Size(_key->size().w + _text->size().w, _key->size().h) * 0.3;
  
}
