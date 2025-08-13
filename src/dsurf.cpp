/*
  dsurf.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 25-Jul-2025
    
  Command line driver for dsurf.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "resources.hpp"
#include "font.hpp"
#include "renderer.hpp"
#include "builder.hpp"
#include "box.hpp"
#include "filledbox.hpp"

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
  string version = "dsurf v0.0.1, 25-Jul-2025.";
    
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
  
  // read in a JSON file and create a rendered almost as large as the device
  string infn = vm["input-file"].as< string >();
  auto result = rfl::json::load<rfl::Generic>(infn);
  unique_ptr<Box> root(Builder::walk(*result));
  Renderer renderer(Renderer::displaySize() - Size(20, 120), 0.01, 0.3, Size(0.0, 0.0));

//  alternate startup which just draws a blue box right in the middle of a small
//  screen
//   unique_ptr<Box> root(new FilledBox(Size(200, 200)));
//   Size wsize(500, 500);
//   Renderer renderer(wsize, 0.01, 1.5, Size(0.0, 0.0));

  if (!renderer.init()) {
    return 1;
  }
  
  Font font;
  if (!font.init("../fonts/Monaco.ttf")) {
    return 1;
  }
  
  Resources res;
  
  // allow all objects to build.
  res.build(renderer, font);
  root->build(renderer, font);

  // lay it all out.
  Size size = root->layout(res);
  
  // center it.  
  renderer.center(size);
  
  bool done = false;
  while (!done) {
  
    if (renderer.processEvents()) {
      done = true;
    }

    renderer.prepare();
    root->render(renderer, res, renderer.rootPoint());
    renderer.present();
  }
  
}
