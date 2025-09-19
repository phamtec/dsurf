/*
  test.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 19-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "renderer.hpp"

#include "keyable.hpp"
#include "listable.hpp"
#include "hudable.hpp"

#include <rfl/json.hpp>
#include <rfl.hpp>

using namespace std;

void Renderer::setupTest(int rep) {

  _context.reset(new zmq::context_t(1));
  _rep.reset(new zmq::socket_t(*_context, ZMQ_REP));
  _rep->bind("tcp://127.0.0.1:" + to_string(rep));
	cout << "Bound to ZMQ as Local REP on " << rep << endl;

}

void Renderer::processTestMsg() {

  zmq::pollitem_t items [] = {
      { *_rep, 0, ZMQ_POLLIN, 0 }
  };
  const std::chrono::milliseconds timeout{20};

  zmq::message_t message;
  zmq::poll(&items[0], 1, timeout);
  if (items[0].revents & ZMQ_POLLIN) {
  
    // get the message.
    zmq::message_t req;
    auto res = _rep->recv(&req);
    string m((const char *)req.data(), req.size());
    
    auto result = rfl::json::read<TestMsg>(m);
    if (!result) {
      testErr("unknown msg " + m);
      return;
    }
    if (result->type == "key") {
    
      auto target = getTestTarget(result->target);
      if (!target) {
        return;
      }
      if (!result->payload) {
        testErr("missing pqyload");
        return;
      }
      _hud->setEditingLoc(localToGlobal(target->origin()));
      HUDable::cast(target)->setMode(*this, _hud.get());
      SDL_Keycode code = (*(result->payload))[0];
//      cout << "sending key " << code << endl;
      Keyable::cast(target)->processKey(*this, code);
      testAck();
      return;
    }
    if (result->type == "count") {
      auto target = getTestTarget(result->target);
      if (!target) {
        return;
      }
      stringstream ss;
      ss << Listable::cast(target)->count();
      TestMsg reply{ .type = "count", .payload = ss.str() };
      testSend(reply);
      return;
    }
    testErr("unknown type " + result->type);
  }

}

Element *Renderer::getTestTarget(const optional<string> &name) {

  if (!name) {
    testErr("missing target");
    return nullptr;
  }
  
  auto element = Listable::getByPath(_root.get(), *name);
  if (!element) {
    testErr(*name + " invalid");
    return nullptr;
  }
  return element;
  
}

void Renderer::testSend(const TestMsg &reply) {

  string r(rfl::json::write(reply)); 
  zmq::message_t rep(r.length());
  memcpy(rep.data(), r.c_str(), r.length());
  _rep->send(rep);

}

void Renderer::testAck() {

  testSend({ .type = "ack" });

}

void Renderer::testErr(const string &msg) {

  cerr << "Test error: "<< msg << endl;
  testSend({ .type = "err", .payload = msg });

}
