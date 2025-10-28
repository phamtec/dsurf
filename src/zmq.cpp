/*
  zmq.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "renderer.hpp"

#include "builder.hpp"
#include "generic.hpp"
#include "flo.hpp"
#include "root.hpp"

#include <rfl/json.hpp>
#include <rfl.hpp>

using namespace std;
using flo::Generic;

void Renderer::processMsg() {

  const std::chrono::milliseconds timeout{20};

  if (_remotereq) {
    zmq::pollitem_t items [] = {
        { *_rep, 0, ZMQ_POLLIN, 0 },
        { *_remotereq, 0, ZMQ_POLLIN, 0 }
    };
  
    zmq::message_t message;
    zmq::poll(&items[0], 2, timeout);
    if (items[0].revents & ZMQ_POLLIN) {
      handleTestMsg();
    }
    if (items[1].revents & ZMQ_POLLIN) {
    
      // get the message.
      zmq::message_t req;
  #if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)
      auto res = _remotereq->recv(&req);
  #else
      auto res = _remotereq->recv(req, zmq::recv_flags::none);
  #endif
//      cout << "poll result " << res << endl;
      string m((const char *)req.data(), req.size());
      cout << "<- " << m << endl;
      auto result = rfl::json::read<rfl::Generic>(m);
      if (!result) {
        msgError("unknown result " + m);
        return;
      }
      evalMsg(*result);
      return;
    }
    return;
  }
  
  zmq::pollitem_t items [] = {
      { *_rep, 0, ZMQ_POLLIN, 0 }
  };

  zmq::message_t message;
  zmq::poll(&items[0], 1, timeout);
  if (items[0].revents & ZMQ_POLLIN) {
    handleTestMsg();
  }

}

void Renderer::msgError(const string &err) {
  cerr << "ZMQ error: "<< err << endl;
  _remotereq->close();
  _remotereq.reset();
}

void Renderer::evalMsg(const rfl::Generic &msg) {
  
//  cout << "next " << Generic::toString(_next) << endl;
  
  auto cmsg = _flo->evalObj(msg, _next);
  if (!cmsg) {
    msgError("unknown reply " + Generic::toString(msg));
    return;
  }

  auto open = Generic::getBool(cmsg, "open");
  if (open && *open) {
    cout << "opening" << endl;
    addRoot(new Root("<zme>", Builder::walk(0, msg)));
    _remotereq->close();
    _remotereq.reset();
    return;
  }
  
  auto close = Generic::getBool(cmsg, "close");
  if (close && *close) {
    cout << "closing" << endl;
    _remotereq->close();
    _remotereq.reset();
    return;
  }
  
  auto ignore = Generic::getBool(cmsg, "ignore");
  if (ignore && *ignore) {
    cout << "ignoring" << endl;
    return;
  }
  
  auto err = Generic::getString(cmsg, "error");
  if (err) {
    msgError(*err);
    return;
  }
  
  auto send = Generic::getObject(cmsg, "send");
  auto next = Generic::getObject(cmsg, "next");
  if (!send || !next) {
    msgError("missing send or next");
    return;
  }
  
  sendRemote(*send);
  
  _next = *next;

}

bool Renderer::setupRemote(const string &server, int req, 
  const string &remotePubKey, const string &privateKey, const string &pubKey) {

  if (!_context) {
    cout << "new ZMQ context" << endl;
    _context.reset(new zmq::context_t(1));
  }
  
  if (_remotereq) {
    cerr << "already online" << endl;
    return false;
  }
  
  _remotereq.reset(new zmq::socket_t(*_context, ZMQ_REQ));
  
  int linger = 0;
#if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)
  _remotereq->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
#else
  _remotereq->set(zmq::sockopt::linger, linger);
#endif
  
  if (remotePubKey.size() > 0) {
    cout << "remotePubKey " << remotePubKey << endl;
    cout << "privateKey " << privateKey << endl;
    cout << "pubKey " << pubKey << endl;
    
    int curveServer = 0;
    try {
#if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)
    _remotereq->setsockopt(ZMQ_CURVE_SERVER, &curveServer, sizeof(curveServer));
    _remotereq->setsockopt(ZMQ_CURVE_SERVERKEY, remotePubKey.c_str(), remotePubKey.size());
    _remotereq->setsockopt(ZMQ_CURVE_SECRETKEY, privateKey.c_str(), privateKey.size());
    _remotereq->setsockopt(ZMQ_CURVE_PUBLICKEY, pubKey.c_str(), pubKey.size());
#else
    _remotereq->set(zmq::sockopt::curve_server, curveServer);
    _remotereq->set(zmq::sockopt::curve_serverkey, remotePubKey);
    _remotereq->set(zmq::sockopt::curve_secretkey, privateKey);
    _remotereq->set(zmq::sockopt::curve_publickey, pubKey);
#endif
    }
    catch (zmq::error_t &ex) {
      // something failed!
      cout << "ZMQ can't pmmect: " << ex.what() << endl;
      _remotereq.reset();
      return false;
    }
  }

  string addr = "tcp://" + server + ":" + to_string(req);
  cout << "connecting to rmemote " << addr << endl;
  _remotereq->connect(addr);
	cout << "Bound to ZMQ on remote" << endl;

  return true;
  
}

void Renderer::startRemote(std::shared_ptr<Flo> &flo, const rfl::Object<rfl::Generic> &msg, const rfl::Object<rfl::Generic> &next) {
  
  _flo = flo;
  
  // no message has come in yet.
  rfl::Generic empty;
  auto cmsg = _flo->evalObj(empty, msg);
  if (!cmsg) {
    return;
  }
  sendRemote(*cmsg);

  // what to do next!
  _next = next;
  
}

void Renderer::sendRemote(const rfl::Object<rfl::Generic> &msg) {

  string r(rfl::json::write(msg)); 
  cout << "-> " << r << endl;
  zmq::message_t req(r.length());
  memcpy(req.data(), r.c_str(), r.length());
#if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)
  _remotereq->send(req);
#else
  _remotereq->send(req, zmq::send_flags::none);
#endif

}


