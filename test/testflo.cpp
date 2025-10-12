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

#include <rfl/json.hpp>

using namespace std;

Generic::Object<Generic::Generic> parseSONObject(const string &json) {

  auto g = Generic::json::read<Generic::Generic>(json);
  BOOST_CHECK(g);
  
  auto dict = Flo::getObject(*g);
  BOOST_CHECK(dict);

  return *dict;
  
}

string dumpJSONObject(const Generic::Generic &obj) {

  return Generic::json::write(obj); 

}

BOOST_AUTO_TEST_CASE( evalStringMember )
{
  cout << "=== evalStringMember ===" << endl;

  Flo flo;
  
  auto s = flo.evalStringMember(parseSONObject("{ \"test\": \"string\" }"), "test");
  BOOST_CHECK_EQUAL(*s, "string");

}

// BOOST_AUTO_TEST_CASE( nullTest )
// {
//   cout << "=== nullTest ===" << endl;
//   
//   auto json = "{\"message\":\"hello\"}";
//   auto hello = parseSONObject(json);
//   Flo flo;
// 
//   auto result = flo.transform(hello, parseSONObject("{ \"null\": {} }"));
//   
//   BOOST_CHECK_EQUAL(dumpJSONObject(*result), json);
//   
// }
// 
// BOOST_AUTO_TEST_CASE( noFuncTest )
// {
//   cout << "=== noFuncTest ===" << endl;
//   
//   auto none = parseSONObject("{}");
//   Flo flo;
// 
//   auto result = flo.transform(none, parseSONObject("{ \"xxxx\": {} }"));
//   BOOST_CHECK(result);
// 
//   BOOST_CHECK_EQUAL(dumpJSONObject(result), "{\"error\":\"function xxxx not found\"}");
//   
// }

/*
BOOST_AUTO_TEST_CASE( ifTrueTest )
{
  cout << "=== ifTrueTest ===" << endl;
  
  json hello = {
    { "message", "hello ignored" }
  };
  Functions f;
  Processor p(hello, f);

  json transform = {
    { "if", {
        { "p", {
            { "true", {} }
          }
        },
        { "then", {
            { "dict", {
              { "message", "world" }
              } 
            }
          }
        }
      } 
    }
  };
  
  auto result = p.transform(transform);
  BOOST_CHECK(result);
  BOOST_CHECK(result->is_object());
  BOOST_CHECK(result->as_object().if_contains("message"));
  BOOST_CHECK_EQUAL(boost::json::value_to<string>(result->at_pointer("/message")), "world");
  
}
*/