/*
  hud.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 5-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "hud.hpp"

#include "renderer.hpp"
#include "colours.hpp"

using namespace std;

// bool operator | (HUDState lhs, HUDState rhs) {
//   return static_cast<std::underlying_type_t<HUDState>>(lhs) |
//     static_cast<std::underlying_type_t<HUDState>>(rhs);
// }
// 
// bool operator & (HUDState lhs, HUDState rhs) {
//   return static_cast<std::underlying_type_t<HUDState>>(lhs) &
//     static_cast<std::underlying_type_t<HUDState>>(rhs);
// }

HUD::HUD() {

  _Append.set("A", "ppend");
  _Copy.set("C", "opy");
  _Paste.set("P", "aste");
  _Escape.set("Esc", "ape");

}

void HUD::build(Renderer &renderer) {
  
  _Append.build(renderer);
  _Copy.build(renderer);
  _Paste.build(renderer);
  _Escape.build(renderer);
  
}
  
void HUD::render(Renderer &renderer, const Point &mouse) {

  Point origin = mouse + Size(40, -40);
//    cout << "origin " << origin << endl;
  
  bool draw = false;
  Size l(0, 0);
  Size s(0, 0);
  switch (_state) {
    case HUDState::Text:
      draw = true;
      s = _Append.size();
      l = origin;
      break;
    case HUDState::All:
      draw = true;
      s = _Paste.size();
      s.h += _Copy.size().h;
      l = origin;
      break;
    case HUDState::Editing:
      draw = true;
      s = _Escape.size();
      l = _loc + Size(-10, -30);
      break;
    default:
      break;
  }
  
  if (draw) {
    renderer.setDrawColor(Colours::white);
    renderer.fillRect(Rect(l + Size(-4, -4), s + Size(8, 8)));
    renderer.setDrawColor(Colours::grey);
    renderer.drawRect(Rect(l + Size(-4, -4), s + Size(8, 8)));
  }
  
  switch (_state) {
    case HUDState::Text:
      _Append.render(renderer, origin);
      break;
    case HUDState::All:
      _Copy.render(renderer, origin + Size(0, 0));
      _Paste.render(renderer, origin + Size(0, _Copy.size().h));
      break;
    case HUDState::Editing:
      _Escape.render(renderer, _loc + Size(-10, -30));
      break;
    default:
      break;
  }

}
