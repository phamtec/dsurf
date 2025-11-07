/*
  scenario.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 23-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "scenario.hpp"

#include "core.hpp"
#include "unicode.hpp"
#include "root.hpp"
#include "builder.hpp"
#include "dict.hpp"

using namespace std;
using namespace vops;

CodeScenario::CodeScenario(const rfl::Generic &scenario, int index): 
  _parent(0), _hudobj(-1), _loaded(false), _index(index) {

  _scenario = scenario;
  auto obj = Dict::getObject(scenario);
  if (!obj) {
    cerr << "scenario is not an obj" << endl;
    return;
  }
  
  auto name = Dict::getString(obj, "name");
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

void CodeScenario::build(Core &core) {

  _name.build(core);

}

void CodeScenario::render(Core &core, const Point &origin) {

  auto i = _layout.begin();
  
  _name.render(core, origin + (*i).origin);

//  core.renderRect(_r);

}

void CodeScenario::initHUD(HUD *hud) {

  _hudobj = hud->findMode("scenario");
  
}

void CodeScenario::setMode(Core &core, HUD *hud) {

  hud->setFlag(core, canLoad, !_loaded);

  hud->setMode(_hudobj);
  
}

void CodeScenario::processKey(Core &core, SDL_Keycode code) {

  if (core.processGlobalKey(code)) {
    return;
  }

  switch (code) {      
    case SDLK_L:
      {
        _loaded = true;
        auto root = dynamic_cast<CodeRoot *>(getParent());
        root->setScenario(core, _scenario, _index);
        root->run(core);
      }
      break;
  }

}
