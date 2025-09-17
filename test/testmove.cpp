/*
  testmove.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 16-Sep-2025
    
  Moving tests.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "indexable.hpp"
#include "move.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <fstream>

using namespace std;

class Obj: public Indexable {

public:
  Obj(int value, int index): _index(index), _value(value) {}
  
  virtual void setIndex(int index) { _index = index; }
  virtual int getIndex() { return _index; }
  
  int _index;
  int _value;
  
};

void moveObj(vector<unique_ptr<Obj> > *testobjs, int from, int to) {

  // get all the indexable objects.
  vector<Indexable *> objs;
  transform(testobjs->begin(), testobjs->end(), back_inserter(objs), [](auto& e) { return e.get(); });
  
  Move::moveObj(objs, from, to);
    
}

void buildObjs(vector<unique_ptr<Obj> > *testobjs, int count) {

  for (int i=0; i<count; i++) {
    testobjs->push_back(unique_ptr<Obj>(new Obj(i, i)));
  }

}

void testVector(vector<unique_ptr<Obj> > *testobjs, const vector<int> &test) {

  // sort by index
  sort(testobjs->begin(), testobjs->end(), [](auto& a, auto& b) {
    return a.get()->_index < b.get()->_index;
  });

  // collect the values in order.
  vector<int> values;
  transform(testobjs->begin(), testobjs->end(), back_inserter(values), [](auto& e) { return e->_value; });

  // print them out.
  for_each(values.begin(), values.end(), [](auto &e) { cout << e << " "; });
  cout << endl;

  BOOST_CHECK_EQUAL_COLLECTIONS(values.begin(), values.end(), test.begin(), test.end());

}

BOOST_AUTO_TEST_CASE( swapFirst2 )
{
  cout << "=== swapFirst2 ===" << endl;
   
  vector<unique_ptr<Obj> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 0, 1);
  testVector(&objs, {1, 0, 2, 3, 4, 5});
  
}

BOOST_AUTO_TEST_CASE( swapFirst2Backwards )
{
  cout << "=== swapFirst2Backwards ===" << endl;
   
  vector<unique_ptr<Obj> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 1, 0);
  testVector(&objs, {1, 0, 2, 3, 4, 5});
  
}

BOOST_AUTO_TEST_CASE( swapEnd2 )
{
  cout << "=== swapEnd2 ===" << endl;
   
  vector<unique_ptr<Obj> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 4, 5);
  testVector(&objs, {0, 1, 2, 3, 5, 4});
  
}

BOOST_AUTO_TEST_CASE( swapEnd2Backwards )
{
  cout << "=== swapEnd2Backwards ===" << endl;
   
  vector<unique_ptr<Obj> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 5, 4);
  testVector(&objs, {0, 1, 2, 3, 5, 4});
  
}

BOOST_AUTO_TEST_CASE( swapMiddle )
{
  cout << "=== swapMiddle ===" << endl;
   
  vector<unique_ptr<Obj> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 2, 3);
  testVector(&objs, {0, 1, 3, 2, 4, 5});
  
}

BOOST_AUTO_TEST_CASE( swapMiddleBackwards )
{
  cout << "=== swapMiddleBackwards ===" << endl;
   
  vector<unique_ptr<Obj> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 3, 2);
  testVector(&objs, {0, 1, 3, 2, 4, 5});
  
}

BOOST_AUTO_TEST_CASE( moveStartToEnd )
{
  cout << "=== moveStartToEnd ===" << endl;
   
  vector<unique_ptr<Obj> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 0, objs.size()-1);
  testVector(&objs, {1, 2, 3, 4, 5, 0});
  
}

BOOST_AUTO_TEST_CASE( moveEndToStart )
{
  cout << "=== moveEndToStart ===" << endl;
   
  vector<unique_ptr<Obj> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, objs.size()-1, 0);
  testVector(&objs, {5, 0, 1, 2, 3, 4});
  
}

BOOST_AUTO_TEST_CASE( moveNotAdjacent )
{
  cout << "=== moveNotAdjacent ===" << endl;
   
  vector<unique_ptr<Obj> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 2, 4);
  testVector(&objs, {0, 1, 3, 4, 2, 5});
  
}

BOOST_AUTO_TEST_CASE( moveNotAdjacentBackwards )
{
  cout << "=== moveNotAdjacentBackwards ===" << endl;
   
  vector<unique_ptr<Obj> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 4, 2);
  testVector(&objs, {0, 1, 4, 2, 3, 5});
  
}

BOOST_AUTO_TEST_CASE( moveCase1 )
{
  cout << "=== moveCase1 ===" << endl;
   
  vector<unique_ptr<Obj> > objs;
  buildObjs(&objs, 6);
  moveObj(&objs, 0, 2);
  testVector(&objs, {1, 2, 0, 3, 4, 5});
  
}


