/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Client.cpp
 * Author: praveen
 * 
 * Created on September 6, 2017, 10:39 PM
 */

#include "Client.h"

Client::Client(int socket_descriptor, sockaddr_in client_address) {
    sd = socket_descriptor;
    bzero((char *) &client_addr, sizeof (client_addr));
    client_addr = client_address;
}

int Client::getSocketDescriptor() {
    return sd;
}

struct sockaddr_in Client::getAddress() {
    return client_addr;
}

void Client::setSocketDescriptor(int socket_descriptor) {
    sd = socket_descriptor;
}

