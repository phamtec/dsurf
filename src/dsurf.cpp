/*
  dsurf.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 25-Jul-2025
    
  Command line driver for dsurf.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "renderer.hpp"
#include "box.hpp"
#include "filledbox.hpp"

#include <iostream>
#include <boost/program_options.hpp> 

using namespace std;
namespace po = boost::program_options;

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
  
//   Size wsize(500, 500);
//   Renderer renderer(wsize, 0.01, 1.5, Size(0.0, 0.0));
  Renderer renderer(Renderer::displaySize() - Size(20, 120), 0.01, 0.3, Size(0.0, 0.0));

  // build all of the internal renderer state.
  if (!renderer.init("../fonts/Monaco.ttf")) {
    return 1;
  }
  
  // read in a JSON file if one is specified.
  if (vm.count("input-file") > 0) {
    renderer.loadFile(vm["input-file"].as< string >());
  }

//  alternate startup which just draws a blue box right in the middle of a small
//  screen
//  renderer.setRoot(new FilledBox(Size(200, 200)));

  // the render loop
  renderer.loop();
  
}
