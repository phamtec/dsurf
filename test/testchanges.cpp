/*
  testchanges.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 26-Sep-2025
    
  Changes tests.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "changes.hpp"

#include "hudflags.hpp"
#include "element.hpp"
#include "hud.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <fstream>

using namespace std;

class Renderer {
};

vector<tuple<HUDFlags, bool> > flags;

HUD::HUD() {
}
void HUD::setFlag(Renderer &renderer, HUDFlags flag, bool state) {
  flags.push_back(tuple<HUDFlags, bool>(flag, state));
}

Text::~Text() {
}

BOOST_AUTO_TEST_CASE( noChanges )
{
  cout << "=== noChanges ===" << endl;
   
  Changes changes;
  Renderer r;
  HUD h;
  flags.clear();
  changes.setUndoFlags(r, &h);
  BOOST_CHECK_EQUAL(flags.size(), 2);
  BOOST_CHECK_EQUAL(get<0>(flags[0]), canRedo);
  BOOST_CHECK_EQUAL(get<1>(flags[0]), false);
  BOOST_CHECK_EQUAL(get<0>(flags[1]), canUndo);
  BOOST_CHECK_EQUAL(get<1>(flags[1]), false);
}

class TestChange: public Change {

public:
  TestChange(const string &name): _name(name) {}
  
  // Change
  virtual std::string describe() { return _name; }
  virtual void exec(Renderer &renderer) {}
  virtual void undo(Renderer &renderer) {}

private:
  string _name;
  
};

class TestElement: public Element {

public:

  // Element
  virtual void layout() {}
  virtual void build(Renderer &renderer) {}
  virtual void render(Renderer &renderer, const Point &origin) {}
  virtual Size size() { return Size(); }

};

std::string Element::describe() {
  return "e";
}
Element *Element::root() {
  return nullptr;
}
Element *Element::hitTest(const Point &origin, const Point &p) {
  return nullptr;
}

BOOST_AUTO_TEST_CASE( change )
{
  cout << "=== change ===" << endl;
   
  flags.clear();

  Changes changes;
  Renderer r;
  HUD h;
  TestElement e;
  
  // make a change
  changes.exec(r, &h, &e, new TestChange("1"));

  BOOST_CHECK_EQUAL(changes.size(), 1);

  BOOST_CHECK_EQUAL(flags.size(), 2);
  
  BOOST_CHECK_EQUAL(get<0>(flags[0]), canRedo);
  BOOST_CHECK_EQUAL(get<1>(flags[0]), false);
  
  BOOST_CHECK_EQUAL(get<0>(flags[1]), canUndo);
  BOOST_CHECK_EQUAL(get<1>(flags[1]), true);
}

BOOST_AUTO_TEST_CASE( undo )
{
  cout << "=== undo ===" << endl;
   
  flags.clear();

  Changes changes;
  Renderer r;
  HUD h;
  TestElement e;
  
  // make a change
  changes.exec(r, &h, &e, new TestChange("1"));
  
  // undo it.
  flags.clear();
  changes.undo(r, &h, &e);

  BOOST_CHECK_EQUAL(changes.size(), 1);

  BOOST_CHECK_EQUAL(flags.size(), 2);
  
  BOOST_CHECK_EQUAL(get<0>(flags[0]), canRedo);
  BOOST_CHECK_EQUAL(get<1>(flags[0]), true);
  
  BOOST_CHECK_EQUAL(get<0>(flags[1]), canUndo);
  BOOST_CHECK_EQUAL(get<1>(flags[1]), false);
}

BOOST_AUTO_TEST_CASE( redo )
{
  cout << "=== redo ===" << endl;
   
  flags.clear();

  Changes changes;
  Renderer r;
  HUD h;
  TestElement e;
  
  // make a change
  changes.exec(r, &h, &e, new TestChange("1"));
  changes.undo(r, &h, &e);

  flags.clear();
  changes.redo(r, &h, &e);

  BOOST_CHECK_EQUAL(changes.size(), 1);

  BOOST_CHECK_EQUAL(flags.size(), 2);
  
  BOOST_CHECK_EQUAL(get<0>(flags[0]), canRedo);
  BOOST_CHECK_EQUAL(get<1>(flags[0]), false);
  
  BOOST_CHECK_EQUAL(get<0>(flags[1]), canUndo);
  BOOST_CHECK_EQUAL(get<1>(flags[1]), true);
}

BOOST_AUTO_TEST_CASE( multipleUndo )
{
  cout << "=== multipleUndo ===" << endl;
   
  flags.clear();

  Changes changes;
  Renderer r;
  HUD h;
  TestElement e;
  
  // make a bunch of changes
  changes.exec(r, &h, &e, new TestChange("1"));
  changes.exec(r, &h, &e, new TestChange("2"));
  changes.exec(r, &h, &e, new TestChange("3"));
  changes.exec(r, &h, &e, new TestChange("4"));
  
  // undo them all.
  changes.undo(r, &h, &e);
  changes.undo(r, &h, &e);
  changes.undo(r, &h, &e);

  // undo it.
  flags.clear();
  changes.undo(r, &h, &e);

  BOOST_CHECK_EQUAL(changes.size(), 4);

  BOOST_CHECK_EQUAL(flags.size(), 2);
  
  BOOST_CHECK_EQUAL(get<0>(flags[0]), canRedo);
  BOOST_CHECK_EQUAL(get<1>(flags[0]), true);
  
  BOOST_CHECK_EQUAL(get<0>(flags[1]), canUndo);
  BOOST_CHECK_EQUAL(get<1>(flags[1]), false);

  // redo them all.
  changes.redo(r, &h, &e);
  changes.redo(r, &h, &e);
  changes.redo(r, &h, &e);

  flags.clear();
  changes.redo(r, &h, &e);

  BOOST_CHECK_EQUAL(changes.size(), 4);

  BOOST_CHECK_EQUAL(flags.size(), 2);
  
  BOOST_CHECK_EQUAL(get<0>(flags[0]), canRedo);
  BOOST_CHECK_EQUAL(get<1>(flags[0]), false);
  
  BOOST_CHECK_EQUAL(get<0>(flags[1]), canUndo);
  BOOST_CHECK_EQUAL(get<1>(flags[1]), true);
}

BOOST_AUTO_TEST_CASE( execAfterUndo )
{
  cout << "=== execAfterUndo ===" << endl;
   
  flags.clear();

  Changes changes;
  Renderer r;
  HUD h;
  TestElement e;
  
  // make a bunch of changes
  changes.exec(r, &h, &e, new TestChange("1"));
  changes.exec(r, &h, &e, new TestChange("2"));
  changes.exec(r, &h, &e, new TestChange("3"));
  changes.exec(r, &h, &e, new TestChange("4"));
  
  // undo 2 of them
  changes.undo(r, &h, &e);
  changes.undo(r, &h, &e);

  BOOST_CHECK_EQUAL(changes.size(), 4);

  // make a change.
  flags.clear();
  changes.exec(r, &h, &e, new TestChange("5"));

  // can't redo now
  BOOST_CHECK_EQUAL(flags.size(), 2);
  
  BOOST_CHECK_EQUAL(changes.size(), 3);

  BOOST_CHECK_EQUAL(get<0>(flags[0]), canRedo);
  BOOST_CHECK_EQUAL(get<1>(flags[0]), false);
  
  BOOST_CHECK_EQUAL(get<0>(flags[1]), canUndo);
  BOOST_CHECK_EQUAL(get<1>(flags[1]), true);

  flags.clear();
  changes.undo(r, &h, &e);

  BOOST_CHECK_EQUAL(flags.size(), 2);
  
  BOOST_CHECK_EQUAL(get<0>(flags[0]), canRedo);
  BOOST_CHECK_EQUAL(get<1>(flags[0]), true);
  
  BOOST_CHECK_EQUAL(get<0>(flags[1]), canUndo);
  BOOST_CHECK_EQUAL(get<1>(flags[1]), true);
}

BOOST_AUTO_TEST_CASE( redoPastEnd )
{
  cout << "=== redoPastEnd ===" << endl;
   
  flags.clear();

  Changes changes;
  Renderer r;
  HUD h;
  TestElement e;
  
  // make a bunch of changes
  changes.exec(r, &h, &e, new TestChange("1"));
  changes.undo(r, &h, &e);

  flags.clear();
  changes.redo(r, &h, &e);

  BOOST_CHECK_EQUAL(flags.size(), 2);
  
  BOOST_CHECK_EQUAL(get<0>(flags[0]), canRedo);
  BOOST_CHECK_EQUAL(get<1>(flags[0]), false);
  
  BOOST_CHECK_EQUAL(get<0>(flags[1]), canUndo);
  BOOST_CHECK_EQUAL(get<1>(flags[1]), true);

  flags.clear();
  changes.redo(r, &h, &e);

  BOOST_CHECK_EQUAL(changes.size(), 1);

  BOOST_CHECK_EQUAL(flags.size(), 0);

}
