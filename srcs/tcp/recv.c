#include "../../lib/nmap.h"

ssize_t recv_packet(int sockfd, char *buffer)
{
    ssize_t len;
    struct sockaddr_in src;
    socklen_t src_len = sizeof(src);

    len = recvfrom(sockfd,buffer,4096,0,(struct sockaddr *)&src,&src_len);

    if (len < 0)
    {
        printf("Error : Timeout recv\n");
        return -1;
    }

    if (DEBUG)
        printf("Recv packet\n");

    return len;
}