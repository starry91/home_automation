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
#include "Activity.h"
#include "Client.h"
#include "ClientData.h"
#include "Data.h"
#include "RelayServerClient.h"
#include "Room.h"
#include "Server.h"

int main(int argc, char *argv[]) {
    ClientData client_data;
    Data data;
    Json::Value json_data;
    Activity activity;
    if (data.read_from_config(json_data) < 0)
        std::cout << "error" << std::endl;
    std::cout << json_data << std::endl;
    UDPServer udp(8080);
    TCPServer tcp(8080);

    udp.UDPListen();
    tcp.TCPListen();
    int udp_sd = udp.getSockDescriptor();
    int tcp_sd = tcp.getSockDescriptor();

    //char *host_ip = get_host_addr();

    while (1) {
        activity.set_active_descriptors(udp_sd, tcp_sd, client_data);

        activity.wait_for_activity(client_data);

        activity.check_udp_activity_and_respond(udp);

        activity.check_tcp_activity_and_respond(tcp, client_data, data, json_data);

        for (int i = 0; i < client_data.getClientListSize(); i++) {
            std::cout << "final loop starting" << std::endl;
            Client& client = client_data.getClient(i);
            int sd = client.getSocketDescriptor();
            std::cout << "test" << std::endl;
            if (activity.check_client_activity_update_and_respond(client_data, data, json_data, sd) > 0)
                std::cout << "error" << std::endl;
        }
    }
    return 0;
}
