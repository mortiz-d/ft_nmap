#include "../lib/nmap.h"

int scan_2_port(t_scan scan)
{
    switch (scan)
    {
    case SYN_SCAN:
        return SYN_PORT;
        break;
    case NUL_SCAN:
        return NUL_PORT;
        break;
    case FIN_SCAN: 
        return FIN_PORT;
        break;
    case XMAS_SCAN:
        return XMAS_PORT;
        break;    
    case ACK_SCAN:
        return ACK_PORT;
        break;
    case UDP_SCAN:
        return UDP_PORT;
        break;
    default:
        break;
    }
    return 0;
}

int port_2_scan(int port)
{
    switch (port)
    {
        case SYN_PORT:
            return SYN_SCAN;
            break;
        case NUL_PORT:
            return NUL_SCAN;
            break;
        case FIN_PORT: 
            return FIN_SCAN;
            break;
        case XMAS_PORT:
            return XMAS_SCAN;
            break;    
        case ACK_PORT:
            return ACK_SCAN;
            break;
        case UDP_PORT:
            return UDP_SCAN;
            break;
        default:
            break;
    }
    return SCAN_UNKNOWN;
}