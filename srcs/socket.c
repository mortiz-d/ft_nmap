#include "../lib/nmap.h"


int socket_connection_tcp(t_params *params)
{
    // struct timeval tv_timeout;
    int sockfd = 0;
    int activate = 1;

    if (params->active_scan == UDP_SCAN)
        sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    else
        sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

    if (sockfd < 0)
    {
        return -1;
    }

    //Tal vez innecesario ya que usamos pcap , no recv para leer mensajes
	// tv_timeout.tv_sec = TIMEOUT_MS / 1000;
    // tv_timeout.tv_usec = (TIMEOUT_MS % 1000) * 1000;
    // if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv_timeout, sizeof(tv_timeout)) < 0)
    // {
    //     close(sockfd);
    //     return -1;
    // }
    
    if ( setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &activate, sizeof(activate)) < 0)
    {
        close(sockfd);
        return -1;
    }

    return sockfd;
}


int socket_connection_udp(t_params *params)
{
    struct sockaddr_in local;
    int sockfd;

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //UDP

    memset(&local, 0, sizeof(local));

    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons(UDP_DEFAULT_BASE_PORT); // puerto origen

    if (bind(sockfd, (struct sockaddr *)&local, sizeof(local)) < 0)
    {
        return -1;
    }
    inet_pton(AF_INET, params->active_ip, &local.sin_addr);
    local.sin_port = htons(80);
    return sockfd;
}