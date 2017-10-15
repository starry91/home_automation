/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Server.h
 * Author: praveen
 *
 * Created on September 6, 2017, 10:28 PM
 */

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>


class Server {
protected:
    int sock_descriptor;
    int opt_val = 1;
    struct sockaddr_in serv_addr;
    int port_number;
    char serv_buff[1024];

public:
    Server(int port_no);
    struct sockaddr_in getServer_addr();
    int getSockDescriptor();
    char* readServBuff();
    void setPortNumber(int port);
    void writeServBuff(const std::string buff);
};

class TCPServer : public Server {
public:
    TCPServer(int port_no);
    int TCPListen();
};

class UDPServer : public Server {
public:
    UDPServer(int port_no);
    int UDPListen();
};

#endif /* SERVER_H */

