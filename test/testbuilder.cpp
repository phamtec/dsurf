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
#include "text.hpp"
#include "corner.hpp"
#include "renderer.hpp"
#include "texteditor.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <fstream>

using namespace std;

// dummy these up so we don't recompile all code
Text::~Text() {
}
void Text::build(Renderer &renderer) {
}
void Text::render(Renderer &renderer, const Point &origin, bool offs) {
}
void Corner::render(Renderer &renderer, const Point &origin) {
}
void Renderer::renderFilledRect(const Rect &rect, const SDL_Color &color) {
}
void Renderer::copy(Element *element) {
}
void Renderer::paste() {
}
void Renderer::editText(Element *element, const Point &origin, const Size &size) {
}
void TextEditor::focus(const Point &origin, const Size &size, Editable *obj) {
}

void testParse(const string &fname) {

  // parse in the JSON to our objects.
  unique_ptr<Element> element(Builder::loadFile(fname));
  string json = Builder::getJson(element.get());
  
  // read in the original file.
  ifstream f(fname);
  BOOST_CHECK(f);
  string input(istreambuf_iterator<char>(f), {});
    
  // make sure they are identical.
  BOOST_CHECK_EQUAL(json, input);

}

BOOST_AUTO_TEST_CASE( parseEmptyDict )
{
  cout << "=== parseEmptyDict ===" << endl;

  testParse("../test/emptydict.json");
  
}

BOOST_AUTO_TEST_CASE( parseDict )
{
  cout << "=== parseDict ===" << endl;

  testParse("../test/dict.json");

}

BOOST_AUTO_TEST_CASE( parseEmptyList )
{
  cout << "=== parseEmptyList ===" << endl;

  testParse("../test/emptylist.json");
  
}

BOOST_AUTO_TEST_CASE( parseList )
{
  cout << "=== parseList ===" << endl;

  testParse("../test/list.json");
  
}

BOOST_AUTO_TEST_CASE( parseDictProp )
{
  cout << "=== parseDictProp ===" << endl;

  testParse("../test/dictprop.json");
  
}

BOOST_AUTO_TEST_CASE( parseListProp )
{
  cout << "=== parseListProp ===" << endl;

  testParse("../test/listprop.json");
  
}

BOOST_AUTO_TEST_CASE( parseEmptyDictProp )
{
  cout << "=== parseEmptyDictProp ===" << endl;

  testParse("../test/emptydictprop.json");
  
}

BOOST_AUTO_TEST_CASE( parseEmptyListProp )
{
  cout << "=== parseEmptyListProp ===" << endl;

  testParse("../test/emptylistprop.json");
  
}

BOOST_AUTO_TEST_CASE( parseListBool )
{
  cout << "=== parseListBool ===" << endl;

  testParse("../test/listbool.json");
  
}

BOOST_AUTO_TEST_CASE( parseListLong )
{
  cout << "=== parseListLong ===" << endl;

  testParse("../test/listlong.json");
  
}

BOOST_AUTO_TEST_CASE( parseComplex )
{
  cout << "=== parseComplex ===" << endl;

  testParse("../test/complex.json");
  
}

BOOST_AUTO_TEST_CASE( parseCode )
{
  cout << "=== parseCode ===" << endl;

  testParse("../test/code.json");
  
}

BOOST_AUTO_TEST_CASE( parseSimple )
{
  cout << "=== parseSimple ===" << endl;

  testParse("../test/simple.json");
  
}

BOOST_AUTO_TEST_CASE( parseSimpleProps )
{
  cout << "=== parseSimpleProps ===" << endl;

  testParse("../test/simpleprops.json");
  
}

BOOST_AUTO_TEST_CASE( genericReflection )
{
  cout << "=== genericReflection ===" << endl;

  // this is the basic mechanism we went with.  
  auto obj = rfl::Object<rfl::Generic>();
  obj["a"] = "x";
  obj["b"] = "y";
  obj["c"] = 10;

  BOOST_CHECK_EQUAL(rfl::json::write(obj), "{\"a\":\"x\",\"b\":\"y\",\"c\":10}");

  obj["c"] = true;

  BOOST_CHECK_EQUAL(rfl::json::write(obj), "{\"a\":\"x\",\"b\":\"y\",\"c\":true}");
  
  auto other = rfl::Object<rfl::Generic>();
  other["name"] = "joe";
  other["nums"] = vector<rfl::Generic>{1, 2, 3};
  obj["c"] = other;
  
  BOOST_CHECK_EQUAL(rfl::json::write(obj), 
    "{\"a\":\"x\",\"b\":\"y\",\"c\":{\"name\":\"joe\",\"nums\":[1,2,3]}}");

}

