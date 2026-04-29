#include "../lib/nmap.h"

void packet_handler(u_char *args, const struct pcap_pkthdr *hdr, const u_char *pkt)
{
    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];
    (void)args;

    struct iphdr  *ip  = (struct iphdr *)(pkt + 14);
    struct tcphdr *tcp = (struct tcphdr *)(pkt + 14 + ip->ihl * 4);

    inet_ntop(AF_INET, &ip->saddr, src_ip, sizeof(src_ip));
    inet_ntop(AF_INET, &ip->daddr, dst_ip, sizeof(dst_ip));

    printf("[%s:%d] → [%s:%d] | flags: %s%s%s | len: %d bytes\n",
        src_ip, ntohs(tcp->th_sport),
        dst_ip, ntohs(tcp->th_dport),
        tcp->th_flags & TH_SYN ? "SYN " : "",
        tcp->th_flags & TH_ACK ? "ACK " : "",
        tcp->th_flags & TH_RST ? "RST " : "",
        hdr->len
    );
}

void capture_packets(){
    char        errbuf[PCAP_ERRBUF_SIZE];
    pcap_t      *handle;
    pcap_if_t   *dev_lst;
    char        *dev;

    if (pcap_findalldevs(&dev_lst, errbuf) < 0) {
        printf("Couldn't find device: %s\n", errbuf);
        return;
    }
    dev = dev_lst->name;
    printf("Listening on: %s\n", dev);

    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (!handle) {
        printf("Couldn't open device: %s\n", errbuf);
        return;
    }

    struct bpf_program fp;
    char filter[] = "tcp";
    pcap_compile(handle, &fp, filter, 0, PCAP_NETMASK_UNKNOWN);
    pcap_setfilter(handle, &fp);

    pcap_loop(handle, 0, packet_handler, NULL);

    pcap_freealldevs(dev_lst);
    pcap_freecode(&fp);
    pcap_close(handle);
}
