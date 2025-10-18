/*
  layout.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 18-Oct-2025
    
  Utilities for laying things out.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_layout
#define H_layout

#include "rect.hpp"

#include <vector>

/*
  For convenience, we use a simple list of rectangles to represent the visual layout
  of an object.
  
  Each rectangle represents an object that might be drawn on the screem.
  
  The bottom-right of very last entry represents the total size of the object.
  
  If this ISN'T the same as the size (say you want to pad your object out, just put
  1x1 pixel square in there so that "size()" will still work ok
*/
typedef std::vector<Rect> RectList;

class Layout {

public:

  static void addSize(RectList *layout, const Size &size);
    // add the size to the layout.
    
  static Size size(const RectList &layout);
    // get the size of the layout.
    
  static void print(const RectList &layout);
    // print the layout to cout.
    
};

#endif // H_layout
