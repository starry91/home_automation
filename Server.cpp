/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Server.cpp
 * Author: praveen
 * 
 * Created on September 6, 2017, 10:28 PM
 */

#include "Server.h"

Server::Server(int port_no) {
    this->port_number = port_no;
};

struct sockaddr_in Server::getServer_addr() {
    return serv_addr;
}

int Server::getSockDescriptor() {
    return sock_descriptor;
}

char* Server::readServBuff() {
    char *p = serv_buff;
    return p;
}

void Server::setPortNumber(int port) {
    port_number = port;
}

void Server::writeServBuff(const std::string buff) {
    memset(serv_buff, 0, sizeof (serv_buff));
    strcpy(serv_buff, buff.c_str());
}

//TCP Derived Class

TCPServer::TCPServer(int port_no) : Server(port_no) {
};

int TCPServer::TCPListen() {
    sock_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_descriptor == -1) {
        perror("Erro: server socket not created");
        return 1;
    }
    if (setsockopt(sock_descriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt_val, sizeof (opt_val))) {
        perror("setsockopt");
        return 2;
    }
    if (sock_descriptor < 0) {
        perror("ERROR opening socket");
        return 3;
    }
    bzero((char *) &serv_addr, sizeof (serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_number);
    if (bind(sock_descriptor, (struct sockaddr *) &serv_addr, sizeof (serv_addr))
            < 0) {
        perror("ERROR on binding TCP socket");
        return 4;
    }
    if (listen(sock_descriptor, 5) < 0)
        return 5;
    return 0;
}

UDPServer::UDPServer(int port_no) : Server(port_no) {
};

int UDPServer::UDPListen() {
    sock_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_descriptor == -1) {
        perror("Erro: server socket not created");
        return 1;
    }
    if (sock_descriptor < 0) {
        perror("ERROR opening socket");
        return 3;
    }
    bzero((char *) &serv_addr, sizeof (serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_number);
    if (bind(sock_descriptor, (struct sockaddr *) &serv_addr, sizeof (serv_addr))
            < 0) {
        perror("ERROR on binding TCP socket");
        return 4;
    }
    if (listen(sock_descriptor, 5) < 0)
        return 5;
    return 0;
}

