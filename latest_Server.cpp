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
    fd_set readfds;
    ClientData client_data;
    Data data;
    Json::Value json_data;
    if (read_from_config(data, json_data) < 0)
        std::cout << "error" << std::endl;
    cout << json_data << endl;
    UDPServer udp(8080);
    TCPServer tcp(8080);

    udp.UDPListen();
    tcp.TCPListen();
    int udp_sd = udp.getSockDescriptor();
    int tcp_sd = tcp.getSockDescriptor();

    //char *host_ip = get_host_addr();

    while (1) {
        set_active_descriptors(readfds, udp_sd, tcp_sd, client_data);

        wait_for_activity(client_data, readfds);

        check_udp_activity_and_respond(udp, readfds);

        check_tcp_activity_and_respond(tcp, client_data, data, json_data, readfds);

        for (int i = 0; i < client_data.getClientListSize(); i++) {
            cout << "final loop starting" << endl;
            Client& client = client_data.getClient(i);
            int sd = client.getSocketDescriptor();
            cout << "test" << endl;
            if (FD_ISSET(sd, &readfds)) {
                int status = check_client_activity_update_and_respond(readfds, client_data, data, json_data, sd);
                if (status > 0)
                    cout << "error" << endl;
            } else {
                cout << "isset: sd error" << endl;
            }
        }
    }
    return 0;
}
