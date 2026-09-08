#include "../../lib/nmap.h"

static bool has_local_target(t_params *params)
{
    char *ip;
    int  local = 0;

    for (t_list *ip_list = *params->ip_list; ip_list && !local; ip_list = ip_list->next)
    {
        ip = dns_lookup((char *)ip_list->content);
        if (!ip)
            continue;
        if (!ft_strncmp(ip, params->internal_ip, INET_ADDRSTRLEN))
            local = 1;
        free(ip);
    }
    return local;
}

static char *pick_device(pcap_if_t *lst, char *local_ip, bool loopback)
{
    struct sockaddr_in *sin;
    char buf[INET_ADDRSTRLEN];

    for (pcap_if_t *d = lst; d; d = d->next)
    {
        if (loopback)
        {
            if (d->flags & PCAP_IF_LOOPBACK)
                return d->name;
            continue;
        }
        for (pcap_addr_t *a = d->addresses; a; a = a->next)
        {
            if (!a->addr || a->addr->sa_family != AF_INET)
                continue;
            sin = (struct sockaddr_in *)a->addr;
            inet_ntop(AF_INET, &sin->sin_addr, buf, sizeof(buf));
            if (!ft_strncmp(buf, local_ip, INET_ADDRSTRLEN))
                return d->name;
        }
    }
    return lst->name;
}


pcap_t *capture_setup(t_params *params, struct bpf_program *fp, pcap_if_t **dev_lst){
    char        errbuf[PCAP_ERRBUF_SIZE];
    pcap_t      *handle;
    char        *dev, *filter;

    if (pcap_findalldevs(dev_lst, errbuf) < 0) {
        printf("Couldn't find device: %s\n", errbuf);
        return NULL;
    }
    dev = pick_device(*dev_lst, params->internal_ip, has_local_target(params));
    if (DEBUG)
        printf("DEVICE -> %s (local ip %s)\n", dev, params->internal_ip);

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
        pcap_dispatch(handle, -1, packet_handler, (u_char *)params);
        usleep(1000);
    }

    while (time(NULL) - start < 5)
    {
        if (params->n_packet_recieved >= params->n_packet_sended)
            break;
        pcap_dispatch(handle, -1, packet_handler, (u_char *)params);
        usleep(10000);
    }

    pcap_freecode(fp);
    pcap_close(handle);
    pcap_freealldevs(dev_lst);
}
