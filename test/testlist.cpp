/*
  testlist.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 19-Sep-2025
    
  Listable tests.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "listable.hpp"
#include "element.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <fstream>

using namespace std;

class Obj: public Element, public Listable {

public:
  Obj(const string &name): _name(name) {}
  
  // Element
  virtual string describe() { return _name; }
  virtual Size layout() { return Size(); }
  virtual void build(Renderer &renderer) {}
  virtual void render(Renderer &renderer, const Point &origin) {}
  virtual Size size() { return Size(); }

  // Listable
  virtual void remove(Renderer &renderer, Element *element) {}
  virtual int count() { return _elements.size(); }
  virtual Element *at(int index) { return _elements[index]; }
  
  string _name;
  vector<Element *> _elements;
  
};

BOOST_AUTO_TEST_CASE( getByPathSimple )
{
  cout << "=== getByPathSimple ===" << endl;
   
  Obj root("root");
  Obj obj1("obj1");
  Obj obj2("obj2");
  Obj obj3("obj3");
  root._elements.push_back(&obj1);
  root._elements.push_back(&obj2);
  root._elements.push_back(&obj3);
  
  auto elem = Listable::getByPath(&root, "/2");
  BOOST_CHECK(elem != nullptr);
  BOOST_CHECK_EQUAL(elem->describe(), "obj3");
  
}

BOOST_AUTO_TEST_CASE( getByPath )
{
  cout << "=== getByPath ===" << endl;
   
  Obj root("root");
  Obj obj1("obj1");
  Obj obj2("obj2");
  Obj obj21("obj21");
  Obj obj22("obj22");
  Obj obj23("obj23");
  Obj obj3("obj3");
  root._elements.push_back(&obj1);
  root._elements.push_back(&obj2);
  root._elements.push_back(&obj3);
  obj2._elements.push_back(&obj21);
  obj2._elements.push_back(&obj22);
  obj2._elements.push_back(&obj23);
  
  auto elem = Listable::getByPath(&root, "/1/2");
  BOOST_CHECK(elem != nullptr);
  BOOST_CHECK_EQUAL(elem->describe(), "obj23");
  
}

BOOST_AUTO_TEST_CASE( getByPathDeeper )
{
  cout << "=== getByPathDeeper ===" << endl;
   
  Obj root("root");
  Obj obj1("obj1");
  Obj obj2("obj2");
  Obj obj21("obj21");
  Obj obj22("obj22");
  Obj obj23("obj23");
  Obj obj231("obj231");
  Obj obj232("obj232");
  Obj obj3("obj3");
  root._elements.push_back(&obj1);
  root._elements.push_back(&obj2);
  root._elements.push_back(&obj3);
  obj2._elements.push_back(&obj21);
  obj2._elements.push_back(&obj22);
  obj2._elements.push_back(&obj23);
  obj23._elements.push_back(&obj231);
  obj23._elements.push_back(&obj232);
  
  auto elem = Listable::getByPath(&root, "/1/2/1");
  BOOST_CHECK(elem != nullptr);
  BOOST_CHECK_EQUAL(elem->describe(), "obj232");
  
}


