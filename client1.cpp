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
int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0, udp_sock = 0, broadcastOn = 1, broadcast_msg_len;
    char str[INET_ADDRSTRLEN];
    char recvBuff[1024];
    char *broadcast = "192.168.124.255", *broadcast_msg = "Hi";
    struct sockaddr_in serv_addr, remote_addr;
    broadcast_msg_len = strlen(broadcast_msg);
    /*if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    }*/

    bzero((char *) recvBuff, sizeof(recvBuff));

    if((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("\n Error : Could not create udp socket \n");
		return 1;
	}
    if(setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, &broadcastOn, 4) == -1)
        perror("Error: setsockopt call failed");
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(8080);
    remote_addr.sin_addr.s_addr = inet_addr(broadcast);

    if(sendto(udp_sock, broadcast_msg, broadcast_msg_len, 0, (struct sockaddr *) &remote_addr, sizeof(remote_addr)) == -1)
    {
        perror("Error: sendto call failed");
    }
    std::cout << "done" << std::endl;
    unsigned int serverlen = sizeof(serv_addr);
    if(recvfrom(udp_sock, recvBuff, 1024, 0, (struct sockaddr *) &serv_addr, &serverlen) == -1)
         perror("Error: recvfrom call failed");
    recvBuff[serverlen] = 0;
    std::cout << recvBuff << std::endl;
    inet_ntop(AF_INET, &(serv_addr.sin_addr), str, INET_ADDRSTRLEN);
    printf("%s\n", str);
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create TCP socket \n");
        return 1;
    }

    /*bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }*/



    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }


    int i = 0;
    while(i == 0)
    {
        std::string s = "{\"bedroom\":{\"fans\":{\"fan2\":1}}}";
        char *a = new char[s.size()];
        strcpy(a, s.c_str());
        write(sockfd, a, strlen(a));
        i++;
    };

    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }


    }

    if(n < 0)
    {
        printf("\n Read error \n");
    }

    return 0;
}
