/*
  testerrable.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 15-Sep-2025
    
  Builder tests.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "element.hpp"
#include "editable.hpp"
#include "hudable.hpp"
#include "keyable.hpp"
#include "parentable.hpp"
#include "listable.hpp"
#include "sizeable.hpp"
#include "writeable.hpp"
#include "list.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <fstream>

using namespace std;

class X: public Element {
public:
  virtual Size layout() { return Size(); }
  virtual void build(Renderer &renderer) {}
  virtual void render(Renderer &renderer, const Point &origin) {}
};

BOOST_AUTO_TEST_CASE( nullEditable )
{
  cout << "=== nullEditable ===" << endl;

  try {
    Editable::cast(0)->getString();
    BOOST_FAIL("Didn't catch");
  }
  catch (string &ex) {
    // all good.
    cout << ex << endl;
  }
  
}

BOOST_AUTO_TEST_CASE( editable )
{
  cout << "=== editable ===" << endl;

  X x;
  Editable::cast(&x)->getString();
  
}

BOOST_AUTO_TEST_CASE( nullHudable )
{
  cout << "=== nullHudable ===" << endl;

  try {
    HUDable::cast(0)->initHUD(0);
    BOOST_FAIL("Didn't catch");
  }
  catch (string &ex) {
    // all good.
    cout << ex << endl;
  }
  
}

BOOST_AUTO_TEST_CASE( hudable )
{
  cout << "=== hudable ===" << endl;

  X x;
  HUDable::cast(&x)->initHUD(0);
  
}

BOOST_AUTO_TEST_CASE( nullKeyable )
{
  cout << "=== nullKeyable ===" << endl;

  try {
    Renderer *r = nullptr;
    Keyable::cast(0)->processKey(*r, 0);
    BOOST_FAIL("Didn't catch");
  }
  catch (string &ex) {
    // all good.
    cout << ex << endl;
  }
  
}

BOOST_AUTO_TEST_CASE( keyable )
{
  cout << "=== keyable ===" << endl;

  Renderer *r = nullptr;
  X x;
  Keyable::cast(&x)->processKey(*r, 0);
  
}

BOOST_AUTO_TEST_CASE( nullParentable )
{
  cout << "=== nullParentable ===" << endl;

  try {
    Parentable::cast(0)->getParent();
    BOOST_FAIL("Didn't catch");
  }
  catch (string &ex) {
    // all good.
    cout << ex << endl;
  }
  
}

BOOST_AUTO_TEST_CASE( parentable )
{
  cout << "=== parentable ===" << endl;

  X x;
  Parentable::cast(&x)->getParent();
  
}

BOOST_AUTO_TEST_CASE( nullListable )
{
  cout << "=== nullListable ===" << endl;

  try {
    Listable::cast(0)->push(0);
    BOOST_FAIL("Didn't catch");
  }
  catch (string &ex) {
    // all good.
    cout << ex << endl;
  }
  
}

BOOST_AUTO_TEST_CASE( listable )
{
  cout << "=== listable ===" << endl;

  X x;
  Listable::cast(&x)->push(0);
  
}

BOOST_AUTO_TEST_CASE( nullSizeable )
{
  cout << "=== nullSizeable ===" << endl;

  try {
    Sizeable::cast(0)->getSize();
    BOOST_FAIL("Didn't catch");
  }
  catch (string &ex) {
    // all good.
    cout << ex << endl;
  }
  
}

BOOST_AUTO_TEST_CASE( sizeable )
{
  cout << "=== sizeable ===" << endl;

  X x;
  Sizeable::cast(&x)->getSize();
  
}

BOOST_AUTO_TEST_CASE( nullWriteable )
{
  cout << "=== nullWriteable ===" << endl;

  try {
    Writeable::cast(0)->getName();
    BOOST_FAIL("Didn't catch");
  }
  catch (string &ex) {
    // all good.
    cout << ex << endl;
  }
  
}

BOOST_AUTO_TEST_CASE( writeable )
{
  cout << "=== writeable ===" << endl;

  X x;
  Writeable::cast(&x)->getName();
  
}

BOOST_AUTO_TEST_CASE( listNull )
{
  cout << "=== listClass ===" << endl;
 
  try {
    List::cast(0);
    BOOST_FAIL("Didn't catch");
  }
  catch (string &ex) {
    // all good.
    cout << ex << endl;
  }
  
}

BOOST_AUTO_TEST_CASE( listNot )
{
  cout << "=== listNot ===" << endl;
   
  try {
    X x;
    List::cast(&x);
    BOOST_FAIL("Didn't catch");
  }
  catch (string &ex) {
    // all good.
    cout << ex << endl;
  }
  
}


