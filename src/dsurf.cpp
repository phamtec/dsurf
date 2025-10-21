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
#include "list.hpp"
#include "root.hpp"
#include "macutil.hpp"
#include <iostream>
#include <boost/program_options.hpp> 
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

using namespace std;
namespace po = boost::program_options;

int main(int argc, char *argv[])
{
  string version = "dsurf v0.10, 21-Oct-2025.";
    
  string logLevel;
  int repPort;

  string usage = "Usage: " + string(argv[0]) + " input-file";
  po::options_description desc("Allowed options");
  desc.add_options()
    ("repPort", po::value<int>(&repPort)->default_value(3013), "ZMQ Rep port.")
    ("help", "produce help message")
    ("input-files", po::value<vector<string> >(), "input files")
    ("logLevel", po::value<string>(&logLevel)->default_value("info"), "Logging level [trace, debug, warn, info].")
    ;
  po::positional_options_description p;
  p.add("input-files", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
          options(desc).positional(p).run(), vm);
          
  try {
    po::notify(vm);   
  }
  catch (boost::program_options::required_option &ex) {
    cout << ex.what() << endl;
    cout << desc << endl;
    return 1;
  }

  if (logLevel == "trace") {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::trace);
  }
  else if (logLevel == "debug") {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::debug);
  }
  else if (logLevel == "warn") {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::warning);
  }
  else {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
  }

  boost::log::formatter logFmt =
         boost::log::expressions::format("%1%\tdsurf\t[%2%]\t%3%")
        %  boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") 
        %  boost::log::expressions::attr< boost::log::trivial::severity_level>("Severity")
        %  boost::log::expressions::smessage;
  boost::log::add_common_attributes();
  boost::log::add_console_log(clog)->set_formatter(logFmt);

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
#ifdef MAC_APP
  if (!renderer.init(MacUtil::getFontPath())) {
#else
  if (!renderer.init("../fonts/Monaco.ttf")) {
#endif
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
    renderer.addRoot(new Root("<new>", new List(true)));
  }

//  alternate startup which just draws a blue box right in the middle of a small
//  screen
//  renderer.setRoot(new FilledBox(Size(200, 200)));

  // the render loop
  renderer.loop(repPort);
  
}
