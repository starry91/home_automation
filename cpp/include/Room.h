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
/*#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>*/
#include <vector>

#include "Appliances.h"

class Room {
private:
    std::string room_name;
    std::vector<Appliance> appliances;

public:

    enum class State {
        OFF = 0,
        ON = 1
    };
    Room(const std::string room_Name);
    std::string getRoomName();
    void setRoomName(std::string name);
    void addAppliance(std::string name, std::string type, int state = 0);
    int getApplianceState(std::string name);
    void setApplianceState(std::string name, int state);
    std::vector<Appliance>::iterator iter_begin_appliance();
    std::vector<Appliance>::iterator iter_end_appliance();

};

#endif /* ROOM_H */

