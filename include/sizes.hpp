/*
  sizes.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Sizes class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_sizes
#define H_sizes

class Sizes {

public:

  static float group_indent;
    // the indent for each grouping
    
  static float text_padding;
    // the amount to pad between text elements
    
  static float name_var_padding;
    // the amount to pad between names and variables
    
  static float thickness;
    // line thickness.
    
  static float toplinelength;
    // length of the topline on dicts and lists
    
  static float bottomlinelength;
    // length of the bottonm lineon dicts and lists.
    
  static float leftlinelength;
    // length of the line 
    
  static float listgap;
    // gap between each item in a list.

};

#endif // H_sizes
