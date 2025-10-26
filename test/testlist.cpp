/*
  testlist.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 19-Sep-2025
    
  Listable tests.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "list.hpp"

#include "listable.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <fstream>

using namespace std;

class TestList: public List {

public:
  TestList(const string &name): List(false), _name(name) {}
  
  // Element
  virtual string describe() { return _name; }
  virtual void layout() {}
  virtual void build(Renderer &renderer) {}
  virtual void render(Renderer &renderer, const Point &origin) {}
  virtual Size size() { return Size(); }

  void push(Element *element) { _elements.push_back(unique_ptr<Element>(element)); }
  
  string _name;
  
};

BOOST_AUTO_TEST_CASE( getByPathSimple )
{
  cout << "=== getByPathSimple ===" << endl;
   
  TestList root("root");
  root.push(new TestList("obj1"));
  root.push(new TestList("obj2"));
  root.push(new TestList("obj3"));

  auto elem = Listable::getByPath(&root, "/2");
  BOOST_CHECK(elem != nullptr);
  BOOST_CHECK_EQUAL(elem->describe(), "obj3");
  
}

BOOST_AUTO_TEST_CASE( getByPath )
{
  cout << "=== getByPath ===" << endl;
   
  TestList root("root");
  auto obj2 = new TestList("obj2");
  root.push(new TestList("obj1"));
  root.push(obj2);
  root.push(new TestList("obj3"));
  obj2->push(new TestList("obj21"));
  obj2->push(new TestList("obj22"));
  obj2->push(new TestList("obj23"));
  
  auto elem = Listable::getByPath(&root, "/1/2");
  BOOST_CHECK(elem != nullptr);
  BOOST_CHECK_EQUAL(elem->describe(), "obj23");
  
}

BOOST_AUTO_TEST_CASE( getByPathDeeper )
{
  cout << "=== getByPathDeeper ===" << endl;
   
  TestList root("root");
  auto obj2 = new TestList("obj2");
  auto obj23 = new TestList("obj23");
  root.push(new TestList("obj1"));
  root.push(obj2);
  root.push(new TestList("obj3"));
  obj2->push(new TestList("obj21"));
  obj2->push(new TestList("obj22"));
  obj2->push(obj23);
  obj23->push(new TestList("obj231"));
  obj23->push(new TestList("obj232"));
  
  auto elem = Listable::getByPath(&root, "/1/2/1");
  BOOST_CHECK(elem != nullptr);
  BOOST_CHECK_EQUAL(elem->describe(), "obj232");
  
}


