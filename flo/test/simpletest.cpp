/*
  simpletest.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include <iostream>

#include "processor.hpp"
#include "functions.hpp"
#include "generic.hpp"

#include <fstream>
#include <rfl/json.hpp>
#include <filesystem>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace std;
namespace fs = std::filesystem;
using namespace flo;

rfl::Generic loadJSON(const string &fn) {

  auto g = rfl::json::load<rfl::Generic>(fn);
  if (!g) {
    cout << g.error().what() << endl;
    return 0;
  }
  return *g;

}

BOOST_AUTO_TEST_CASE( nullTest )
{
  cout << "=== nullTest ===" << endl;
  
  auto hello = loadJSON("../flo/test/hello.json");
//  cout << rfl::json::write<rfl::Generic>(hello) << endl;
  
  Functions f;
  Processor p(hello, f);

  auto transform = loadJSON("../flo/test/null-t.json");
  auto result = p.transform(transform);
  BOOST_CHECK(result);
  auto obj = Generic::getObject(*result);
  BOOST_CHECK(obj);
  auto m = Generic::getString(obj, "message");
  BOOST_CHECK(m);
  BOOST_CHECK_EQUAL(*m, "hello");
  
}

BOOST_AUTO_TEST_CASE( noFuncTest )
{
  cout << "=== noFuncTest ===" << endl;
  
  Functions f;
  Processor p(f);

  auto transform = loadJSON("../flo/test/bad-t.json");
  
  auto result = p.transform(transform);
  BOOST_CHECK(result);
  auto obj = Generic::getObject(*result);
  BOOST_CHECK(obj);
  auto m = Generic::getString(obj, "error");
  BOOST_CHECK(m);
  BOOST_CHECK_EQUAL(*m, "function xxxx not found");
  
}

BOOST_AUTO_TEST_CASE( ifTrueTest )
{
  cout << "=== ifTrueTest ===" << endl;
  
  auto hello = loadJSON("../flo/test/message.json");
  
  Functions f;
  Processor p(hello, f);

  auto transform = loadJSON("../flo/test/true-t.json");
  
  auto result = p.transform(transform);
  BOOST_CHECK(result);
  auto obj = Generic::getObject(*result);
  BOOST_CHECK(obj);
  auto m = Generic::getString(obj, "message");
  BOOST_CHECK(m);
  BOOST_CHECK_EQUAL(*m, "world");
  
}
