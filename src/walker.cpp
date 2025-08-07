/*
  walker.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 7-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dead
*/

#include "walker.hpp"

using namespace std;

// template <class Func>
// void Walker::walk(const rfl::Generic &g, const Func& f, int indent) {

//   tabs(indent);
//   std::visit([indent](const auto &field) {
//   
//     using Type = std::decay_t<decltype(field)>;
//     if constexpr (std::is_same<Type, vector<rfl::Generic >>() || std::is_same<Type, rfl::Object<rfl::Generic> >()) {
//       cout << endl;
//       walk(field, indent+1);
//     }
//     else if constexpr (std::is_same<Type, string>() || std::is_same<Type, bool>()) {
//       cout << field;
//     }
//     else {
//       cout << "unknown type " << typeid(field).name() << endl;
//     }
//   }, g.variant());
// 
// }

// void Walker::walk(const rfl::Object<rfl::Generic> &obj, int indent) {
// 
//   for (const auto& [k, v]: obj) {
//     
//     tabs(indent);
//     cout << k << ": ";
//     using V = remove_cvref_t<decltype(v)>;
//     if (is_same<rfl::Generic, V>::value) {
//       walk(v, indent+1);
//       cout << endl;
//     }
//     else {
//       cout << "unknown type " << typeid(v).name();
//     }
//   }
//   
// }
// 
// void Walker::walk(const std::vector<rfl::Generic > &v, int indent) {
//   tabs(indent);
//   cout << "[";
//   for (auto i: v) {
//     walk(i, indent+1);
//   }
//   cout << endl;
//   tabs(indent);
//   cout << "]";
// } 

void Walker::tabs(int n) {
  for (int i=0; i<n; i++) {
    cout << "  ";
  }
}
