/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RelayServerClient.cpp
 * Author: praveen
 * 
 * Created on September 6, 2017, 10:44 PM
 */

#include "RelayServerClient.h"
#include "proto/relay_server.pb.h"
#include <grpc++/grpc++.h>

RelayServerClient::RelayServerClient(std::shared_ptr<grpc::Channel> channel)
: stub_(py_server::RelayServer::NewStub(channel)) {
}

int RelayServerClient::get_state(const std::string& room_name, const std::string& appliance_type, const std::string& appliance_name) {
    // Data we are sending to the server.
    py_server::Details request;
    request.set_room_name(room_name);
    request.set_appliance_type(appliance_type);
    request.set_appliance_name(appliance_name);
    // Container for the data we expect from the server.
    py_server::State reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    grpc::ClientContext context;

    // The actual RPC.
    grpc::Status status = stub_->get_state(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
        return reply.value();
    } else {
        std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
        return -1;
    }
}

int RelayServerClient::set_state(const std::string& room_name, const std::string& appliance_type, const std::string& appliance_name, int state) {
    // Data we are sending to the server.
    py_server::Details request;
    request.set_room_name(room_name);
    request.set_appliance_type(appliance_type);
    request.set_appliance_name(appliance_name);
    request.set_state(state);
    // Container for the data we expect from the server.
    py_server::Response reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    grpc::ClientContext context;

    // The actual RPC.
    grpc::Status status = stub_->set_state(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
        return reply.value();
    } else {
        std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
        return -1;
    }
}
