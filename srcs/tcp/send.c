#include "../../lib/nmap.h"

int send_packet_tcp(int sockfd, char *packet, struct sockaddr_in addr)
{
    ssize_t sent;
    ssize_t payload_size = (sizeof(struct iphdr) + sizeof(struct tcphdr));

    sent = sendto(sockfd,packet, payload_size ,0,(struct sockaddr *)&addr,sizeof(addr));

    if (sent < 0)
    {
        if (DEBUG)
            printf("Error : TCP Sending to port\n");
        return -1;
    }
    return sent;
}