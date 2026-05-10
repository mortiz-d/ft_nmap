#include "../lib/nmap.h"

// typedef struct s_result_scan
// {
//     int    port_nbr;
//     t_port_state state;
// }	t_result_scan;

// typedef enum e_scan
// {
//     SYN_SCAN, //SYN-ACK signal = Port open | RST signal = port closed | No anwser = filtered
//     NUL_SCAN, //No anwser = Port open | RST signal  = port open
//     FIN_SCAN,
//     XMAS_SCAN,
//     ACK_SCAN,
//     UDP_SCAN,
//     SCAN_UNKNOWN
// } t_scan;

// typedef enum e_port_state
// {
//     PORT_UNKNOWN,
//     PORT_OPEN,
//     PORT_CLOSED,
//     PORT_FILTERED
// } t_port_state;


t_port_state determine_status (struct tcphdr *tcp, t_scan scan_type)
{
    int syn,ack,rst;
    (void)syn;
    (void)ack;
    (void)rst;


    syn = tcp->th_flags & TH_SYN;
    ack = tcp->th_flags & TH_ACK;
    rst = tcp->th_flags & TH_RST;

    switch (scan_type)
    {
    case SYN_SCAN:
        if ( ack && rst )
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


void packet_handler(u_char *args, const struct pcap_pkthdr *hdr, const u_char *pkt)
{
    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];
    t_params * params = (t_params *)args;
    t_result_scan result;
    (void)params;
    (void)result;
    (void)hdr;
    struct iphdr  *ip  = (struct iphdr *)(pkt + 14);
    struct tcphdr *tcp = (struct tcphdr *)(pkt + 14 + ip->ihl * 4);

    inet_ntop(AF_INET, &ip->saddr, src_ip, sizeof(src_ip));
    inet_ntop(AF_INET, &ip->daddr, dst_ip, sizeof(dst_ip));

    if (ft_strncmp(src_ip, params->internal_ip,INET_ADDRSTRLEN))
    {
        printf("RECV  [%s:%d] → [%s:%d] | flags: %s%s%s | len: %d bytes\n",
        src_ip, ntohs(tcp->th_sport),
        dst_ip, ntohs(tcp->th_dport),
        tcp->th_flags & TH_SYN ? "SYN " : "",
        tcp->th_flags & TH_ACK ? "ACK " : "",
        tcp->th_flags & TH_RST ? "RST " : "",
        hdr->len
        );
        // printf("Port %d , status %d\n", ntohs(tcp->th_sport),determine_status(tcp,params->active_scan));
        alter_port_status(params,ntohs(tcp->th_sport),determine_status(tcp,params->active_scan),params->active_scan);
        // result = ft_calloc(1,sizeof(t_result_scan));
        // result->port_nbr;
        // printf("Anwser from  [%s:%d] → [%s:%d] ");


    }else
    {
        printf("SEND [%s:%d] → [%s:%d] | flags: %s%s%s | len: %d bytes\n",
        src_ip, ntohs(tcp->th_sport),
        dst_ip, ntohs(tcp->th_dport),
        tcp->th_flags & TH_SYN ? "SYN " : "",
        tcp->th_flags & TH_ACK ? "ACK " : "",
        tcp->th_flags & TH_RST ? "RST " : "",
        hdr->len
        );
    }
}

void capture_packets(t_params *params){
    char        errbuf[PCAP_ERRBUF_SIZE];
    pcap_t      *handle;
    pcap_if_t   *dev_lst;
    char        *dev;
    struct bpf_program fp;

    if (pcap_findalldevs(&dev_lst, errbuf) < 0) {
        printf("Couldn't find device: %s\n", errbuf);
        return;
    }
    dev = dev_lst->name;

    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (!handle) {
        printf("Couldn't open device: %s\n", errbuf);
        return;
    }


    char filter[] = "(tcp and dst host 192.168.1.136 and dst port 52341) or (tcp and src host 192.168.1.136 and src port 52341)";

    pcap_compile(handle, &fp, filter, 0, PCAP_NETMASK_UNKNOWN);
    pcap_setfilter(handle, &fp);

    pcap_setnonblock(handle, 1, errbuf);

    time_t start = time(NULL);

    while (time(NULL) - start < 15)
    {
        pcap_dispatch(handle, -1, packet_handler, (u_char *)params);

        usleep(10000);
    }
    pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(dev_lst);
    
    // char filter[] = "tcp and port 52341";
    // char filter[] = "tcp and dst host 192.168.1.136 and dst port 52341"; //Filtro los que recibo
    // char filter[] = "tcp and src host 192.168.1.136 and src port 52341"; //Filtro os que envio
    // char filter[] = "(tcp and dst host 192.168.1.136 and dst port 52341) or (tcp and src host 192.168.1.136 and src port 52341)";
    // pcap_compile(handle, &fp, filter, 0, PCAP_NETMASK_UNKNOWN);
    // pcap_setfilter(handle, &fp);

    // pcap_loop(handle, 0, packet_handler, (u_char *)params);

    // pcap_freealldevs(dev_lst);
    // pcap_freecode(&fp);
    // pcap_close(handle);
}