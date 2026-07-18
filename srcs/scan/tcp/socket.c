#include "../../../lib/nmap.h"

int socket_connection_tcp(struct sockaddr_in addr)
{
    (void) addr;
    int sockfd = 0;
    int activate = 1;

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

    if (sockfd < 0)
        return -1;

    if ( setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &activate, sizeof(activate)) < 0)
    {
        close(sockfd);
        return -1;
    }

    return sockfd;
}
