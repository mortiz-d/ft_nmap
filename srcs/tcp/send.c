#include "../../lib/nmap.h"

int send_packet(int sockfd, char *packet, struct sockaddr_in addr)
{
    ssize_t sent;
    ssize_t payload_size = (sizeof(struct iphdr) + sizeof(struct tcphdr));

    sent = sendto(sockfd,packet, payload_size ,0,(struct sockaddr *)&addr,sizeof(addr));

    if (sent < 0)
    {
        printf("Error : Sending to port\n");
        return -1;
    }
    if (DEBUG)
        printf("Sending packet to port\n");
    return sent;
}