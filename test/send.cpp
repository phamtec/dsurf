/*
  send.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 19-Sep-2024
    
  Command line driver for testing.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes
*/

#include <iostream>
#include <boost/program_options.hpp> 
#include <zmq.hpp>

namespace po = boost::program_options;

using namespace std;

int main(int argc, char *argv[]) {

  int reqPort;
  string str;
  
  po::options_description desc("Allowed options");
  desc.add_options()
    ("reqPort", po::value<int>(&reqPort)->default_value(3013), "ZMQ Req port.")
    ("str", po::value<string>(&str)->required(), "The string to send")
    ("help", "produce help message")
    ;
  po::positional_options_description p;
  p.add("str", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
          options(desc).positional(p).run(), vm);
  po::notify(vm);   

  if (vm.count("help")) {
    cout << desc << endl;
    return 1;
  }
  
  zmq::context_t context(1);
  zmq::socket_t req(context, ZMQ_REQ);
  req.connect("tcp://127.0.0.1:" + to_string(reqPort));
//  cout << "Connect to ZMQ as Local REQ on " << reqPort << endl;
  
  zmq::message_t msg(str.length());
  memcpy(msg.data(), str.c_str(), str.length());
#if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)
  req.send(msg);
#else
  req.send(msg, zmq::send_flags::none);
#endif

  zmq::message_t reply;
#if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)
  req.recv(&reply);
#else
  req.recv(reply, zmq::recv_flags::none);
#endif
  string r((const char *)reply.data(), reply.size());

  cout << r << endl;
    
  return 0;
 
}
