#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <string>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <memory>
#include <grpc++/grpc++.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "relay_server.grpc.pb.h"
#endif

class Appliance {
public:

    enum class State {
        OFF = 0,
        ON = 1
    };
protected:
    std::string name; //name of the appliance
    State state; //state of the appliance

public:

    void setName(std::string s);
    void setState(State n);
    std::string getName();
    int getState();
    Appliance(std::string s, State n);
};

class Light : public Appliance {
public:
    Light(std::string s = "abc", Light::State n = State::OFF);
};

class Fan : public Appliance {
public:
    Fan(std::string s = "abc", Fan::State n = State::OFF);
};

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

class Room {
private:
    std::string room_name;
    std::vector<Light> light;
    std::vector<Fan> fan;
public:

    enum class State {
        OFF = 0,
        ON = 1
    };
    Room(const std::string room_Name);
    std::string getRoomName();
    int getLightState(std::string name);
    int getFanState(std::string name);
    void setRoomName(std::string name);
    void addLight(std::string name, int state = 0);
    void addFan(std::string name, int state = 0);
    void setFanState(std::string name, int state);
    void setLightState(std::string name, int state);
};

class Data {
private:
    std::vector<Room> room;
public:
    Data();
    void addRoom(std::string name);
    Room& getRoom(std::string name);
    int read_from_config(Json::Value& json_data);
    //friend std::ostream& operator<< (std::ostream &out, const Data &data);
};

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

class RelayServerClient {
public:

    RelayServerClient(std::shared_ptr<grpc::Channel> channel);

    // Assembles the client's payload, sends it and presents the response back
    // from the server.

    int get_state(const std::string& room_name, const std::string& appliance_type, const std::string& appliance_name);

    int set_state(const std::string& room_name, const std::string& appliance_type, const std::string& appliance_name, int state);

private:
    std::unique_ptr<py_server::RelayServer::Stub> stub_;
};

class Activity {
private:
    fd_set readfds; 
public:
    Activity();
    void set_active_descriptors(const int& udp_descriptor,const int& tcp_descriptor, ClientData& client_data);
    void wait_for_activity(ClientData& client_data);
    void check_udp_activity_and_respond(UDPServer& udp);
    void check_tcp_activity_and_respond(TCPServer& tcp, ClientData& client_data, Data& data, const Json::Value json_data);
    int check_client_activity_update_and_respond(ClientData& client_data, Data& data, Json::Value& json_data, int client_sd);  
};



#endif
