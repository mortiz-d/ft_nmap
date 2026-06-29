#include "../../lib/nmap.h"



int cmp_ip(void *a, void *b)
{
    t_result_scan *aux = a;

    if (ft_strncmp(aux->ip,(char *)b,sizeof(INET_ADDRSTRLEN)) == 0)
        return 1;
    return 0;
}

int cmp_port(void *a, void *b)
{
    t_result_port *aux = a;
    uint16_t port = *(uint16_t *)b;

    return aux->port_nbr == port;
}

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

void alter_port_status (t_params *params, t_list *port,  t_port_state state)
{
    t_result_port *aux = port->content;
    switch (params->active_scan)
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
       
}

void packet_handler_tcp(u_char *args, const struct pcap_pkthdr *hdr, const u_char *pkt)
{
    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];
    t_params * params = (t_params *)args;
    struct iphdr  *ip  = (struct iphdr *)(pkt + 14);
    struct tcphdr *tcp = (struct tcphdr *)(pkt + 14 + ip->ihl * 4);
    t_result_scan *scan = NULL;
    
    inet_ntop(AF_INET, &ip->saddr, src_ip, sizeof(src_ip));
    inet_ntop(AF_INET, &ip->daddr, dst_ip, sizeof(dst_ip));

    uint16_t port = ntohs(tcp->th_sport);



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
        (void)scan;
        t_list *aux = ft_lstfind_match(*params->results, cmp_ip, src_ip);
        t_list *aux_s = ft_lstfind_match( *((t_result_scan *)aux->content)->port , cmp_port, &port);
        
        t_port_state state = determine_status_tcp (tcp,params->active_scan);

        alter_port_status(params,aux_s,state);
        // printf("ESTADO ALTERADO ;)\n");
        (void) aux_s;
        (void) aux;
        // scan = (t_result_scan *))->content;
        // scan = get_ip_table(src_ip);
        // if (aux)
        // {
        //     printf("Encontrado match para %s\n",((t_result_scan *)aux->content)->ip);
        // }
        // alter_port_status(params,ntohs(tcp->th_sport),determine_status_tcp(tcp,params->active_scan),params->active_scan);
        params->n_packet_recieved++;

    }
    else
    {
        // if ((tcp->th_flags & TH_RST) == 0)
        //     params->n_packet_sended++;
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
