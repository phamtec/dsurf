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

#include <iostream>
#include <boost/program_options.hpp> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;
namespace po = boost::program_options;

#define WIDTH   640
#define HEIGHT  480

int main(int argc, char *argv[])
{
  string version = "dead v0.0.1, 25-Jul-2025.";
    
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ;
  po::positional_options_description p;

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
          options(desc).positional(p).run(), vm);
  po::notify(vm);   

  if (vm.count("help")) {
    cout << desc << endl;
    return 1;
  }
  
  Renderer renderer(WIDTH, HEIGHT);
  if (!renderer.init()) {
    return 1;
  }
  
  Font font;
  if (!font.init("/System/Library/Fonts/Courier.ttc")) {
    return 1;
  }

  Text text(WIDTH, HEIGHT);
  if (!text.init(renderer, font, "Hello World")) {
    return 1;
  }
  
  bool done = false;
  while (!done) {
  
    if (renderer.processEvents()) {
      done = true;
    }

    renderer.prepare();
    text.render(renderer, 100, 100);  
    renderer.present();
  }
  
}
