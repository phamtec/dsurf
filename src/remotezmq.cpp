/*
  remotezmq.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 29-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "remotezmq.hpp"

#include "dict.hpp"
#include "core.hpp"

using namespace std;
using namespace vops;

void RemoteZMQ::msgError(Core &core, const string &err) {
  cerr << "ZMQ error: "<< err << endl;
  core.closeRemote();
}

void RemoteZMQ::evalMsg(Core &core, const DictG &msg) {
  
  if (!_next || _next->size() == 0) {
    core.addRoot("<zmq-result>", msg);
    core.closeRemote();
    return;
  }
  
//  cout << "next " << Dict::toString(_next) << endl;
  
  auto result = _flo->evalObj(msg, *_next);
  if (!result) {
    msgError(core, "unknown reply " + Dict::toString(msg));
    return;
  }

  auto close = Dict::getBool(result, "close");
  if (close && *close) {
    cout << "closing" << endl;
    core.closeRemote();
    return;
  }
  
  auto ignore = Dict::getBool(result, "ignore");
  if (ignore && *ignore) {
    cout << "ignoring" << endl;
    return;
  }
  
  auto err = Dict::getString(result, "error");
  if (err) {
    msgError(core, *err);
    return;
  }
  
  auto send = Dict::getObject(result, "send");
  if (send) {
    core.sendRemote(*send);
  }
  else {
    core.addRoot("<zmq-result>", *result);
    core.closeRemote();
    return;
  }
  
  auto next = Dict::getObject(result, "next");
  if (next) {
    _next = *next;
  }

}

void RemoteZMQ::startRemote(Core &core, shared_ptr<Flo> &flo, const DictO &msg, optional<DictO> next) {
  
  _flo = flo;
  
  // no message has come in yet.
  DictG empty;
  auto cmsg = _flo->evalObj(empty, msg);
  if (!cmsg) {
    return;
  }
  core.sendRemote(*cmsg);

  // what to do next!
  _next = next;
  
}
