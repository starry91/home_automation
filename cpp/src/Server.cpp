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

//#include "Server.h"
#include "ExtFunctions.h" 
#include <iostream>
#include <string>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>
#include <vector>


int UDPListenAndRespond(int port_number) {
    int sock_descriptor;
    sockaddr_in serv_addr;
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
    std::cout << port_number << std::endl;
    serv_addr.sin_port = htons(port_number);
    if (bind(sock_descriptor, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) {
        perror("ERROR on binding TCP socket");
        return 4;
    }
    
    while (1) {
        int len;
        char udp_buf[1024];
        struct sockaddr_in cli_addr;
        socklen_t clilen;
        bzero((char *) &cli_addr, sizeof(cli_addr));
        if ((len = recvfrom(sock_descriptor, udp_buf, sizeof (udp_buf), 0, (struct sockaddr*) &cli_addr, &clilen)) <= 0) {
            perror("Error: recvfrom call failed");
        }
        udp_buf[len] = '\0';
        std::string temp(udp_buf);
        std::cout << temp << std::endl;
        if (trim(temp) == "Hi") {
            if (sendto(sock_descriptor, (char*) "xyz", 3, 0, (struct sockaddr*) &cli_addr, sizeof (cli_addr)) == -1) {
                perror("Error: sendto call failed");
            }
        }
    }
    return 0;
}

