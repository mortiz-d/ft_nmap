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
    return PORT_UNKNOWN;
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
        t_port_state state = determine_status_tcp (tcp,params->active_scan);
        modify_result_table (params, src_ip, port, state);
       
        params->n_packet_recieved++;

    }
}
