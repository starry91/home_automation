/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Data.h
 * Author: praveen
 *
 * Created on September 6, 2017, 10:31 PM
 */

#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <string>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <json/json.h>

#include "Room.h"

class Data {
private:
    std::vector<Room> room;
public:
    Data();
    void addRoom(std::string name);
    Room& getRoom(std::string name);
    int read_from_config(Json::Value& json_data);
    //friend std::ostream& operator<< (std::ostream &out, const Data &data);
};

#endif /* DATA_H */

