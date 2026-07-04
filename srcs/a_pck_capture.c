#include "../lib/nmap.h"
#include <stdio.h>

char *create_filter(t_params *params){
    char *filter = NULL;
    char *temp = NULL;
    t_list *ips = *params->ip_list;
    char *ip = NULL;
    char *port_str = NULL;

    int i = 0;
    while (ips){
        ip = (char *)ips->content;
        if (i == 0)
            temp = ft_strdup("(host ");
        else
            temp = ft_strjoin(filter, " or host ");
        filter = ft_strjoin(temp, ip);
        free(temp);

        ips = ips->next;
        ++i;
    }

    if (params->active_scan != UDP_SCAN)
        port_str = ft_itoa(SOURCE_PORT);



    temp = filter;
    if (params->active_scan == UDP_SCAN)
        filter = ft_strjoin(filter, ") and (udp or icmp)");
    else
        filter = ft_strjoin(filter, ") and tcp and dst port ");
    free(temp);

    if (params->active_scan != UDP_SCAN)
    {
        temp = filter;
        filter = ft_strjoin(filter, port_str);
        free(temp);
        free(port_str);
    }

    if (DEBUG)
        printf("filter is %s\n", filter);
    // free(temp);
    return filter;
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

    filter = create_filter(params);
    pcap_compile(handle, fp, filter, 0, PCAP_NETMASK_UNKNOWN);
    pcap_setfilter(handle, fp);
    pcap_setnonblock(handle, 1, errbuf);
    free(filter);
    if (DEBUG)
        printf("PCAP : ready for scans\n");
    return handle;
}

void capture_listen(t_params *params, pcap_t *handle, pcap_if_t *dev_lst, struct bpf_program *fp, int expected){
    time_t      start;

    start = time(NULL);
    while ( params->n_packet_sended < expected )
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

    pcap_freecode(fp);
    pcap_close(handle);
    pcap_freealldevs(dev_lst);
}
