#include "../lib/nmap.h"
#include <stdio.h>

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

static void packet_handler(u_char *args, const struct pcap_pkthdr *hdr, const u_char *pkt)
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
        printf("UDP OPEN?\n");
        if (DEBUG)
        {
            printf("RECV UDP [%s:%d] -> [%s:%d] | len: %d bytes\n", src_ip, ntohs(udp->uh_sport), dst_ip, ntohs(udp->uh_dport), hdr->len);
        }
        //if header is UDP its open
        modify_result_table (params, src_ip,port, PORT_OPEN, UDP_SCAN);
        params->n_packet_recieved++;
        

        break;
    case IPPROTO_ICMP:
        
            //if header is ICMP its closed
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

    

   
    
    

    // uint16_t port = ntohs(tcp->th_sport);

    // if (ft_strncmp(src_ip, params->internal_ip,INET_ADDRSTRLEN))
    // {
    //     if (DEBUG)
    //         printf("RECV  [%s:%d] → [%s:%d] | flags: %s%s%s%s%s%s | len: %d bytes\n", src_ip, ntohs(tcp->th_sport), dst_ip, ntohs(tcp->th_dport),
    //             tcp->th_flags & TH_SYN  ? "SYN " : "",
    //             tcp->th_flags & TH_ACK  ? "ACK " : "",
    //             tcp->th_flags & TH_RST  ? "RST " : "",
    //             tcp->th_flags & TH_FIN  ? "FIN " : "",
    //             tcp->th_flags & TH_PUSH ? "PSH " : "",
    //             tcp->th_flags & TH_URG  ? "URG " : "",
    //             hdr->len
    //         );
    //     t_port_state state = determine_status_tcp (tcp,ntohs(tcp->th_dport));
    //     t_scan scan = port_2_scan(ntohs(tcp->th_dport));
    //     modify_result_table (params, src_ip, port, state, scan);
       
    //     params->n_packet_recieved++;

    // }

}


char * get_port_filter(t_params *params)
{
    char *filter = NULL;
    char *temp = NULL;
    t_list *scans = *params->scan;
    t_scan *scan = NULL;
    char *port;

    int i = 0;
    while (scans){
        scan = (t_scan *)scans->content;
        if (i == 0)
            temp = ft_strdup("(dst port ");
        else
            temp = ft_strjoin(filter, " or dst port ");
        port = ft_itoa(scan_2_port(*scan));
        filter = ft_strjoin(temp,port );
        free(port);
        free(temp);
        
        scans = scans->next;
        ++i;
    }
    temp = ft_strjoin(filter, ") ");
    free(filter);

    return temp;
}

char * get_ip_filter(t_params *params)
{
    char *filter = NULL;
    char *temp = NULL;
    t_list *ips = *params->ip_list;
    char *ip = NULL;

    int i = 0;
    while (ips){
        ip = (char *)ips->content;
        if (i == 0)
            temp = ft_strdup("(src host ");
        else
            temp = ft_strjoin(filter, " or src host ");
        filter = ft_strjoin(temp, ip);
        free(temp);
        
        ips = ips->next;
        ++i;
    }
    temp = ft_strjoin(filter, ") ");
    free(filter);

    return temp;

}


char *create_filter(t_params *params)
{
    char *ips = NULL;
    char *ports = NULL;
    char *result;

    ips = get_ip_filter(params);
    ports = get_port_filter(params);

    ft_asprintf(&result,"%s and %s and tcp", ips,ports);
    
    return "(icmp and dst host 192.168.1.136) or (udp and src host 192.168.1.136 and src port 33434)";
    return "(src host 192.168.1.1)  and (dst port 52341 or dst port 52346)  and tcp";
    return result;
}

pcap_t *capture_setup(t_params *params, struct bpf_program *fp, pcap_if_t **dev_lst){
    char        errbuf[PCAP_ERRBUF_SIZE];
    pcap_t      *handle;
    char        *dev;
    char        *filter;

    if (pcap_findalldevs(dev_lst, errbuf) < 0) {
        printf("Couldn't find device: %s\n", errbuf);
        return NULL;
    }
    dev = (*dev_lst)->name;

    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (!handle) {
        printf("Couldn't open device: %s\n", errbuf);
        pcap_freealldevs(*dev_lst);
        return NULL;
    }
    params->n_packet_sended = 0;
    params->n_packet_recieved = 0;

    // (src host 192.168.1.1)  and (dst port 52341 or dst port 52346)  and tcp 
    filter = create_filter(params);




    if (DEBUG)
        printf("FILTER -> %s\n",filter);
    pcap_compile(handle, fp, filter, 0, PCAP_NETMASK_UNKNOWN);
    pcap_setfilter(handle, fp);
    pcap_setnonblock(handle, 1, errbuf);
    // free(filter);
    if (DEBUG)
        printf("PCAP : ready for scans\n");
    return handle;
}

void capture_listen(t_params *params, pcap_t *handle, pcap_if_t *dev_lst, struct bpf_program *fp, int expected){
    time_t      start;

    start = time(NULL);
    while ( params->n_packet_sended < expected )
    {
        // if (params->active_scan == UDP_SCAN)
            pcap_dispatch(handle, -1, packet_handler, (u_char *)params);
        // else
        //     pcap_dispatch(handle, -1, packet_handler_tcp, (u_char *)params);
        usleep(1000);
    }

    while (time(NULL) - start < 5)
    {
        if (params->n_packet_recieved >= params->n_packet_sended)
            break;
        // if (params->active_scan == UDP_SCAN)
            pcap_dispatch(handle, -1, packet_handler, (u_char *)params);
        // else
        //     pcap_dispatch(handle, -1, packet_handler_tcp, (u_char *)params);
        usleep(10000);
    }

    pcap_freecode(fp);
    pcap_close(handle);
    pcap_freealldevs(dev_lst);
}
