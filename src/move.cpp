/*
  move.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 17-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "move.hpp"

#include "element.hpp"

#include <iostream>

using namespace std;

int Move::getIndex(const vector<unique_ptr<Element> > &objs, Element *elem) {

  for (int i=0; i<objs.size(); i++) {
    if (objs[i].get() == elem) {
      return i;
    }
  }
  return -1;
}

void Move::moveObj(vector<unique_ptr<Element> > *objs, Element *from, Element *to) {

  cout << "move " << from->describe() << " -> " << to->describe() << endl;

  int fromi = getIndex(*objs, from);
  int toi = getIndex(*objs, to);
  cout << "indexes " << fromi << " -> " << toi << endl;
  
  vector<unique_ptr<Element> > newlist;
  if (abs(fromi - toi) == 1) {
    int low, high;
    if (fromi < toi) {
      low = fromi;
      high = toi;
    }
    else {
      low = toi;
      high = fromi;
    }
    for (int i=0; i<low; i++) {
      newlist.push_back(std::move((*objs)[i]));
    }
    newlist.push_back(std::move((*objs)[high]));
    newlist.push_back(std::move((*objs)[low]));
    for (int i=high+1; i<objs->size(); i++) {
      newlist.push_back(std::move((*objs)[i]));
    }
  }
  else if (fromi < toi) {
    for (int i=0; i<fromi; i++) {
      newlist.push_back(std::move((*objs)[i]));
    }
    for (int i=fromi+1; i<=toi; i++) {
      newlist.push_back(std::move((*objs)[i]));
    }
    newlist.push_back(std::move((*objs)[fromi]));
    for (int i=toi+1; i<=objs->size()-1; i++) {
      newlist.push_back(std::move((*objs)[i]));
    }
  }
  else if (fromi > toi) {
    for (int i=0; i<toi; i++) {
      newlist.push_back(std::move((*objs)[i]));
    }
    newlist.push_back(std::move((*objs)[fromi]));
    for (int i=toi; i<fromi; i++) {
      newlist.push_back(std::move((*objs)[i]));
    }
    for (int i=fromi+1; i<=objs->size()-1; i++) {
      newlist.push_back(std::move((*objs)[i]));
    }
  }
  else {
    cerr << "no case for " << fromi << " -> " << toi << endl;
    return;
  }
  
  objs->clear();
  *objs = std::move(newlist);

}

