#include "../../lib/nmap.h"


static char *join_filter(t_list *list, char *prefix, char *(*to_value)(void *content))
{
    char *filter = NULL;
    char *piece, *value , *joined;
    int first = 1;
     
    while (list)
    {
        value = to_value(list->content);
        if (value)
        {
            piece = ft_strjoin(prefix, value);
            free(value);
            if (first)
                joined = ft_strjoin("(", piece);
            else
            {
                char *sep = ft_strjoin(filter, " or ");
                joined = ft_strjoin(sep, piece);
                free(sep);
            }
            free(piece);
            free(filter);
            filter = joined;
            first = 0;
        }
        list = list->next;
    }
    joined = ft_strjoin(filter, ") ");
    free(filter);
    return joined;
}

static char *ip_value(void *content)
{
    return dns_lookup((char *)content);
}

static char *tcp_port_value(void *content)
{
    t_scan *scan = (t_scan *)content;

    if (*scan == UDP_SCAN)
        return NULL;
    return ft_itoa(scan_2_port(*scan));
}

static char *udp_port_value(void *content)
{
    return ft_itoa(((t_port *)content)->port_nbr);
}

static char * get_port_filter(t_params *params)
{
    return join_filter(*params->scan, "dst port ", tcp_port_value);
}

static char * get_udp_port_filter(t_params *params)
{
    return join_filter(*params->ports, "src port ", udp_port_value);
}

bool scan_list_has(t_params *params, t_scan target)
{
    t_list *scans = *params->scan;
    t_scan *scan;

    while (scans){
        scan = (t_scan *)scans->content;
        if (*scan == target)
            return true;
        scans = scans->next;
    }
    return false;
}

char * get_ip_filter(t_params *params)
{
    return join_filter(*params->ip_list, "src host ", ip_value);
}


char *create_filter(t_params *params)
{
    char *ips = NULL;
    char *tcp_ports = NULL;
    char *udp_ports = NULL;
    char *result;
    bool has_udp = scan_list_has(params, UDP_SCAN);
    bool has_tcp = scan_list_has(params, SYN_SCAN) || scan_list_has(params, NUL_SCAN)
        || scan_list_has(params, FIN_SCAN) || scan_list_has(params, XMAS_SCAN)
        || scan_list_has(params, ACK_SCAN);

    ips = get_ip_filter(params);

    if (has_tcp && has_udp)
    {
        tcp_ports = get_port_filter(params);
        udp_ports = get_udp_port_filter(params);
        ft_asprintf(&result, "((%s and %s and tcp) or (icmp and dst host %s) or (%s and %s and udp))",
            ips, tcp_ports, params->internal_ip, ips, udp_ports);
    }
    else if (has_udp)
    {
        udp_ports = get_udp_port_filter(params);
        ft_asprintf(&result, "((icmp and dst host %s) or (%s and %s and udp))",
            params->internal_ip, ips, udp_ports);
    }
    else
    {
        tcp_ports = get_port_filter(params);
        ft_asprintf(&result, "(%s and %s and tcp)", ips, tcp_ports);
    }

    free(ips);
    free(tcp_ports);
    free(udp_ports);

    return result;
}