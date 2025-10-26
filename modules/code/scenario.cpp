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
using flo::Generic;

CodeScenario::CodeScenario(const rfl::Generic &scenario, int index): 
  _parent(0), _hudobj(-1), _loaded(false), _index(index) {

  _scenario = scenario;
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

  cout << *name << endl;
  
  _name.set(Unicode::convert(*name), Colours::black);
  
}

RectList CodeScenario::calcLayout() {

  RectList layout;
  auto o = Point();
  auto s = _name.size();
  layout.push_back(Rect(o, s));
  Layout::addSize(&layout, s);

  return layout;
  
}

void CodeScenario::layout() {

  // calculate the layout.
  _layout = calcLayout();
  _size = Layout::size(_layout);
  
}

void CodeScenario::build(Renderer &renderer) {

  _name.build(renderer);

}

void CodeScenario::render(Renderer &renderer, const Point &origin) {

  auto i = _layout.begin();
  
  _name.render(renderer, origin + (*i).origin);

//  renderer.renderRect(_r);

}

void CodeScenario::initHUD(HUD *hud) {

  _hudobj = hud->findMode("scenario");
  
}

void CodeScenario::setMode(Renderer &renderer, HUD *hud) {

  hud->setFlag(renderer, canLoad, !_loaded);

  hud->setMode(_hudobj);
  
}

void CodeScenario::processKey(Renderer &renderer, SDL_Keycode code) {

  if (renderer.processGlobalKey(code)) {
    return;
  }

  switch (code) {      
    case SDLK_L:
      {
        _loaded = true;
        auto root = dynamic_cast<CodeRoot *>(getParent());
        root->setScenario(renderer, _scenario, _index);
        root->run(renderer);
      }
      break;
  }

}
