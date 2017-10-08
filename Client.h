/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Client.h
 * Author: praveen
 *
 * Created on September 6, 2017, 10:39 PM
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

class Client {
private:
    int sd;
    struct sockaddr_in client_addr;
public:
    Client(int socket_descriptor, sockaddr_in client_address);
    int getSocketDescriptor();
    void setSocketDescriptor(int socket_descriptor);
    struct sockaddr_in getAddress();
};

#endif /* CLIENT_H */

