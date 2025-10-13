/*
  libtest.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 13-Oct-2025
    
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

optional<rfl::Object<rfl::Generic> > loadJSON(const string &fn) {

  auto g = rfl::json::load<rfl::Generic>(fn);
  if (!g) {
    cout << g.error().what() << endl;
    return nullopt;
  }
  
  return Generic::getObject(*g);

}

BOOST_AUTO_TEST_CASE( simple )
{
  cout << "=== simple ===" << endl;
  
  auto file = loadJSON("../flo/test/library-t.json");
  BOOST_CHECK(file);
  
  auto library = Generic::getVector(file, "library");
  BOOST_CHECK(library);
  auto transform = Generic::getObject(file, "public");
  BOOST_CHECK(transform);
  
  Functions f(*library);
  Processor p(f);

  auto result = p.transform(*transform);
  BOOST_CHECK(result);
  auto s = Generic::getString(*result);
  BOOST_CHECK(s);
  BOOST_CHECK_EQUAL(*s, "a key");
  
}
