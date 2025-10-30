/*
  remotezmq.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 29-Oct-2025
    
  Class for accesing a remote ZMQ socket..
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_remotezmq
#define H_remotezmq

#include "flo.hpp"

#include <string>
#include <optional>
#include <zmq.hpp>
#include <rfl.hpp>

class Renderer;

class RemoteZMQ {

public:
  RemoteZMQ() {};
  
  // remote server.
  void startRemote(Renderer &renderer, std::shared_ptr<Flo> &flo, const rfl::Object<rfl::Generic> &msg, std::optional<rfl::Object<rfl::Generic> > next);
  void evalMsg(Renderer &renderer, const rfl::Generic &msg);
  void msgError(Renderer &renderer, const std::string &err);
  
private:

  std::shared_ptr<Flo> _flo;
  std::optional<rfl::Object<rfl::Generic> > _next;
  
};

#endif // H_remotezmq
