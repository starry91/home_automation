#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <string>
#include <json/json.h>
#include <vector>
#include "Classes.h"

char* get_host_addr();

std::string &ltrim(std::string &s);

std::string &rtrim(std::string &s);

std::string trim(const std::string &s);

void error(const char *msg);

int read_from_config(Data& data, Json::Value& json_data);

void set_active_descriptors(fd_set& fd_set, const int& udp_descriptor,
		const int& tcp_descriptor, ClientData& client_data);

void wait_for_activity(ClientData& client_data, fd_set& readfds);

void check_udp_activity_and_respond(UDPServer& udp, const fd_set& readfds);

void check_tcp_activity_and_respond(TCPServer& tcp, ClientData& client_data, Data& data, const Json::Value json_data, const fd_set& readfds);

int check_client_activity_update_and_respond(fd_set& readfds, ClientData& client_data, Data& data, Json::Value& json_data, int client_sd);

int connect_to_py_server();

int get_state(Json::Value data, int py_sd);

int set_state(Json::Value data, int py_sd);

#endif
