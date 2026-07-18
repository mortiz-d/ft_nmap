#include "../lib/nmap.h"

static const int scan_port_map[] = {SYN_PORT ,NUL_PORT ,FIN_PORT, XMAS_PORT, ACK_PORT ,UDP_PORT};

int scan_2_port(t_scan scan)
{
    if (scan < SYN_SCAN || scan > UDP_SCAN)
        return 0;
    return scan_port_map[scan];
}

int port_2_scan(int port)
{
    for (int scan =SYN_SCAN; scan <= UDP_SCAN ; ++scan)
        if (scan_port_map[scan] == port)
            return scan; 
    return SCAN_UNKNOWN;
}