#include "../lib/nmap.h"

void packet_handler(u_char *args, const struct pcap_pkthdr *hdr, const u_char *pkt)
{
    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];
    t_params * params = (t_params *)args;
    (void)params;

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

    if (pcap_findalldevs(&dev_lst, errbuf) < 0) {
        printf("Couldn't find device: %s\n", errbuf);
        return;
    }
    dev = dev_lst->name;
    pcap_if_t *d;
    for (d = dev_lst; d; d = d->next)
    {
        printf("Interface to listen %s\n", d->name);
    }
    printf("Listening on: %s\n", dev);

    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (!handle) {
        printf("Couldn't open device: %s\n", errbuf);
        return;
    }
    
    struct bpf_program fp;
    // char filter[] = "tcp and port 52341";
    // char filter[] = "tcp and dst host 192.168.1.136 and dst port 52341"; //Filtro los que recibo
    char filter[] = "tcp and src host 192.168.1.136 and src port 52341"; //Filtro os que envio

    // char filter[] = "tcp and (dst host 192.168.1.1 or dst host 192.168.1.128 or dst host 192.168.1.130 )";
    // char *filter_2 = ft_strjoin("tcp and port", ft_itoa(SOURCE_PORT)); //Recordar esto esta allocado
    // printf("FILTER IS: '%s'\n",filter_2);
    // printf("FILTER IS: '%s'\n",filter);
    pcap_compile(handle, &fp, filter, 0, PCAP_NETMASK_UNKNOWN);
    pcap_setfilter(handle, &fp);

    pcap_loop(handle, 0, packet_handler, (u_char *)params);

    pcap_freealldevs(dev_lst);
    pcap_freecode(&fp);
    pcap_close(handle);
}