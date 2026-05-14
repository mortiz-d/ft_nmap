#include "../lib/nmap.h"

void capture_packets(t_params *params){
    char        filter[256];
    char        errbuf[PCAP_ERRBUF_SIZE];
    pcap_t      *handle;
    pcap_if_t   *dev_lst;
    char        *dev;
    struct bpf_program fp;
    time_t      start;

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
    if (DEBUG)
        printf("PCAP : ready for scans\n");
    params->n_packet_sended = 0;
    params->n_packet_recieved = 0;

   
    // filter = "(icmp and dst host 192.168.1.136) or (udp and src host 192.168.1.136 and src port 33434)"; //Solo detecta open|filtered o closed falta open (viene de un mensaje UDP)
    // char filter[] = "(tcp and dst host 192.168.1.136 and dst port 52341) or (tcp and src host 192.168.1.136 and src port 52341)";
    ft_memset(filter,0,256);
    if (params->active_scan == UDP_SCAN)
        ft_strlcpy(filter,"(icmp and dst host 192.168.1.136) or (udp and src host 192.168.1.136 and src port 33434)",256);
    else
        ft_strlcpy(filter,"(tcp and dst host 192.168.1.136 and dst port 52341) or (tcp and src host 192.168.1.136 and src port 52341)",256);

    pcap_compile(handle, &fp, filter, 0, PCAP_NETMASK_UNKNOWN);
    pcap_setfilter(handle, &fp);

    pcap_setnonblock(handle, 1, errbuf);
    
    start = time(NULL);
    while ( params->n_packet_sended < params->n_ports)
    {
        if (params->active_scan == UDP_SCAN)
            pcap_dispatch(handle, -1, packet_handler_udp, (u_char *)params);
        else
            pcap_dispatch(handle, -1, packet_handler_tcp, (u_char *)params);
        usleep(1000);
    }

    while (time(NULL) - start < 5)
    {
        if (params->n_packet_recieved >= params->n_packet_sended)
            break;
        if (params->active_scan == UDP_SCAN)
            pcap_dispatch(handle, -1, packet_handler_udp, (u_char *)params);
        else
            pcap_dispatch(handle, -1, packet_handler_tcp, (u_char *)params);
        usleep(10000);
    }

    pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(dev_lst);
   
}