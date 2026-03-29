#include "../lib/nmap.h"

// #include "../lib/nmap.h"

static void print_ports(t_list *lst)
{
    t_port *p;

    while (lst)
    {
        p = (t_port *)lst->content;
        if (p->range)
            printf("PORT RANGE: %d - %d\n", p->port_min, p->port_max);
        else
            printf("PORT: %d\n", p->port_nbr);
        lst = lst->next;
    }
}

static void print_ips(t_list *lst)
{
    char *ip;

    while (lst)
    {
        ip = (char *)lst->content;
        printf("IP: %s", ip); // ya incluye \n
        lst = lst->next;
    }
}

static char *scan_to_string(t_scan scan)
{
    static char *names[] = {"SYN","NUL","FIN","XMAS","ACK","UDP","UNKNOWN"};
    if (scan >= SYN && scan <= UDP)
        return names[scan];
    return names[6];
}

static void print_scans(t_list *lst)
{
    t_scan *s;

    while (lst)
    {
        s = (t_scan *)lst->content;
        printf("SCAN: %s (%d)\n", scan_to_string(*s), *s);
        lst = lst->next;
    }
}

void debug_params(t_params *params)
{
    printf("======= PARAMS DEBUG =======\n");

    if (params->destination)
        printf("Destination: %s\n", params->destination);
        
    printf("\n--- HELP ---\n");
    if (params->help)
        printf("ACTIVATED\n");
    else
        printf("ACTIVATED\n");


    printf("\n--- THREADS ---\n");
    printf("Nº threads %i\n",params->threads);

    printf("\n--- PORTS ---\n");
    if (params->ports)
        print_ports(*params->ports);
    else
        printf("No ports\n");

    printf("\n--- IPS FROM FILE ---\n");
    if (params->filename_ip)
        print_ips(*params->filename_ip);
    else
        printf("No IPs\n");

    printf("\n--- SCANS ---\n");
    if (params->scan)
        print_scans(*params->scan);
    else
        printf("No scans\n");

    printf("======= END DEBUG =======\n");
}