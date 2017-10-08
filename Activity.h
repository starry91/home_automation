/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Activity.h
 * Author: praveen
 *
 * Created on September 6, 2017, 10:45 PM
 */

#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <iostream>
#include <json/json.h>
#include "ClientData.h"
#include "Server.h"
#include "Data.h"
#include "relay_server.grpc.pb.h"

class Activity {
private:
    fd_set readfds; 
public:
    Activity();
    void set_active_descriptors(const int& udp_descriptor,const int& tcp_descriptor, ClientData& client_data);
    void wait_for_activity(ClientData& client_data);
    void check_udp_activity_and_respond(UDPServer& udp);
    void check_tcp_activity_and_respond(TCPServer& tcp, ClientData& client_data, Data& data, const Json::Value json_data);
    int check_client_activity_update_and_respond(ClientData& client_data, Data& data, Json::Value& json_data, int client_sd);  
};


#endif /* ACTIVITY_H */

