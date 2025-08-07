/*
  dead.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 25-Jul-2025
    
  Command line driver for dead.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dead
*/

#include "text.hpp"
#include "font.hpp"
#include "renderer.hpp"
#include "node.hpp"
#include "walker.hpp"

#include <iostream>
#include <boost/program_options.hpp> 
#include <memory>
#include <rfl/json.hpp>
#include <rfl.hpp>

using namespace std;
namespace po = boost::program_options;

#define WIDTH   640
#define HEIGHT  480

int main(int argc, char *argv[])
{
  string version = "dead v0.0.1, 25-Jul-2025.";
    
  string usage = "Usage: " + string(argv[0]) + " input-file";
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("input-file", po::value<string>(), "input file")
    ;
  po::positional_options_description p;
  p.add("input-file", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
          options(desc).positional(p).run(), vm);
  po::notify(vm);   

  if (vm.count("help")) {
    cout << desc << endl;
    return 1;
  }
  
  if (!vm.count("input-file")) {
    cout << usage << endl;
    cout << desc << endl;
    return 1;
  }
  
  string infn = vm["input-file"].as< string >();
  auto result = rfl::json::load<rfl::Generic>(infn);
  
  class Builder {
    public:
      Node makeList() const {
        return Node::makeList();
      }
      Node makeObject() const {
        return Node::makeObject();
      }
  };
  Builder b;
  Node root;
  Walker::walk(*result, b, &root, 0);
  
  Renderer renderer(WIDTH, HEIGHT);
  if (!renderer.init()) {
    return 1;
  }
  
  Font font;
  if (!font.init("../fonts/Monaco.ttf")) {
    return 1;
  }
  
//   vector<unique_ptr<Node> > objs;
//   transform(obj->begin(), obj->end(), back_inserter(objs), 
//     [&renderer, &font](auto o) {
//       unique_ptr<Node> t;
//       auto obj = o.second.to_object();
//       if (obj) {
//         // handle objects
//       }
//       else {
//         auto arr = o.second.to_array();
//         if (arr) {
//           // handle arrays          
//           t.reset(new Node());
//           for (auto a: *arr) {
//             unique_ptr<Node> t2;
//             get<List>(t->_obj)._objs.push_back(t2);
//           }
//         }
//         else {
//           // an intrinsic value.
//           t.reset(new Node());
//           if (!get<Text>(t->_obj).init(renderer, font, o.first)) {
//             t.reset(0);
//           }
//         }
//       }
//       return t; 
//     }
//   );
  
  Text text("Hello World");
  
  bool done = false;
  while (!done) {
  
    if (renderer.processEvents()) {
      done = true;
    }

    renderer.prepare();
    
    float y = 0;
    text.render(renderer, font, 0, &y);
//     for (auto&& t: objs) {
//       if (t) {
//         get<Text>(t->_obj).render(renderer, 0, &y);
//       }
//     }

    renderer.present();
  }
  
}
