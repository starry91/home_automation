
#ifndef APPTOCPP_SERVER_H
#define APPTOCPP_SERVER_H

#include "AppToCppServer.grpc.pb.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <algorithm>
#include <unistd.h>
#include <json/json.h>
#include <fstream>

#include "ExtFunctions.h"
#include "Appliances.h"
#include "Data.h"
#include "RelayServerClient.h"
#include "Room.h"
#include "Server.h"

class AppToCppGrpcServer : public app_cpp_server::AppCppServer::Service {
private:
    Data data;
    //int udp_sd;
public:
    AppToCppGrpcServer(Data& d);
    void start_UDP_server();
    void start_server();
    grpc::Status get_all(::grpc::ServerContext* context, const ::app_cpp_server::rpcId* request, ::app_cpp_server::rpcHouse* response); 
    grpc::Status set_state(::grpc::ServerContext* context, const ::app_cpp_server::rpcData* request, ::app_cpp_server::rpcResponse* response);
    grpc::Status notification(::grpc::ServerContext* context, const ::app_cpp_server::rpcId* request,
            ::grpc::ServerWriter< ::app_cpp_server::rpcData>* writer);
};

#endif /* APPTOCPP_SERVER_H */

