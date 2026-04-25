#include "../lib/nmap.h"
// #include <netinet/ip.h>
// #include <netinet/tcp.h>



int nmap(t_params *params, char *ip)
{
    (void) params;
    (void) ip;
    struct sockaddr_in addr;
    t_list *ports;
    t_port *p;

    ft_memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    
    if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0)
    {
        printf("Invalid IP\n");
        return 0;
    }

    ports = *params->ports;

    while (ports)
    {
        p = ports->content;

        printf("Scanning port %d...\n", p->port_nbr);

        scan_tcp(params,SYN_SCAN, addr, p->port_nbr);

        ports = ports->next;
        break;
    }

    return 1;
}

//___________________________________________________________________________________________________
//___________________________________________________________________________________________________
//___________________________________________________________________________________________________
