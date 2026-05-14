#include "../../lib/nmap.h"

void alter_port_status_udp (t_params *params, int port , t_port_state state)
{
    t_list *res = NULL;
    t_result_scan *aux;
    
    res = *(params->results);
    while (res)
    {
        aux = (t_result_scan *)res->content;
        if (aux->port_nbr == port)
        {
            aux->udp = state;        
        }
        res = res->next;
    }
}

void packet_handler_udp(u_char *args, const struct pcap_pkthdr *hdr, const u_char *pkt)
{
    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];
    t_params *params = (t_params *)args;
    struct iphdr *ip = (struct iphdr *)(pkt + 14);
    struct iphdr *orig_ip;
    struct udphdr *udp;
    struct udphdr *orig_udp;
    struct icmphdr *icmp;

    inet_ntop(AF_INET, &ip->saddr, src_ip, sizeof(src_ip));
    inet_ntop(AF_INET, &ip->daddr, dst_ip, sizeof(dst_ip));

    if (ip->protocol == IPPROTO_UDP) //UDP
    {
        udp = (struct udphdr *)(pkt + 14 + ip->ihl * 4);

        if (!ft_strncmp(src_ip, params->internal_ip, INET_ADDRSTRLEN))
        {
            if (DEBUG)
                printf("SEND UDP [%s:%d] -> [%s:%d] | len: %d bytes\n", src_ip, ntohs(udp->uh_sport), dst_ip, ntohs(udp->uh_dport), hdr->len);
            params->n_packet_sended++;
        }
        else
        {
            if (DEBUG)
                printf("RECV UDP [%s:%d] -> [%s:%d] | len: %d bytes\n", src_ip, ntohs(udp->uh_sport), dst_ip, ntohs(udp->uh_dport), hdr->len);
            alter_port_status_udp( params, ntohs(udp->uh_sport), PORT_OPEN);
            params->n_packet_recieved++;
        }
    }
    else if (ip->protocol == IPPROTO_ICMP) //ICMP
    {
        icmp = (struct icmphdr *)(pkt + 14 + ip->ihl * 4);

        if (icmp->type == 3 && icmp->code == 3)
        {
            orig_ip = (struct iphdr *)((u_char *)icmp + sizeof(struct icmphdr));
            orig_udp = (struct udphdr *)((u_char *)orig_ip + orig_ip->ihl * 4);

            if (DEBUG)
                printf("RECV ICMP [%s] -> [%s] | type=%d code=%d | UDP port CLOSED: %d | len: %d bytes\n", src_ip, dst_ip, icmp->type, icmp->code, ntohs(orig_udp->uh_dport),hdr->len);

            alter_port_status_udp(params,ntohs(orig_udp->uh_dport),PORT_CLOSED);
            params->n_packet_recieved++;
        }
    }
}