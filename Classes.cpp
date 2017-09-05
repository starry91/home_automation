#include <iostream>
#include <string>
#include <syslog.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "Classes.h"
using namespace std;

void Appliance::setName(string s) {
    name = s;
}

void Appliance::setState(State n) {
    if (Appliance::State::OFF <= n && Appliance::State::ON >= n)
        state = n;
    else {
        throw (string("Invalid State value") + to_string(static_cast<int> (n)));
    }
}

string Appliance::getName() {
    return name;
}

int Appliance::getState() {
    return static_cast<int> (state);
}

Appliance::Appliance(string s, State n) {
    this->name = s;
    this->setState(n);
}

Light::Light(string s, State n) : Appliance(s, n) {
}

Fan::Fan(string s, State n) : Appliance(s, n) {
}

//Base Class

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

Room::Room(const std::string room_Name)
: room_name(room_Name) {
};

string Room::getRoomName() {
    return room_name;
}

int Room::getLightState(string name) {
    for (std::vector<Light>::iterator it = light.begin(); it != light.end(); ++it) {
        Light temp = *it;
        if (temp.getName() == name)
            return temp.getState();
    };
    return -1;
}

int Room::getFanState(string name) {
    for (std::vector<Fan>::iterator it = fan.begin(); it != fan.end(); ++it) {
        Fan temp = *it;
        if (temp.getName() == name)
            return temp.getState();
    }
    return -1;
}

void Room::setRoomName(std::string room_Name) {
    room_name = room_Name;
}

void Room::addLight(std::string name, int state) {
    light.push_back(Light(name, Light::State(state)));
}

void Room::addFan(std::string name, int state) {
    fan.push_back(Fan(name, Fan::State(state)));
}

void Room::setFanState(std::string name, int state) {
    for (std::vector<Fan>::iterator it = fan.begin(); it != fan.end(); ++it) {
        Fan& temp = *it;
        if (temp.getName() == name) {
            temp.setState(Appliance::State(state));
            break;
        }

    }
}

void Room::setLightState(std::string name, int state) {
    for (std::vector<Light>::iterator it = light.begin(); it != light.end(); ++it) {
        Light& temp = *it;
        if (temp.getName() == name) {
            temp.setState(Appliance::State(state));
            break;
        }

    }
}

Data::Data() {
};

void Data::addRoom(std::string name) {
    room.push_back(Room(name));
}

Room& Data::getRoom(std::string name) {
    for (std::vector<Room>::iterator it = room.begin(); it != room.end(); ++it) {
        Room& room = *it;
        if (room.getRoomName() == name)
            return room;
    }
    //throw error;
}

Client::Client(int socket_descriptor, sockaddr_in client_address) {
    sd = socket_descriptor;
    bzero((char *) &client_addr, sizeof (client_addr));
    client_addr = client_address;
}

int Client::getSocketDescriptor() {
    return sd;
}

struct sockaddr_in Client::getAddress() {
    return client_addr;
}

void Client::setSocketDescriptor(int socket_descriptor) {
    sd = socket_descriptor;
}

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

RelayServerClient::RelayServerClient(std::shared_ptr<grpc::Channel> channel)
: stub_(py_server::RelayServer::NewStub(channel)) {
}

int RelayServerClient::get_state(const std::string& room_name, const std::string& appliance_type, const std::string& appliance_name) {
    // Data we are sending to the server.
    py_server::Details request;
    request.set_room_name(room_name);
    request.set_appliance_type(appliance_type);
    request.set_appliance_name(appliance_name);
    // Container for the data we expect from the server.
    py_server::State reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    grpc::ClientContext context;

    // The actual RPC.
    grpc::Status status = stub_->get_state(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
        return reply.value();
    } else {
        std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
        return -1;
    }
}

int RelayServerClient::set_state(const std::string& room_name, const std::string& appliance_type, const std::string& appliance_name, int state) {
    // Data we are sending to the server.
    py_server::Details request;
    request.set_room_name(room_name);
    request.set_appliance_type(appliance_type);
    request.set_appliance_name(appliance_name);
    request.set_state(state);
    // Container for the data we expect from the server.
    py_server::Response reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    grpc::ClientContext context;

    // The actual RPC.
    grpc::Status status = stub_->set_state(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
        return reply.value();
    } else {
        std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
        return -1;
    }
}

