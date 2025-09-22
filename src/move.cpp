/*
  move.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 17-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "move.hpp"

#include "indexable.hpp"

#include <iostream>

using namespace std;

void Move::moveObj(vector<Indexable *> &objs, int from, int to) {

//  cout << "move " << from << " -> " << to << endl;
  
  // sort them by index.
  sort(objs.begin(), objs.end(), [](Indexable *a, Indexable *b) {
    return a->getIndex() < b->getIndex();
  });

  if (abs(from - to) == 1) {  
    // swap adjacent objects.
    objs[from]->setIndex(to);
    objs[to]->setIndex(from);
  }
  else if (from < to) {
    // shuffle down
    for (int i=(from+1); i<=to; i++) {
      objs[i]->setIndex(i-1);
    }
    // move object
    objs[from]->setIndex(to);
  }
  else if (from > to) {
    // shuffle up
    for (int i=(to+1); i<=from; i++) {
      objs[i-1]->setIndex(i);
    }
    // move object
    objs[from]->setIndex(to);
  }
  else {
    cerr << "no case for " << from << " -> " << to << endl;
  }

}

void Move::shuffleDownFrom(std::vector<Indexable *> *objs, int from) {

  for_each(objs->begin(), objs->end(), [from](auto e) {
    int ix = e->getIndex();
    if (ix > from) {
      e->setIndex(ix-1);
    }
  });

}

void Move::shuffleUpFrom(std::vector<Indexable *> *objs, int from) {

  for_each(objs->begin(), objs->end(), [from](auto e) {
    int ix = e->getIndex();
    if (ix >= from) {
      e->setIndex(ix+1);
    }
  });

}
