#include "../lib/nmap.h"

// #include "../lib/nmap.h"

static void print_ports(t_list *lst)
{
    t_port *p;

    while (lst)
    {
        p = (t_port *)lst->content;
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
        printf("IP: '%s'\n", ip);
        lst = lst->next;
    }
}

static char *scan_to_string(t_scan scan)
{
    static char *names[] = {"SYN","NUL","FIN","XMAS","ACK","UDP","UNKNOWN"};
    if (scan >= SYN_SCAN && scan <= UDP_SCAN)
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
        
    printf("\n--- HELP ---\n");
    if (params->help)
        printf("ACTIVATED\n");
    else
        printf("ACTIVATED\n");


    printf("\n--- THREADS ---\n");
    printf("Nº threads %i\n",params->threads);

    printf("\n--- PORTS ---\n");
    if (params->ports)
    {
        printf("Nº Ports : %i\n",params->n_ports);
        if (params->n_ports > 20)
            printf("To many ports to show here\n");
        else
            print_ports(*params->ports);
    }
    else
        printf("No ports\n");

    printf("\n--- IPS FROM FILE ---\n");
    if (params->ip_list)
        print_ips(*params->ip_list);
    else
        printf("No IPs\n");

    printf("\n--- SCANS ---\n");
    if (params->scan)
        print_scans(*params->scan);
    else
        printf("No scans\n");

    printf("======= END DEBUG =======\n");
}

void debug_tcp_header(struct tcphdr *tcp)
{
    printf("---- TCP HEADER DEBUG ----\n");

    printf("Source Port: %u\n", ntohs(tcp->source));
    printf("Dest Port: %u\n", ntohs(tcp->dest));

    printf("Sequence Number: %u\n", ntohl(tcp->seq));
    printf("Ack Number: %u\n", ntohl(tcp->ack_seq));

    printf("Data Offset: %u (%u bytes)\n", tcp->doff, tcp->doff * 4);

    printf("Flags:\n");
    printf("  SYN: %u\n", tcp->syn);
    printf("  ACK: %u\n", tcp->ack);
    printf("  FIN: %u\n", tcp->fin);
    printf("  RST: %u\n", tcp->rst);
    printf("  PSH: %u\n", tcp->psh);
    printf("  URG: %u\n", tcp->urg);

    printf("Window Size: %u\n", ntohs(tcp->window));
    printf("Checksum: %u\n", ntohs(tcp->check));

    printf("---------------------------\n");
}

void debug_ip_header(struct iphdr *ip)
{
    struct in_addr saddr, daddr;

    saddr.s_addr = ip->saddr;
    daddr.s_addr = ip->daddr;

    printf("---- IP HEADER DEBUG ----\n");
    printf("Version: %d\n", ip->version);
    printf("IHL: %d (%d bytes)\n", ip->ihl, ip->ihl * 4);
    printf("TOS: %d\n", ip->tos);
    printf("Total Length: %d\n", ntohs(ip->tot_len));
    printf("ID: %d\n", ntohs(ip->id));
    printf("Fragment Offset: %d\n", ip->frag_off);
    printf("TTL: %d\n", ip->ttl);
    printf("Protocol: %d\n", ip->protocol);
    printf("Checksum: %d\n", ip->check);
    printf("Source IP: %s\n", inet_ntoa(saddr));
    printf("Dest IP: %s\n", inet_ntoa(daddr));
    printf("--------------------------\n");
}