/* A simple server in the internet domain usingog TCP
 *  The port number is passed as an argument */
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
#include "Classes.h"
#include <json/json.h>
#include <fstream>
#include "functions.h"

using namespace std;

int main(int argc, char *argv[]) {

	/*
	int master_socket, new_socket, portno, client_socket[30], max_clients = 30,
			i, max_sd, sd, activity, udp_sock;
	socklen_t clilen;
	char read_buf[256], udp_buf[256];
	fd_set readfds;
	struct sockaddr_in serv_addr, cli_addr;
	int n = 1; //light_index = -1, fan_index = -1 ;
	vector<Fan> fan;
	vector<Light> light;
	Json::Value data, sig_change, config;
	Json::FastWriter stream;
	Json::Reader reader;
	ifstream input;
	stringstream buff;
	input.open("server_config.conf");
    buff << input.rdbuf();
    string js = buff.str();
    //cout << js <<endl;
    input.close();
    reader.parse(js, config);
    cout << config << endl;
    clilen = sizeof(cli_addr);
	*/
	fd_set readfds;
	Data data;
    if(read_from_config(data) < 0)
    	std::cout << "error" << std::endl;

    /*
    Json::ValueIterator it = config.begin();
	clilen = sizeof(cli_addr);
    //
    //connect to aurdino

    for(it; it != config.end(); it++)
    {
    	Json::Value a = *it;
    	//cout << a.isObject() << endl;

    	Json::ValueIterator it1 = a.begin();
    	//cout << it1.key().asString() << endl;
    	for(it1; it1 != a.end(); it1++)
    	{
    		if((it1.key()).asString() == "lights")
    		{
    			Json::Value b = *it1;
    			Json::ValueIterator it2 = b.begin();
    			for(it2; it2!= b.end(); it2++)
    			{

    				light.push_back(Light());
    				light_index++;
    				int light_state = 0; //get from aurdino
    				data[(it.key()).asString()][(*it2).asString()] = light_state;
    				light[light_index].setState(Appliance::State(light_state));
    				light[light_index].setName((*it2).asString());
    				//delete[] l;
    			}
    		}
    		else if((it1.key()).asString() == "fans")
    		{
    			Json::Value b = *it1;
    			//cout << "list" << b.isArray() << endl;
    			Json::ValueIterator it2 = b.begin();
    			for(it2; it2!= b.end(); it2++)
    			{
    				//cout << *it2 << endl;
    				fan.push_back(Fan());
    				fan_index++;
    				int fan_state = 0; //get from aurdino
    				data[(it.key()).asString()][(*it2).asString()] = fan_state;
    				fan[fan_index].setState(Appliance::State(fan_state));
    				fan[fan_index].setName((*it2).asString());
    			}
    		}
    		else
    		{
    			cout << "Input to data error" <<endl;//throw error
    		}
    	}
    }
    */
    /*
    if(read_from_config(config, data, light, fan) < 0)
    	std::cout << "error" << std::endl;

    cout << "original data: \n" << data << endl;

	for (i = 0; i < max_clients; i++){
		client_socket[i] = 0;
	}
	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	*/
    UDPServer udp(8080);
    TCPServer tcp(8080);

    udp.UDPListen();
    tcp.TCPListen();



	/*
	master_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(master_socket == -1)
	{
		perror("Erro: server socket not created");
	}

	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &n,sizeof(n))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	if (master_socket < 0)
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(master_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr))
			< 0)
		error("ERROR on binding TCP socket");
	listen(master_socket, 5);
	*/

	//Get host ipV4 addr
	char *host_ip = get_host_addr();

	//Set variables for UDP connection

	/*int udp_sock;
	char udp_buf[256];
	udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(udp_sock == -1)
	{
		perror("Error: server udp socket not created");
	}
	bzero((char *) &cli_addr, sizeof(cli_addr));
	if (bind(udp_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))
				< 0)
			error("ERROR on binding UDP socket");
	*/
	while (1)
	{
		FD_ZERO(&readfds);
		FD_SET(udp_sock, &readfds);
		max_sd = udp_sock;
		FD_SET(master_socket, &readfds);
		max_sd = master_socket > max_sd ? master_socket : max_sd;
		for (i = 0; i < max_clients; i++)
		{
			//cout << "fdisset of sd" <<endl;
			sd = client_socket[i];
			//cout << "sd: " << sd <<endl;
			if (sd > 0)
				FD_SET(sd, &readfds);
			if (sd > max_sd)
				max_sd = sd;
		}

		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
		if ((activity < 0) && (errno != EINTR))
		{
			cout << "select error" << endl;
		}

		if (FD_ISSET(udp_sock, &readfds))
		{
			cout << "heard something" <<endl;
			int len;
			if((len = recvfrom(udp_sock, udp_buf, sizeof(udp_buf), 0,
			          (struct sockaddr*) &cli_addr, &clilen)) <= 0)
			{
				perror("Error: recvfrom call failed");
			}
			udp_buf[len] = '\0';
			string temp(udp_buf);
			cout << temp <<endl;
			if(trim(temp) == "Hi")
			{
				if(sendto(udp_sock, host_ip, strlen(host_ip), 0, (struct sockaddr*) &cli_addr, sizeof(cli_addr)) == -1)
				{
					perror("Error: sendto call failed");
				}
			}
		}
		if (FD_ISSET(master_socket, &readfds))
		{
			cout << "isset: mastersoc" <<endl;
			if ((new_socket = accept(master_socket,(struct sockaddr *) &cli_addr, &clilen)) < 0)
			{
				cout << "new soc error" <<endl;//write the linux error code here
			}
			for (i = 0; i < max_clients; i++)
			{
				//if position is empty
				if (client_socket[i] == 0)
				{
					cout << "setting clientsoc" <<endl;
					client_socket[i] = new_socket;
					cout<< "csocket: " << client_socket[i]<<endl;
					max_sd = new_socket;
					string s = stream.write(data);
					char *write_buf = new char[s.size()];
					strcpy(write_buf, s.c_str());
					write(client_socket[i], write_buf, strlen(write_buf));
					delete write_buf;
					break;
				}
			}

		}
		cout << "max sd: " << max_sd << endl;
		for (i = 0; i <= max_sd; i++)
		{
			cout << "final loop starting" <<endl;
			sd = client_socket[i];
			cout << "test" <<endl;
			if (FD_ISSET(sd, &readfds))
			{
				cout << "fdisset" <<endl;
				n = read(sd, read_buf, 256);
				string r(read_buf);
				if (n == 0)
				{
					close(sd);
					client_socket[i] = 0;
				}
				else if(reader.parse(trim(r), sig_change))
				{
					cout << "sig change" <<endl;
					Json::Value::iterator it_data = data.begin();
					Json::Value::iterator it_sig_change = sig_change.begin();
					while(1)
					{

					}
					while(it_data != data.end())
					{
						if((it_sig_change.key()).asString() == (it_data.key()).asString())
						{
							Json::Value sig_change_temp = *it_sig_change;
							Json::Value data_temp = *it_data;

							Json::Value::iterator it_data_temp = data_temp.begin();
							Json::Value::iterator it_sig_change_temp = sig_change_temp.begin();
							while(it_data_temp != data_temp.end())
							{
								if((it_sig_change_temp.key()).asString() == (it_data_temp.key()).asString())
								{
									data[it_data.key().asString()][(it_data_temp.key()).asString()] = (*it_sig_change_temp).asInt();
									int a = (*it_sig_change_temp).asInt();

									for(int i = 0; i < max(fan.size(), light.size()) ; i++)
									{
										if(i < light.size() && (((it_sig_change_temp.key()).asString()) == light[i].getName()))
										{
											light[i].setState(Appliance::State(a));
											cout << light[i].getState() <<endl;
											//aurdino signal
										}
										if(i < fan.size() && (((it_sig_change_temp.key()).asString()) == fan[i].getName()))
										{
											fan[i].setState(Appliance::State(a));
											cout << fan[i].getState() <<endl;
										}
									}

									break;
								}
								it_data_temp++;
							}
							break;
						}
						it_data++;
					}
					string s = stream.write(data);
					char *write_buf_others = new char[s.size()];
					strcpy(write_buf_others, s.c_str());
					for(int j = 0; j <= max_sd; j++)
					{
						if(j != i)
						{
							write(client_socket[j], write_buf_others, strlen(write_buf_others));
						}
					}
					delete write_buf_others;
				}
				else
				{
					cout << "read mismatch" << endl;//throw error;
				}
				cout << "data updated: \n" << data << endl;
			}
			else
			{
				cout << "isset: sd error" <<endl;
			}

		}
	}

	return 0;
}




/*
new_socket = accept(master_socket, (struct sockaddr *) &cli_addr, &clilen);
if (new_socket < 0)
	error("ERROR on accept");
while ((n = read(new_socket, buffer, 255)) >= 0) {
	string s;
	buffer[n] = '\0';
	s = trim(string(buffer));
	if (s == "abort")
		break;
	cout << s << endl;
}
if (n < 0)
	error("ERROR reading from socket");
n = write(new_socket, "Conversation ends\n", 19);
if (n < 0)
	error("ERROR writing to socket");
close (newmaster_socket);
close(master_socket);
return 0;
}*/
