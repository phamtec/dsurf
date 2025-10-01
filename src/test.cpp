/*
  test.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 19-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "renderer.hpp"

#include "listable.hpp"
#include "commandable.hpp"
#include "objable.hpp"

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
//    auto res = _rep->recv(&req);
    auto res = _rep->recv(req, zmq::recv_flags::none);
    string m((const char *)req.data(), req.size());
    
    auto result = rfl::json::read<TestMsg>(m);
    if (!result) {
      testErr("unknown msg " + m);
      return;
    }
    if (result->type == "key") {
    
      handleTestKey(*result);
      return;
      
    }
    if (result->type == "count") {
    
      handleTestCount(*result);
      return;
      
    }
    testErr("unknown type " + result->type);
  }

}

void Renderer::handleTestCount(const TestMsg &msg) {

  auto target = getTestTarget(msg.target);
  if (!target) {
    return;
  }
//  cout << "counting: " << target->describe() << endl;
  
  stringstream ss;
  ss << Listable::cast(target)->count();
  TestMsg reply{ .type = "count", .payload = ss.str() };
  testSend(reply);

}

void Renderer::handleTestKey(const TestMsg &msg) {

  auto target = getTestTarget(msg.target);
  if (!target) {
    return;
  }
  if (!msg.payload) {
    testErr("missing pqyload");
    return;
  }
  Commandable *cx = dynamic_cast<Commandable *>(target);
  if (!cx) {
    testErr("target not Commandable");
    return;
  }
  
  // set the mouse to be the center of the objects.
  Rect r(target->origin(), target->size());
  
  _mouse = Point((_offs.w + r.origin.x + (r.size.w / 2)) * _scale, (_offs.h + r.origin.y + (r.size.h / 2)) * _scale);
  
  SDL_Keycode code = (*msg.payload)[0];
//  cout << "sending key " << code << endl;
  cx->processKey(*this, code);
  
  // the key might have invalidated the object (like a paste)
  // so find the target again just in case.
  target = getTestTarget(msg.target, true);
  if (!target) {
    // target is gone, probably deleted.
    testAck();
    return;
  }
  
  // render the hud
  cx = dynamic_cast<Commandable *>(target);
  if (!cx) {
    testErr("target not Commandable anymore");
    return;
  }
  
  _hud->setEditingLoc(localToGlobal(r.origin));
  cx->setMode(*this, _hud.get());
  _hud->render(*this, _mouse);

  testAck();

}

Element *Renderer::getTestTarget(const optional<string> &name, bool silent) {

  if (!name) {
    if (!silent) {
      testErr("missing target");
    }
    return nullptr;
  }
  
  if (_roots.size() == 0) {
    if (!silent) {
      testErr("no roots");
    }
    return nullptr;
  }
  
  auto element = Listable::getByPath(get<1>(_roots[0]).get(), *name);
  if (!element) {
    if (!silent) {
      testErr(*name + " invalid");
    }
    return nullptr;
  }
  
  // the object we foujnd is just a container for another.
  auto obj = dynamic_cast<Objable *>(element);
  if (obj) {
//    cout << "found objable." << endl;
    return obj->getObj();
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
