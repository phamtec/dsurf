/*
  walker.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 7-Aug-2025
    
  Walker class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dead
*/

#ifndef H_walker
#define H_walker

#include <rfl/json.hpp>
#include <rfl.hpp>
#include <vector>

typedef enum {
  wtList = 0,
  wtObject = 1,
  wtString = 2,
  wtBool = 3
} walkType;

class Walker {

public:
  
  template <class Builder, class Obj>
  static void walk(const rfl::Generic &g, const Builder& builder, Obj *obj, int indent) {

    std::visit([builder, obj, indent](const auto &field) {
    
      using Type = std::decay_t<decltype(field)>;
      if constexpr (std::is_same<Type, std::vector<rfl::Generic >>()) {
        auto n = builder.makeList();
        walk(field, builder, &n, indent+1);
      }
      else if (std::is_same<Type, rfl::Object<rfl::Generic> >()) {
        auto n = builder.makeObject();
//        walk(field, builder, obj, n, indent+1);
  //      auto n = f(wtObject, field);
  //      walk(field, f, l, indent+1);
      }
      else if constexpr (std::is_same<Type, std::string>()) {
        obj->setString(field);
  //      auto n = f(wtString, field);
      }
      else if constexpr (std::is_same<Type, bool>()) {
        obj->setBool(field);
  //      auto n = f(wtBool, field);
      }
      else {
        std:: cout << "unknown type " << typeid(field).name() << std::endl;
      }
    }, g.variant());

  }

private:
  static void tabs(int n);
  
//   static void walk(const rfl::Object<rfl::Generic> &o, int indent);

  template <class Builder, class Obj>
  static void walk(const std::vector<rfl::Generic > &v, const Builder& builder, Obj *parent, int indent) {
  
// //   tabs(indent);
// //   cout << "[";
    for (auto i: v) {
      walk(i, builder, parent, indent+1);
    }
// //   cout << endl;
// //   tabs(indent);
// //   cout << "]";

 }
  
};

#endif // H_walker
