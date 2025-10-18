/*
  layout.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 18-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "layout.hpp"

#include <iostream>
#include <algorithm>

using namespace std;

void Layout::addSize(RectList *layout, const Size &size) {

  layout->push_back(Rect(size - Size(1, 1), Size(1, 1)));

}
Size Layout::size(const RectList &layout) {

  if (layout.size() == 0) {
    return Size();
  }
  
  return layout.back().origin + layout.back().size;
  
}

void Layout::print(const RectList &layout) {

  for (auto i=layout.begin(); i != layout.end(); i++) {
    cout << *i;
    if (i != layout.end()) {
      cout << "; ";
    }
  }
  cout << endl;

}

