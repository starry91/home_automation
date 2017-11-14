/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Configuration.cpp
 * Author: praveen
 * 
 * Created on November 13, 2017, 10:13 PM
 */
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <Data.h>
#include <json/json.h>
#include <RelayServerClient.h>
#include <grpc++/create_channel.h>


Data read_from_config() {
    Data data;
    Json::Value config;
    //Json::FastWriter stream;
    Json::Reader reader;
    std::ifstream input;
    std::stringstream buff;
    input.open("server_config.conf");
    buff << input.rdbuf();
    std::string js = buff.str();
    //cout << js <<endl;
    input.close();
    reader.parse(js, config);
    std::cout << config << std::endl;
    for (Json::ValueIterator it = config.begin(); it != config.end(); it++) {
        std::string room_name = it.key().asString();
        data.addRoom(room_name);
        Room& room = data.getRoom(room_name);
        Json::Value a = *it;
        Json::ValueIterator it1 = a.begin();
        for (it1; it1 != a.end(); it1++) {
            std::string appliance_type = it1.key().asString();
            Json::Value appliance_arr = *it1;
            Json::ValueIterator it2 = appliance_arr.begin();
            RelayServerClient RelayServer(grpc::CreateChannel("192.168.0.104:50051", grpc::InsecureChannelCredentials()));
            for (it2; it2 != appliance_arr.end(); it2++) {
                std::string name = (*it2).asString();
                int appliance_state = RelayServer.get_state(room_name, appliance_type, name);
                std::cout << "appliance state: " << appliance_state << std::endl;
                std::cout << "received appliance state" << std::endl;
                room.addAppliance(name, appliance_type, appliance_state);
            }
        }
    }
    return data;    
}
