#include <sys/socket.h>
#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string>

#include <grpc++/grpc++.h>
#include "proto/AppToCppServer.grpc.pb.h"
#include <Room.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using app_cpp_server::rpcData;
using app_cpp_server::rpcHouse;
using app_cpp_server::rpcRoom;
using app_cpp_server::rpcAppliance;

class Client {
private:
    std::unique_ptr<app_cpp_server::AppCppServer::Stub> stub_;
public:

    Client(std::shared_ptr<grpc::Channel> channel) : stub_(app_cpp_server::AppCppServer::NewStub(channel)) {
    }

    rpcHouse get_all(const std::string id) {
        // Data we are sending to the server.
        app_cpp_server::rpcId request;
        request.set_id(id);

        // Container for the data we expect from the server.
        app_cpp_server::rpcHouse reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        grpc::ClientContext context;

        // The actual RPC.
        grpc::Status status = stub_->get_all(&context, request, &reply);
        // Act upon its status.
            return reply;
    }
};

int main(int argc, char *argv[]) {
//    int sockfd = 0, n = 0, udp_sock = 0, broadcastOn = 1, broadcast_msg_len;
//    char recvBuff[1024];
//    char *broadcast = "192.168.0.255", *broadcast_msg = "Hi";
//    struct sockaddr_in serv_addr, remote_addr;
//    broadcast_msg_len = strlen(broadcast_msg);
//    /*if(argc != 2)
//    {
//        printf("\n Usage: %s <ip of server> \n",argv[0]);
//        return 1;
//    }*/
//
//    bzero((char *) recvBuff, sizeof (recvBuff));
//
//    if ((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
//        printf("\n Error : Could not create udp socket \n");
//        return 1;
//    }
//    if (setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, &broadcastOn, 4) == -1)
//        perror("Error: setsockopt call failed");
//    remote_addr.sin_family = AF_INET;
//    remote_addr.sin_port = htons(8089);
//    remote_addr.sin_addr.s_addr = inet_addr(broadcast);
//
//    if (sendto(udp_sock, broadcast_msg, broadcast_msg_len, 0, (struct sockaddr *) &remote_addr, sizeof (remote_addr)) == -1) {
//        perror("Error: sendto call failed");
//    }
//    std::cout << "done" << std::endl;
//    unsigned int serverlen = sizeof (serv_addr);
//    if (recvfrom(udp_sock, recvBuff, 1024, 0, (struct sockaddr *) &serv_addr, &serverlen) == -1)
//        perror("Error: recvfrom call failed");
//
//    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//        printf("\n Error : Could not create TCP socket \n");
//        return 1;
//    }
//    std::cout << inet_ntoa(serv_addr.sin_addr) << std::endl;

    Client greeter(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    std::string user("192.168.0.103");
    rpcHouse reply = greeter.get_all(user);
    for(int i = 0; i < reply.room_size(); i++) {
        rpcRoom room = reply.room(i);
        for(int j = 0; j < room.appliance_size(); j++) {
           rpcAppliance app = room.appliance(j);
           std::cout << room.room_name() << std::endl;
           std::cout << app.app_name() << std::endl;
           std::cout << app.app_type() << std::endl;
           std::cout << app.app_state() << std::endl;
        }
        std::cout << std::endl;
    }
    

    return 0;
}
