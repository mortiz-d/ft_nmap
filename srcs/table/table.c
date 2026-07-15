#include "../../lib/nmap.h"
#include <netdb.h>
#include <sys/socket.h>

static int cmp_ip(void *a, void *b)
{
    t_result_scan *aux = a;

    if (ft_strncmp(aux->ip,(char *)b,INET_ADDRSTRLEN) == 0)
        return 1;
    return 0;
}

static int cmp_port(void *a, void *b)
{
    t_result_port *aux = a;
    uint16_t port = *(uint16_t *)b;

    return aux->port_nbr == port;
}

void free_result(void *result)
{
    t_result_scan *scan;

    scan = (t_result_scan *)result;
    free(scan->ip);
    ft_lstiter(*scan->port,free);
    ft_lstclear(scan->port);

    free(scan->port);
    free(scan);
}

static const char *port_state_str(t_port_state state)
{
    switch (state)
    {
        case PORT_OPEN:
            return "OPEN";
            break;
        case PORT_CLOSED:
            return "CLOSED";
            break;
        case PORT_FILTERED:
            return "FILTERED";
            break;
        case PORT_UNFILTERED:
            return "UNFILTERED";
            break;
        case PORT_UNCALLED:
            return "----------";
            break;
        case PORT_OPENFILTERED:
            return "OP|FILT";
            break;

        default:
            break;
    }
    return "UNKNOWN";
}

t_result_port * generate_default_port (t_params *params, t_port *port)
{
    t_result_port *r_scan;
    t_list * scans;
    t_scan *scan;

    r_scan = ft_calloc(1,sizeof(t_result_port));
    r_scan->port_nbr = port->port_nbr;

    r_scan->syn = PORT_UNCALLED;
    r_scan->nul = PORT_UNCALLED;
    r_scan->ack = PORT_UNCALLED;
    r_scan->fin = PORT_UNCALLED;
    r_scan->xmas = PORT_UNCALLED;
    r_scan->udp = PORT_UNCALLED;
    scans = *params->scan;
    while (scans){
        scan = (t_scan *)scans->content;
        switch (*scan)
        {
            case SYN_SCAN:
                r_scan->syn = PORT_FILTERED;
            break;
            case NUL_SCAN:
                r_scan->nul = PORT_CLOSED;
            break;
            case ACK_SCAN:
                r_scan->ack = PORT_FILTERED;
            break;
            case FIN_SCAN:
                r_scan->fin = PORT_OPENFILTERED;
            break;
            case XMAS_SCAN:
                r_scan->xmas = PORT_OPENFILTERED;
                break;
            case UDP_SCAN:
                r_scan->udp = PORT_OPENFILTERED;
                break;
            default:
                break;
        }
        scans = scans->next;
    }

    return r_scan;
}

void alter_port_status (t_params *params, t_list *port,  t_port_state state)
{
    t_result_port *aux = port->content;
    switch (params->active_scan)
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
}

void generate_result_table(t_params *params)
{
    (void)params;
    t_list **lst_res ;//= ft_calloc(1,sizeof(t_list*));
    t_list **lst_ports = NULL;
    t_list *ips, *ports;
    t_result_scan *aux;
    t_result_port *aux_p;

    if (!params->ip_list)
        return;

    lst_res = ft_calloc(1,sizeof(t_list*));
    ips = *(params->ip_list);
    while(ips)
    {
        aux = ft_calloc(1,sizeof(t_result_scan));
        aux->ip = ft_strdup((char *)ips->content);
        ports = *(params->ports);
        lst_ports = ft_calloc(1,sizeof(t_list*));
        while (ports)
        {
            // aux_p = ft_calloc(1,sizeof(t_result_port));
            aux_p = generate_default_port(params,(t_port *)ports->content);
            ft_lstadd_back(lst_ports, ft_lstnew(aux_p));
            ports = ports->next;
        }

        aux->port = lst_ports;

        ft_lstadd_back(lst_res, ft_lstnew(aux));
        ips = ips->next;
    }
    params->results = lst_res;
}

void modify_result_table (t_params *params, char *ip, uint16_t port, t_port_state state)
{
    t_list *ip_result = NULL;
    t_list *ip_port_result = NULL;

    ip_result = ft_lstfind_match(*params->results, cmp_ip, ip);
    if (ip_result)
        ip_port_result = ft_lstfind_match( *((t_result_scan *)ip_result->content)->port , cmp_port, &port);
    if (ip_port_result)
        alter_port_status(params,ip_port_result,state);
}




void print_result_table(t_params *params)
{
    t_list *a_scan, *p_scan;
    t_result_scan *scan ;
    t_result_port *port ;
    struct servent *service;
    char *srvname;

    a_scan = ((t_list *)*params->results);
    while (a_scan)
    {
        scan = a_scan->content;
        p_scan = ((t_list *)*scan->port);
        printf("\n");
        printf("%s\n",scan->ip);
        printf("+--------+------------+------------+------------+------------+------------+------------+------------+\n");
        printf("| PORT   | SERVICE    | SYN        | NULL       | FIN        | XMAS       | ACK        | UDP        |\n");
        printf("+--------+------------+------------+------------+------------+------------+------------+------------+\n");

        while (p_scan)
        {
            port = p_scan->content;
            service = getservbyport(htons(port->port_nbr), NULL);
            if (!service)
                srvname = ft_strdup("Unassigned");
            else
                srvname = ft_strdup(service->s_name);
            printf("| %-6d | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s |\n",port->port_nbr,srvname,port_state_str(port->syn),port_state_str(port->nul),port_state_str(port->fin),port_state_str(port->xmas),port_state_str(port->ack),port_state_str(port->udp));
            free(srvname);
            p_scan = p_scan->next;
        }
        printf("+--------+------------+------------+------------+------------+------------+------------+------------+\n");
        a_scan = a_scan->next;
    }


    // t_result_port *s;


    // while (lst)
    // {
    //     s = (t_result_port *)lst->content;

    //     printf("| %-6d | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s |\n",s->port_nbr,port_state_str(s->syn),port_state_str(s->nul),port_state_str(s->fin),port_state_str(s->xmas),port_state_str(s->ack),port_state_str(s->udp));
    //     lst = lst->next;
    // }


}
