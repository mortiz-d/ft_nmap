#include "../lib/nmap.h"
#include <stdio.h>

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
