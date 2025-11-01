/*
  testmove.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 16-Sep-2025
    
  Moving tests.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "element.hpp"
#include "move.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <fstream>

using namespace std;

vector<int> gDestroyed;

class Obj: public Element {

public:
  Obj(int value, int index): /*_index(index),*/ _value(value) {}
  ~Obj() { gDestroyed.push_back(_value); }
  
  virtual string describe() { 
    stringstream ss;
    ss << _value;
    return ss.str();
  }
  virtual void layout() {}
  virtual void build(Core &core) {}
  virtual void render(Core &core, const Point &origin) {}
  virtual Size size() { return Size(); }
  
  int _value;
  
};

void moveObj(vector<unique_ptr<Element> > *testobjs, int from, int to) {

  Move::moveObj(testobjs, (testobjs->begin() + from)->get(), (testobjs->begin() + to)->get());
    
}

void buildObjs(vector<unique_ptr<Element> > *testobjs, int count) {

  for (int i=0; i<count; i++) {
    testobjs->push_back(unique_ptr<Element>(new Obj(i, i)));
  }

}

void testVector(vector<unique_ptr<Element> > *testobjs, const vector<int> &testvals) {

  // collect the values in order.
  vector<int> values;
  transform(testobjs->begin(), testobjs->end(), back_inserter(values), [](auto& e) {
    auto o = e.get();
    if (!o) {
      cout << "empty obj!" << endl;
      return -1;
    }
    return dynamic_cast<Obj *>(o)->_value; 
  });

  // print them out.
  for_each(values.begin(), values.end(), [](auto &e) { cout << e << ", "; });
  cout << endl;

  BOOST_CHECK_EQUAL_COLLECTIONS(values.begin(), values.end(), testvals.begin(), testvals.end());

}

BOOST_AUTO_TEST_CASE( swapFirst2 )
{
  cout << "=== swapFirst2 ===" << endl;
   
  gDestroyed.clear();
  vector<unique_ptr<Element> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 0, 1);
  BOOST_CHECK_EQUAL(gDestroyed.size(), 0);
  testVector(&objs, {1, 0, 2, 3, 4, 5});
  
}

BOOST_AUTO_TEST_CASE( swapFirst2Backwards )
{
  cout << "=== swapFirst2Backwards ===" << endl;
   
  gDestroyed.clear();
  vector<unique_ptr<Element> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 1, 0);
  BOOST_CHECK_EQUAL(gDestroyed.size(), 0);
  testVector(&objs, {1, 0, 2, 3, 4, 5});
  
}

BOOST_AUTO_TEST_CASE( swapEnd2 )
{
  cout << "=== swapEnd2 ===" << endl;
   
  gDestroyed.clear();
  vector<unique_ptr<Element> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 4, 5);
  BOOST_CHECK_EQUAL(gDestroyed.size(), 0);
  testVector(&objs, {0, 1, 2, 3, 5, 4});
  
}

BOOST_AUTO_TEST_CASE( swapEnd2Backwards )
{
  cout << "=== swapEnd2Backwards ===" << endl;
   
  gDestroyed.clear();
  vector<unique_ptr<Element> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 5, 4);
  BOOST_CHECK_EQUAL(gDestroyed.size(), 0);
  testVector(&objs, {0, 1, 2, 3, 5, 4});
  
}

BOOST_AUTO_TEST_CASE( swapMiddle )
{
  cout << "=== swapMiddle ===" << endl;
   
  gDestroyed.clear();
  vector<unique_ptr<Element> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 2, 3);
  BOOST_CHECK_EQUAL(gDestroyed.size(), 0);
  testVector(&objs, {0, 1, 3, 2, 4, 5});
  
}

BOOST_AUTO_TEST_CASE( swapMiddleBackwards )
{
  cout << "=== swapMiddleBackwards ===" << endl;
   
  gDestroyed.clear();
  vector<unique_ptr<Element> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 3, 2);
  BOOST_CHECK_EQUAL(gDestroyed.size(), 0);
  testVector(&objs, {0, 1, 3, 2, 4, 5});
  
}

BOOST_AUTO_TEST_CASE( moveStartToEnd )
{
  cout << "=== moveStartToEnd ===" << endl;
   
  gDestroyed.clear();
  vector<unique_ptr<Element> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 0, objs.size()-1);
  BOOST_CHECK_EQUAL(gDestroyed.size(), 0);
  testVector(&objs, {1, 2, 3, 4, 5, 0});
  
}

BOOST_AUTO_TEST_CASE( moveEndToStart )
{
  cout << "=== moveEndToStart ===" << endl;
   
  gDestroyed.clear();
  vector<unique_ptr<Element> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, objs.size()-1, 0);
  BOOST_CHECK_EQUAL(gDestroyed.size(), 0);
  testVector(&objs, {5, 0, 1, 2, 3, 4});
  
}

BOOST_AUTO_TEST_CASE( moveNotAdjacent )
{
  cout << "=== moveNotAdjacent ===" << endl;
   
  gDestroyed.clear();
  vector<unique_ptr<Element> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 2, 4);
  BOOST_CHECK_EQUAL(gDestroyed.size(), 0);
  testVector(&objs, {0, 1, 3, 4, 2, 5});
  
}

BOOST_AUTO_TEST_CASE( moveNotAdjacentBackwards )
{
  cout << "=== moveNotAdjacentBackwards ===" << endl;
   
  gDestroyed.clear();
  vector<unique_ptr<Element> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 4, 2);
  BOOST_CHECK_EQUAL(gDestroyed.size(), 0);
  testVector(&objs, {0, 1, 4, 2, 3, 5});
  
}

BOOST_AUTO_TEST_CASE( moveCase1 )
{
  cout << "=== moveCase1 ===" << endl;
   
  gDestroyed.clear();
  vector<unique_ptr<Element> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 0, 2);
  BOOST_CHECK_EQUAL(gDestroyed.size(), 0);
  testVector(&objs, {1, 2, 0, 3, 4, 5});
  
}


