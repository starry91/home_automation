/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Appliances.h
 * Author: praveen
 *
 * Created on September 6, 2017, 10:24 PM
 */

#ifndef APPLIANCES_H
#define APPLIANCES_H

#include <iostream>
#include <string>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

class Appliance {
public:

    enum class State {
        OFF = 0,
        ON = 1
    };
protected:
    std::string name; //name of the appliance
    State state; //state of the appliance

public:

    void setName(std::string s);
    void setState(State n);
    std::string getName();
    int getState();
    Appliance(std::string s, State n);
};

class Light : public Appliance {
public:
    Light(std::string s = "abc", Light::State n = State::OFF);
};

class Fan : public Appliance {
public:
    Fan(std::string s = "abc", Fan::State n = State::OFF);
};

#endif /* APPLIANCES_H */

