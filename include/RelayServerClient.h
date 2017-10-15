/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RelayServerClient.h
 * Author: praveen
 *
 * Created on September 6, 2017, 10:44 PM
 */

#ifndef RELAYSERVERCLIENT_H
#define RELAYSERVERCLIENT_H

#include <iostream>
#include <string>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "proto/relay_server.grpc.pb.h"

class RelayServerClient {
public:

    RelayServerClient(std::shared_ptr<grpc::Channel> channel);

    // Assembles the client's payload, sends it and presents the response back
    // from the server.

    int get_state(const std::string& room_name, const std::string& appliance_type, const std::string& appliance_name);

    int set_state(const std::string& room_name, const std::string& appliance_type, const std::string& appliance_name, int state);

private:
    std::unique_ptr<py_server::RelayServer::Stub> stub_;
};

#endif /* RELAYSERVERCLIENT_H */

