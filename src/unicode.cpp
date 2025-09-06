/*
  unicode.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "unicode.hpp"

#include <locale>
#include <codecvt>

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

using namespace std;

std::wstring Unicode::convert(const std::string &s) {

  wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
  return converter.from_bytes(s);

}

std::string Unicode::convert(const std::wstring &s) {

  wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
  return converter.to_bytes(s);

}
