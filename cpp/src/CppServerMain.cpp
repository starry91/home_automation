/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CppServerMain.cpp
 * Author: praveen
 *
 * Created on November 12, 2017, 1:21 PM
 */

#include <cstdlib>
#include <AppToCpp_Server.h>
#include <thread>
#include <Server.h>
#include <Data.h>
#include <Configuration.h>

int main(int argc, char** argv) {
    Data data = read_from_config();
    std::thread t1(UDPListenAndRespond, 8080);
    AppToCppGrpcServer server(data);
    server.start_server();
    t1.join();
    return 0;
}

