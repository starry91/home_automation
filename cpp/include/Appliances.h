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
/*#include <sys/types.h>       
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>*/
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
    std::string type;

public:
    Appliance(std::string s, State n, std::string type);
    void setName(std::string s);
    std::string getName();
    void setState(State n);
    int getState();
    void setType(std::string s);
    std::string getType();
};


#endif /* APPLIANCES_H */

