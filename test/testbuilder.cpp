/*
  testbuilder.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 15-Aug-2025
    
  Builder tests.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "builder.hpp"
#include "element.hpp"

#include "../modules/project/main.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <filesystem>
#include <rfl/json.hpp>
#include <rfl/yaml.hpp>

using namespace std;
namespace fs = std::filesystem;

void testJSON(const string &fname) {

  // parse in the JSON to our objects.
  unique_ptr<Element> element(Builder::loadFile(fname));
  string json = Builder::getJson(element.get());
  
  // only test things that are writeable.
  if (json == "not Writeable") {
    return;
  }
  
  // read in the original file.
  ifstream f(fname);
  BOOST_CHECK(f);
  string input(istreambuf_iterator<char>(f), {});
    
  // make sure they are identical.
  BOOST_CHECK_EQUAL(json, input);

}

void testYAML(const string &fname) {

  // parse in the JSON to our objects.
  unique_ptr<Element> element(Builder::loadFile(fname));
  string yaml = Builder::getYaml(element.get());
  
  // only test things that are writeable.
  if (yaml == "not Writeable") {
    return;
  }
  
  // read in the original file.
  ifstream f(fname);
  BOOST_CHECK(f);
  string input(istreambuf_iterator<char>(f), {});
    
  // make sure they are identical.
  BOOST_CHECK_EQUAL(yaml, input);

}

BOOST_AUTO_TEST_CASE( parseJSON )
{
  cout << "=== parseJSON ===" << endl;

  for (auto& e : fs::directory_iterator("../test")) {
    if (e.path().extension() == ".json") {
      cout << "  === " << e.path() << " ===" << endl;
      testJSON(e.path().string());
    }
  }
    
}

BOOST_AUTO_TEST_CASE( parseYAML )
{
  cout << "=== parseYAML ===" << endl;

  for (auto& e : fs::directory_iterator("../test")) {
    if (e.path().extension() == ".yaml") {
      cout << "  === " << e.path() << " ===" << endl;
      testYAML(e.path().string());
    }
  }
    
}
