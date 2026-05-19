//#include <studio.h>
#include "../lib/nmap.h"

int free_all(t_list *flags, t_params *params)
{
    // ft_lstiter(flags,print_flag);
    free_flags(&flags);
    free_params(params);
    return 1;
}


// int send_probe_udp_2(int sockfd, struct sockaddr_in addr,t_params *params)
// {
//         (void)params;

//     char *mensaje;
//     int     send;
//     ssize_t mensaje_size;
//     uint16_t dest_port ;

//     dest_port = UDP_DEFAULT_BASE_PORT;
//     mensaje_size = 500;

//     mensaje = ft_calloc(mensaje_size, sizeof(char));
//     addr.sin_port = htons(dest_port);
    

//     send = sendto(sockfd, mensaje, sizeof(mensaje), 0, (struct sockaddr *)&addr, sizeof(addr));
//     if (send < 0) {
//         if (DEBUG)
//             fprintf(stderr,"traceroute : Error sending UDP probe\n");
//         return 0;
//     }

//     free(mensaje);
//     return 1;
// }


int main(int argc, char **argv) 
{
    t_list *flags = NULL;
    t_params *params = NULL;
    t_list *ip_selected = NULL;
    char *aux;

    params =  params_default_config();
    flags = flags_config();

    if (!process_flags(argc, argv, flags,params))
    {
        return free_all(flags, params);
    }

    generate_result_table(params);
    // debug_params(params);

    if (params->ip_list == NULL)
    {
        printf("Error :No IPs where given to nmap\n");
        return free_all(flags, params);
    }

    ip_selected = *params->ip_list;
    while (ip_selected)
    {
        printf("argv->%s\n", (char *)ip_selected->content);
        aux = dns_lookup((char *)ip_selected->content);
        get_local_ip(aux, params->internal_ip);
        params->active_ip = aux;
        main_scan_logic(params);
        free(aux);
        ip_selected = ip_selected->next;
    }
    printf("F\n");
    return free_all(flags, params);
}