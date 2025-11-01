/*
  test.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 19-Sep-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "core.hpp"

#include "commandable.hpp"
#include "objable.hpp"
#include "root.hpp"
#include "list.hpp"
#include "listable.hpp"

#include <rfl/json.hpp>
#include <rfl.hpp>

using namespace std;

void Core::setupTest(int rep) {

  if (!_context) {
    cout << "new ZMQ context" << endl;
    _context.reset(new zmq::context_t(1));
  }
  _context.reset(new zmq::context_t(1));
  _rep.reset(new zmq::socket_t(*_context, ZMQ_REP));
  _rep->bind("tcp://127.0.0.1:" + to_string(rep));
	cout << "Bound to ZMQ as Local REP on " << rep << endl;

}

void Core::handleTestMsg() {

  // get the message.
  zmq::message_t req;
#if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)
  auto res = _rep->recv(&req);
#else
  auto res = _rep->recv(req, zmq::recv_flags::none);
#endif
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

void Core::handleTestCount(const TestMsg &msg) {

  auto target = getTestTarget(msg.target);
  if (!target) {
    return;
  }
//  cout << "counting: " << target->describe() << endl;
  
  stringstream ss;
  ss << List::cast(target)->count();
  TestMsg reply{ .type = "count", .payload = ss.str() };
  testSend(reply);

}

void Core::handleTestKey(const TestMsg &msg) {

  auto target = getTestTarget(msg.target);
  if (!target) {
    return;
  }
  if (!msg.payload) {
    testErr("missing pqyload");
    return;
  }
  
//  cout << "sending to " << target->describe() << endl;

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

Element *Core::getTestTarget(const optional<string> &name, bool silent) {

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
  
  auto slash = name->find("/");
  if (slash == 0) {
    // use the first root and find the path.
    return getRootPath(_roots[0].get(), *name, silent);
  }
  else if (slash != string::npos) {
    // find the root, and then use the rest of the path to find where in the root.
    return getRootPath(findRoot(name->substr(0, slash)), name->substr(slash+1), silent);
  }

  // find a target by command verb.
  Element *target = 0;
  _roots[0]->visit([this, name, &target](auto e) {
    auto cx = dynamic_cast<Commandable *>(e);
    if (cx) {
      if (cx->getVerb() == *name) {
        target = e;
        return false;
      }
    }
    return true;
  });
  if (target) {
    return target;
  }
  
  if (!silent) {
    testErr("root obj is not a Root");
  }
  return nullptr;
  
}

Element *Core::getRootPath(Element *elem, const string &name, bool silent) {

  auto root = dynamic_cast<Root *>(elem);
  if (root) {
  
    auto element = Listable::getByPath(List::cast(root->getObj()), name);
    if (!element) {
      if (!silent) {
        testErr(name + " invalid");
      }
      return nullptr;
    }
    
    // the object we found is just a container for another.
    auto obj = dynamic_cast<Objable *>(element);
    if (obj) {
  //    cout << "found objable." << endl;
      return obj->getObj();
    }
    
    return element;
  
  }

  return nullptr;
  
}

Element *Core::findRoot(const string &name) {

  for (auto& i: _roots) {
    auto wx = dynamic_cast<Writeable *>(i.get());
    if (wx) {
      auto fn = wx->getFilename();
      if (fn) {
        if (name == *fn) {
          return i.get();
        }
      }
    }
  }
  return nullptr;
}

void Core::testSend(const TestMsg &reply) {

  string r(rfl::json::write(reply)); 
  zmq::message_t rep(r.length());
  memcpy(rep.data(), r.c_str(), r.length());
#if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)
  _rep->send(rep);
#else
  _rep->send(rep, zmq::send_flags::none);
#endif

}

void Core::testAck() {

  testSend({ .type = "ack" });

}

void Core::testErr(const string &msg) {

  cerr << "Test error: "<< msg << endl;
  testSend({ .type = "err", .payload = msg });

}
