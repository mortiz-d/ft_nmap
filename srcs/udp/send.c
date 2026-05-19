#include "../../lib/nmap.h"

int send_probe_udp(int sockfd ,struct sockaddr_in addr,t_params *params, int port)
{
    (void)params;
    char *mensaje;
    int     send;
    ssize_t mensaje_size;
    uint16_t dest_port ;

    dest_port = port;
    mensaje_size = 500;

    mensaje = ft_calloc(mensaje_size, sizeof(char));
    addr.sin_port = htons(dest_port);
    

    send = sendto(sockfd, mensaje, sizeof(mensaje), 0, (struct sockaddr *)&addr, sizeof(addr));
    if (send < 0) {
        if (DEBUG)
            printf("Error : UDP Sending to port\n");
        return 0;
    }

    free(mensaje);
    return 1;
}