/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ClientData.cpp
 * Author: praveen
 * 
 * Created on September 6, 2017, 10:42 PM
 */

#include "ClientData.h"

ClientData::ClientData() {
    client_list_size = 0;
    max_descriptor = 0;
}

Client& ClientData::getClient(int client_index) {
    return clients[client_index];
}

int ClientData::getMaxDescriptor() {
    return max_descriptor;
}

void ClientData::setMaxDescriptor(int max_sd) {
    max_descriptor = max_sd;
}

int ClientData::getClientListSize() {
    return client_list_size;
}

void ClientData::addClient(int sd, sockaddr_in cli_addr) {
    clients.push_back(Client(sd, cli_addr));
    client_list_size++;
}

void ClientData::removeClient(int client_sd) {
    for (int i = 0; i < client_list_size; i++) {
        Client& temp = clients[i];
        if (temp.getSocketDescriptor() == client_sd) {
            clients.erase(clients.begin() + i);
            client_list_size--;
            break;
        }
    }
}
