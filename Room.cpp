/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Room.cpp
 * Author: praveen
 * 
 * Created on September 6, 2017, 10:33 PM
 */

#include "Room.h"

Room::Room(const std::string room_Name)
: room_name(room_Name) {
};

std::string Room::getRoomName() {
    return room_name;
}

int Room::getLightState(std::string name) {
    for (std::vector<Light>::iterator it = light.begin(); it != light.end(); ++it) {
        Light temp = *it;
        if (temp.getName() == name)
            return temp.getState();
    };
    return -1;
}

int Room::getFanState(std::string name) {
    for (std::vector<Fan>::iterator it = fan.begin(); it != fan.end(); ++it) {
        Fan temp = *it;
        if (temp.getName() == name)
            return temp.getState();
    }
    return -1;
}

void Room::setRoomName(std::string room_Name) {
    room_name = room_Name;
}

void Room::addLight(std::string name, int state) {
    light.push_back(Light(name, Light::State(state)));
}

void Room::addFan(std::string name, int state) {
    fan.push_back(Fan(name, Fan::State(state)));
}

void Room::setFanState(std::string name, int state) {
    for (std::vector<Fan>::iterator it = fan.begin(); it != fan.end(); ++it) {
        Fan& temp = *it;
        if (temp.getName() == name) {
            temp.setState(Appliance::State(state));
            break;
        }

    }
}

void Room::setLightState(std::string name, int state) {
    for (std::vector<Light>::iterator it = light.begin(); it != light.end(); ++it) {
        Light& temp = *it;
        if (temp.getName() == name) {
            temp.setState(Appliance::State(state));
            break;
        }

    }
}

