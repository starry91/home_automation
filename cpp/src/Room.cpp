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

Room::Room(const std::string room_Name) :
		room_name(room_Name) {
}

std::string Room::getRoomName() {
	return room_name;
}

int Room::getApplianceState(std::string name) {
	for (std::vector<Appliance>::iterator it = appliances.begin();
			it != appliances.end(); ++it) {
		Appliance temp = *it;
		if (temp.getName() == name)
			return temp.getState();
	};
	return -1;
}

void Room::setRoomName(std::string room_Name) {
	room_name = room_Name;
}

void Room::addAppliance(std::string name, std::string type, int state) {
	appliances.push_back(Appliance(name, Appliance::State(state), type));
}

void Room::setApplianceState(std::string name, int state) {
	for (std::vector<Appliance>::iterator it = appliances.begin();
			it != appliances.end(); ++it) {
		Appliance& temp = *it;
		if (temp.getName() == name) {
			temp.setState(Appliance::State(state));
			break;
		}

	}
}

std::vector<Appliance>::iterator Room::iter_begin_appliance() {
	return appliances.begin();
}

std::vector<Appliance>::iterator Room::iter_end_appliance() {
	return appliances.end();
}

