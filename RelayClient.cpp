#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "relay_server.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using py_server::Data;
using py_server::State;
using py_server::RelayServer;
using py_server::Response;

class RelayServerClient {
 public:
  RelayServerClient(std::shared_ptr<Channel> channel)
      : stub_(RelayServer::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  int get_state(const std::string& room_name, const std::string& appliance_type, const std::string& appliance_name) {
    // Data we are sending to the server.
    Data request;
    request.set_room_name(room_name);
    request.set_appliance_type(appliance_type);
    request.set_appliance_name(appliance_name);
    // Container for the data we expect from the server.
    State reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->get_state(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.value();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return -1;
    }
  }
  
  int set_state(const std::string& room_name, const std::string& appliance_type, const std::string& appliance_name, int state) {
    // Data we are sending to the server.
    Data request;
    request.set_room_name(room_name);
    request.set_appliance_type(appliance_type);
    request.set_appliance_name(appliance_name);
    request.set_state(state);
    // Container for the data we expect from the server.
    Response reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->set_state(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.value();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return -1;
    }
  }  
  
  
   private:
  std::unique_ptr<RelayServer::Stub> stub_; 
};

int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50051). We indicate that the channel isn't authenticated
  // (use of InsecureChannelCredentials()).
  RelayServerClient RelayServer(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
  std::string room("Hall");
  std::string app("light");
  std::string app_name("light1");
  int state = 0;
  int reply = RelayServer.get_state(room, app, app_name);
  std::cout << "get state received: " << reply << std::endl;
  reply = RelayServer.set_state(room, app, app_name, state);
  std::cout << "set state received: " << reply << std::endl;
  return 0;
}
