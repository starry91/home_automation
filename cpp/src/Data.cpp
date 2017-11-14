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

std::vector<Room>::iterator Data::iter_begin_data() {
    return room.begin();
}

std::vector<Room>::iterator Data::iter_end_data() {
    return room.end();
}
