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

HUD::HUD() {

  _Append.set(L"A", L"ppend");
  _Copy.set(L"C", L"opy");
  _Paste.set(L"P", L"aste");
  _Close.set(L"Esc", L"(revert)");
  _Move.set(L"Arrows", L"(navigate)");
  _Delete.set(L"Bksp", L"(delete)");
  _Finish.set(L"Ret|Tab", L"(close)");

}

void HUD::build(Renderer &renderer) {
  
  _Append.build(renderer);
  _Copy.build(renderer);
  _Paste.build(renderer);
  _Close.build(renderer);
  _Move.build(renderer);
  _Delete.build(renderer);
  _Finish.build(renderer);
  
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
      s = _Move.size();
      s.h *= 4;
      l = _loc + Size(0, -10 -s.h);
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
      {
        Size s = _Move.size();
        _Close.render(renderer, _loc + Size(0, -10 - (s.h*4)));
        _Move.render(renderer, _loc + Size(0, -10 - (s.h*3)));
        _Delete.render(renderer, _loc + Size(0, -10 - (s.h*2)));
        _Finish.render(renderer, _loc + Size(0, -10 - s.h));
      }
      break;
    default:
      break;
  }

}
