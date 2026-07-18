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

// Default state assumed when a scan gets no answer for a port, indexed by t_scan.
static const t_port_state no_answer_default[] = {
    PORT_FILTERED,      // SYN_SCAN
    PORT_CLOSED,        // NUL_SCAN
    PORT_OPENFILTERED,  // FIN_SCAN
    PORT_OPENFILTERED,  // XMAS_SCAN
    PORT_FILTERED,      // ACK_SCAN
    PORT_OPENFILTERED,  // UDP_SCAN
};

t_result_port * generate_default_port (t_params *params, t_port *port)
{
    t_result_port *r_scan;
    t_list * scans;
    t_scan *scan;

    r_scan = ft_calloc(1,sizeof(t_result_port));
    r_scan->port_nbr = port->port_nbr;

    for (int s = SYN_SCAN; s <= UDP_SCAN; ++s)
        r_scan->states[s] = PORT_UNCALLED;

    scans = *params->scan;
    while (scans){
        scan = (t_scan *)scans->content;
        r_scan->states[*scan] = no_answer_default[*scan];
        scans = scans->next;
    }

    return r_scan;
}

void alter_port_status (t_list *port,  t_port_state state, t_scan scan)
{
    t_result_port *aux = port->content;

    if (scan >= SYN_SCAN && scan <= UDP_SCAN)
        aux->states[scan] = state;
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

void modify_result_table (t_params *params, char *ip, uint16_t port, t_port_state state, t_scan scan)
{
    t_list *ip_result = NULL;
    t_list *ip_port_result = NULL;

    ip_result = ft_lstfind_match(*params->results, cmp_ip, ip);
    if (ip_result)
        ip_port_result = ft_lstfind_match( *((t_result_scan *)ip_result->content)->port , cmp_port, &port);
    if (ip_port_result)
        alter_port_status(ip_port_result,state, scan);
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

            printf("| %-6d ", port->port_nbr);
            printf("| %-10s ", srvname);
            for (int s = SYN_SCAN; s <= UDP_SCAN; ++s)
                printf("| %-10s ", port_state_str(port->states[s]));
            printf("|\n");
            free(srvname);
            p_scan = p_scan->next;
        }
        printf("+--------+------------+------------+------------+------------+------------+------------+------------+\n");
        a_scan = a_scan->next;
    }
}
