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

static std::list<std::string> client_list; 
static std::queue<app_cpp_server::rpcData> rpcdata;
static std::mutex queuemutex_data;
static std::condition_variable cv_data;
static std::mutex queuemutex_list;
static std::condition_variable cv_list;


AppToCppGrpcServer::AppToCppGrpcServer(Data& d) {
    data = d;
}

grpc::Status AppToCppGrpcServer::get_all(::grpc::ServerContext* context, const app_cpp_server::rpcId* request,
        app_cpp_server::rpcHouse* response) {
    for(std::vector<Room>::iterator it_room = data.iter_begin_data(); it_room != data.iter_end_data(); it_room++) {
        app_cpp_server::rpcRoom* proto_room = response->add_room();
        Room room = *it_room;
        proto_room->set_room_name(room.getRoomName());
        for(std::vector<Appliance>::iterator it_appliance = room.iter_begin_appliance(); it_appliance != room.iter_end_appliance(); it_appliance++) {
            app_cpp_server::rpcAppliance *proto_appliance = proto_room->add_appliance();
            Appliance appliance = *it_appliance;
            proto_appliance->set_app_name(appliance.getName());
            proto_appliance->set_app_type(appliance.getType());
            proto_appliance->set_app_state(appliance.getState());
        }
    }
    return grpc::Status::OK;   
}

grpc::Status AppToCppGrpcServer::notification(::grpc::ServerContext* context, const ::app_cpp_server::rpcId* request,
            ::grpc::ServerWriter< ::app_cpp_server::rpcData>* writer) {
    std::unique_lock<std::mutex> qlock_list(queuemutex_list);
    cv_list.wait_for(qlock_list, std::chrono::seconds(1)); 
    client_list.push_front(request->id());
    qlock_list.unlock();
    cv_list.notify_all();
    
    app_cpp_server::rpcData check;
    while(1) {
        if(!google::protobuf::util::MessageDifferencer::Equals(check, rpcdata.front())) {
            std::unique_lock<std::mutex> qlock(queuemutex_data);
            cv_data.wait_for(qlock, std::chrono::seconds(1));
            check = rpcdata.front();
            if(check.id() != request->id()) {          
                writer->Write(check);
            }
        }
        //cv_data.notify_all();
    }
    return grpc::Status::OK;
}


grpc::Status AppToCppGrpcServer::set_state(::grpc::ServerContext* context, const ::app_cpp_server::rpcData* request, 
        ::app_cpp_server::rpcResponse* response) {
    RelayServerClient RelayServer(grpc::CreateChannel(grpc::string("192.168.0.104:50051"), grpc::InsecureChannelCredentials()));
    int exit_state = RelayServer.set_state(request->room_name(), request->appliance_type(), request->appliance_name(), request->state());
    if(exit_state == 0) {
        {
            std::lock_guard<std::mutex> qlock(queuemutex_data);
            rpcdata.push(*request);
            rpcdata.pop();
        }
        for(std::vector<Room>::iterator it_room = data.iter_begin_data(); it_room != data.iter_end_data(); it_room++) {
            Room& room = *it_room;
            if(room.getRoomName() == request->room_name()) {
                room.setApplianceState(request->appliance_name(), request->state());
            }
        }  
    }
    cv_data.notify_all();
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

