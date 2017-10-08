/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ClientData.h
 * Author: praveen
 *
 * Created on September 6, 2017, 10:42 PM
 */

#ifndef CLIENTDATA_H
#define CLIENTDATA_H

#include <iostream>
#include <string>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "Client.h"

class ClientData {
private:
    std::vector<Client> clients;
    int max_descriptor;
    int client_list_size;
public:
    ClientData();
    void addClient(int sd, sockaddr_in cli_addr);
    void removeClient(int client_sd);
    int getMaxDescriptor();
    void setMaxDescriptor(int max_sd);
    Client& getClient(int client_index);
    int getClientListSize();
};

#endif /* CLIENTDATA_H */

