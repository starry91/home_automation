#include <stdio.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <string.h> /* For strncpy */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <string>
#include <algorithm>
#include <json/json.h>
#include <fstream>
#include <sstream>
#include <sys/select.h>
#include "functions.h"


char* get_host_addr()
{
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;

    /* I want an IP address attached to "eth0" */
    strncpy(ifr.ifr_name, "wlp8s0", IFNAMSIZ-1);

    ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);

    /* Display result */
    //printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

    return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr) ;
}

std::string &ltrim(std::string &s)
{
	s.erase(s.begin(),
			find_if_not(s.begin(), s.end(), [](int c) {return isspace(c);}));
	return s;
}

std::string &rtrim(std::string &s)
{
	s.erase(
			find_if_not(s.rbegin(), s.rend(), [](int c) {return isspace(c);}).base(),
			s.end());
	return s;
}

std::string trim(const std::string &s) {
	std::string t = s;
	return ltrim(rtrim(t));
}

void error(const char *msg) {
	perror(msg);
	exit(1);
}

int get_state(Json::Value data, int py_sd)
{
	Json::FastWriter stream;
	char buffer[10];
	int len;
	std::string s = stream.write(data);
	std::cout << trim(s) << std::endl;
	char *write_buf = new char[s.size()];
	strcpy(write_buf, trim(s).c_str());
	write(py_sd, (char*)"get", std::string("get").length());
	write(py_sd, write_buf, strlen(write_buf));
	if((len = recvfrom(py_sd, buffer, sizeof(buffer), 0, NULL, NULL)) <= 0)
	{
		perror("Error: recvfrom call failed");
	}
	buffer[len] = 0;
	return atoi(buffer);
}

int set_state(Json::Value data, int py_sd)
{
	Json::FastWriter stream;
	int len;
	std::string s = stream.write(data);
	char *write_buf = new char[s.size()];
	strcpy(write_buf, s.c_str());
	;
	if(write(py_sd, write_buf, strlen(write_buf)) <= 0)
	{
		perror("Error: write call failed");
	}
	return 0;
}


int read_from_config(Data& data, Json::Value& json_data, int py_sd)
{
	Json::Value config;
	Json::FastWriter stream;
	Json::Reader reader;
	std::ifstream input;
	std::stringstream buff;
	input.open("server_config.conf");
    buff << input.rdbuf();
    std::string js = buff.str();
    //cout << js <<endl;
    input.close();
    reader.parse(js, config);
    std::cout << config << std::endl;
	Json::ValueIterator it = config.begin();
	for(it; it != config.end(); it++)
	{
		std::string room_name = it.key().asString();
		data.addRoom(room_name);
		Room& room = data.getRoom(room_name);
		Json::Value a = *it;
		Json::ValueIterator it1 = a.begin();
		for(it1; it1 != a.end(); it1++)
		{
			std::string appliance_type = it1.key().asString();
			if(appliance_type == "lights")
			{
				Json::Value light_arr = *it1;
				Json::ValueIterator it2 = light_arr.begin();
				for(it2; it2!= light_arr.end(); it2++)
				{
					//std::string name = it2.key().asString();
					Json::Value temp_data;
					std::string name = (*it2).asString();
					temp_data[room_name]["lights"] = name;
					std::cout << "l" << std::endl;
					int light_state = get_state(temp_data, py_sd);
					std::cout << "received light state" << std::endl;
					room.addLight(name);
					room.setLightState(name, light_state); //get from aurdino
					json_data[room_name]["lights"][name] = light_state; //get from aurdino
				}
			}
			else if(appliance_type == "fans")
			{
				Json::Value fan_arr = *it1;
				Json::ValueIterator it2 = fan_arr.begin();
				for(it2; it2!= fan_arr.end(); it2++)
				{
					Json::Value temp_data;
					//std::string name = it2.key().asString();
					std::string name = (*it2).asString();
					temp_data[room_name]["fans"] = name;
					std::cout << "f" << std::endl;
					int fan_state = get_state(temp_data, py_sd);
					std::cout << "received fan state" << std::endl;
					room.addFan(name);
					room.setFanState(name, fan_state); //get from aurdino
					json_data[room_name]["fans"][name] = fan_state; //get from aurdino
				}
			}
			else
			{
				std::cout << "Input to data error" << std::endl;//throw error
				return -1;
			}
		}
	}
	return 0;
}

