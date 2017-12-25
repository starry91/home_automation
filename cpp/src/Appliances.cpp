/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Appliances.cpp
 * Author: praveen
 * 
 * Created on September 6, 2017, 10:24 PM
 */

#include "Appliances.h"

void Appliance::setName(std::string s) {
	name = s;
}

std::string Appliance::getName() {
	return name;
}

void Appliance::setState(State n) {
	if (Appliance::State::OFF <= n && Appliance::State::ON >= n)
		state = n;
	else {
		throw(std::string("Invalid State value")
				+ std::to_string(static_cast<int>(n)));
	}
}

int Appliance::getState() {
	return static_cast<int>(state);
}

void Appliance::setType(std::string s) {
	type = s;
}

std::string Appliance::getType() {
	return type;
}

Appliance::Appliance(std::string s, State n, std::string type = "None") {
	this->name = s;
	this->setState(n);
	this->type = type;
}