BOOST_AUTO_TEST_CASE( structReflection )
{
  cout << "=== structReflection ===" << endl;

  // this is how cppreflect implements reflection.

  struct Other {
    string name;
    vector<int> nums;
  };
  struct Obj {
    string a;
    string b;
    variant<int, bool, string, Other> c;
  };
  
  // we would really like this.
  BOOST_CHECK_EQUAL(rfl::json::write(Obj{ .a = "x", .b = "y", .c = 10 }), "{\"a\":\"x\",\"b\":\"y\",\"c\":10}");
  BOOST_CHECK_EQUAL(rfl::json::write(Obj{ .a = "x", .b = "y", .c = true }), "{\"a\":\"x\",\"b\":\"y\",\"c\":true}");
  BOOST_CHECK_EQUAL(rfl::json::write(Obj{ .a = "x", .b = "y", .c = "z" }), "{\"a\":\"x\",\"b\":\"y\",\"c\":\"z\"}");
  BOOST_CHECK_EQUAL(rfl::json::write(Obj{ .a = "x", .b = "y", .c = Other{.name = "joe", .nums = {1, 2, 3}} }), 
    "{\"a\":\"x\",\"b\":\"y\",\"c\":{\"name\":\"joe\",\"nums\":[1,2,3]}}");

  // but we have something more like this.  
  struct StringProp {
    string name;
    string value;
  };  
  struct Dict {
    vector<StringProp> fields;
  };
  
  auto dict = Dict{};
  dict.fields.push_back(StringProp{.name = "a", .value = "b"});
  dict.fields.push_back(StringProp{.name = "x", .value = "y"});

  // not quite what we want. 
  BOOST_CHECK_EQUAL(rfl::json::write(dict), "{\"fields\":[{\"name\":\"a\",\"value\":\"b\"},{\"name\":\"x\",\"value\":\"y\"}]}");
 
}

BOOST_AUTO_TEST_CASE( classReflection )
{
  cout << "=== classReflection ===" << endl;

  // this is how cppreflect implements reflection on classes.
  
  struct OtherFields {
    string name;
    vector<int> nums;
  };
  class Other {
    public:
      Other(const string &name, const vector<int> &nums) {
        _impl.name = name;
        _impl.nums = nums;
      }
      
      using ReflectionType = OtherFields;
      Other(const OtherFields& fields): _impl(fields) {}
      ~Other() = default;
      const ReflectionType& reflection() const { return _impl; }
      
    private:
      OtherFields _impl;
  };
  struct ObjFields {
    string a;
    string b;
    variant<int, bool, string, Other> c;
  };
  class Obj {
    public:
      Obj(const string &a, const string &b, int c) {
        _impl.a = a;
        _impl.b = b;
        _impl.c = c;
      }
      Obj(const string &a, const string &b, bool c) {
        _impl.a = a;
        _impl.b = b;
        _impl.c = c;
      }
      Obj(const string &a, const string &b, const char *c) {
        _impl.a = a;
        _impl.b = b;
        _impl.c = c;
      }
      Obj(const string &a, const string &b, const Other &c) {
        _impl.a = a;
        _impl.b = b;
        _impl.c = c;
      }
      
      using ReflectionType = ObjFields;
      Obj(const ObjFields& fields): _impl(fields) {}
      ~Obj() = default;
      const ReflectionType& reflection() const { return _impl; }
      
    private:
      ObjFields _impl;
  };
  
  // we would really like this.
  BOOST_CHECK_EQUAL(rfl::json::write(Obj("x", "y", 10)), "{\"a\":\"x\",\"b\":\"y\",\"c\":10}");
  BOOST_CHECK_EQUAL(rfl::json::write(Obj("x", "y", true)), "{\"a\":\"x\",\"b\":\"y\",\"c\":true}");
  BOOST_CHECK_EQUAL(rfl::json::write(Obj("x", "y", "z")), "{\"a\":\"x\",\"b\":\"y\",\"c\":\"z\"}");
  BOOST_CHECK_EQUAL(rfl::json::write(Obj("x", "y", Other("joe", {1, 2, 3}))), 
    "{\"a\":\"x\",\"b\":\"y\",\"c\":{\"name\":\"joe\",\"nums\":[1,2,3]}}");
  
  // we have more like this.
  struct StringPropFields {
    string name;
    string value;
  };
  class StringProp {
    public:
      StringProp(const string &n, const string &v) {
        _impl.name = n;
        _impl.value = v; 
      }
      
      using ReflectionType = StringPropFields;
      StringProp(const StringPropFields& fields): _impl(fields) {}
      ~StringProp() = default;
      const ReflectionType& reflection() const { return _impl; }

    private:
      StringPropFields _impl;
  };
  
  struct DictFields {
    vector<StringProp> fields;
  };
  class Dict {
    public:
      Dict() {}
      
      using ReflectionType = DictFields;
      Dict(const DictFields& fields): _impl(fields) {}
      ~Dict() = default;
      const ReflectionType& reflection() const { return _impl; }
      
      void push_back(const StringProp &prop) {
        _impl.fields.push_back(prop);
      }
      
    private:
      DictFields _impl;
  };
  auto dict = Dict();
  dict.push_back(StringProp("a", "b"));
  dict.push_back(StringProp("x", "y"));

  // not quite what we want.  
  BOOST_CHECK_EQUAL(rfl::json::write(dict), "{\"fields\":[{\"name\":\"a\",\"value\":\"b\"},{\"name\":\"x\",\"value\":\"y\"}]}");  

}

class TestObj {
};

template <>
struct rfl::Reflector<TestObj> {
  struct ReflType {
    std::string a;
    std::string b;
    int c;
  };

  static ReflType from(const TestObj& v) {
    return { "x", "y", 10 };
  }
};

BOOST_AUTO_TEST_CASE( customReflection )
{
  cout << "=== customReflection ===" << endl;

  BOOST_CHECK_EQUAL(rfl::json::write(TestObj()), "{\"a\":\"x\",\"b\":\"y\",\"c\":10}");

}