void set_active_descriptors(fd_set& fd_set, const int& udp_descriptor,
				const int& tcp_descriptor, ClientData& client_data)
{
	int max_sd;
	FD_ZERO(&fd_set);
	FD_SET(udp_descriptor, &fd_set);
	max_sd = udp_descriptor;
	FD_SET(tcp_descriptor, &fd_set);
	max_sd =  tcp_descriptor > max_sd ? tcp_descriptor : max_sd;
	for (int i = 0; i < client_data.getClientListSize(); i++)
	{
		Client& client = client_data.getClient(i);
		int sd = client.getSocketDescriptor();
		//cout << "sd: " << sd <<endl;
		if (sd > 0)
			FD_SET(sd, &fd_set);
		if (sd > max_sd)
			max_sd = sd;
	}
	client_data.setMaxDescriptor(max_sd);
}


void wait_for_activity(ClientData& client_data, fd_set& readfds) {
	int activity = select(client_data.getMaxDescriptor() + 1, &readfds, NULL, NULL, NULL);
	if ((activity < 0) && (errno != EINTR))
	{
		std::cout << "select error" << std::endl;
	}
}

void check_udp_activity_and_respond(UDPServer& udp, const fd_set& readfds) {
	if (FD_ISSET(udp.getSockDescriptor(), &readfds))
	{

		int len;
		char *udp_buf = udp.readServBuff();
		struct sockaddr_in cli_addr;
		socklen_t clilen;
		if((len = recvfrom(udp.getSockDescriptor(), udp_buf, sizeof(udp_buf), 0,
				  (struct sockaddr*) &cli_addr, &clilen)) <= 0)
		{
			perror("Error: recvfrom call failed");
		}
		udp_buf[len] = '\0';
		std::string temp(udp_buf);
		std::cout << temp << std::endl;
		if(trim(temp) == "Hi")
		{
			if(sendto(udp.getSockDescriptor(), (char*)"xyz", 3, 0, (struct sockaddr*) &cli_addr, sizeof(cli_addr)) == -1)
			{
				perror("Error: sendto call failed");
			}
		}
	}
}

void check_tcp_activity_and_respond(TCPServer& tcp, ClientData& client_data, Data& data, const Json::Value json_data, const fd_set& readfds) {
	int tcp_sd = tcp.getSockDescriptor();
	struct sockaddr_in cli_addr;
	socklen_t clilen;
	int new_socket;
	Json::FastWriter stream;
	Json::Reader reader;

	if (FD_ISSET(tcp_sd, &readfds))
	{
		std::cout << "isset: mastersoc" << std::endl;
		if ((new_socket = accept(tcp_sd,(struct sockaddr *) &cli_addr, &clilen)) < 0)
		{
			std::cout << "new soc error" << std::endl;//write the linux error code here
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

int check_client_activity_update_and_respond(fd_set& readfds, ClientData& client_data, Data& data, Json::Value& json_data, int client_sd, int py_sd) {
	Json::Reader reader;
	Json::Value sig_change;
	Json::FastWriter stream;
	if (FD_ISSET(client_sd, &readfds))
	{
		char read_buf[256];
		std::cout << "fdisset" << std::endl;
		int n = read(client_sd, read_buf, 256);
		std::string r(read_buf);
		if (n == 0)
		{
			close(client_sd);
			client_data.removeClient(client_sd);
		}
		else if(reader.parse(trim(r), sig_change))
		{
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
			if(appliance_type == "lights")
			{
				room.setLightState(appliance_name, state);
				json_data[room_name]["lights"][appliance_name] = state;
			}
			else if(appliance_type == "fans")
			{
				room.setFanState(appliance_name, state);
				json_data[room_name]["fans"][appliance_name] = state;
			}
			else
			{
				std::cout << "error" << std::endl;
			}
			set_state(sig_change, py_sd);
			std::cout << json_data << std::endl;
			std::string s = stream.write(json_data);
			char *write_buf_others = new char[s.size()];
			strcpy(write_buf_others, s.c_str());
			for(int j = 0; j <= client_data.getClientListSize(); j++)
			{
				Client& client = client_data.getClient(j);
				int sd = client.getSocketDescriptor();
				//if(sd != client_sd)
				{
					write(sd, write_buf_others, strlen(write_buf_others));
				}
			}
			delete write_buf_others;
		}
		else
		{
			std::cout << "read mismatch" << std::endl;//throw error;
			return 2;
		}
		std::cout << "data updated: \n" << std::endl;
	}
	return 0;
}

int connect_to_py_server()
{
	int sockfd = 0, n = 0;
	char recvBuff[1024];
	struct sockaddr_in serv_addr;

	bzero((char *) recvBuff, sizeof(recvBuff));
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Error : Could not create socket \n");
		return 1;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8082);

	if(inet_pton(AF_INET, "127.0.0.1" , &serv_addr.sin_addr)<=0)
	{
		printf("\n inet_pton error occured\n");
		return 1;
	}

	if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
	   printf("\n Error : Connect Failed \n");
	   return 1;
	}

	return sockfd;
}

