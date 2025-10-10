/*
  zmq.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "renderer.hpp"

#include <rfl/json.hpp>
#include <rfl.hpp>

using namespace std;

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
      auto result = rfl::json::read<ReplyMsg>(m);
      if (!result) {
        cerr << "unknown msg " << m << endl;
        return;
      }
      if (_waitingonline) {
        if (result->type) {
          if (*(result->type) == "ack" || *(result->type) == "upstream") {
            _waitingonline = false;
            _online = true;
            _lastWatchdog = std::chrono::system_clock::now();
            cout << "online" << endl;
          }
          else {
            cerr << "unknown type " << *(result->type) << endl;
          }
        }
        else if (result->msg) {
          cerr << *(result->msg) << endl;
        }
        else {
          cerr << "unknown reply " << m << endl;
        }
      }
      else {
        if (result->type) {
          if (*(result->type) == "ack") {
            // all good.
          }
          else {
            cerr << "unknown type " << *(result->type) << endl;
          }
        }
        else if (result->msg) {
          cerr << *(result->msg) << endl;
        }
        else {
          cerr << "unknown reply " << m << endl;
        }
      }
    }
    
    if (_online) {
      auto now = std::chrono::system_clock::now();
      if (std::chrono::duration_cast<std::chrono::seconds>(now - _lastWatchdog).count() >= 5) {
        _lastWatchdog = now;
        HeartbeatMsg msg;
        msg.type = "heartbeat";
        msg.src = _src;
        heartbeatSend(msg);
      }
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

void Renderer::setupRemote(const string &server, int req, 
  const string &remotePubKey, const string &privateKey, const string &pubKey) {

  if (!_context) {
    cout << "new ZMQ context" << endl;
    _context.reset(new zmq::context_t(1));
  }
  _remotereq.reset(new zmq::socket_t(*_context, ZMQ_REQ));
  
  cout << "remotePubKey " << remotePubKey << endl;
  cout << "privateKey " << privateKey << endl;
  cout << "pubKey " << pubKey << endl;
  
  int curveServer = 0;
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

  string addr = "tcp://" + server + ":" + to_string(req);
  cout << "connecting to rmemote " << addr << endl;
  _remotereq->connect(addr);
	cout << "Bound to ZMQ on remote" << endl;

}

void Renderer::onlineSend(const OnlineMsg &msg) {

  _waitingonline = true;
  _src = msg.src;
  
  string r(rfl::json::write(msg)); 
  cout << "sending " << r << endl;
  zmq::message_t req(r.length());
  memcpy(req.data(), r.c_str(), r.length());
#if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)
  _remotereq->send(req);
#else
  _remotereq->send(req, zmq::send_flags::none);
#endif

}

void Renderer::heartbeatSend(const HeartbeatMsg &msg) {

  string r(rfl::json::write(msg)); 
  cout << "sending " << r << endl;
  zmq::message_t req(r.length());
  memcpy(req.data(), r.c_str(), r.length());
#if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)
  _remotereq->send(req);
#else
  _remotereq->send(req, zmq::send_flags::none);
#endif

}


