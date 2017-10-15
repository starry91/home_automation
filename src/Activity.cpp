/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Activity.cpp
 * Author: praveen
 * 
 * Created on September 6, 2017, 10:45 PM
 */

#include "Activity.h"
#include "ExtFunctions.h"
#include "RelayServerClient.h"
#include <grpc++/grpc++.h>

Activity::Activity(){};

void Activity::set_active_descriptors(const int& udp_descriptor, const int& tcp_descriptor, ClientData& client_data) {
    int max_sd;
    FD_ZERO(&readfds);
    FD_SET(udp_descriptor, &readfds);
    max_sd = udp_descriptor;
    FD_SET(tcp_descriptor, &readfds);
    max_sd = tcp_descriptor > max_sd ? tcp_descriptor : max_sd;
    for (int i = 0; i < client_data.getClientListSize(); i++) {
        Client& client = client_data.getClient(i);
        int sd = client.getSocketDescriptor();
        //cout << "sd: " << sd <<endl;
        if (sd > 0)
            FD_SET(sd, &readfds);
        if (sd > max_sd)
            max_sd = sd;
    }
    client_data.setMaxDescriptor(max_sd);
}

void Activity::wait_for_activity(ClientData& client_data) {
    int activity = select(client_data.getMaxDescriptor() + 1, &readfds, NULL, NULL, NULL);
    if ((activity < 0) && (errno != EINTR)) {
        std::cout << "select error" << std::endl;
    }    
}

void Activity::check_udp_activity_and_respond(UDPServer& udp) {
    if (FD_ISSET(udp.getSockDescriptor(), &readfds)) {
        int len;
        char *udp_buf = udp.readServBuff();
        struct sockaddr_in cli_addr;
        socklen_t clilen;
        if ((len = recvfrom(udp.getSockDescriptor(), udp_buf, sizeof (udp_buf), 0,
                (struct sockaddr*) &cli_addr, &clilen)) <= 0) {
            perror("Error: recvfrom call failed");
        }
        udp_buf[len] = '\0';
        std::string temp(udp_buf);
        std::cout << temp << std::endl;
        if (trim(temp) == "Hi") {
            if (sendto(udp.getSockDescriptor(), (char*) "xyz", 3, 0, (struct sockaddr*) &cli_addr, sizeof (cli_addr)) == -1) {
                perror("Error: sendto call failed");
            }
        }
    }    
}

void Activity::check_tcp_activity_and_respond(TCPServer& tcp, ClientData& client_data, Data& data, const Json::Value json_data) {
    int tcp_sd = tcp.getSockDescriptor();
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    int new_socket;
    Json::FastWriter stream;
    Json::Reader reader;

    if (FD_ISSET(tcp_sd, &readfds)) {
        std::cout << "isset: mastersoc" << std::endl;
        if ((new_socket = accept(tcp_sd, (struct sockaddr *) &cli_addr, &clilen)) < 0) {
            std::cout << "new soc error" << std::endl; //write the linux error code here
        }
        client_data.addClient(new_socket, cli_addr);
        client_data.setMaxDescriptor(new_socket);
        std::string s = stream.write(json_data);
        char *write_buf = new char[s.size()];
        strcpy(write_buf, s.c_str());
        write(new_socket, write_buf, strlen(write_buf));
        delete write_buf;
    }    
}

int Activity::check_client_activity_update_and_respond(ClientData& client_data, Data& data, Json::Value& json_data, int client_sd) {
    Json::Reader reader;
    Json::Value sig_change;
    Json::FastWriter stream;
    RelayServerClient RelayServer(grpc::CreateChannel(grpc::string("localhost:50051"), grpc::InsecureChannelCredentials()));
    if (FD_ISSET(client_sd, &readfds)) {
        char read_buf[256];
        std::cout << "fdisset" << std::endl;
        int n = read(client_sd, read_buf, 256);
        std::string r(read_buf);
        if (n == 0) {
            close(client_sd);
            client_data.removeClient(client_sd);
        } else if (reader.parse(trim(r), sig_change)) {
            std::cout << sig_change << std::endl;
            Json::Value::iterator it;
            Json::Value it_data;
            it = sig_change.begin();
            it_data = *it;

            std::cout << it_data << std::endl;
            std::string room_name = it.key().asString();
            std::cout << room_name << std::endl;
            Room& room = data.getRoom(room_name);
            it = it_data.begin();
            std::string appliance_type = it.key().asString();
            std::cout << "App type: " << appliance_type << std::endl;

            it_data = *it;
            std::cout << it_data << std::endl;

            it = it_data.begin();
            std::string appliance_name = it.key().asString();
            std::cout << appliance_name << std::endl;
            int state = (*it).asInt();
            if (appliance_type == "lights") {
                room.setLightState(appliance_name, state);
                json_data[room_name]["lights"][appliance_name] = state;
            } else if (appliance_type == "fans") {
                room.setFanState(appliance_name, state);
                json_data[room_name]["fans"][appliance_name] = state;
            } else {
                std::cout << "error" << std::endl;
            }
            int status = RelayServer.set_state(room_name, appliance_type, appliance_name, state);
            std::cout << "get status: " << status << std::endl;
            std::cout << json_data << std::endl;
            std::string s = stream.write(json_data);
            char *write_buf_others = new char[s.size()];
            strcpy(write_buf_others, s.c_str());
            for (int j = 0; j <= client_data.getClientListSize(); j++) {
                Client& client = client_data.getClient(j);
                int sd = client.getSocketDescriptor();
                //if(sd != client_sd)
                {
                    write(sd, write_buf_others, strlen(write_buf_others));
                }
            }
            delete write_buf_others;
        } else {
            std::cout << "read mismatch" << std::endl; //throw error;
            return 2;
        }
        std::cout << "data updated: \n" << std::endl;
    }
    return 0;        
}

