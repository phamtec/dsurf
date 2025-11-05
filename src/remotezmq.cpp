/*
  remotezmq.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 29-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "remotezmq.hpp"

#include "generic.hpp"
#include "core.hpp"

#include <rfl.hpp>
#include <rfl/json.hpp>

using namespace std;
using flo::Generic;

void RemoteZMQ::msgError(Core &core, const string &err) {
  cerr << "ZMQ error: "<< err << endl;
  core.closeRemote();
}

void RemoteZMQ::evalMsg(Core &core, const rfl::Generic &msg) {
  
  if (_next) {
    if (_next->size() > 0) {
      auto result = _flo->evalObj(msg, *_next);
      if (result) {
        core.addRoot("<zmq-result>", *result);
      }
    }
    else {
      core.addRoot("<zmq-result>", msg);
    }
//     // just open the message.
//     addRoot(new Root("<zmq-result>", Builder::walk(0, msg)));
    
    // and close the request.
    core.closeRemote();
    return;
  }
  
//  cout << "next " << Generic::toString(_next) << endl;
  
  auto result = _flo->evalObj(msg, *_next);
  if (!result) {
    msgError(core, "unknown reply " + Generic::toString(msg));
    return;
  }

  auto close = Generic::getBool(result, "close");
  if (close && *close) {
    cout << "closing" << endl;
    core.closeRemote();
    return;
  }
  
  auto ignore = Generic::getBool(result, "ignore");
  if (ignore && *ignore) {
    cout << "ignoring" << endl;
    return;
  }
  
  auto err = Generic::getString(result, "error");
  if (err) {
    msgError(core, *err);
    return;
  }
  
  auto send = Generic::getObject(result, "send");
  auto next = Generic::getObject(result, "next");
  if (!send || !next) {
    msgError(core, "missing send or next");
    return;
  }
  
  core.sendRemote(*send);
  
  _next = *next;

}

void RemoteZMQ::startRemote(Core &core, shared_ptr<Flo> &flo, const rfl::Object<rfl::Generic> &msg, optional<rfl::Object<rfl::Generic> > next) {
  
  _flo = flo;
  
  // no message has come in yet.
  rfl::Generic empty;
  auto cmsg = _flo->evalObj(empty, msg);
  if (!cmsg) {
    return;
  }
  core.sendRemote(*cmsg);

  // what to do next!
  _next = next;
  
}
