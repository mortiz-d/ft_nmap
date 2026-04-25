#include "../lib/nmap.h"


int socket_connection(t_params *params)
{
    (void)params;
    struct timeval tv_timeout;
    int sockfd = 0;
    int activate = 1;

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

    if (sockfd < 0)
    {
        return -1;
    }

    
	tv_timeout.tv_sec = TIMEOUT_MS / 1000;
    tv_timeout.tv_usec = (TIMEOUT_MS % 1000) * 1000;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv_timeout, sizeof(tv_timeout)) < 0)
    {
        close(sockfd);
        return -1;
    }
    
    if ( setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &activate, sizeof(activate)) < 0)
    {
        close(sockfd);
        return -1;
    }

    return sockfd;
}