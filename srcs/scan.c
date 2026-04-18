#include "../lib/nmap.h"


t_port_state scan_syn(char *ip, int port)
{
    printf("[SYN] %s:%d\n", ip, port);
    return PORT_UNKNOWN;
}

t_port_state scan_udp(char *ip, int port)
{
    printf("[UDP] %s:%d\n", ip, port);
    return PORT_UNKNOWN;
}