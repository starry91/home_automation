/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AppToCpp_Server.cpp
 * Author: praveen
 * 
 * Created on November 4, 2017, 4:12 PM
 */

#include "AppToCpp_Server.h"
#include "AppToCppServer.pb.h"
#include <google/protobuf/util/message_differencer.h>
#include <list>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "RelayServerClient.h"
#include <grpc++/grpc++.h>
#include <grpc++/impl/codegen/sync_stream.h>

class Notification_obj {
private:
	std::string id;
	std::queue<app_cpp_server::rpcData> rpcq;
	std::mutex mutex_;
	std::condition_variable cond_;

public:

	Notification_obj(std::string name) {
		this->id = name;
	}

	std::string getId() {
		return id;
	}

	void push(const app_cpp_server::rpcData message) {
		std::lock_guard<std::mutex> mlock(mutex_);
		rpcq.push(message);

		cond_.notify_one();
	}

	app_cpp_server::rpcData pop() {
		std::lock_guard<std::mutex> mlock(mutex_);
		if (rpcq.empty())
			throw "List is empty!";
		app_cpp_server::rpcData data = rpcq.front();
		rpcq.pop();
		return data;
	}

	void wait(int seconds) {
		std::unique_lock<std::mutex> mlock(mutex_);
		cond_.wait_for(mlock, std::chrono::seconds(seconds));
	}

	bool empty() {
		std::lock_guard<std::mutex> mlock(mutex_);
		return rpcq.empty();
	}

};
typedef std::shared_ptr<Notification_obj> Notification_obj_Sptr;

class NotificationsRegistry {
private:
	std::list<Notification_obj_Sptr> globalQ;
	std::mutex mutex_;
	NotificationsRegistry() {
	}
public:
	static NotificationsRegistry& instance() {
		static NotificationsRegistry object;
		return object;
	}

	void addQueue(Notification_obj_Sptr q_ptr) {
		std::lock_guard<std::mutex> mlock(mutex_);
		globalQ.push_back(q_ptr);
	}

	void popQueue(Notification_obj_Sptr q_ptr) {
		std::lock_guard<std::mutex> mlock(mutex_);
		globalQ.remove(q_ptr);
	}

	void notify(app_cpp_server::rpcData data) {
		std::lock_guard<std::mutex> mlock(mutex_);
		for (auto q : globalQ) {
			if (q->getId() != data.id())
				q->push(data);
		}
	}

};

class RelayServiceManager {
private:
	RelayServerClient RelayServer;
	RelayServiceManager() {
		this->RelayServer = RelayServerClient(
				grpc::CreateChannel(grpc::string("192.168.0.102:50051"),
						grpc::InsecureChannelCredentials()));
	}

public:
	static RelayServiceManager& instance() {
		static RelayServiceManager object;
		return object;
	}
	RelayServerClient& getService() {
		return RelayServer;
	}

};

AppToCppGrpcServer::AppToCppGrpcServer(Data& d) {
	data = d;
}

grpc::Status AppToCppGrpcServer::get_all(::grpc::ServerContext* context,
		const app_cpp_server::rpcId* request,
		app_cpp_server::rpcHouse* response) {
	for (std::vector<Room>::iterator it_room = data.iter_begin_data();
			it_room != data.iter_end_data(); it_room++) {
		app_cpp_server::rpcRoom* proto_room = response->add_room();
		Room room = *it_room;
		proto_room->set_room_name(room.getRoomName());
		for (std::vector<Appliance>::iterator it_appliance =
				room.iter_begin_appliance();
				it_appliance != room.iter_end_appliance(); it_appliance++) {
			app_cpp_server::rpcAppliance *proto_appliance =
					proto_room->add_appliance();
			Appliance appliance = *it_appliance;
			proto_appliance->set_app_name(appliance.getName());
			proto_appliance->set_app_type(appliance.getType());
			proto_appliance->set_app_state(appliance.getState());
		}
	}
	return grpc::Status::OK;
}

grpc::Status AppToCppGrpcServer::notification(::grpc::ServerContext* context,
		const ::app_cpp_server::rpcId* request,
		::grpc::ServerWriter<::app_cpp_server::rpcData>* writer) {

	Notification_obj_Sptr q_Sptr = std::make_shared<Notification_obj>(
			request->id());
	NotificationsRegistry::instance().addQueue(q_Sptr);
	std::cout << "Registered for notifications: " << request->id() << std::endl;
	while (!context->IsCancelled()) {
		q_Sptr->wait(1);
		if (q_Sptr->empty())
			continue;
		auto check = q_Sptr->pop();
		std::cout << "Sending notification: " << check.appliance_name()
				<< " type: " << check.appliance_type() << " value: "
				<< check.state() << std::endl;
		writer->Write(check);
	}
	/*int counter = 1;
	 while(!context->IsCancelled()) {
	 app_cpp_server::rpcData d;
	 d.set_id(std::to_string(counter));
	 d.set_appliance_name("light2");
	 d.set_room_name("bedroom");
	 d.set_appliance_type("light");
	 d.set_state(counter%2);
	 writer->Write(d);
	 counter++;
	 std::cout << "Sending notification: " << d.appliance_name()
	 << " type: " << d.appliance_type() << " value: " << d.state() << std::endl;
	 std::this_thread::sleep_for(std::chrono::seconds(1));
	 //("1", "bedroom", "light", "light2", 1);
	 }
	 */
	NotificationsRegistry::instance().popQueue(q_Sptr);
	std::cout << "Exiting notifications: " << request->id() << std::endl;
	return grpc::Status::OK;
}

grpc::Status AppToCppGrpcServer::set_state(::grpc::ServerContext* context,
		const ::app_cpp_server::rpcData* request,
		::app_cpp_server::rpcResponse* response) {
	//RelayServerClient RelayServer(grpc::CreateChannel(grpc::string("192.168.0.104:50051"), grpc::InsecureChannelCredentials()));
	int exit_state = RelayServiceManager::instance().getService().set_state(
			request->room_name(), request->appliance_type(),
			request->appliance_name(), request->state());
	std::cout << "Received request: "
			<< request->appliance_name() + ":" + request->appliance_type() + ":"
			<< request->state() << std::endl;
	for (std::vector<Room>::iterator it_room = data.iter_begin_data();
			it_room != data.iter_end_data(); it_room++) {
		Room& room = *it_room;
		if (room.getRoomName() == request->room_name()) {
			room.setApplianceState(request->appliance_name(), request->state());
		}
	}
	NotificationsRegistry::instance().notify(*request);
	response->set_value(exit_state);
	return grpc::Status::OK;
}

void AppToCppGrpcServer::start_server() {
	std::string server_address("0.0.0.0:50051");
	//AppToCppGrpcServer service;
	grpc::ServerBuilder builder;
	// Listen on the given address without any authentication mechanism.
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	// Register "service" as the instance through which we'll communicate with
	// clients. In this case it corresponds to an *synchronous* service.
	builder.RegisterService(this);
	// Finally assemble the server.
	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << server_address << std::endl;

	// Wait for the server to shutdown. Note that some other thread must be
	// responsible for shutting down the server for this call to ever return.
	(*server).Wait();
}

