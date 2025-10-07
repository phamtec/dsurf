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
#include "commandable.hpp"
#include "listable.hpp"
#include "writeable.hpp"
#include "list.hpp"
#include "objable.hpp"
#include "renderer.hpp"
#include "locatable.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <fstream>

using namespace std;

class X: public Element {
public:
  virtual Size layout() { return Size(); }
  virtual void build(Renderer &renderer) {}
  virtual void render(Renderer &renderer, const Point &origin) {}
  virtual Size size() { return Size(); }
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

BOOST_AUTO_TEST_CASE( nullCommandable )
{
  cout << "=== nullCommandable ===" << endl;

  try {
    Commandable::cast(0)->initHUD(0);
    BOOST_FAIL("Didn't catch");
  }
  catch (string &ex) {
    // all good.
    cout << ex << endl;
  }
  
}

BOOST_AUTO_TEST_CASE( commandable )
{
  cout << "=== commandable ===" << endl;

  X x;
  Commandable::cast(&x)->initHUD(0);
  
}

BOOST_AUTO_TEST_CASE( nullListable )
{
  cout << "=== nullListable ===" << endl;

  try {
    Listable::cast(0)->count();
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
  Listable::cast(&x)->count();
  
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

BOOST_AUTO_TEST_CASE( nullLocatable )
{
  cout << "=== nullLocatable ===" << endl;

  try {
    Locatable::cast(0)->getLocation();
    BOOST_FAIL("Didn't catch");
  }
  catch (string &ex) {
    // all good.
    cout << ex << endl;
  }
  
}

BOOST_AUTO_TEST_CASE( locatable )
{
  cout << "=== locatable ===" << endl;

  X x;
  Locatable::cast(&x)->getLocation();
  
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


