#include "../../lib/nmap.h"

char *create_filter(t_params *params)
{
    char *ips = NULL;
    char *ports = NULL;
    char *result;

    ips = get_ip_filter(params);
    ports = get_port_filter(params);

    ft_asprintf(&result,"%s and %s and tcp", ips,ports);

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

    filter = create_filter(params);
    if (DEBUG)
        printf("FILTER -> %s\n",filter);
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

    if (DEBUG)
        printf("PCAP : start scans\n");
    start = time(NULL);
    while ( params->n_packet_sended < expected )
    {
        // printf("SENDED %i/%i",params->n_packet_sended,expected);
        if (params->active_scan == UDP_SCAN)
            pcap_dispatch(handle, -1, packet_handler_udp, (u_char *)params);
        else
            pcap_dispatch(handle, -1, packet_handler_tcp, (u_char *)params);
        usleep(1000);
    }

    if (DEBUG)
        printf("PCAP : middle scans\n");

    while (time(NULL) - start < 5)
    {
        // printf("SENDED %i/%i",params->n_packet_sended,expected);
        if (params->n_packet_recieved >= params->n_packet_sended)
            break;
        if (params->active_scan == UDP_SCAN)
            pcap_dispatch(handle, -1, packet_handler_udp, (u_char *)params);
        else
            pcap_dispatch(handle, -1, packet_handler_tcp, (u_char *)params);
        usleep(10000);
    }

    if (DEBUG)
        printf("PCAP : ends scans\n");

    pcap_freecode(fp);
    pcap_close(handle);
    pcap_freealldevs(dev_lst);
}
