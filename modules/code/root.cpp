/*
  root.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 23-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "root.hpp"

#include "core.hpp"
#include "unicode.hpp"
#include "dict.hpp"
#include "builder.hpp"
#include "list.hpp"
#include "sizes.hpp"
#include "scenario.hpp"

using namespace std;
using namespace vops;

CodeRoot::CodeRoot(const std::string &filename, const DictG &obj): 
  _parent(0), _hudobj(-1), _running(false) {
  
  _filename.set(Unicode::convert(filename), Colours::black);
  _scenarioslabel.set(L"Scenarios", Colours::black);
  _librarylabel.set(L"Library", Colours::black);

  auto code = Dict::getObject(obj);
  if (code) {
    _flo.reset(new Flo(*code));
    auto transform = Dict::getObject(*code, "transform");
    if (transform) {
      _transform = unique_ptr<Element>(Builder::walk(this, *transform));
    }
    auto library = Dict::getVector(*code, "library");
    if (library) {
      _library = unique_ptr<Element>(Builder::walk(this, *library));
    }
    auto scenarios = Dict::getVector(*code, "scenarios");
    if (scenarios) {
      for (int i=0; i<(*scenarios).size(); i++) {
        auto s = (*scenarios)[i];
        auto scenario = new CodeScenario(s, i);
        scenario->setParent(this);
        _scenarios.push_back(unique_ptr<Element>(scenario));
      }
    }
  }
  
  if (!_transform) {
    _transform = unique_ptr<Element>(new List(false));
    _transform->setParent(this);
  }
  if (!_library) {
    _library = unique_ptr<Element>(new List(true));
    _library->setParent(this);
  }
  
  _scenario = unique_ptr<Element>(new List(false));
  _scenario->setParent(this);
  _output = unique_ptr<Element>(new List(false));
  _output->setParent(this);

}

optional<string> CodeRoot::getFilename() { 

  return Unicode::convert(_filename.str()); 
  
}

void CodeRoot::setDirty(Core &core, bool state) {

  _filename.set(_filename.str(), state ? Colours::red : Colours::black);
  _filename.build(core);

}

void CodeRoot::build(Core &core) {

  _filename.build(core);
  _scenarioslabel.build(core);
  _librarylabel.build(core);

}

bool CodeRoot::visit(std::function<bool (Element *)> f) {

  if (!_transform->visit(f)) {
    return false;
  }
  if (!_library->visit(f)) {
    return false;
  }
  for (auto& e: _scenarios) {
    if (!e->visit(f)) {
      return false;
    }
  }
  if (!_scenario->visit(f)) {
    return false;
  }
  if (!_output->visit(f)) {
    return false;
  }

  return f(this);
  
}

RectList CodeRoot::calcLayout() {

  RectList layout;
  auto o = Point();

  auto fs = _filename.size();
  layout.push_back(Rect(o, fs));

  o.y += fs.h + Sizes::text_padding;

  auto sls = _scenarioslabel.size();
  layout.push_back(Rect(o, sls));

  o.y += sls.h + Sizes::text_padding;
  
  for (auto& e: _scenarios) {
    auto ss = e->size();
    layout.push_back(Rect(o + Size(Sizes::group_indent, 0), ss));
    o.y += ss.h;
  }

  auto ts = _transform->size();
  ts.h += Sizes::text_padding * 2;
  ts.w += Sizes::text_padding * 2;
  auto tsy = o.y;
  layout.push_back(Rect(o, ts));
  o.y += ts.h;
  
  auto lls = _librarylabel.size();
  layout.push_back(Rect(o, lls));

  o.y += lls.h + Sizes::text_padding;
  
  auto ls = _library->size();
  ls.h += Sizes::text_padding * 2;
  ls.w += Sizes::text_padding * 2;
  layout.push_back(Rect(o, ls));
  o.y += ls.h;

  if (_running) {
    auto ins = _scenario->size();
    layout.push_back(Rect(Point(-ins.w - Sizes::group_indent, tsy), ins + (Sizes::text_padding * 2)));
    auto outs = _output->size();
    layout.push_back(Rect(Point(ts.w + Sizes::group_indent, tsy), outs + (Sizes::text_padding * 2)));
  }
  if (fs.w > ts.w) {
    ts.w = fs.w;
  }
  Layout::addSize(&layout, Size(ts.w, o.y));

  return layout;
  
}

void CodeRoot::layout() {

  _layout = calcLayout();
  _size = Layout::size(_layout);

}

void CodeRoot::render(Core &core, const Point &origin) {

  auto i = _layout.begin();
  
  _filename.render(core, origin + (*i).origin);
  i++;

  _scenarioslabel.render(core, origin + (*i).origin);
  i++;

  for (auto& e: _scenarios) {
    e->render(core, origin + (*i).origin);
    i++;
  }

  core.renderFilledRect(*i + origin, Colours::straw);
  _transform->render(core, origin + (*i).origin + Size(Sizes::text_padding, Sizes::text_padding));
  i++;

  _librarylabel.render(core, origin + (*i).origin);
  i++;

  _library->render(core, origin + (*i).origin + Size(Sizes::text_padding, Sizes::text_padding));
  i++;

  if (_running) {
    core.renderFilledRect(*i + origin, Colours::lavender);
    _scenario->render(core, origin + (*i).origin + Sizes::text_padding);
    i++;
    core.renderFilledRect(*i + origin, Colours::teaGreen);
    _output->render(core, origin + (*i).origin + Sizes::text_padding);
  }

}

Element *CodeRoot::hitTest(const Point &origin, const Point &p) { 

  auto i = _layout.begin();
  // skip filename
  i++;

  // skip scenarios label
  i++;

  for (auto& e: _scenarios) {
    auto hit = e->hitTest(origin + i->origin, p);
    if (hit) {
      return hit;
    }
    i++;
  }

  auto hit = _transform->hitTest(origin + i->origin, p);
  if (hit) {
    return hit;
  }
  i++;
  
  // skip library label
  i++;

  hit = _library->hitTest(origin + i->origin, p);
  if (hit) {
    return hit;
  }
  i++;
  
  if (_running) {
    hit = _scenario->hitTest(origin + i->origin, p);
    if (hit) {
      return hit;
    }
    i++;
  }
  
  return super::hitTest(origin, p);
  
}


Point CodeRoot::localOrigin(Element *elem) {

  auto i = _layout.begin();
  // skip filename
  i++;

  // skip scenarios label
  i++;

  for (auto& e: _scenarios) {
    if (elem == e.get()) {
      return i->origin;
    }
    i++;
  }

  if (elem == _transform.get()) {
    return i->origin + Sizes::text_padding;
  }
  i++;
  
  // skip library label
  i++;

  if (elem == _library.get()) {
    return i->origin + Sizes::text_padding;
  }
  i++;
  
  if (_running) {
    if (elem == _scenario.get()) {
      return i->origin + Sizes::text_padding;
    }
    i++;
    if (elem == _output.get()) {
      return i->origin + Sizes::text_padding;
    }
  }
  
  return Point(0, 0);
  
}

void CodeRoot::initHUD(HUD *hud) {

  _hudobj = hud->findMode("code");
  
  Commandable::cast(_transform.get())->initHUD(hud);
  Commandable::cast(_library.get())->initHUD(hud);
  for (auto& e: _scenarios) {
    Commandable::cast(e.get())->initHUD(hud);
  }
  Commandable::cast(_scenario.get())->initHUD(hud);
  Commandable::cast(_output.get())->initHUD(hud);
  
}

void CodeRoot::setMode(Core &core, HUD *hud) {

  // we can run if we have an input context.
  hud->setFlag(core, canRun, !_running);
  
  hud->setMode(_hudobj);
  
}

void CodeRoot::processKey(Core &core, SDL_Keycode code) {

  if (core.processRootKey(this, code)) {
    return;
  }

}

void CodeRoot::setScenario(Core &core, const DictG &scenario, int index) {

  _scenario->destroy(core);
  _scenario = unique_ptr<Element>(Builder::walk(this, scenario));
  core.build(_scenario.get());
  core.layout(this);
  _running = true;
  _scindex = index;
  
}

void CodeRoot::run(Core &core) {

  if (!_running) {
    return;
  }

  auto sobj = Dict::getObject(Writeable::cast(_scenario.get())->getGeneric());
  if (!sobj) {
    cerr << "scenario is not an obj" << endl;
    return;
  }
  auto in = Dict::getGeneric(sobj, "input");
  if (!in) {
    cerr << "scenario missing input" << endl;
    return;
  }

  auto t = Writeable::cast(_transform.get())->getGeneric();
  auto to = Dict::getObject(t);
  auto out = _flo->eval(*in, *to);
  
  // clear out the output
  _output->destroy(core);
  
  // rebuild it.
  if (out) {
//    cout << Dict::toString(*out) << endl;
    _output = unique_ptr<Element>(Builder::walk(this, *out));
  }
  else {
    _output = unique_ptr<Element>(new List(false));
    _output->setParent(this);
  }
  
  core.build(_output.get());
  
  core.layout(this);
  
}

void CodeRoot::rebuildScenarios(Core &core) {

  vector<Element *> scenarios;
  for (int i=0; i<_scenarios.size(); i++) {
    auto s = std::move(_scenarios[i]).get();
    if (i == _scindex) {
      // create a new scenario out of the one we are editing so it will be written out.
      auto scenario = new CodeScenario(Writeable::cast(_scenario.get())->getGeneric(), i);
      scenario->setParent(this);
      core.destroy(s);
      scenarios.push_back(scenario);
      core.build(scenario);
    }
    else {
      scenarios.push_back(s);
    }
  }
  _scenarios.clear();
  transform(scenarios.begin(), scenarios.end(), back_inserter(_scenarios), [](auto e) {
    return unique_ptr<Element>(e);
  });
  
  core.layout(root());
  
}

void CodeRoot::changed(Core &core, Element *obj) {

//  cout << "CodeRoot::changed " << obj->describe() << endl;

  // test the transform.
  if (!_transform->visit([this, &core, obj](auto e) {
//    cout << "testing " << e->describe() << endl;
    if (e == obj) {
//      cout << "running" << endl;
      run(core);
      return false;
    }
    return true;
  })) {
    return;
  }

  // test the scenarios.
  for (auto& e: _scenarios) {
    e->visit([this, &core, obj](auto e2) {
      if (e2 == obj) {
        run(core);
        return false;
      }
      return true;
    });
  }

  // test the scenario we are actually in.
  _scenario->visit([this, &core, obj](auto e) {
    if (e == obj) {
      run(core);
      rebuildScenarios(core);
      return false;
    }
    return true;
  });
  
  // test the library.
  if (!_library->visit([this, &core, obj](auto e) {
    if (e == obj) {
      DictO lib;
      lib["library"] = Writeable::cast(_library.get())->getGeneric();
      _flo.reset(new Flo(lib));
      run(core);
      return false;
    }
    return true;
  })) {
    return;
  }

}

std::string CodeRoot::getName() {

  return "code";
  
}

DictG CodeRoot::getGeneric() { 

  DictO obj;
  
  // build a code object from the parts
  obj["library"] = Writeable::cast(_library.get())->getGeneric();
  obj["transform"] = Writeable::cast(_transform.get())->getGeneric();
  vector<DictG> scenarios;
  transform(_scenarios.begin(), _scenarios.end(), back_inserter(scenarios), [](auto& e) {
    return Writeable::cast(e.get())->getGeneric();
  });
  obj["scenarios"] = scenarios;

  return obj;
  
}

std::wstring CodeRoot::getString() {

  return _filename.str();
  
}

void CodeRoot::setString(Core &core, const wstring &s) {

  _filename.set(s, Colours::black);
  _filename.build(core);
  
}

