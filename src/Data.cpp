/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Data.cpp
 * Author: praveen
 * 
 * Created on September 6, 2017, 10:31 PM
 */

#include "Data.h"
#include <fstream>
#include <sstream>
#include "RelayServerClient.h"
#include <grpc++/grpc++.h>

Data::Data() {
};

void Data::addRoom(std::string name) {
    room.push_back(Room(name));
}

Room& Data::getRoom(std::string name) {
    for (std::vector<Room>::iterator it = room.begin(); it != room.end(); ++it) {
        Room& room = *it;
        if (room.getRoomName() == name)
            return room;
    }
    //throw error;
}

int Data::read_from_config(Json::Value& json_data) {
    Json::Value config;
    Json::FastWriter stream;
    Json::Reader reader;
    std::ifstream input;
    std::stringstream buff;
    RelayServerClient RelayServer(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    input.open("server_config.conf");
    buff << input.rdbuf();
    std::string js = buff.str();
    //cout << js <<endl;
    input.close();
    reader.parse(js, config);
    std::cout << config << std::endl;
    Json::ValueIterator it = config.begin();
    for (it; it != config.end(); it++) {
        std::string room_name = it.key().asString();
        addRoom(room_name);
        Room& room = getRoom(room_name);
        Json::Value a = *it;
        Json::ValueIterator it1 = a.begin();
        for (it1; it1 != a.end(); it1++) {
            std::string appliance_type = it1.key().asString();
            if (appliance_type == "lights") {
                Json::Value light_arr = *it1;
                Json::ValueIterator it2 = light_arr.begin();
                for (it2; it2 != light_arr.end(); it2++) {
                    //std::string name = it2.key().asString();
                    Json::Value temp_data;
                    std::string name = (*it2).asString();
                    temp_data[room_name]["lights"] = name;
                    int light_state = RelayServer.get_state(room_name, appliance_type, name);
                    std::cout << "received light state" << std::endl;
                    room.addLight(name);
                    room.setLightState(name, light_state); //get from aurdino
                    json_data[room_name]["lights"][name] = light_state; //get from aurdino
                }
            } else if (appliance_type == "fans") {
                Json::Value fan_arr = *it1;
                Json::ValueIterator it2 = fan_arr.begin();
                for (it2; it2 != fan_arr.end(); it2++) {
                    Json::Value temp_data;
                    //std::string name = it2.key().asString();
                    std::string name = (*it2).asString();
                    temp_data[room_name]["fans"] = name;
                    int fan_state = RelayServer.get_state(room_name, appliance_type, name);
                    std::cout << "received fan state" << std::endl;
                    room.addFan(name);
                    room.setFanState(name, fan_state); //get from aurdino
                    json_data[room_name]["fans"][name] = fan_state; //get from aurdino
                }
            } else {
                std::cout << "Input to data error" << std::endl; //throw error
                return -1;
            }
        }
    }
    return 0;    
}

