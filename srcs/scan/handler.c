#include "../../lib/nmap.h"

t_port_state determine_status_tcp (struct tcphdr *tcp, int port_recv)
{
    int syn,ack,rst;

    syn = tcp->th_flags & TH_SYN;
    ack = tcp->th_flags & TH_ACK;
    rst = tcp->th_flags & TH_RST;

    switch (port_recv)
    {
        case SYN_PORT:
            if ( ack && rst)
                return PORT_CLOSED;
            else if ( ack && syn )
                return PORT_OPEN;
            break;
        case NUL_PORT:
            if ( ack && rst )
                return PORT_CLOSED;
            else if ( rst )
                return PORT_OPEN;
            break;
        case ACK_PORT:
            if ( rst )
                return PORT_UNFILTERED;
            break;
        case XMAS_PORT:
        case FIN_PORT:
            if ( rst )
                return PORT_CLOSED;
            break;
        default:
            break;
    }
    return PORT_UNKNOWN;
}

void packet_handler(u_char *args, const struct pcap_pkthdr *hdr, const u_char *pkt)
{
    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];
    t_params *params = (t_params *)args;
    struct iphdr *ip = (struct iphdr *)(pkt + 14);
    struct iphdr *orig_ip;
    struct udphdr *udp;
    struct udphdr *orig_udp;
    struct icmphdr *icmp;
    struct tcphdr *tcp;
    uint16_t port;

    inet_ntop(AF_INET, &ip->saddr, src_ip, sizeof(src_ip));
    inet_ntop(AF_INET, &ip->daddr, dst_ip, sizeof(dst_ip));
    
    tcp = (struct tcphdr *)(pkt + 14 + ip->ihl * 4);

    if (ft_strncmp(src_ip, params->internal_ip,INET_ADDRSTRLEN)) //ICMP
    {
        switch (ip->protocol)
        {
            case IPPROTO_TCP:
                port = ntohs(tcp->th_sport);
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
                    t_port_state state = determine_status_tcp (tcp,ntohs(tcp->th_dport));
                    t_scan scan = port_2_scan(ntohs(tcp->th_dport));
                    modify_result_table (params, src_ip, port, state, scan);
                
                    params->n_packet_recieved++;

                break;
            case IPPROTO_UDP:
                udp = (struct udphdr *)(pkt + 14 + ip->ihl * 4);
                port = ntohs(udp->uh_sport);

                if (DEBUG)
                {
                    printf("RECV UDP [%s:%d] -> [%s:%d] | len: %d bytes\n", src_ip, ntohs(udp->uh_sport), dst_ip, ntohs(udp->uh_dport), hdr->len);
                }
                //if header is UDP its open
                modify_result_table (params, src_ip,port, PORT_OPEN, UDP_SCAN);
                params->n_packet_recieved++;
                

                break;
            case IPPROTO_ICMP:
                    icmp = (struct icmphdr *)(pkt + 14 + ip->ihl * 4);

                    if (icmp->type == 3 && icmp->code == 3)
                    {
                        orig_ip = (struct iphdr *)((u_char *)icmp + sizeof(struct icmphdr));
                        orig_udp = (struct udphdr *)((u_char *)orig_ip + orig_ip->ihl * 4);
                        port =  ntohs(orig_udp->uh_dport);
                        if (DEBUG)
                            printf("RECV ICMP [%s] -> [%s] | type=%d code=%d | UDP port CLOSED: %d | len: %d bytes\n", src_ip, dst_ip, icmp->type, icmp->code, ntohs(orig_udp->uh_dport),hdr->len);
                        modify_result_table (params, src_ip,port, PORT_CLOSED, UDP_SCAN);
                        params->n_packet_recieved++;
                    }
                
                break;
            default:
                break;
            }
    }
}
