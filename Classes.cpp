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

int Data::read_from_config(Json::Value& json_data) {
    Json::Value config;
    Json::FastWriter stream;
    Json::Reader reader;
    std::ifstream input;
    std::stringstream buff;
    RelayServerClient RelayServer(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    input.open("server_config.conf");
    buff << input.rdbuf();
    std::string js = buff.str();
    //cout << js <<endl;
    input.close();
    reader.parse(js, config);
    std::cout << config << std::endl;
    Json::ValueIterator it = config.begin();
    for (it; it != config.end(); it++) {
        std::string room_name = it.key().asString();
        addRoom(room_name);
        Room& room = getRoom(room_name);
        Json::Value a = *it;
        Json::ValueIterator it1 = a.begin();
        for (it1; it1 != a.end(); it1++) {
            std::string appliance_type = it1.key().asString();
            if (appliance_type == "lights") {
                Json::Value light_arr = *it1;
                Json::ValueIterator it2 = light_arr.begin();
                for (it2; it2 != light_arr.end(); it2++) {
                    //std::string name = it2.key().asString();
                    Json::Value temp_data;
                    std::string name = (*it2).asString();
                    temp_data[room_name]["lights"] = name;
                    int light_state = RelayServer.get_state(room_name, appliance_type, name);
                    std::cout << "received light state" << std::endl;
                    room.addLight(name);
                    room.setLightState(name, light_state); //get from aurdino
                    json_data[room_name]["lights"][name] = light_state; //get from aurdino
                }
            } else if (appliance_type == "fans") {
                Json::Value fan_arr = *it1;
                Json::ValueIterator it2 = fan_arr.begin();
                for (it2; it2 != fan_arr.end(); it2++) {
                    Json::Value temp_data;
                    //std::string name = it2.key().asString();
                    std::string name = (*it2).asString();
                    temp_data[room_name]["fans"] = name;
                    int fan_state = RelayServer.get_state(room_name, appliance_type, name);
                    std::cout << "received fan state" << std::endl;
                    room.addFan(name);
                    room.setFanState(name, fan_state); //get from aurdino
                    json_data[room_name]["fans"][name] = fan_state; //get from aurdino
                }
            } else {
                std::cout << "Input to data error" << std::endl; //throw error
                return -1;
            }
        }
    }
    return 0;    
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

Activity::Activity(){};

Activity::set_active_descriptors(const int& udp_descriptor, const int& tcp_descriptor, ClientData& client_data) {
    int max_sd;
    FD_ZERO(&readfds);
    FD_SET(udp_descriptor, &readfds);
    max_sd = udp_descriptor;
    FD_SET(tcp_descriptor, &readfds);
    max_sd = tcp_descriptor > max_sd ? tcp_descriptor : max_sd;
    for (int i = 0; i < client_data.getClientListSize(); i++) {
        Client& client = client_data.getClient(i);
        int sd = client.getSocketDescriptor();
        //cout << "sd: " << sd <<endl;
        if (sd > 0)
            FD_SET(sd, &readfds);
        if (sd > max_sd)
            max_sd = sd;
    }
    client_data.setMaxDescriptor(max_sd);
}

Activity::wait_for_activity(ClientData& client_data) {
    int activity = select(client_data.getMaxDescriptor() + 1, &readfds, NULL, NULL, NULL);
    if ((activity < 0) && (errno != EINTR)) {
        std::cout << "select error" << std::endl;
    }    
}

Activity::check_udp_activity_and_respond(UDPServer& udp) {
    if (FD_ISSET(udp.getSockDescriptor(), &readfds)) {
        int len;
        char *udp_buf = udp.readServBuff();
        struct sockaddr_in cli_addr;
        socklen_t clilen;
        if ((len = recvfrom(udp.getSockDescriptor(), udp_buf, sizeof (udp_buf), 0,
                (struct sockaddr*) &cli_addr, &clilen)) <= 0) {
            perror("Error: recvfrom call failed");
        }
        udp_buf[len] = '\0';
        std::string temp(udp_buf);
        std::cout << temp << std::endl;
        if (trim(temp) == "Hi") {
            if (sendto(udp.getSockDescriptor(), (char*) "xyz", 3, 0, (struct sockaddr*) &cli_addr, sizeof (cli_addr)) == -1) {
                perror("Error: sendto call failed");
            }
        }
    }    
}

Activity::check_tcp_activity_and_respond(TCPServer& tcp, ClientData& client_data, Data& data, const Json::Value json_data) {
    int tcp_sd = tcp.getSockDescriptor();
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    int new_socket;
    Json::FastWriter stream;
    Json::Reader reader;

    if (FD_ISSET(tcp_sd, &readfds)) {
        std::cout << "isset: mastersoc" << std::endl;
        if ((new_socket = accept(tcp_sd, (struct sockaddr *) &cli_addr, &clilen)) < 0) {
            std::cout << "new soc error" << std::endl; //write the linux error code here
        }
        client_data.addClient(new_socket, cli_addr);
        client_data.setMaxDescriptor(new_socket);
        std::string s = stream.write(json_data);
        char *write_buf = new char[s.size()];
        strcpy(write_buf, s.c_str());
        write(new_socket, write_buf, strlen(write_buf));
        delete write_buf;
    }    
}

int Activity::check_client_activity_update_and_respond(ClientData& client_data, Data& data, Json::Value& json_data, int client_sd) {
    Json::Reader reader;
    Json::Value sig_change;
    Json::FastWriter stream;
    RelayServerClient RelayServer(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    if (FD_ISSET(client_sd, &readfds)) {
        char read_buf[256];
        std::cout << "fdisset" << std::endl;
        int n = read(client_sd, read_buf, 256);
        std::string r(read_buf);
        if (n == 0) {
            close(client_sd);
            client_data.removeClient(client_sd);
        } else if (reader.parse(trim(r), sig_change)) {
            std::cout << sig_change << std::endl;
            Json::Value::iterator it;
            Json::Value it_data;
            it = sig_change.begin();
            it_data = *it;

            std::cout << it_data << std::endl;
            std::string room_name = it.key().asString();
            std::cout << room_name << std::endl;
            Room& room = data.getRoom(room_name);
            it = it_data.begin();
            std::string appliance_type = it.key().asString();
            std::cout << "App type: " << appliance_type << std::endl;

            it_data = *it;
            std::cout << it_data << std::endl;

            it = it_data.begin();
            std::string appliance_name = it.key().asString();
            std::cout << appliance_name << std::endl;
            int state = (*it).asInt();
            if (appliance_type == "lights") {
                room.setLightState(appliance_name, state);
                json_data[room_name]["lights"][appliance_name] = state;
            } else if (appliance_type == "fans") {
                room.setFanState(appliance_name, state);
                json_data[room_name]["fans"][appliance_name] = state;
            } else {
                std::cout << "error" << std::endl;
            }
            int status = RelayServer.set_state(room_name, appliance_type, appliance_name, state);
            std::cout << "get status: " << status << std::endl;
            std::cout << json_data << std::endl;
            std::string s = stream.write(json_data);
            char *write_buf_others = new char[s.size()];
            strcpy(write_buf_others, s.c_str());
            for (int j = 0; j <= client_data.getClientListSize(); j++) {
                Client& client = client_data.getClient(j);
                int sd = client.getSocketDescriptor();
                //if(sd != client_sd)
                {
                    write(sd, write_buf_others, strlen(write_buf_others));
                }
            }
            delete write_buf_others;
        } else {
            std::cout << "read mismatch" << std::endl; //throw error;
            return 2;
        }
        std::cout << "data updated: \n" << std::endl;
    }
    return 0;        
}



