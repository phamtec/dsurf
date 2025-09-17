/*
  err.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 16-Sep-2025
    
  Methods for error handling.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_err
#define H_err

#include "element.hpp"

#include <typeinfo>
#include <string>

class Element;

class Err {

public:

  static void typeError(const std::type_info &type);
    // return a message that the object is not of the type given.
    
  static std::string demangle(const std::type_info &type);
    // demangle a type

};

// given an object, if it can't be dynamic cast to the type, then return
// an error object with the correct methods implemented.

template <typename T, typename ErrT>
class Errable {

public:

  static T *cast(Element *obj) {
    T *px = dynamic_cast<T *>(obj);
    if (!px) {
      if (!obj) {
        throw std::string("nullptr is not a " + Err::demangle(typeid(T)) + "!");
      }
      if (!T::_err) {
       T::_err = new ErrT();
      }
      px = T::_err;
    }
    return px;
  }

};

// given an object, if it can't be dynamic cast to the type, then throw
// an error message.

template <typename T>
class ThrowErrable {

public:

  static T *cast(Element *obj) {
    if (!obj) {
      throw std::string("nullptr is not a " + Err::demangle(typeid(T)) + "!");
    }
    T *o = dynamic_cast<T *>(obj);
    if (!o) {
      throw std::string(Err::demangle(typeid(*obj)) + " not a " + Err::demangle(typeid(T)) + "!");
    }
    return o;
  }

};

#endif // H_err
