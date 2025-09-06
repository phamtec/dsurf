/*
  unicode.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Sep-2025
    
  Unicode utilities.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_unicode
#define H_unicode

#include <string>

class Unicode {

public:

  static std::wstring convert(const std::string &string);
  static std::string convert(const std::wstring &string);
  
};

#endif // H_unicode
