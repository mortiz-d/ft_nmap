#include "../../../lib/nmap.h"

int socket_connection_udp(struct sockaddr_in addr)
{
    struct sockaddr_in local;
    int sockfd;
    char ip[INET_ADDRSTRLEN];


    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //UDP

    memset(&local, 0, sizeof(local));

    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons(UDP_DEFAULT_BASE_PORT); // puerto origen


    if (bind(sockfd, (struct sockaddr *)&local, sizeof(local)) < 0)
    {
        return -1;
    }

    inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
    inet_pton(AF_INET, ip, &local.sin_addr);
    local.sin_port = htons(80);


    return sockfd;
}
