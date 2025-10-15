/*
  testflo.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 11-Oct-2025
    
  Basic fundamental tests for flo
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "flo.hpp"
#include "generic.hpp"

#include <rfl/json.hpp>

using namespace std;

rfl::Generic loadJSON(const string &fn) {

  auto g = rfl::json::load<rfl::Generic>(fn);
  if (!g) {
    cout << g.error().what() << endl;
    return 0;
  }
  return *g;

}

BOOST_AUTO_TEST_CASE( evalStringMember )
{
  cout << "=== evalStringMember ===" << endl;

  Flo flo;
  auto json = loadJSON("../test/string-t.json");
  auto obj = Generic::getObject(json);
  auto s = flo.evalStringMember(obj, "test");
  BOOST_CHECK(s);
  BOOST_CHECK_EQUAL(*s, "hello");

}

BOOST_AUTO_TEST_CASE( evalNumMember )
{
  cout << "=== evalNumMember ===" << endl;

  Flo flo;
  auto json = loadJSON("../test/num-t.json");
  auto obj = Generic::getObject(json);
  auto s = flo.evalNumMember(obj, "test");
  BOOST_CHECK(s);
  BOOST_CHECK_EQUAL(*s, 42);

}


BOOST_AUTO_TEST_CASE( evalObj )
{
  cout << "=== evalObj ===" << endl;

  Flo flo;
  auto obj = loadJSON("../test/obj-t.json");
  auto transform = Generic::getObject(obj);
  BOOST_CHECK(transform);
  rfl::Generic m;
  auto o = flo.evalObj(m, *transform);
  BOOST_CHECK(o);
  auto xxx = Generic::getString(o, "xxx");
  BOOST_CHECK(xxx);
  BOOST_CHECK_EQUAL(*xxx, "yyy");

}

