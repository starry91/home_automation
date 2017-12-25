#include <sys/socket.h>
#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string>
#include <thread>

#include <grpc++/grpc++.h>
#include "AppToCppServer.grpc.pb.h"
#include <Room.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using app_cpp_server::rpcData;
using app_cpp_server::rpcHouse;
using app_cpp_server::rpcRoom;
using app_cpp_server::rpcAppliance;
using app_cpp_server::rpcResponse;

class Client {
private:
	std::unique_ptr<app_cpp_server::AppCppServer::Stub> stub_;
public:

	Client(std::shared_ptr<grpc::Channel> channel) :
			stub_(app_cpp_server::AppCppServer::NewStub(channel)) {
	}

	rpcHouse get_all(const std::string id) {
		// Data we are sending to the server.
		app_cpp_server::rpcId request;
		request.set_id(id);

		// Container for the data we expect from the server.
		app_cpp_server::rpcHouse reply;

		// Context for the client. It could be used to convey extra information to
		// the server and/or tweak certain RPC behaviors.
		grpc::ClientContext context;

		// The actual RPC.
		grpc::Status status = (*stub_).get_all(&context, request, &reply);
		// Act upon its status.
		return reply;
	}

	rpcResponse set_state(const std::string id, std::string room_name,
			std::string app_type, std::string app_name, int state) {
		// Data we are sending to the server.
		app_cpp_server::rpcData request;
		request.set_id(id);
		request.set_room_name(room_name);
		request.set_appliance_type(app_type);
		request.set_appliance_name(app_name);
		request.set_state(state);

		// Container for the data we expect from the server.
		app_cpp_server::rpcResponse reply;

		// Context for the client. It could be used to convey extra information to
		// the server and/or tweak certain RPC behaviors.
		grpc::ClientContext context;

		// The actual RPC.
		grpc::Status status = (*stub_).set_state(&context, request, &reply);
		// Act upon its status.
		return reply;
	}

	void notification(const std::string id) {
		// Data we are sending to the server.
		app_cpp_server::rpcId request;
		request.set_id(id);

		// Container for the data we expect from the server.
		app_cpp_server::rpcData reply;

		// Context for the client. It could be used to convey extra information to
		// the server and/or tweak certain RPC behaviors.
		grpc::ClientContext context;
		std::unique_ptr<ClientReader<rpcData> > reader(
				(*stub_).notification(&context, request));
		// The actual RPC.
		std::cout << "before while " << std::endl;
		while ((*reader).Read(&reply)) {
			std::cout << "Got " << reply.appliance_type() << " with name "
					<< reply.appliance_name() << " and state = "
					<< reply.state() << std::endl;
		}

		Status status = (*reader).Finish();
		if (status.ok()) {
			std::cout << "notification rpc succeeded." << std::endl;
		} else {
			std::cout << "notification rpc failed." << std::endl;
		}
	}
};

void run_notify(Client *client) {
	std::string user("192.168.0.107");
	client->notification(user);
}

int main(int argc, char *argv[]) {

	Client greeter(
			grpc::CreateChannel("localhost:50051",
					grpc::InsecureChannelCredentials()));
	std::string user("192.168.0.107");
	rpcHouse reply = greeter.get_all(user);
	for (int i = 0; i < reply.room_size(); i++) {
		rpcRoom room = reply.room(i);
		for (int j = 0; j < room.appliance_size(); j++) {
			rpcAppliance app = room.appliance(j);
			std::cout << room.room_name() << std::endl;
			std::cout << app.app_name() << std::endl;
			std::cout << app.app_type() << std::endl;
			std::cout << app.app_state() << std::endl;
		}
		std::cout << std::endl;
	}
	std::thread t(run_notify, &greeter);
	for (int i = 0; i < 100; i++)
		greeter.set_state(user, "hall", "light", "light1", i % 2);
	reply = greeter.get_all(user);
	for (int i = 0; i < reply.room_size(); i++) {
		rpcRoom room = reply.room(i);
		for (int j = 0; j < room.appliance_size(); j++) {
			rpcAppliance app = room.appliance(j);
			std::cout << room.room_name() << std::endl;
			std::cout << app.app_name() << std::endl;
			std::cout << app.app_type() << std::endl;
			std::cout << app.app_state() << std::endl;
		}
		std::cout << std::endl;
	}
	t.join();
	return 0;
}
