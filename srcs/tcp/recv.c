#include "../../lib/nmap.h"

t_port_state determine_status_tcp (struct tcphdr *tcp, t_scan scan_type)
{
    int syn,ack,rst;

    syn = tcp->th_flags & TH_SYN;
    ack = tcp->th_flags & TH_ACK;
    rst = tcp->th_flags & TH_RST;

    switch (scan_type)
    {
        case SYN_SCAN:
            if ( ack && rst)
                return PORT_CLOSED;
            else if ( ack && syn )
                return PORT_OPEN;
            break;
        case NUL_SCAN:
            if ( rst )
                return PORT_OPEN;
            break;
        case FIN_SCAN:
            if ( ack && rst )
                return PORT_CLOSED;
            break;
        case XMAS_SCAN:
            if ( ack && rst )
                return PORT_CLOSED;
            break;
        case ACK_SCAN:
            if ( rst )
                return PORT_UNFILTERED;
            break;
        default:
            break;
    }
    // printf("Could not identify %i \n",scan_type);
    return PORT_UNKNOWN;

}

void alter_port_status (t_params *params, int port , t_port_state state, t_scan scan_type)
{
    t_list *res = NULL;
    t_result_scan *aux;
    
    res = *(params->results);
    while (res)
    {
        aux = (t_result_scan *)res->content;
        if (aux->port_nbr == port)
        {
            switch (scan_type)
            {
            case SYN_SCAN:
                aux->syn = state;
                break;
            case NUL_SCAN:
                aux->nul = state;
                break;
            case FIN_SCAN:
                aux->fin = state;
                break;
            case XMAS_SCAN:
                aux->xmas = state;
                break;
            case ACK_SCAN:
                aux->ack = state;
                break;
            case UDP_SCAN:
                aux->udp = state;
            default:
                break;
            }
            break;
        }
        res = res->next;
    }
}

void packet_handler_tcp(u_char *args, const struct pcap_pkthdr *hdr, const u_char *pkt)
{
    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];
    t_params * params = (t_params *)args;
    struct iphdr  *ip  = (struct iphdr *)(pkt + 14);
    struct tcphdr *tcp = (struct tcphdr *)(pkt + 14 + ip->ihl * 4);

    inet_ntop(AF_INET, &ip->saddr, src_ip, sizeof(src_ip));
    inet_ntop(AF_INET, &ip->daddr, dst_ip, sizeof(dst_ip));

    if (ft_strncmp(src_ip, params->internal_ip,INET_ADDRSTRLEN))
    {
        if (DEBUG)
            printf("RECV  [%s:%d] → [%s:%d] | flags: %s%s%s%s%s%s | len: %d bytes\n", src_ip, ntohs(tcp->th_sport), dst_ip, ntohs(tcp->th_dport),
        tcp->th_flags & TH_SYN  ? "SYN " : "",
        tcp->th_flags & TH_ACK  ? "ACK " : "",
        tcp->th_flags & TH_RST  ? "RST " : "",
        tcp->th_flags & TH_FIN  ? "FIN " : "",
        tcp->th_flags & TH_PUSH ? "PSH " : "",
        tcp->th_flags & TH_URG  ? "URG " : "",
        hdr->len
        );
    
        alter_port_status(params,ntohs(tcp->th_sport),determine_status_tcp(tcp,params->active_scan),params->active_scan);
        params->n_packet_recieved++;

    }
    else
    {
        if ((tcp->th_flags & TH_RST) == 0)
            params->n_packet_sended++;
        if (DEBUG)
            printf("SEND  [%s:%d] → [%s:%d] | flags: %s%s%s%s%s%s | len: %d bytes\n", src_ip, ntohs(tcp->th_sport), dst_ip, ntohs(tcp->th_dport),
            tcp->th_flags & TH_SYN  ? "SYN " : "",
            tcp->th_flags & TH_ACK  ? "ACK " : "",
            tcp->th_flags & TH_RST  ? "RST " : "",
            tcp->th_flags & TH_FIN  ? "FIN " : "",
            tcp->th_flags & TH_PUSH ? "PSH " : "",
            tcp->th_flags & TH_URG  ? "URG " : "",
            hdr->len
            );
    }
}
