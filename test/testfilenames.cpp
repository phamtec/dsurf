/*
  testfilenames.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 1-Nov-2025
    
  Basic fundamental tests for file names.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "filenames.hpp"

using namespace std;

BOOST_AUTO_TEST_CASE( simple )
{
  cout << "=== simple ===" << endl;

  auto msg = Filenames::isInvalid("myfile.json");
  BOOST_CHECK(!msg);

}

BOOST_AUTO_TEST_CASE( badExtension )
{
  cout << "=== badExtension ===" << endl;

  auto msg = Filenames::isInvalid("xxxx");
  BOOST_CHECK(msg);
  BOOST_CHECK_EQUAL(*msg, "Filename must have .json or .yml extension.");

}

BOOST_AUTO_TEST_CASE( badPath )
{
  cout << "=== badPath ===" << endl;

  auto msg = Filenames::isInvalid("/ThisProbablyIsBad/myfile.json");
  BOOST_CHECK(msg);
  BOOST_CHECK_EQUAL(*msg, "Path to file does not exist.");

}

BOOST_AUTO_TEST_CASE( addPathHasPath )
{
  cout << "=== addPathHasPath ===" << endl;

  string fn = "/SomePath/myfile.json";
  BOOST_CHECK_EQUAL(Filenames::addPath(fn), fn);

}

BOOST_AUTO_TEST_CASE( addPathHasNoPath )
{
  cout << "=== addPathHasNoPath ===" << endl;

  string fn = "myfile.json";
#ifdef __APPLE__
  BOOST_CHECK_EQUAL(Filenames::addPath(fn), "~/Documents/" + fn);
#else
  BOOST_CHECK_EQUAL(Filenames::addPath(fn), "~/" + fn);
#endif
}
