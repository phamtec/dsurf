/*
  genkeys.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 10-Oct-2025
    
  Generate a a new UUID and a set of CURVE private and public keys.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes
*/

#include <iostream>
#include <boost/program_options.hpp> 
#include <zmq.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp> 

namespace po = boost::program_options;

using namespace std;

int main(int argc, char *argv[]) {

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
  
  char public_key [41];
  char secret_key [41];
  auto result = zmq_curve_keypair(public_key, secret_key);
  if (result != 0) {
    cerr << "failed." << endl;
    return 1;
  }

  boost::uuids::uuid uuid = boost::uuids::random_generator()();
  stringstream ss;
  ss << uuid;
  
  cout << "uuid '" << ss.str() << "'" << endl;
  cout << "private '" << secret_key << "'" << endl;
  cout << "public '" << public_key << "'" << endl;
  
  return 0;
 
}
