/*
  root.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 23-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "root.hpp"

#include "renderer.hpp"
#include "unicode.hpp"
#include "generic.hpp"
#include "builder.hpp"
#include "list.hpp"
#include "sizes.hpp"
#include "scenario.hpp"

using namespace std;

CodeRoot::CodeRoot(const rfl::Generic &obj): 
  _parent(0), _hudobj(-1), _running(false) {
  
  _scenarioslabel.set(L"Scenarios", Colours::black);
  _librarylabel.set(L"Library", Colours::black);

  auto code = Generic::getObject(obj);
  if (code) {
    _flo.reset(new Flo(*code));
    auto transform = Generic::getObject(*code, "transform");
    if (transform) {
      _transform = unique_ptr<Element>(Builder::walk(this, *transform));
    }
    auto library = Generic::getVector(*code, "library");
    if (library) {
      _library = unique_ptr<Element>(Builder::walk(this, *library));
    }
    auto scenarios = Generic::getVector(*code, "scenarios");
    if (scenarios) {
      std::transform((*scenarios).begin(), (*scenarios).end(), back_inserter(_scenarios), [this](auto e) {
        auto scenario = new CodeScenario(e);
        scenario->setParent(this);
        return unique_ptr<Element>(scenario);
      });
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
  
  _input = unique_ptr<Element>(new List(false));
  _input->setParent(this);
  _output = unique_ptr<Element>(new List(false));
  _output->setParent(this);

}

void CodeRoot::build(Renderer &renderer) {

  _scenarioslabel.build(renderer);
  _librarylabel.build(renderer);

}

bool CodeRoot::visit(std::function<bool (Element *)> f) {

  if (!f(this)) {
    return false;
  }
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
  if (!_input->visit(f)) {
    return false;
  }
  if (!_output->visit(f)) {
    return false;
  }

  return true;
  
}

RectList CodeRoot::calcLayout() {

  RectList layout;
  auto o = Point();

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
    auto ins = _input->size();
    layout.push_back(Rect(Point(-ins.w - Sizes::group_indent, tsy), ins + (Sizes::text_padding * 2)));
    auto outs = _output->size();
    layout.push_back(Rect(Point(ts.w + Sizes::group_indent, tsy), outs + (Sizes::text_padding * 2)));
  }
  Layout::addSize(&layout, Size(ts.w, o.y));

  return layout;
  
}

void CodeRoot::layout() {

  _transform->layout();
  _input->layout();
  _output->layout();
  _library->layout();
  for (auto& e: _scenarios) {
    e->layout();
  }
  
  // calculate the layout.
  _layout = calcLayout();
  _size = Layout::size(_layout);

}

void CodeRoot::render(Renderer &renderer, const Point &origin) {

  auto i = _layout.begin();
  
  _scenarioslabel.render(renderer, origin + (*i).origin);
  i++;

  for (auto& e: _scenarios) {
    e->render(renderer, origin + (*i).origin);
    i++;
  }

  renderer.renderFilledRect(*i + origin, Colours::straw);
  _transform->render(renderer, origin + (*i).origin + Size(Sizes::text_padding, Sizes::text_padding));
  i++;

  _librarylabel.render(renderer, origin + (*i).origin);
  i++;

  _library->render(renderer, origin + (*i).origin + Size(Sizes::text_padding, Sizes::text_padding));
  i++;

  if (_running) {
    renderer.renderFilledRect(*i + origin, Colours::lightPlum);
    _input->render(renderer, origin + (*i).origin + Sizes::text_padding);
    i++;
    renderer.renderFilledRect(*i + origin, Colours::teaGreen);
    _output->render(renderer, origin + (*i).origin + Sizes::text_padding);
  }

}

Element *CodeRoot::hitTest(const Point &origin, const Point &p) { 

  auto i = _layout.begin();
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
    hit = _input->hitTest(origin + i->origin, p);
    if (hit) {
      return hit;
    }
    i++;
  }
  
  return super::hitTest(origin, p);
  
}


Point CodeRoot::localOrigin(Element *elem) {

  auto i = _layout.begin();
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
    if (elem == _input.get()) {
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
  Commandable::cast(_input.get())->initHUD(hud);
  Commandable::cast(_output.get())->initHUD(hud);
  
}

void CodeRoot::setMode(Renderer &renderer, HUD *hud) {

  // we can run if we have an input context.
  hud->setFlag(renderer, canRun, !_running);
  
  hud->setMode(_hudobj);
  
}

void CodeRoot::processKey(Renderer &renderer, SDL_Keycode code) {

  if (renderer.processGlobalKey(code)) {
    return;
  }

}

void CodeRoot::setInput(Renderer &renderer, const rfl::Generic &input) {

  _input->destroy(renderer);
  _input = unique_ptr<Element>(Builder::walk(this, input));
  _input->setParent(this);
  renderer.build(_input.get());
  layout();
  
}

void CodeRoot::run(Renderer &renderer) {

  _running = true;

  auto in = Writeable::cast(_input.get())->getGeneric();
  auto t = Writeable::cast(_transform.get())->getGeneric();
  auto to = Generic::getObject(t);
  auto out = _flo->evalObj(in, *to);
  
  // clear out the output
  _output->destroy(renderer);
  
  // rebuild it.
  if (out) {
    cout << Generic::toString(*out) << endl;
    _output = unique_ptr<Element>(Builder::walk(this, *out));
  }
  else {
    _output = unique_ptr<Element>(new List(false));
    _output->setParent(this);
  }
  
  renderer.build(_output.get());
  
  layout();
  
}

void CodeRoot::changed(Renderer &renderer, Element *obj) {

  // test the transform.
  if (!_transform->visit([this, &renderer, obj](auto e) {
    if (e == obj) {
      run(renderer);
      return false;
    }
    return true;
  })) {
    return;
  }

  // test the input.
  _input->visit([this, &renderer, obj](auto e) {
    if (e == obj) {
      run(renderer);
      return false;
    }
    return true;
  });
  
  // test the sceenarios.
  for (auto& e: _scenarios) {
    e->visit([this, &renderer, obj](auto e2) {
      if (e2 == obj) {
        run(renderer);
        return false;
      }
      return true;
    });
  }

  _input->visit([this, &renderer, obj](auto e) {
    if (e == obj) {
      run(renderer);
      return false;
    }
    return true;
  });
  
  // test the library.
  if (!_library->visit([this, &renderer, obj](auto e) {
    if (e == obj) {
      rfl::Object<rfl::Generic> lib;
      lib["library"] = Writeable::cast(_library.get())->getGeneric();
      _flo.reset(new Flo(lib));
      run(renderer);
      return false;
    }
    return true;
  })) {
    return;
  }

}
