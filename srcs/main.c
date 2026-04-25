//#include <studio.h>
#include "../lib/nmap.h"

int free_all(t_list *flags, t_params *params)
{
    // ft_lstiter(flags,print_flag);
    free_flags(&flags);
    free_params(params);
    return 1;
}

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
    // debug_params(params);

    if (params->ip_list == NULL)
    {
        printf("Error :No IPs where given to nmap\n");
        return free_all(flags, params);
    }
    ip_selected = *params->ip_list;
    while (ip_selected)
    {
        // printf("A\n");
        printf("argv->%s\n", (char *)ip_selected->content);
        aux = dns_lookup((char *)ip_selected->content);
        get_local_ip(aux, params->internal_ip);
        
        // printf("ip->%s origen->%s\n",aux,params->internal_ip);
        nmap(params,aux);
        // printf("B\n");
        free(aux);
        ip_selected = ip_selected->next;
        // printf("C\n");
    }
    // printf("F\n");
    return free_all(flags, params);
}