/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Room.h
 * Author: praveen
 *
 * Created on September 6, 2017, 10:33 PM
 */

#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <string>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "Appliances.h"

class Room {
private:
    std::string room_name;
    std::vector<Light> light;
    std::vector<Fan> fan;
public:

    enum class State {
        OFF = 0,
        ON = 1
    };
    Room(const std::string room_Name);
    std::string getRoomName();
    int getLightState(std::string name);
    int getFanState(std::string name);
    void setRoomName(std::string name);
    void addLight(std::string name, int state = 0);
    void addFan(std::string name, int state = 0);
    void setFanState(std::string name, int state);
    void setLightState(std::string name, int state);
};

#endif /* ROOM_H */

