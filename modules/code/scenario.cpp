/*
  scenario.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 23-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "scenario.hpp"

#include "renderer.hpp"
#include "unicode.hpp"
#include "root.hpp"
#include "builder.hpp"
#include "generic.hpp"

using namespace std;

CodeScenario::CodeScenario(const rfl::Generic &scenario): 
  _parent(0), _hudobj(-1), _editing(true) {

  auto obj = Generic::getObject(scenario);
  if (!obj) {
    cerr << "scenario is not an obj" << endl;
    return;
  }
  
  auto name = Generic::getString(obj, "name");
  if (!name) {
    cerr << "scenario missing name" << endl;
    return;
  }
  
  auto input = Generic::getObject(obj, "input");
  if (!input) {
    cerr << "scenario missing input" << endl;
    return;
  }

  _input = *input;
  _name.set(Unicode::convert(*name), Colours::black);
  _editor = unique_ptr<Element>(Builder::walk(this, scenario));
  
}

RectList CodeScenario::calcLayout() {

  RectList layout;
  auto o = Point();
  if (_editing) {
    auto s = _editor->size();
    layout.push_back(Rect(o, s));
    Layout::addSize(&layout, s);
  }
  else {
    auto s = _name.size();
    layout.push_back(Rect(o, s));
    Layout::addSize(&layout, s);
  }

  return layout;
  
}

void CodeScenario::layout() {

  _editor->layout();
  
  // calculate the layout.
  _layout = calcLayout();
  _size = Layout::size(_layout);
  
}

void CodeScenario::build(Renderer &renderer) {

  _name.build(renderer);

}

void CodeScenario::render(Renderer &renderer, const Point &origin) {

  auto i = _layout.begin();
  
  if (_editing) {
    _editor->render(renderer, origin + (*i).origin);
  }
  else {
    _name.render(renderer, origin + (*i).origin);
  }

//  renderer.renderRect(_r);

}

bool CodeScenario::visit(std::function<bool (Element *)> f) {

  if (!f(this)) {
    return false;
  }
  if (!_editor->visit(f)) {
    return false;
  }

  return true;
  
}

void CodeScenario::initHUD(HUD *hud) {

  _hudobj = hud->findMode("scenario");
  _hudedit = hud->findMode("scenarioedit");
  
}

void CodeScenario::setMode(Renderer &renderer, HUD *hud) {

  hud->setMode(_editing ? _hudedit : _hudobj);
  
}

void CodeScenario::processKey(Renderer &renderer, SDL_Keycode code) {

  if (renderer.processGlobalKey(code)) {
    return;
  }

  switch (code) {      
    case SDLK_R:
      {
        auto root = dynamic_cast<CodeRoot *>(getParent());
        root->setInput(renderer, _input);
        root->run(renderer);
      }
      break;

    case SDLK_E:
      _editing = true;
      root()->layout();
      break;

    case SDLK_ESCAPE:
      _editing = false;
      root()->layout();
      break;
  }

}
