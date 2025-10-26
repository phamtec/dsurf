/*
  testproject.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 26-Aug-2025
    
  Builder tests.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "builder.hpp"
#include "element.hpp"
#include "writeable.hpp"
#include "generic.hpp"

#include "../modules/project/main.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <filesystem>
#include <rfl/json.hpp>

using namespace std;
namespace fs = std::filesystem;
using flo::Generic;

BOOST_AUTO_TEST_CASE( isAProject )
{
  cout << "=== isAProject ===" << endl;

  auto result = rfl::json::load<rfl::Generic>("../modules/project/test/project.json");
  BOOST_CHECK(result);
  Project p;
  BOOST_CHECK(p.isA(*result));
  
}

BOOST_AUTO_TEST_CASE( loadAndSave )
{
  cout << "=== loadAndSave ===" << endl;

  auto result = rfl::json::load<rfl::Generic>("../modules/project/test/project.json");
  BOOST_CHECK(result);
  Project p;
  auto project = p.load(*result, "test.json");
  BOOST_CHECK(project);
//  auto out = Writeable::cast(project)->getGeneric();
  string json = Builder::getJson(project);

  // read in the original file.
  ifstream f("../modules/project/test/project.json");
  BOOST_CHECK(f);
  string input(istreambuf_iterator<char>(f), {});
    
  // make sure they are identical.
  BOOST_CHECK_EQUAL(json, input);

//  cout << Generic::toString(out) << endl;
  
}
