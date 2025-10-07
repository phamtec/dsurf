/*
  dsurf.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 25-Jul-2025
    
  Command line driver for dsurf.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "renderer.hpp"
#include "builder.hpp"
#include "element.hpp"
#include "filledbox.hpp"
#include "dict.hpp"
#include "root.hpp"

#include <iostream>
#include <boost/program_options.hpp> 

using namespace std;
namespace po = boost::program_options;

int main(int argc, char *argv[])
{
  string version = "dsurf v0.9, 7-Oct-2025.";
    
  int repPort;

  string usage = "Usage: " + string(argv[0]) + " input-file";
  po::options_description desc("Allowed options");
  desc.add_options()
    ("repPort", po::value<int>(&repPort)->default_value(3013), "ZMQ Rep port.")
    ("help", "produce help message")
    ("input-files", po::value<vector<string> >(), "input files")
    ;
  po::positional_options_description p;
  p.add("input-files", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
          options(desc).positional(p).run(), vm);
  po::notify(vm);   

  if (vm.count("help")) {
    cout << desc << endl;
    return 1;
  }
  
//  bool edit = true;
  bool edit = false;
  Size wsize = Renderer::displaySize() - Size(200, 300);
//  Size wsize(500, 500);
//  Size wsize(1024, 768);
  double scale = 0.3;
//   double scale = 1.5;
//   double scale = 0.5;

  Renderer renderer(wsize, 0.01, scale, Size(0.0, 0.0), edit);

  // build all of the internal renderer state.
  if (!renderer.init("../fonts/Monaco.ttf")) {
//  if (!renderer.init("../fonts/Geneva.ttf")) {
    return 1;
  }
  
  // read in a file if one is specified.
  if (vm.count("input-files")) {
    vector<string> files = vm["input-files"].as<vector<string> >();
    for_each(files.begin(), files.end(), [&renderer](auto f) { 
      renderer.addFile(f, false);
    });
  }
  else {
    // always just a new dictiionary.
    renderer.addRoot(new Root("<new>", new Dict()));
  }

//  alternate startup which just draws a blue box right in the middle of a small
//  screen
//  renderer.setRoot(new FilledBox(Size(200, 200)));

  // the render loop
  renderer.loop(repPort);
  
}
