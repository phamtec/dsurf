/*
  testmem.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 23-Sep-2025
    
  Basic fundamental tests on how vectors of unique_ptr and std::move works.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <vector>
#include <memory>
#include <ranges>

using namespace std;

// when the objects are destroyed they fill this vector with the number
// assigned to them.

vector<int> gDestroyed;

class Obj {

public:
  Obj(int n): _n(n) {}
  ~Obj() { gDestroyed.push_back(_n); }
  
private:
  int _n;
};

void testDestroyed(const vector<int> &test) {

  // print out the destroyed array.
  copy(test.begin(), test.end() - 1, ostream_iterator<int>(cout, ", "));
  cout << test.back() << endl;

  BOOST_CHECK_EQUAL_COLLECTIONS(gDestroyed.begin(), gDestroyed.end(), test.begin(), test.end());

}

void addObjs(vector<unique_ptr<Obj> > *objs) {

  // most complicated for loop :-)
  ranges::copy(views::transform(views::iota(0, 6), [](auto i) {
    return unique_ptr<Obj>(new Obj(i));
  }), back_inserter(*objs));
                          
}

BOOST_AUTO_TEST_CASE( vectorUniqPtr )
{
  cout << "=== vectorUniqPtr ===" << endl;

  gDestroyed.clear();
  {
    vector<unique_ptr<Obj> > objs;
    addObjs(&objs);
    // and let them be destroyed.
  }
  
  testDestroyed({5, 4, 3, 2, 1, 0});
  
}

BOOST_AUTO_TEST_CASE( eraseVectorUniqPtr )
{
  cout << "=== eraseVectorUniqPtr ===" << endl;

  gDestroyed.clear();
  {
    vector<unique_ptr<Obj> > objs;
    addObjs(&objs);
    
    auto i = objs.begin();
    i += 3;
    objs.erase(i);

    // array element really gone.
    BOOST_CHECK_EQUAL(objs.size(), 5);
    
    // and let them be destroyed.
  }
  
  testDestroyed({3, 5, 4, 2, 1, 0});
  
}

BOOST_AUTO_TEST_CASE( moveVectorUniqPtr )
{
  cout << "=== moveVectorUniqPtr ===" << endl;

  gDestroyed.clear();
  {
    unique_ptr<Obj> x;
    {
      vector<unique_ptr<Obj> > objs;
      addObjs(&objs);
      
      // move 1 out.
      auto i = objs.begin();
      i++;
      x = std::move(*i);

      BOOST_CHECK_EQUAL(objs.size(), 6);
      BOOST_CHECK_EQUAL(objs[1], nullptr);
    }
    // and let them be destroyed.
  }
  
  testDestroyed({5, 4, 3, 2, 0, 1});
  
}

BOOST_AUTO_TEST_CASE( moveVectorUniqPtrToVector )
{
  cout << "=== moveVectorUniqPtrToVector ===" << endl;

  gDestroyed.clear();
  {
    vector<unique_ptr<Obj> > x;
    {
      vector<unique_ptr<Obj> > objs;
      addObjs(&objs);
      
      // move 1 out.
      auto i = objs.begin();
      i += 2;
      x.push_back(std::move(*i));
      
      // doesn't really remove the element BUT the object will be gone.
      BOOST_CHECK_EQUAL(objs.size(), 6);
      BOOST_CHECK_EQUAL(objs[2], nullptr);
      
      i += 2;
      x.push_back(std::move(*i));

      BOOST_CHECK_EQUAL(objs.size(), 6);
      BOOST_CHECK_EQUAL(objs[4], nullptr);
    }
    // and let them be destroyed.
  }
  
  testDestroyed({5, 3, 1, 0, 4, 2});
  
}




